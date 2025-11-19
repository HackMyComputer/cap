/* 
 * This file is part of the Cap Firmware distribution 
 * (https://github.com/VahatraSylvio/cap or http://cap.github.io).
 * 
 * Copyright (c) 2025 Sergio Randriamihoatra.
 * 
 * This program is free software: you can redistribute it and/or modify  
 * it under the terms of the GNU General Public License as published by  
 * the Free Software Foundation, version 3.
 *
 * This program is distributed in the hope that it will be useful, but 
 * WITHOUT ANY WARRANTY; without even the implied warranty of 
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE. See the GNU 
 * General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License 
 * along with this program. If not, see <http://www.gnu.org/licenses/>.
 */

#ifndef __LM35_H__
#define __LM35_H__

/**
 * @file lm35.h
 * @brief Calibrated, thread-safe LM35 driver using esp_adc (adc_oneshot + adc_cali).
 *
 * NOTES:
 *  - `channel` refers to an ADC channel (adc_channel_t) for ADC unit 1 (or as configured).
 *  - This header expects ESP-IDF v5.x+ where `esp_adc/adc_oneshot.h` and `esp_adc/adc_cali.h`
 *    are available.
 *
 *  Configure logging by defining LM35_DEBUG (e.g. in component CMakeLists or sdkconfig).
 */

#include <stdint.h>
#include <stdbool.h>
#include <stddef.h>

#include "freertos/FreeRTOS.h"
#include "freertos/semphr.h"
#include "esp_err.h"

#include "esp_adc/adc_oneshot.h"
#include "esp_adc/adc_cali.h"
#include "esp_adc/adc_cali_scheme.h"

