idf_component_register(
    SRCS "webserver_gpio.c"
    INCLUDE_DIRS "include"
    REQUIRES
	freertos
        esp_system
        esp_wifi
        esp_event
        nvs_flash
        lwip	
)
