#include <stdio.h>

#include "bt_app_core.h"
#include "qp.h"
#include "spi.h"
#include "foxen-flac.h"
#include "bt_flac.h"

sdmmc_card_t *card = NULL;

static void __initializer(void) {
  // Should be changed to a function which
  // returns esp_err_t value in next update
  init_spi(card);

#ifdef __cplusplus
  __BEGIN_OBJ_DEF_XS_CC
  __qp_monitor_internal __qpObject = __qp_monitor_internal();
  }
  __qpObject.writeToMonitor = write_to_monitor;
  __qpObject.qpObjectInitLCD = __qp_monitor_init;
  __END_OBJ_DEF_XS_CC
#else
  __qp_init_lcd(); // Init the screen task
#endif // __cplusplus
}

void decode_flac_test(const char *filepath)
{
    uint8_t in_buf[4096];
    int32_t out_buf[4096];
    uint32_t in_buf_wr_cur = 0;
    bool done_reading = false;

    fx_flac_t *flac = FX_FLAC_ALLOC_DEFAULT();
    if (!flac) {
        ESP_LOGE(TAG, "Failed to allocate FLAC decoder");
        return;
    }

    FILE *f = fopen(filepath, "rb");
    if (!f) {
        ESP_LOGE(TAG, "Failed to open %s", filepath);
        free(flac);
        return;
    }

    ESP_LOGI(TAG, "Decoding %s ...", filepath);

    size_t total_samples = 0;

    while (1) {
        size_t to_read = sizeof(in_buf) - in_buf_wr_cur;
        size_t n_read = 0;

        if (!done_reading && to_read > 0) {
            n_read = fread(in_buf + in_buf_wr_cur, 1, to_read, f);
            if (n_read == 0)
                done_reading = true;
            in_buf_wr_cur += n_read;
        }

        uint32_t in_buf_len = in_buf_wr_cur;
        uint32_t out_buf_len = sizeof(out_buf) / sizeof(out_buf[0]);

        int res = fx_flac_process(flac, in_buf, &in_buf_len, out_buf, &out_buf_len);
        if (res == FLAC_ERR) {
            ESP_LOGE(TAG, "Decoder error");
            break;
        }

        // Shift unread input bytes to beginning
        for (uint32_t i = 0; i < in_buf_wr_cur - in_buf_len; ++i)
            in_buf[i] = in_buf[i + in_buf_len];
        in_buf_wr_cur -= in_buf_len;

        if (out_buf_len > 0) {
            total_samples += out_buf_len;
            // In the future: send PCM to Bluetooth here
        }

        if (done_reading && out_buf_len == 0 && n_read == 0)
            break;
    }

    ESP_LOGI(TAG, "Decoded %zu samples total", total_samples);

    fclose(f);
    free(flac);
}

void app_main(void) {
    __initializer();

    ls_dir("/sdcard/MUSIC");
    //decode_flac_test("/sdcard/MUSIC/song.flac");
    
    ESP_LOGI("FLAC_DEC", "stack high-water mark: %u words", uxTaskGetStackHighWaterMark(NULL));
    stream_a2dp_to_device(); 
}
