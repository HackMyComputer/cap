#ifndef __QAPASS_LED 
#define __QAPASS_LED

#define I2C_MASTER_SCL_IO           22	 	// SCL 
#define I2C_MASTER_SDA_IO           21		// SDA
#define I2C_MASTER_NUM              I2C_NUM_0
#define I2C_MASTER_FREQ_HZ          10000
#define I2C_MASTER_TX_BUF_DISABLE   0
#define I2C_MASTER_RX_BUF_DISABLE   0

#define LCD_ADDR                    0x27  // Common for Qapass I2C LCDs
#define LCD_BACKLIGHT               0x08
#define LCD_ENABLE_BIT              0x04

#define MAX_BUF_ALLOWED             0xff
#define WRITE_TO_MONITOR_TASK_STK_SZ    0x1000

extern const char *tag; 

/**
 * @brief Writes a text buffer to the LCD monitor on a specified line.
 *
 * This function displays a string on the LCD screen, positioning it according to
 * the specified line index. It handles synchronization and ensures proper cursor
 * positioning for concurrent writes.
 *
 * @param buffer  Pointer to the null-terminated text string to display.
 * @param line    Target LCD line index (e.g., 0 for first line, 1 for second).
 */
void write_to_monitor(const char* buffer, int line); 


/**
 * @brief Initializes the LCD monitor.
 *
 * Sets up the LCD hardware, configures communication interfaces, and clears
 * the display. Must be called before any other LCD-related functions.
 */
void __qp_init_lcd(void); 


/**
 * @brief Clears the LCD monitor display.
 *
 * Erases all visible text from the LCD and resets the cursor to the default 
 * position (line 0, column 0).
 */
void cls_monitor(void); 


/**
 * @brief Displays blinking text on the LCD monitor.
 *
 * Continuously blinks a text message for a specified duration, useful for
 * drawing attention to status or alert messages.
 *
 * @param buffer     Pointer to the null-terminated text to blink.
 * @param __seconds  Duration of the blinking effect, in seconds.
 */
void blink_text_monitor(const char *buffer, int __seconds);


/**
 * @brief Animates text movement to the right on a specific LCD line.
 *
 * Shifts the displayed text horizontally to the right across the screen,
 * one character at a time, creating a scrolling effect.
 *
 * @param line  Target LCD line index for the animation.
 */
void move_text_right(int line);


/**
 * @brief Animates text movement to the left on a specific LCD line.
 *
 * Shifts the displayed text horizontally to the left across the screen,
 * one character at a time, creating a scrolling effect.
 *
 * @param line  Target LCD line index for the animation.
 */
void move_text_left(int line);


#ifndef __LCD_WRITE_FLAG 
# define __LCD_WRITE_FLAG 
/* Write flags to handle overwriting issue */
typedef enum 
{
    QP_OVERWRITE_CURRENT_BUFFER     = 0x63,
    QP_OVERWRITE_FIRST_BUFFER       = 0x64, 
    QP_OVERWRITE_SECOND_BUFFER      = 0x65,
} qp_monitor_flags_t; 
#endif // __LCD_WRITE_FLAG 

#if defined __GNUC__ && defined EXCEPTIONS 
#ifdef __cpluscplus 
class __qapass_monitor_internal {
    public: 
        __qapass_monitor_internal(int default_write_addr); 
        ~__qapass_monitor_internal() = default; 

        [inline] 
        void write_to_monitor(const char* buffer, int line);  

        __qp_init_lcd(void); 

    private: 
        SemaphoreHandle_t write_lock; 
        std::string current_buffer; 
        std::size_t buf_len; 
        int width; 
        int height; 
};
#endif // __cpluscplus
#endif // __GNUC__
#endif // __QAPASS_LED
