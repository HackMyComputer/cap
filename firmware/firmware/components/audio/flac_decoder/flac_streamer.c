/* flac_streamer.c
   Requires: foxen-flac.h, esp_log.h, freertos/task.h, freertos/semphr.h, stdio.h
*/

#include <stdio.h>
#include <string.h>
#include <stdint.h>
#include <stdbool.h>
#include "freertos/FreeRTOS.h"
#include "freertos/task.h"
#include "freertos/semphr.h"
#include "esp_log.h"
#include "foxen-flac.h"
#include "esp_a2dp_api.h"   // for A2DP callback signature

static const char *TAG = "FLAC_STREAMER";

#define IN_BUF_SZ       2048    // bytes read from file per loop
#define OUT_SAMPLES_MAX 1024    // max int32 samples output from decoder per call
#define RING_BYTES      (8 * 1024) // ring buffer size in bytes (choose per RAM budget)
#define DECODER_TASK_STACK  12288
#define DECODER_TASK_PRIO   5

/* ring buffer state (byte-oriented) */
static uint8_t *ring = NULL;
static size_t ring_size = 0;
static size_t ring_head = 0; // write index
static size_t ring_tail = 0; // read index
static size_t ring_count = 0; // bytes stored
static SemaphoreHandle_t ring_mutex = NULL;
static SemaphoreHandle_t ring_data_sem = NULL; // count of bytes available (used optionally)

/* decoder task handle & control */
static TaskHandle_t decoder_task_handle = NULL;
static volatile bool decoder_running = false;
static char decoder_path[128] = {0};

static esp_err_t ring_init(size_t bytes)
{
    if (ring) 
        return ESP_OK;
    ring = (uint8_t*)malloc(bytes);
    if (!ring) 
        return ESP_ERR_NO_MEM;
    ring_size = bytes;
    ring_head = ring_tail = ring_count = 0;
    ring_mutex = xSemaphoreCreateMutex();
    ring_data_sem = xSemaphoreCreateBinary();
    return ESP_OK;
}

static void ring_free(void)
{
    if (ring) {
        free(ring);
        ring = NULL;
    }
    if (ring_mutex) { 
        vSemaphoreDelete(ring_mutex); 
        ring_mutex = NULL; 
    }
    if (ring_data_sem) { 
        vSemaphoreDelete(ring_data_sem); 
        ring_data_sem = NULL; 
    }

    ring_size = ring_head = ring_tail = ring_count = 0;
}

/* write up to len bytes into ring; returns number of bytes actually written */
static size_t ring_write_bytes(const uint8_t *buf, size_t len)
{
    if (!ring || !ring_mutex) 
        return 0;
    xSemaphoreTake(ring_mutex, portMAX_DELAY);
    size_t free_space = ring_size - ring_count;
    size_t to_write = (len > free_space) ? free_space : len;
    // two-phase copy
    size_t first = (ring_head + to_write <= ring_size) ? to_write : (ring_size - ring_head);
    if (first > 0) 
        memcpy(ring + ring_head, buf, first);
    if (to_write > first) 
        memcpy(ring, buf + first, to_write - first);
    
    ring_head = (ring_head + to_write) % ring_size;
    ring_count += to_write;
    xSemaphoreGive(ring_mutex);
    if (to_write > 0 && ring_data_sem) 
        xSemaphoreGive(ring_data_sem); // signal availability
    
    assert(ring_head < ring_size);
    assert(ring_tail < ring_size);
    assert(ring_count <= ring_size);
    return to_write;
}

/* read up to len bytes from ring into buf; returns number of bytes read */
static size_t ring_read_bytes(uint8_t *buf, size_t len)
{
    if (!ring || !ring_mutex) 
        return 0;
    
    xSemaphoreTake(ring_mutex, portMAX_DELAY);
    size_t to_read = (len > ring_count) ? ring_count : len;
    size_t first = (ring_tail + to_read <= ring_size) ? to_read : (ring_size - ring_tail);
    if (first > 0) 
        memcpy(buf, ring + ring_tail, first);
    if (to_read > first) 
        memcpy(buf + first, ring, to_read - first);
    ring_tail = (ring_tail + to_read) % ring_size;
    ring_count -= to_read;
    xSemaphoreGive(ring_mutex);

    assert(ring_head < ring_size);
    assert(ring_tail < ring_size);
    assert(ring_count <= ring_size);
    return to_read;
}

/* check free bytes in ring (not locking for speed; call under mutex if needed) */
static size_t ring_free_space()
{
    return ring_size - ring_count;
}

