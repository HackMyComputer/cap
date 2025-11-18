idf_component_register(
    SRCS "app_main.c"
    INCLUDE_DIRS "include"
    PRIV_REQUIRES a2dp https rtp sdmmc spi lm35 sim808 web_gpio qapass format flac_decoder
)
