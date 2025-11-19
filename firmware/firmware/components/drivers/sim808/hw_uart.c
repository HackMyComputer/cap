#include <string.h>
#include <stdio.h>
#include "driver/uart.h"
#include "driver/gpio.h"
#include "freertos/task.h" 

#define TX_PIN 		15 
#define RX_PIN 		18 
#define PWRKEY_PIN 	4
#define UART_PORT 	UART_NUM_1

void __init_uart() 
{
    uart_config_t uart_config = {
	.baud_rate = 115200, 
	.data_bits = UART_DATA_8_BITS, 
	.parity = UART_PARITY_DISABLE, 
	.stop_bits = UART_STOP_BITS_1, 
	.flow_ctrl = UART_HW_FLOWCTRL_DISABLE,
    };

    uart_param_config(UART_PORT, &uart_config);
    uart_set_pin(UART_PORT, TX_PIN, RX_PIN, UART_PIN_NO_CHANGE, UART_PIN_NO_CHANGE);
    uart_driver_install(UART_PORT, 1024, 0, 0, NULL, 0);
}

void power_on_sim808() 
{
    gpio_set_direction(PWRKEY_PIN, GPIO_MODE_OUTPUT);
    gpio_set_level(PWRKEY_PIN, 0);
    vTaskDelay(1000 / portTICK_PERIOD_MS);
    gpio_set_level(PWRKEY_PIN, 1);
    vTaskDelay(5000 / portTICK_PERIOD_MS);
}

void app2_main(void)
{

    __init_uart(); 
    power_on_sim808(); 

    char *test_cmd = "AT\r\n"; 
    uart_write_bytes(UART_PORT, test_cmd, strlen(test_cmd)); 

    while (true) {
	char data[128]; 

	int len = uart_read_bytes(UART_PORT, data, sizeof(data), 100 / portTICK_PERIOD_MS);
	if (len > 0) {
	   data[len] = '\0'; 
	   printf("Response: %s\n", data);
	}
	vTaskDelay(1000/portTICK_PERIOD_MS);
    }
}
