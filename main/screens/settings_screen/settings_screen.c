#include "settings_screen.h"
#include "TPPSD_nvs.h"
#include "esp_log.h"
#include "lvgl.h"

static const char* TAG = "SETTINGS_SCREEN";

extern lv_indev_t* rotary_indev;

lv_obj_t* settings_scr;
lv_group_t* settings_input_group;

lv_obj_t* label1;

static void sub1_event_cb(lv_event_t* e);

void settings_screen_init() {
    settings_scr = lv_obj_create(NULL);
    settings_input_group = lv_group_create();

    lv_obj_t* menu = lv_menu_create(settings_scr);
    lv_menu_set_mode_root_back_btn(menu, LV_MENU_ROOT_BACK_BTN_DISABLED);
    lv_obj_set_size(menu, lv_disp_get_hor_res(NULL), lv_disp_get_ver_res(NULL));
    lv_obj_center(menu);

    lv_obj_t* cont;
    lv_obj_t* label;

    lv_obj_t* sub_1_page = lv_menu_page_create(menu, "Page 1");

    lv_obj_t* main_page = lv_menu_page_create(menu, NULL);
    lv_menu_set_page(menu, main_page);

    cont = lv_menu_cont_create(sub_1_page);
    lv_obj_center(cont);
    lv_obj_t* slider = lv_slider_create(cont);
    lv_obj_clear_flag(slider, LV_OBJ_FLAG_CLICK_FOCUSABLE);
    lv_slider_set_range(slider, PANEL_BRIGHTNESS_MIN, PANEL_BRIGHTNESS_MAX);
    label1 = lv_label_create(cont);
    PANEL_BRIGHTNESS_T last_brightness;
    nvs_get_stn_brig(&last_brightness);
    char last_brightness_str[2];
    itoa(last_brightness, last_brightness_str, 10);
    lv_label_set_text(label1, last_brightness_str);
    lv_obj_align_to(label1, slider, LV_ALIGN_OUT_TOP_MID, 0, -15);
    lv_obj_set_flex_grow(slider, 1);
    lv_slider_set_value(slider, (uint32_t)last_brightness, LV_ANIM_OFF);
    lv_obj_add_event_cb(slider, sub1_event_cb, LV_EVENT_ALL, NULL);

    cont = lv_menu_cont_create(main_page);
    label = lv_label_create(cont);
    lv_label_set_text(label, "Item 1 (Click me!)");
    lv_menu_set_load_page_event(menu, cont, sub_1_page);
    lv_group_add_obj(settings_input_group, cont);

    cont = lv_menu_cont_create(main_page);
    label = lv_label_create(cont);
    lv_label_set_text(label, "Item 2 (Click me!)");
    lv_menu_set_load_page_event(menu, cont, sub_1_page);
    lv_group_add_obj(settings_input_group, cont);
    lv_group_add_obj(settings_input_group, slider);
}

void settings_screen_load() {
    lv_indev_set_group(rotary_indev, settings_input_group);
    lv_scr_load(settings_scr);
}

static void sub1_event_cb(lv_event_t* e) {
    lv_event_code_t event = lv_event_get_code(e);
    lv_obj_t* slider = lv_event_get_target(e);
    uint32_t slider_value = lv_slider_get_value(slider);

    switch (event) {
    case LV_EVENT_VALUE_CHANGED:
        i80_st7789_panel_set_brightness(&panel, (PANEL_BRIGHTNESS_T)slider_value);
        lv_label_set_text_fmt(label1, "%" LV_PRId32, slider_value);
        lv_obj_align_to(label1, slider, LV_ALIGN_OUT_TOP_MID, 0, -15);
        break;
    case LV_EVENT_LEAVE:
        ESP_LOGI(TAG, "LEAVED");
        nvs_set_stn_brig((PANEL_BRIGHTNESS_T*)&slider_value);
        nvs_save();
        break;
    default:
        break;
    }
}