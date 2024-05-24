#pragma once
#include "esp_check.h"
#include "esp_err.h"
#include "esp_log.h"
#include "product_config.h"

// COMPONENTS
#include "TPPSD_nvs.h"
#include "TPPSD_spiffs.h"
#include "aes_256_cbc.h"
#include "i80_st7789_panel.h"
#include "rotary_encoder.h"
#include "salt_table.h"
#include "usb_keyboard.h"
#include "usb_msc.h"

extern i80_st7789_panel_handle_t panel;
extern rotary_encoder_handle_t rotary_encoder;
extern salt_table_handle_t salt_table;
extern usb_msc_handle_t usb_msc;
extern usb_keyboard_handle_t usb_keyboard;