static void decoder_task(void *pv)
{
    const char *path = decoder_path;
    ESP_LOGI(TAG, "Decoder task started for: %s", path);

    /* prepare buffers and decoder */
    uint8_t in_buf[IN_BUF_SZ];
    int32_t out_buf[OUT_SAMPLES_MAX];
    uint32_t in_wr = 0;
    bool done_reading = false;
    fx_flac_t *flac = FX_FLAC_ALLOC_DEFAULT();
    if (!flac) {
        ESP_LOGE(TAG, "Failed to allocate FLAC decoder");
        decoder_running = false;
        vTaskDelete(NULL);
        return;
    }

    FILE *f = fopen(path, "rb");
    if (!f) {
        ESP_LOGE(TAG, "Failed to open FLAC file: %s", path);
        free(flac);
        decoder_running = false;
        vTaskDelete(NULL);
        return;
    }

    while (decoder_running) {
        /* fill input buffer if space */
        size_t to_read = IN_BUF_SZ - in_wr;
        size_t n_read = 0;
        if (!done_reading && to_read > 0) {
            n_read = fread(in_buf + in_wr, 1, to_read, f);
            if (n_read == 0) done_reading = true;
            in_wr += n_read;
        }

        uint32_t in_len = in_wr;
        uint32_t out_len = OUT_SAMPLES_MAX;

        int res = fx_flac_process(flac, in_buf, &in_len, out_buf, &out_len);
        if (res == FLAC_ERR) {
            ESP_LOGE(TAG, "FLAC decode error");
            break;
        }

        /* shift leftover input bytes to buffer start */
        if (in_wr > in_len) {
            memmove(in_buf, in_buf + in_len, in_wr - in_len);
            in_wr -= in_len;
        } else {
            in_wr = 0;
        }

        if (out_len > 0) {
            /* convert int32 samples to int16 little-endian bytes */
            // out_len = number of samples (e.g., stereo interleaved)
            // create a temporary byte buffer
            size_t bytes_needed = out_len * sizeof(int16_t);
            uint8_t tmp_buf[OUT_SAMPLES_MAX * 2];
            for (uint32_t i = 0; i < out_len; ++i) {
                int32_t s32 = out_buf[i];
                int16_t s16 = (int16_t)(s32 >> 16); // proper conversion may use clipping
                tmp_buf[(i<<1)]     = (uint8_t)(s16 & 0xff);
                tmp_buf[(i<<1) + 1] = (uint8_t)((s16 >> 8) & 0xff);
            }

            /* write into ring buffer (may be partially written if ring nearly full) */
            size_t written = 0;
            const uint8_t *p = tmp_buf;
            size_t remaining = bytes_needed;
            while (remaining > 0 && decoder_running) {
                size_t w = ring_write_bytes(p + written, remaining);
                if (w == 0) {
                    // ring full, yield and wait a bit for consumer to drain
                    vTaskDelay(10 / portTICK_PERIOD_MS);
                    continue;
                }
                written += w;
                remaining -= w;
            }
        }

        /* termination condition */
        if (done_reading && out_len == 0 && n_read == 0) {
            ESP_LOGI(TAG, "Decoder reached EOF");
            break;
        }

        /* small yield to be cooperative */
        taskYIELD();
    }

    fclose(f);
    free(flac);
    decoder_running = false;
    ESP_LOGI(TAG, "Decoder task exiting");
    vTaskDelete(NULL);
}


/* call this when you want to stream a given FLAC file (e.g. when A2DP media is started) */
bool decoder_start(const char *flac_path)
{
    if (decoder_running) return false;
    if (ring_init(RING_BYTES) != ESP_OK) {
        ESP_LOGE(TAG, "Failed to init ring");
        return false;
    }

    strncpy(decoder_path, flac_path, sizeof(decoder_path)-1);
    decoder_running = true;
    BaseType_t r = xTaskCreate(decoder_task, "flac_decoder", DECODER_TASK_STACK, NULL, DECODER_TASK_PRIO, &decoder_task_handle);
    if (r != pdPASS) {
        ESP_LOGE(TAG, "Failed to create decoder task");
        decoder_running = false;
        ring_free();
        return false;
    }
    return true;
}

/* call this when you want to stop (e.g. media stop or disconnect) */
void decoder_stop(void)
{
    if (!decoder_running) return;
    decoder_running = false;
    // wait for task to exit
    int wait = 0;
    while (decoder_task_handle && wait++ < 50) {
        vTaskDelay(50 / portTICK_PERIOD_MS);
    }
    if (decoder_task_handle) {
        vTaskDelete(decoder_task_handle);
        decoder_task_handle = NULL;
    }
    ring_free();
}

/* ==== A2DP data callback: consume from ring ==== */
/* Integrate by replacing your bt_app_a2d_data_cb with this one or call this from it. */
int32_t flac_a2d_data_cb(uint8_t *data, int32_t len)
{
    if (!data || len <= 0) return 0;

    // try to read requested bytes from ring
    size_t bytes_read = ring_read_bytes(data, (size_t)len);

    if (bytes_read < (size_t)len) {
        // not enough data; fill remainder with silence (zeros)
        memset(data + bytes_read, 0, (size_t)len - bytes_read);
    }

    return len; // always return len to indicate buffer filled (real bytes may be zeros)
}

