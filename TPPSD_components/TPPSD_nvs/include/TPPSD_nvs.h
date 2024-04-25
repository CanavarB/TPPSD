#pragma once
#include "esp_err.h"
#include "i80_st7789_panel.h"
#include "nvs.h"

extern size_t DEVICE_PASSWORD_LEN;

esp_err_t nvs_init();

esp_err_t nvs_set_dev_passwd(uint8_t* read_buff);
esp_err_t nvs_get_dev_passwd(uint8_t* read_buff);

esp_err_t nvs_set_stn_brig(PANEL_BRIGHTNESS_T* brightness);
esp_err_t nvs_get_stn_brig(PANEL_BRIGHTNESS_T* brightness);

esp_err_t nvs_save();
