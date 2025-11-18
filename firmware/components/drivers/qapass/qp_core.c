#include <stdint.h> 
#include <string.h> 
 
#include "freertos/FreeRTOSConfig.h"
#include "freertos/FreeRTOS.h"
#include "freertos/queue.h"
#include "freertos/task.h"
#include "esp_log.h"

#include "qp.h"

static TaskHandle_t write_op_task_handle = NULL; 
static QueueHandle_t write_op_task_queue = NULL; 

void write_op_task_start_up(void) {
    write_op_task_queue = xQueueCreate(10, 10);
    if (write_op_task_queue == NULL) {
        ESP_LOGE(tag, "Failed to allocate queue (%s)", __func__); 
        return; 
    }

    /*xTaskCreate(__write_to_monitor_task_handler, 
                "Wops", 
                WRITE_TO_MONITOR_TASK_STK_SZ, 
                (void*)params, 
                4, 
                &write_op_task_handle);*/
    return; 
}

void write_op_task_shut_down() {
    if (write_op_task_handle) {
        vTaskDelete(write_op_task_handle); 
        write_op_task_handle = NULL; 
    } 

    if (write_op_task_queue) {
        vQueueDelete(write_op_task_queue); 
        write_op_task_queue = NULL; 
    }
}
