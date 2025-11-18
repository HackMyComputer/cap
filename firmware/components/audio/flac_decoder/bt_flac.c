#include <stdio.h>
#include <stdint.h>
#include <stdbool.h>

#include "esp_log.h"
#include "esp_a2dp_api.h"
#include "foxen-flac.h"

#define IN_BUF_SIZE 1024
#define OUT_BUF_SIZE 512

// This function streams FLAC from SD card to Bluetooth A2DP
int decode_flac_bt(const char *filename)
{
    uint8_t in_buf[IN_BUF_SIZE];
    int32_t out_buf[OUT_BUF_SIZE];
    uint32_t in_buf_wr_cur = 0;
    bool done_reading = false;

    // Allocate FLAC decoder
    fx_flac_t *flac = FX_FLAC_ALLOC_DEFAULT();
    if (!flac) {
        ESP_LOGE("FLAC", "Failed to allocate decoder");
        return -1;
    }

    // Open file on SD card
    // So it supposes that the filename is 
    // prefixed by /sdcard
    FILE *f = fopen(filename, "rb");
    if (!f) {
        ESP_LOGE("FLAC", "Error opening file %s", filename);
        free(flac);
        return -1;
    }

    while (true) {
        // Fill input buffer
        size_t to_read = IN_BUF_SIZE - in_buf_wr_cur;
        size_t n_read = 0;
        if (!done_reading && to_read > 0) {
            n_read = fread(in_buf + in_buf_wr_cur, 1, to_read, f);
            if (n_read == 0) done_reading = true;
            in_buf_wr_cur += n_read;
        }

        uint32_t in_buf_len = in_buf_wr_cur;
        uint32_t out_buf_len = OUT_BUF_SIZE;

        int res = fx_flac_process(flac, in_buf, &in_buf_len, out_buf, &out_buf_len);
        if (res == FLAC_ERR) {
            ESP_LOGE("FLAC", "Decoder error");
            break;
        }

        // Shift leftover bytes to start of buffer
        for (uint32_t i = 0; i < in_buf_wr_cur - in_buf_len; i++)
            in_buf[i] = in_buf[i + in_buf_len];
        in_buf_wr_cur -= in_buf_len;

        // Send PCM to Bluetooth (16-bit)
        for (uint32_t i = 0; i < out_buf_len; i++) {
            int16_t sample16 = out_buf[i] >> 16;
            //esp_a2d_source_write_pcm_data((uint8_t*)&sample16, sizeof(int16_t));
        }

        if (done_reading && out_buf_len == 0 && n_read == 0) break;
    }

    fclose(f);
    free(flac);
    return 0;
}

