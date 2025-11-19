#include "driver/spi_common.h"
#include "driver/sdspi_host.h"
#include "sdmmc_cmd.h"
#include "esp_vfs_fat.h"

#include <dirent.h> 

#include "spi.h"

const char mount_point[] = MNT_NODE;

void 
init_spi(sdmmc_card_t *__card)
{
    esp_err_t ret;
    bool is_null = (__card == NULL); 
    bool not_configured = (sizeof(*__card) != sizeof(sdmmc_card_t));


    if (is_null || not_configured) {
        ESP_LOGI(TAG, "Proceding to Sdcard initialization..."); 
        vTaskDelay(1000 / portTICK_PERIOD_MS); 
    } else {
        goto state_ok; 
    }

    // Use settings defined above to initialize SD card and mount FAT filesystem.
    ESP_LOGI(TAG, "Using SPI peripheral");
    vTaskDelay(1000 / portTICK_PERIOD_MS);

    __esp_log("Using SPI peripheral");
    sdmmc_host_t host = SDSPI_HOST_DEFAULT(); 
    
    host.slot = SPI2_HOST;
    host.max_freq_khz = 1000;

    spi_bus_config_t bus_cfg = {
        .mosi_io_num = PIN_NUM_MOSI,
        .miso_io_num = PIN_NUM_MISO,
        .sclk_io_num = PIN_NUM_CLK,
        .quadwp_io_num = -1,
        .quadhd_io_num = -1,
        .max_transfer_sz = 4000,
    };

    ESP_ERROR_CHECK(spi_bus_initialize(host.slot, &bus_cfg, SDSPI_DEFAULT_DMA));

    // This initializes the slot without card detect (CD) and write protect (WP) signals.
    // Modify slot_config.gpio_cd and slot_config.gpio_wp if your board has these signals.
    sdspi_device_config_t slot_config = SDSPI_DEVICE_CONFIG_DEFAULT();
    slot_config.gpio_cs = PIN_NUM_CS;
    slot_config.host_id = host.slot;

    // Options for mounting the filesystem.
    // If format_if_mount_failed is set to true, SD card will be partitioned and
    // formatted in case when mounting fails.
    esp_vfs_fat_sdmmc_mount_config_t mount_config = {
        .format_if_mount_failed = false,
        .max_files = 5,
        .allocation_unit_size = 16 * 1024
    };

state_ok: 
    ESP_LOGI(TAG, "Mounting filesystem\n");
    vTaskDelay(1000 / portTICK_PERIOD_MS);

    ret = esp_vfs_fat_sdspi_mount(mount_point, &host, &slot_config, &mount_config, &__card);

    if (ret != ESP_OK) {
        if (ret == ESP_FAIL) {
            ESP_LOGE(TAG, "Failed to mount filesystem. ");
        } else {
            ESP_LOGE(TAG, "Failed to initialize the card (%s). "
                     "Make sure SD card lines have pull-up resistors in place.", esp_err_to_name(ret));
        }
        return;
    }

    ESP_LOGI(TAG, "Filesystem mounted\n");    
    ESP_LOGI(TAG, "Displaying sdcard information...\n");

    sdmmc_card_print_info(stdout, __card);
}

void
deinit_spi(sdmmc_card_t* card) 
{
    // Do something like unmounting the filesystem
}

// Public function 
void ls_dir(const char* path) {
    ESP_LOGI(TAG, "Listing files in: %s", path);

    DIR *dir = opendir(path);
    if (!dir) {
        ESP_LOGE(TAG, "Failed to open directory: %s", path);
        return;
    }

    struct dirent *entry;
    while ((entry = readdir(dir)) != NULL) {
        ESP_LOGI(TAG, "File: %s", entry->d_name);
    }

    closedir(dir);
}

