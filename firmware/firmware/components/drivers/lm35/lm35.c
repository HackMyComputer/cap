/**
 * lm35_sensor.c
 *
 * Calibrated LM35 driver with mutex, error counters and logging control.
 *
 * Notes:
 *  - Intended for ESP-IDF (uses esp_adc_cal)
 *  - Ensure LM35 VCC is stable and Vref is measured/calibrated on your board.
 */

#include "lm35.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "esp_log.h"
#include "esp_check.h"

#define TAG "LM35_DRIVER"

/* Internal constants */
#define LM35_MV_PER_DEGREE_C     10.0f
#define LM35_MAX_CONSECUTIVE_FAILS 5u
#define LM35_SAMPLE_DELAY_MS     5u


static uint16_t compute_checksum(const uint16_t *data, size_t len)
{
  uint16_t csum = 0;
  for (size_t i = 0; i < len; ++i)
    csum ^= data[i];
  return csum;
}

static uint16_t avg_samples(const uint16_t *data, size_t len)
{
  uint64_t s = 0;
  for (size_t i = 0; i < len; ++i)
    s += data[i];
  return (uint16_t)(s / len);
}


esp_err_t lm35_init(LM35_t *lm,
                    adc_unit_t unit_id,
                    adc_channel_t channel,
                    adc_atten_t atten,
                    uint32_t default_vref_mv)
{
  ESP_RETURN_ON_FALSE(lm, ESP_ERR_INVALID_ARG, TAG, "lm == NULL");

  memset(lm, 0, sizeof(*lm));

  lm->channel = channel;
  lm->atten = atten;
  lm->bitwidth = LM35_DEFAULT_BITWIDTH;
  lm->sample_count = LM35_DEFAULT_SAMPLE_COUNT;
  lm->min_valid_temp_c = -55.0f;
  lm->max_valid_temp_c = 150.0f;

  lm->lock = xSemaphoreCreateMutex();
  if (!lm->lock) {
    ESP_LOGE(TAG, "Failed to create mutex");
    return ESP_ERR_NO_MEM;
  }

  /* Configure ADC oneshot driver */
  adc_oneshot_unit_init_cfg_t init_cfg = {
    .unit_id = unit_id,
  };
  esp_err_t ret = adc_oneshot_new_unit(&init_cfg, &lm->adc_unit);
  if (ret != ESP_OK) {
    ESP_LOGE(TAG, "Failed to create ADC unit: %s", esp_err_to_name(ret));
    goto err_exit;
  }

  adc_oneshot_chan_cfg_t chan_cfg = {
    .bitwidth = lm->bitwidth,
    .atten = lm->atten,
  };
  ret = adc_oneshot_config_channel(lm->adc_unit, lm->channel, &chan_cfg);
  if (ret != ESP_OK) {
    ESP_LOGE(TAG, "Failed to configure ADC channel: %s", esp_err_to_name(ret));
    goto err_exit;
  }

  /* Try to initialize calibration */
  adc_cali_handle_t cali_handle = NULL;
  bool calibrated = false;

  adc_cali_line_fitting_config_t cali_cfg = {
    .unit_id = unit_id,
    .atten = lm->atten,
    .bitwidth = lm->bitwidth,
  };
  ret = adc_cali_create_scheme_line_fitting(&cali_cfg, &cali_handle);
  if (ret == ESP_OK) {
    lm->adc_cali = cali_handle;
    calibrated = true;
    ESP_LOGI(TAG, "ADC calibration: line fitting scheme created");
  } else {
    ESP_LOGW(TAG, "ADC calibration unavailable, using raw values");
  }

  lm->is_initialized = true;
  ESP_LOGI(TAG, "LM35 initialized (unit=%d ch=%d atten=%d cali=%d)",
           unit_id, channel, atten, calibrated);
  return ESP_OK;

err_exit:
  if (lm->adc_unit) {
    adc_oneshot_del_unit(lm->adc_unit);
    lm->adc_unit = NULL;
  }
  if (lm->lock) {
    vSemaphoreDelete(lm->lock);
    lm->lock = NULL;
  }
  return ret;
}


void lm35_deinit(LM35_t *lm)
{
  if (!lm) return;

  if (lm->lock) xSemaphoreTake(lm->lock, portMAX_DELAY);

  if (lm->adc_cali) {
    adc_cali_delete_scheme_line_fitting(lm->adc_cali);
    lm->adc_cali = NULL;
  }

  if (lm->adc_unit) {
    adc_oneshot_del_unit(lm->adc_unit);
    lm->adc_unit = NULL;
  }

  if (lm->lock) {
    xSemaphoreGive(lm->lock);
    vSemaphoreDelete(lm->lock);
    lm->lock = NULL;
  }

  memset(lm, 0, sizeof(*lm));
  ESP_LOGI(TAG, "LM35 deinitialized");
}



