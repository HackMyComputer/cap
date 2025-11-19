idf_component_register(SRCS "bt_flac.c" "foxen-flac.c" "flac_streamer.c"
    INCLUDE_DIRS "include"

    REQUIRES 
    freertos
    a2dp
    esp_system
    esp_timer
    esp_http_client
    esp_wifi
    fatfs
    spiffs
    sdmmc
    driver
)
