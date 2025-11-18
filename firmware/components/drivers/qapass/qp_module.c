#ifdef __GNUC__
# include <stdio.h>
# include <pthread.h>
#endif // __GNUC__ 

#include "freertos/FreeRTOS.h"
#include "freertos/task.h"
#include "driver/i2c.h"
#include "esp_log.h"
#include "esp_err.h"

#define DEQUE_SIZE 100 

#include "qp.h"
#include "strutil.h"

const char *tag = "QP";

int reg_counter = 0;

#if defined __USE_MISC && defined _PTHREAD 
/* To avoid display error from race condition 
 * Actually it is not safe to use pthread_mutex_t on a 
 * Embarqued system */
static pthread_mutex_t write_lock;

//#elif not defined _PTHREAD 
#else 
/* We use internal ESP32 variables */
 static SemaphoreHandle_t write_lock; 
#endif 

static char register_cursor_position[DEQUE_SIZE][2]; 
static char current_buffer_l1[MAX_BUF_ALLOWED]; 
static char current_buffer_l2[MAX_BUF_ALLOWED]; 
static TaskHandle_t write_ops_handle; 

static esp_err_t lcd_send_byte(uint8_t val, uint8_t mode) {
    uint8_t high = (val & 0xF0) | mode | LCD_BACKLIGHT;
    uint8_t low  = ((val << 4) & 0xF0) | mode | LCD_BACKLIGHT;

    uint8_t data_h[] = { high | LCD_ENABLE_BIT, high, low | LCD_ENABLE_BIT, low };
    return i2c_master_write_to_device(I2C_MASTER_NUM, LCD_ADDR, data_h, sizeof(data_h), 1000 / portTICK_PERIOD_MS);
}

static void lcd_cmd(uint8_t cmd) {
    lcd_send_byte(cmd, 0x00);
}

static void lcd_data(uint8_t data) {
    lcd_send_byte(data, 0x01);
}

static void lcd_init(void) {
    vTaskDelay(pdMS_TO_TICKS(50)); // Wait for LCD to power up

    lcd_cmd(0x33);
    lcd_cmd(0x32);
    lcd_cmd(0x28);
    lcd_cmd(0x0C); 
    lcd_cmd(0x06); 
    lcd_cmd(0x01); 
    vTaskDelay(pdMS_TO_TICKS(5));
}

static void lcd_set_cursor(uint8_t col, uint8_t row) {
    static uint8_t row_offsets[] = {0x00, 0x40};
    lcd_cmd(0x80 | (col + row_offsets[row]));
}

static void lcd_print(const char *str) {
    while (*str) lcd_data(*str++);
}

static void handle_cursor(void (*callback)(uint8_t, uint8_t), uint8_t __col, uint8_t __row, int flag) {
    for(int i=0; i < reg_counter; ++i) {
        if (register_cursor_position[i][0] == __col) {
            if (register_cursor_position[i][1] == __row) {
                // The cursor has already a character displayed to it 
                ESP_LOGI(tag, "Trying to write to an existing emplacement...");  
            }
        }
    }

    if (reg_counter == DEQUE_SIZE - 1) {
        char tmp[DEQUE_SIZE][2]; 
        size_t sz = sizeof (register_cursor_position); 

        if (sz == sizeof(char*)) {
            ESP_LOGI(tag, "Size mismatch, possible buffer overflow (current size %zu)", sz);
        }

        if ((flag & 0xff) == QP_OVERWRITE_CURRENT_BUFFER) {
            memcpy(&tmp, &register_cursor_position, sz); 
            for (int i=0; i < DEQUE_SIZE; ++i) {
                memcpy(&register_cursor_position[i], &tmp[i+1], sz); 
            }
        } else {
            ESP_LOGI(tag, "Cannot overwrite current buffer"); 
            return; 
        }
    }
        
    // The emplacement is free 
    register_cursor_position[reg_counter][0] = __col;
    register_cursor_position[reg_counter][1] = __row;
    reg_counter++; 
    callback(__col, __row); 
}

