#include "i80_st7789_panel.h"
#include "freertos/FreeRTOS.h"

#define MAX_BRIGHTNESS_DUTY_CYCLE (1 << SOC_LEDC_TIMER_BIT_WIDTH) - 10

static const char* TAG = "DISPLAY_PANEL";

typedef struct {
    uint32_t addr;
    uint8_t param[20];
    uint32_t len;
} lcd_cmd_t;

static const lcd_cmd_t lcd_st7789v_commands[] = {
    {0x11, {0}, 0 | 0x80},
    {0x3A, {0X05}, 1},
    {0xB2, {0X0B, 0X0B, 0X00, 0X33, 0X33}, 5},
    {0xB7, {0X75}, 1},
    {0xBB, {0X28}, 1},
    {0xC0, {0X2C}, 1},
    {0xC2, {0X01}, 1},
    {0xC3, {0X1F}, 1},
    {0xC6, {0X13}, 1},
    {0xD0, {0XA7}, 1},
    {0xD0, {0XA4, 0XA1}, 2},
    {0xD6, {0XA1}, 1},
    {0xE0,
     {0XF0, 0X05, 0X0A, 0X06, 0X06, 0X03, 0X2B, 0X32, 0X43, 0X36, 0X11, 0X10, 0X2B, 0X32},
     14},
    {0xE1,
     {0XF0, 0X08, 0X0C, 0X0B, 0X09, 0X24, 0X2B, 0X22, 0X43, 0X38, 0X15, 0X16, 0X2F, 0X37},
     14},
};

i80_st7789_panel_handle_t i80_st7789_panel_init(
    const gpio_num_t I80_CS_PIN, const gpio_num_t I80_RST_PIN, const gpio_num_t I80_DC_PIN,
    const gpio_num_t I80_WR_PIN, const gpio_num_t I80_RD_PIN, const gpio_num_t I80_DATA0_PIN,
    const gpio_num_t I80_DATA1_PIN, const gpio_num_t I80_DATA2_PIN, const gpio_num_t I80_DATA3_PIN,
    const gpio_num_t I80_DATA4_PIN, const gpio_num_t I80_DATA5_PIN, const gpio_num_t I80_DATA6_PIN,
    const gpio_num_t I80_DATA7_PIN, const gpio_num_t BACK_LIGHT_PIN, const size_t buffer_size,
    const uint32_t panel_clock_hz, esp_lcd_panel_io_color_trans_done_cb_t color_trans_done_cb) {

    ESP_LOGI(TAG, "============I80 ST7889 PANEL============");

    ESP_ERROR_CHECK(gpio_set_direction(I80_RD_PIN, GPIO_MODE_OUTPUT));
    ESP_ERROR_CHECK(gpio_set_level(I80_RD_PIN, true));

    i80_st7789_panel_handle_t panel_handle;

    esp_lcd_i80_bus_config_t bus_config = {.dc_gpio_num = I80_DC_PIN,
                                           .wr_gpio_num = I80_WR_PIN,
                                           .clk_src = LCD_CLK_SRC_DEFAULT,
                                           .data_gpio_nums =
                                               {
                                                   I80_DATA0_PIN,
                                                   I80_DATA1_PIN,
                                                   I80_DATA2_PIN,
                                                   I80_DATA3_PIN,
                                                   I80_DATA4_PIN,
                                                   I80_DATA5_PIN,
                                                   I80_DATA6_PIN,
                                                   I80_DATA7_PIN,
                                               },
                                           .bus_width = 8,
                                           .max_transfer_bytes = buffer_size,
                                           .psram_trans_align = 64,
                                           .sram_trans_align = 4};
    ESP_ERROR_CHECK(esp_lcd_new_i80_bus(&bus_config, &panel_handle.bus_handle));

    esp_lcd_panel_io_i80_config_t io_config = {
        .cs_gpio_num = I80_CS_PIN,
        .pclk_hz = panel_clock_hz,
        .trans_queue_depth = 10,
        .on_color_trans_done = color_trans_done_cb,
        .user_ctx = NULL,
        .lcd_cmd_bits = 8,
        .lcd_param_bits = 8,
        .dc_levels =
            {
                .dc_idle_level = 0,
                .dc_cmd_level = 0,
                .dc_dummy_level = 0,
                .dc_data_level = 1,
            },
    };
    ESP_ERROR_CHECK(
        esp_lcd_new_panel_io_i80(panel_handle.bus_handle, &io_config, &panel_handle.io_handle));

    esp_lcd_panel_dev_config_t panel_config = {
        .reset_gpio_num = I80_RST_PIN,
        .color_space = (lcd_rgb_element_order_t)ESP_LCD_COLOR_SPACE_RGB,
        .bits_per_pixel = 16,
        .vendor_config = NULL,
    };

    ESP_ERROR_CHECK(esp_lcd_new_panel_st7789(panel_handle.io_handle, &panel_config,
                                             &panel_handle.esp_lcd_panel_handle));

    esp_lcd_panel_reset(panel_handle.esp_lcd_panel_handle);
    esp_lcd_panel_init(panel_handle.esp_lcd_panel_handle);

    for (uint8_t i = 0; i < (sizeof(lcd_st7789v_commands) / sizeof(lcd_cmd_t)); i++) {
        esp_lcd_panel_io_tx_param(panel_handle.io_handle, lcd_st7789v_commands[i].addr,
                                  lcd_st7789v_commands[i].param,
                                  lcd_st7789v_commands[i].len & 0x7f);
        if (lcd_st7789v_commands[i].len & 0x80)
            vTaskDelay(pdMS_TO_TICKS(120));
    }

    esp_lcd_panel_disp_on_off(panel_handle.esp_lcd_panel_handle, true);

    ledc_timer_config_t ledc_timer = {.speed_mode = LEDC_LOW_SPEED_MODE,
                                      .duty_resolution = SOC_LEDC_TIMER_BIT_WIDTH,
                                      .timer_num = LEDC_TIMER_0,
                                      .freq_hz = 4000,
                                      .clk_cfg = LEDC_AUTO_CLK};
    ESP_ERROR_CHECK(ledc_timer_config(&ledc_timer));
    ledc_channel_config_t ledc_channel = {.speed_mode = LEDC_LOW_SPEED_MODE,
                                          .channel = LEDC_CHANNEL_0,
                                          .timer_sel = LEDC_TIMER_0,
                                          .intr_type = LEDC_INTR_DISABLE,
                                          .gpio_num = BACK_LIGHT_PIN,
                                          .duty = 0, // Set duty to 0%
                                          .hpoint = 0};
    ESP_ERROR_CHECK(ledc_channel_config(&ledc_channel));
    panel_handle.back_light_channel = ledc_channel.channel;

    return panel_handle;
};

