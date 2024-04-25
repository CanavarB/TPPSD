#pragma once
#include "driver/gpio.h"
#include "driver/ledc.h"
#include "esp_lcd_panel_io.h"
#include "esp_lcd_panel_ops.h"
#include "esp_lcd_panel_vendor.h"
#include "esp_log.h"

typedef struct {
    esp_lcd_panel_handle_t esp_lcd_panel_handle;
    esp_lcd_i80_bus_handle_t bus_handle;
    esp_lcd_panel_io_handle_t io_handle;
    ledc_channel_t back_light_channel;
} i80_st7789_panel_handle_t;

typedef enum {
    PANEL_BRIGHTNESS_MIN = 1,
    PANEL_BRIGHTNESS_2,
    PANEL_BRIGHTNESS_3,
    PANEL_BRIGHTNESS_4,

    // MAX
    PANEL_BRIGHTNESS_MAX,
} PANEL_BRIGHTNESS_T;

i80_st7789_panel_handle_t i80_st7789_panel_init(
    const gpio_num_t I80_CS_PIN, const gpio_num_t I80_RST_PIN, const gpio_num_t I80_DC_PIN,
    const gpio_num_t I80_WR_PIN, const gpio_num_t I80_RD_PIN, const gpio_num_t I80_DATA0_PIN,
    const gpio_num_t I80_DATA1_PIN, const gpio_num_t I80_DATA2_PIN, const gpio_num_t I80_DATA3_PIN,
    const gpio_num_t I80_DATA4_PIN, const gpio_num_t I80_DATA5_PIN, const gpio_num_t I80_DATA6_PIN,
    const gpio_num_t I80_DATA7_PIN, const gpio_num_t BACK_LIGHT_PIN, const size_t buffer_size,
    const uint32_t panel_clock_hz, esp_lcd_panel_io_color_trans_done_cb_t color_trans_done_cb);

esp_err_t i80_st7789_panel_set_brightness(i80_st7789_panel_handle_t* panel_handle,
                                          PANEL_BRIGHTNESS_T panel_brightness);
esp_err_t i80_st7789_panel_invert_color(i80_st7789_panel_handle_t* panel_handle,
                                        bool invert_color_data);
esp_err_t i80_st7789_panel_swap_xy(i80_st7789_panel_handle_t* panel_handle, bool swap_axes);
esp_err_t i80_st7789_panel_mirror(i80_st7789_panel_handle_t* panel_handle, bool mirror_x,
                                  bool mirror_y);
esp_err_t i80_st7789_panel_set_gap(i80_st7789_panel_handle_t* panel_handle, int x_gap, int y_gap);