esp_err_t lm35_read(LM35_t *lm, float *out_temperature_c)
{
  if (!lm || !lm->is_initialized)
    return ESP_ERR_INVALID_STATE;

  if (lm->lock)
    xSemaphoreTake(lm->lock, portMAX_DELAY);

  uint16_t *samples = calloc(lm->sample_count, sizeof(uint16_t));
  if (!samples) {
    if (lm->lock) xSemaphoreGive(lm->lock);
    return ESP_ERR_NO_MEM;
  }

  /* Take multiple samples */
  for (uint8_t i = 0; i < lm->sample_count; ++i) {
    int raw = 0;
    esp_err_t ret = adc_oneshot_read(lm->adc_unit, lm->channel, &raw);
    if (ret != ESP_OK) {
      ESP_LOGW(TAG, "ADC read error: %s", esp_err_to_name(ret));
      samples[i] = 0;
      lm->error_count++;
    } else {
      samples[i] = (uint16_t)raw;
    }
    vTaskDelay(pdMS_TO_TICKS(LM35_SAMPLE_DELAY_MS));
  }

  uint16_t checksum = compute_checksum(samples, lm->sample_count);
  uint16_t avg_raw = avg_samples(samples, lm->sample_count);

  uint32_t voltage_mv = 0;
  if (lm->adc_cali) {
    adc_cali_raw_to_voltage(lm->adc_cali, avg_raw, (int*)&voltage_mv);
  } else {
    /* Rough conversion assuming 12-bit ADC, 1.1V ref */
    voltage_mv = (avg_raw * LM35_DEFAULT_VREF_MV) / 4095;
  }

  float temp_c = ((float)voltage_mv / LM35_MV_PER_DEGREE_C) + lm->calibration_offset_c;
  bool valid = (temp_c >= lm->min_valid_temp_c && temp_c <= lm->max_valid_temp_c);

  /* Update internal state */
  lm->last_raw_value = avg_raw;
  lm->last_checksum = checksum;
  lm->last_temperature_c = temp_c;

  if (!valid) {
    lm->consecutive_failures++;
    lm->total_failures++;
    ESP_LOGW(TAG, "Invalid temp %.2f°C (raw=%u, V=%u mV)", temp_c, avg_raw, voltage_mv);

    if (lm->consecutive_failures > LM35_MAX_CONSECUTIVE_FAILS)
      lm->consecutive_failures = 0;

    free(samples);
    if (lm->lock) xSemaphoreGive(lm->lock);
    return ESP_FAIL;
  }

  lm->consecutive_failures = 0;
  if (out_temperature_c)
    *out_temperature_c = temp_c;

  free(samples);
  if (lm->lock) xSemaphoreGive(lm->lock);
  return ESP_OK;
}



bool lm35_validate_samples(LM35_t *lm, const uint16_t *samples, size_t n)
{
  if (!lm || !samples || n == 0) return false;
  uint16_t chk = compute_checksum(samples, n);
  return chk == lm->last_checksum;
}

void lm35_get_error_counters(LM35_t *lm, uint32_t *consecutive, uint32_t *total)
{
  if (!lm) return;
  if (lm->lock) xSemaphoreTake(lm->lock, portMAX_DELAY);
  if (consecutive) *consecutive = lm->consecutive_failures;
  if (total) *total = lm->total_failures;
  if (lm->lock) xSemaphoreGive(lm->lock);
}

esp_err_t lm35_set_sample_count(LM35_t *lm, uint8_t sample_count)
{
  if (!lm || sample_count == 0) return ESP_ERR_INVALID_ARG;
  if (lm->lock) xSemaphoreTake(lm->lock, portMAX_DELAY);
  lm->sample_count = sample_count;
  if (lm->lock) xSemaphoreGive(lm->lock);
  return ESP_OK;
}

esp_err_t lm35_set_valid_range(LM35_t *lm, float min_c, float max_c)
{
  if (!lm || min_c >= max_c) return ESP_ERR_INVALID_ARG;
  if (lm->lock) xSemaphoreTake(lm->lock, portMAX_DELAY);
  lm->min_valid_temp_c = min_c;
  lm->max_valid_temp_c = max_c;
  if (lm->lock) xSemaphoreGive(lm->lock);
  return ESP_OK;
}