#ifdef __cplusplus
extern "C" {
#endif


/* Define LM35_DEBUG (e.g. in build flags) to enable logging */
#ifdef LM35_DEBUG
  #include "esp_log.h"
  #define LM35_LOG_I(...) ESP_LOGI("LM35", __VA_ARGS__)
  #define LM35_LOG_W(...) ESP_LOGW("LM35", __VA_ARGS__)
  #define LM35_LOG_E(...) ESP_LOGE("LM35", __VA_ARGS__)
#else
  #define LM35_LOG_I(...) (void)0
  #define LM35_LOG_W(...) (void)0
  #define LM35_LOG_E(...) (void)0
#endif

/* Default sample count for averaging (can be changed at runtime per-instance). */
#ifndef LM35_DEFAULT_SAMPLE_COUNT
#define LM35_DEFAULT_SAMPLE_COUNT  16u
#endif


/* Default ADC bitwidth used with adc_oneshot configuration */
#ifndef LM35_DEFAULT_BITWIDTH
#define LM35_DEFAULT_BITWIDTH ADC_BITWIDTH_12
#endif


/* Default Vref for boards without measured Vref; prefer measuring real board Vref */
#ifndef LM35_DEFAULT_VREF_MV
#define LM35_DEFAULT_VREF_MV 1100u
#endif


/**
 * LM35_t
 *
 * Opaque-ish data structure representing one LM35 sensor instance.
 *
 * Fields are public for convenience but should be treated as driver-managed;
 * use the API functions to operate on the structure.
 */
typedef struct {
    adc_oneshot_unit_handle_t adc_unit;	/* ADC oneshot unit handle (created in init) */
    adc_cali_handle_t adc_cali; 	/* ADC calibration handle (may be NULL if calibration unavailable) */
    adc_channel_t channel;		/* ADC channel used for reading (adc_channel_t) */
    adc_atten_t atten;			/* ADC attenuation used for this channel */
    float calibration_offset_c;		/* Optional runtime offset (degrees C) to compensate for systematic error */
    SemaphoreHandle_t lock;		/* Mutex protecting this instance state (created in init) */
    uint8_t sample_count;		/* Averaging and sampling configuration (default LM35_DEFAULT_SAMPLE_COUNT) */
    uint32_t last_raw_value;		/* Last averaged raw ADC value (12-bit by default) */
    uint16_t last_checksum;		/* Checksum over last raw sample buffer (simple XOR checksum) */

    float last_temperature_c;		/* Last computed temperature (°C) */

    /* Sanity validation thresholds (°C) */
    float min_valid_temp_c;   /* e.g. -55.0 */
    float max_valid_temp_c;   /* e.g. 150.0 */

    uint32_t consecutive_failures;
    uint32_t total_failures;
    uint32_t error_count; 

    /* User-visible flag: true when initialized successfully */
    bool is_initialized;
    adc_bitwidth_t bitwidth;	/* ADC bitwidth here if you support non-12-bit */
} LM35_t;


/**
 * lm35_init
 *
 * Initialize an LM35 instance:
 *  - configures adc_oneshot unit & channel
 *  - creates calibration (adc_cali) if available
 *  - creates the mutex and initializes state fields
 *
 * Parameters:
 *  - lm: pointer to user-allocated LM35_t (must not be NULL)
 *  - unit_id: ADC unit ID to use (ADC_UNIT_1 recommended for LM35 on ADC1 channels)
 *  - channel: adc_channel_t (ADC1 channel for the sensor)
 *  - atten: adc_atten_t attenuation for the channel
 *  - default_vref_mv: approximate board Vref in mV (LM35_DEFAULT_VREF_MV recommended if unknown)
 *
 * Returns:
 *  - ESP_OK on success
 *  - ESP_ERR_INVALID_ARG for bad parameters
 *  - esp_err_t on other failures (e.g. memory/config errors)
 */
esp_err_t lm35_init(LM35_t *lm,
                    adc_unit_t unit_id,
                    adc_channel_t channel,
                    adc_atten_t atten,
                    uint32_t default_vref_mv);


/**
 * lm35_deinit
 *
 * Deinitialize an LM35 instance:
 *  - deletes calibration handle if present
 *  - deletes adc_oneshot unit handle
 *  - deletes mutex and clears state
 *
 * Safe to call even if instance not fully initialized.
 */
void lm35_deinit(LM35_t *lm);


/**
 * lm35_read
 *
 * Read temperature from LM35 with averaging, calibration conversion and validation.
 * This function takes the internal mutex and returns after updating:
 *  - lm->last_raw_value
 *  - lm->last_checksum
 *  - lm->last_temperature_c
 *
 * Parameters:
 *  - lm: pointer to initialized LM35_t
 *  - out_temperature_c: optional pointer to float; receives temperature in °C
 *
 * Returns:
 *  - ESP_OK on success (valid reading)
 *  - ESP_ERR_INVALID_STATE if not initialized
 *  - ESP_ERR_INVALID_ARG for bad arguments
 *  - ESP_FAIL for measured invalid/suspect reading (counters incremented)
 */
esp_err_t lm35_read(LM35_t *lm, float *out_temperature_c);


/**
 * lm35_validate_samples
 *
 * Validate a raw sample buffer against the instance's last checksum.
 * Useful when transferring raw samples: recompute locally and compare.
 */
bool lm35_validate_samples(LM35_t *lm, const uint16_t *samples, size_t n);


/**
 * lm35_get_error_counters
 *
 * Thread-safe read of error counters.
 */
void lm35_get_error_counters(LM35_t *lm, uint32_t *consecutive, uint32_t *total);


/**
 * lm35_set_sample_count - change number of samples used during lm35_read()
 * (thread-safe).
 */
esp_err_t lm35_set_sample_count(LM35_t *lm, uint8_t sample_count);


/**
 * lm35_set_valid_range - change min/max valid temperature (thread-safe).
 */
esp_err_t lm35_set_valid_range(LM35_t *lm, float min_c, float max_c);


#ifdef __cplusplus
}
#endif

#endif /* __LM35_H__ */

