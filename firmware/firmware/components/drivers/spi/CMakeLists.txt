idf_component_register(
    SRCS "sdspi_driver.c"
    INCLUDE_DIRS "include"
    REQUIRES 
	driver
	fatfs
	esp_driver_sdmmc
	esp_driver_sdspi
)
