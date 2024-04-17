#include "aes_256_cbc.h"
#include "enc_storage.h"
#include "esp_check.h"
#include "i80_st7789_panel.h"
#include "lvgl.h"
#include "product_config.h"
#include "rotary_encoder.h"
#include "salt_table.h"

static const char* TAG = "MAIN";

static lv_disp_drv_t disp_drv;

bool display_notify_lvgl_flush_ready(esp_lcd_panel_io_handle_t panel_io,
                                     esp_lcd_panel_io_event_data_t* edata, void* user_ctx) {
    lv_disp_flush_ready(&disp_drv);
    return false;
}

void app_main(void) {
    /* rotary_encoder_handle_t rotary_encoder =
        rotary_encoder_init(ROT_ENC_A_GPIO, ROT_ENC_B_GPIO, ROT_ENC_BUTTON_GPIO);

    salt_table_handle_t salt_table = salt_table_init(SALT_TABLE_LABEL);
    esp_lcd_panel_handle_t panel = i80_st7789_panel_init(
        BOARD_TFT_CS, BOARD_TFT_RST, BOARD_TFT_DC, BOARD_TFT_WR, BOARD_TFT_RD, BOARD_TFT_DATA0,
        BOARD_TFT_DATA1, BOARD_TFT_DATA2, BOARD_TFT_DATA3, BOARD_TFT_DATA4, BOARD_TFT_DATA5,
        BOARD_TFT_DATA6, BOARD_TFT_DATA7, BOARD_TFT_BL, DISPLAY_BUFFER_SIZE * sizeof(uint16_t),
        LCD_PIXEL_CLOCK_HZ, display_notify_lvgl_flush_ready); */
}
