#pragma once
#include "driver/gpio.h"
#include "esp_lcd_panel_io.h"
#include "esp_lcd_panel_ops.h"
#include "esp_lcd_panel_vendor.h"
#include "esp_log.h"
#include "freertos/FreeRTOS.h"

esp_lcd_panel_handle_t i80_st7789_panel_init(
    const gpio_num_t I80_CS_PIN, const gpio_num_t I80_RST_PIN, const gpio_num_t I80_DC_PIN,
    const gpio_num_t I80_WR_PIN, const gpio_num_t I80_RD_PIN, const gpio_num_t I80_DATA0_PIN,
    const gpio_num_t I80_DATA1_PIN, const gpio_num_t I80_DATA2_PIN, const gpio_num_t I80_DATA3_PIN,
    const gpio_num_t I80_DATA4_PIN, const gpio_num_t I80_DATA5_PIN, const gpio_num_t I80_DATA6_PIN,
    const gpio_num_t I80_DATA7_PIN, const gpio_num_t BACK_LIGHT_PIN, const size_t buffer_size,
    const uint32_t panel_clock_hz, esp_lcd_panel_io_color_trans_done_cb_t color_trans_done_cb);