esp_err_t i80_st7789_panel_set_brightness(i80_st7789_panel_handle_t* panel_handle,
                                          PANEL_BRIGHTNESS_T panel_brightness) {

    uint32_t brightness;
    switch (panel_brightness) {
    case PANEL_BRIGHTNESS_MIN:
        brightness = 1 << 5;
        break;
    case PANEL_BRIGHTNESS_2:
        brightness = 1 << 8;
        break;
    case PANEL_BRIGHTNESS_3:
        brightness = 1 << 10;
        break;
    case PANEL_BRIGHTNESS_4:
        brightness = 1 << 12;
        break;
    default:
        brightness = MAX_BRIGHTNESS_DUTY_CYCLE;
        break;
    }

    ESP_LOGI(TAG, "Brightness set: %" PRId8, panel_brightness);
    ledc_set_duty(LEDC_LOW_SPEED_MODE, panel_handle->back_light_channel, brightness);
    return ledc_update_duty(LEDC_LOW_SPEED_MODE, panel_handle->back_light_channel);
}

esp_err_t i80_st7789_panel_invert_color(i80_st7789_panel_handle_t* panel_handle,
                                        bool invert_color_data) {
    return esp_lcd_panel_invert_color(panel_handle->esp_lcd_panel_handle, invert_color_data);
}

esp_err_t i80_st7789_panel_swap_xy(i80_st7789_panel_handle_t* panel_handle, bool swap_axes) {
    return esp_lcd_panel_swap_xy(panel_handle->esp_lcd_panel_handle, swap_axes);
}

esp_err_t i80_st7789_panel_mirror(i80_st7789_panel_handle_t* panel_handle, bool mirror_x,
                                  bool mirror_y) {
    return esp_lcd_panel_mirror(panel_handle->esp_lcd_panel_handle, mirror_x, mirror_y);
}

esp_err_t i80_st7789_panel_set_gap(i80_st7789_panel_handle_t* panel_handle, int x_gap, int y_gap) {
    return esp_lcd_panel_set_gap(panel_handle->esp_lcd_panel_handle, x_gap, y_gap);
}