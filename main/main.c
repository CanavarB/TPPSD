#include "main.h"
#include "lvgl_task.h"

static const char* TAG = "MAIN";

i80_st7789_panel_handle_t panel;
rotary_encoder_handle_t rotary_encoder;
salt_table_handle_t salt_table;
usb_msc_handle_t usb_msc;
usb_keyboard_handle_t usb_keyboard;

bool display_notify_lvgl_flush_ready(esp_lcd_panel_io_handle_t panel_io,
                                     esp_lcd_panel_io_event_data_t* edata, void* user_ctx) {
    lv_disp_flush_ready(&disp_drv);
    return false;
}

void app_main(void) {
    esp_err_t err;

    gpio_set_direction(BOARD_POWERON, GPIO_MODE_OUTPUT);
    gpio_set_level(BOARD_POWERON, true);

    spiffs_init();
    nvs_init();

    rotary_encoder = rotary_encoder_init(ROT_ENC_A_GPIO, ROT_ENC_B_GPIO, ROT_ENC_BUTTON_GPIO);

    panel = i80_st7789_panel_init(
        BOARD_TFT_CS, BOARD_TFT_RST, BOARD_TFT_DC, BOARD_TFT_WR, BOARD_TFT_RD, BOARD_TFT_DATA0,
        BOARD_TFT_DATA1, BOARD_TFT_DATA2, BOARD_TFT_DATA3, BOARD_TFT_DATA4, BOARD_TFT_DATA5,
        BOARD_TFT_DATA6, BOARD_TFT_DATA7, BOARD_TFT_BL, DISPLAY_BUFFER_SIZE * sizeof(uint16_t),
        LCD_PIXEL_CLOCK_HZ, display_notify_lvgl_flush_ready);

    i80_st7789_panel_invert_color(&panel, true);
    i80_st7789_panel_mirror(&panel, true, true);
    i80_st7789_panel_set_gap(&panel, 35, 0);

    PANEL_BRIGHTNESS_T last_brightness;
    nvs_get_stn_brig(&last_brightness);
    i80_st7789_panel_set_brightness(&panel, last_brightness);
    // TODO: set CB symbol

    usb_msc = usb_msc_init(USB_MSC_LABEL, USB_MSC_MOUNT_PATH);
    usb_keyboard = usb_keyboard_init();
    salt_table = salt_table_init(SALT_TABLE_LABEL);

    start_lvgl_task();
}
