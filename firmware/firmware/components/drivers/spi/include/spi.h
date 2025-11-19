#ifndef __HAVE_SPI_H 
#define __HAVE_SPI_H 

#ifdef __AVR__ 
# error ARDUINO_CODE_NOT_COMPATIBLE 
#endif 

#include <stdio.h>
#include <sys/unistd.h>
#include <sys/stat.h>

#include "freertos/FreeRTOS.h"
#include "freertos/task.h"
#include "esp_log.h"
#include "esp_err.h"
#include "driver/sdspi_host.h"
#include "driver/spi_common.h"

#define MNT_NODE "/sdcard"
#define TAG "SDCard_SPI"


/*
 * By default, the SPI card transmission uses 8 bytes width. 
 * For an ESP32-WROOM-32D (and any other related brand model), 
 * the default pin configuration for wiring an SPI card module 
 * can be choosen freely. 
 * */

#define PIN_NUM_MISO 19		// Master In Slave Out pin
#define PIN_NUM_MOSI 23		// Master Out Slave In pin
#define PIN_NUM_CLK  18		// Serial Clock	pin 
#define PIN_NUM_CS    5		// Card Select (specific for the chip used)
				

//dummy function 
__always_inline esp_err_t __esp_log(const char* log) 
{
    printf("%s:%s", TAG, log);
    vTaskDelay(1000 / portTICK_PERIOD_MS);
    return ESP_OK;
}


void init_spi(sdmmc_card_t *__card);

void deinit_spi(sdmmc_card_t* card);
 
void ls_dir(const char* path); 

/*#define TEST1(x)	\
    do {		\
	volatile sdmmc_card_t *card = x;	\
	const char *fh = MNT_NODE"/hello.txt";	\
							\
	ESP_LOGI(TAG, "Opening file %s", fh);	\
	FILE *f = fopen(fh, "w");		\
	if (f == NULL) {				\
	    ESP_LOGE(TAG, "Failed to open file for writing");	\
	    return;						\
	}							\
								\
	fprintf(f, "Hello %s!\n", card->cid.name);		\
	fclose(f);						\
	ESP_LOGI(TAG, "File written");				\
	const char *ff=  MNT_NODE"/foo.txt";  		\
								\
	struct stat st;						\
	if (stat(ff, &st) == 0) {				\
	    unlink(ff);					\
	}							\
								\
	ESP_LOGI(TAG, "Renaming file %s to %s", fh, ff);		\
	if (rename(fh, ff) != 0) {				\
	    ESP_LOGE(TAG, "Rename failed");					\
	    return;								\
										\
	ESP_LOGI(TAG, "Reading file %s", ff);				\
	f = fopen(ff, "r");						\
	if (f == NULL) {							\
	    ESP_LOGE(TAG, "Failed to open file for reading");			\
	    return;								\
	}									\
										\
	char line[64];								\
	fgets(line, sizeof(line), f);						\
	fclose(f);								\
										\
	char *pos = strchr(line, '\n');						\
	if (pos) {								\
	    *pos = '\0';							\
	}									\
										\
	ESP_LOGI(TAG, "Read from file: '%s'", line);				\
    } while (0)	
*/

#endif // __HAVE_SPI_H
