idf_component_register(
    SRCS "https.c"
    INCLUDE_DIRS "include"
    REQUIRES
        freertos
        esp_system
        esp_wifi
        esp_event
        nvs_flash
        esp_netif
        esp_eth
	esp-tls
        esp_http_server
        lwip
        driver
)