#ifdef __cplusplus 
void __qapass_monitor_internal::__qp_init_lcd(void) {
    __qpObject.write_lock = xSemaphoreCreateMutex(); 
    assert(write_lock != NULL);

#else 
void __qp_init_lcd(void) {
#endif
    ESP_LOGI(tag, "Initializing I2C...");
    
    i2c_config_t conf = {
        .mode           = I2C_MODE_MASTER,
        .sda_io_num     = I2C_MASTER_SDA_IO,
        .scl_io_num     = I2C_MASTER_SCL_IO,
        .sda_pullup_en  = GPIO_PULLUP_ENABLE,
        .scl_pullup_en  = GPIO_PULLUP_ENABLE,
        .master.clk_speed = I2C_MASTER_FREQ_HZ,
    };

    i2c_param_config(I2C_MASTER_NUM, &conf);
    i2c_driver_install(I2C_MASTER_NUM, conf.mode, I2C_MASTER_RX_BUF_DISABLE, I2C_MASTER_TX_BUF_DISABLE, 0);
   
    write_lock = xSemaphoreCreateMutex(); 
    assert(write_lock != NULL);

    ESP_LOGI(tag, "Initializing LCD...");
    lcd_init();
    ESP_LOGI(tag, "Initialization completed!"); 
}    

typedef struct {
    char *buffer;
    size_t sz; 
    BaseType_t line; 
} MonitorBuffer_t; 

// This should not return a value 
// (Nor shoudl not return something generally)
static void _write_to_monitor_task(void *param) {
    MonitorBuffer_t *args = (MonitorBuffer_t*)param; 

    if (args == NULL) {
        ESP_LOGE(tag, "Invalid function pointer argument(line == NULL)");
        vTaskDelete(NULL); 
        return; 
    }; 

    // Everything is fine so we can 
    // use specified argument
    lcd_set_cursor(0,args->line); 
    
    // This line is not safe actually
    // Will immplement this in future version 
    //handle_cursor(lcd_set_cursor, 0, 0, QP_OVERWRITE_CURRENT_BUFFER); 

    //Write the buffer to the monitor 
    xSemaphoreTake(write_lock, portMAX_DELAY); 
    lcd_print((char *)args->buffer);  
    xSemaphoreGive(write_lock);

    // Clean ressource
    free(args);
    vTaskDelete(NULL); 
}

// USERSPACE FUNCTION 
// It's just a metaphore, actually there is no 
// userspace in this context 
#ifdef __cplusplus 
void __qapass_monitor_internal::write_to_monitor(const char *buffer, int line) {
#else

void write_to_monitor(const char* buffer, int line) {

#endif // __cplusplus 

   // Actually, the monitor cannot handle NULL value for lcd_cmd() 
    // (It will break the Core 0 inside the esp32 board)
    if (buffer == NULL || line < 0 || line > 1) {
        ESP_LOGE(tag, "Invalid arguments for write_to_monitor");
        return;
    }
    
    int sz = strlen(buffer); sz++; 
    // Allocate heap memory for task parameters
    MonitorBuffer_t *params = malloc(sizeof(MonitorBuffer_t));
    if (!params) {
        ESP_LOGE(tag, "Failed to allocate memory for monitor task");
        return;
    }
    
    params->buffer = malloc(sz*sizeof(char)); 

    memcpy(params->buffer, buffer, sz);
    // Last version was using strncpy and got use-after-free 
    // error (because of a double free call, and unhandled null terminator). 
    // Also, with strncpy with have to copy '\0' manually. 
    // params->buffer[sz] = '\0';
    
    params->sz = sz;  
    params->line = line;

    if (line & QP_OVERWRITE_SECOND_BUFFER) {
        memcpy(current_buffer_l2, params->buffer, params->sz);  
    } else {
        memcpy(current_buffer_l1, params->buffer, params->sz);  
    }

    BaseType_t res = xTaskCreatePinnedToCore(_write_to_monitor_task, 
                                            "write_to_monitor_task", 
                                            WRITE_TO_MONITOR_TASK_STK_SZ, 
                                            (void*)params, 
                                            4, 
                                            &write_ops_handle, 
                                            0); 
    if (res != pdPASS) {
        ESP_LOGE(tag, "Failed to create monitor task (heap maybe low)");  
        free(params->buffer); 
        free(params); 
    }
}

void cls_monitor(void) {
    xSemaphoreTake(write_lock, portMAX_DELAY); 
    lcd_cmd(0x01);
    vTaskDelay(pdMS_TO_TICKS(5));
    xSemaphoreGive(write_lock); 
}

void blink_text_monitor(const char *buffer, int __seconds) {
    int cnvrt = __seconds * 1000; 

    while (1) {
        cls_monitor(); 
        vTaskDelay(cnvrt / portTICK_PERIOD_MS);
        write_to_monitor(buffer, 1);
        vTaskDelay(cnvrt / portTICK_PERIOD_MS);
    }
}

// Move current text on the specified line 
// to the right 
void move_text_right(int line) {
    // Write to the seoncd line   
    int cursor=0; 
    while(cursor < 16) {
        cls_monitor(); 
        lcd_set_cursor(cursor, line); 
        if (line & QP_OVERWRITE_SECOND_BUFFER) { 
            if (current_buffer_l2[0] == '\0') {
                return; 
            }

            xSemaphoreTake(write_lock, pdMS_TO_TICKS(5)); 
            lcd_print(current_buffer_l2);
            vTaskDelay(500 / portTICK_PERIOD_MS); 
            xSemaphoreGive(write_lock);
        } else {
            if (current_buffer_l1[0] == '\0') {
                return; 
            }

            xSemaphoreTake(write_lock, pdMS_TO_TICKS(5));
            lcd_print(current_buffer_l1);
            vTaskDelay(500 / portTICK_PERIOD_MS);
            xSemaphoreGive(write_lock); 
        }
        cursor++; 
    }
}

// Move current text on the specified line 
// to the left 
void move_text_left(int line) {
    int buf_len = (line & QP_OVERWRITE_SECOND_BUFFER) ? strlen(current_buffer_l2) 
                                                        : strlen(current_buffer_l1);
    int8_t cursor=0; 
    
    while(cursor < buf_len) {
        cls_monitor();  
        lcd_set_cursor(0,line);
        if (line & QP_OVERWRITE_SECOND_BUFFER) { 
            if (current_buffer_l2[0] == '\0') {
                return; 
            }
           
            char* printable = make_substr(current_buffer_l2, cursor, buf_len); 
            
            printable[buf_len] = '\0';
            xSemaphoreTake(write_lock, pdMS_TO_TICKS(5)); 
            lcd_print(printable);
            vTaskDelay(500 / portTICK_PERIOD_MS); 
            xSemaphoreGive(write_lock);
        } else {
            if (current_buffer_l1[0] == '\0') {
                return; 
            }

            char* printable = make_substr(current_buffer_l1, cursor, buf_len); 
            
            printable[buf_len] = '\0';
            xSemaphoreTake(write_lock, pdMS_TO_TICKS(5));
            lcd_print(printable);
            vTaskDelay(500 / portTICK_PERIOD_MS);
            xSemaphoreGive(write_lock); 
        }
        cursor++; 
    }
}

