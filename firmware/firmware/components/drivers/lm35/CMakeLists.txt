idf_component_register(
    SRCS "lm35.c"
    INCLUDE_DIRS "include"
    REQUIRES 
	esp_driver_gpio
	esp_driver_uart
	freertos
	driver
	esp_adc
)

