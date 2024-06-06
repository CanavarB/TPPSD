#include "main_menu_screen.h"

// SCREENS
#include "insert_passwords_screen.h"
#include "register_screen.h"
#include "settings_screen.h"
#include "show_passwords_screen.h"

static const char* TAG = "MAIN_SCREEN";

extern lv_indev_t* rotary_indev;

lv_group_t* main_menu_input_group;

lv_obj_t* main_menu_scr;

static void insert_passwords_btn_event_cb(lv_event_t* e);
static void show_passwords_btn_event_cb(lv_event_t* e);
static void settings_btn_event_cb(lv_event_t* e);
static void generate_btn_event_cb(lv_event_t* e);

void main_menu_screen_init() {
    lv_img_cache_set_size(5);
    LV_IMG_DECLARE(insert_icon);
    LV_IMG_DECLARE(show_icon);
    LV_IMG_DECLARE(settings_icon);
    LV_IMG_DECLARE(generate_icon);

    main_menu_scr = lv_obj_create(NULL);
    main_menu_input_group = lv_group_create();
    lv_group_set_default(main_menu_input_group);
    lv_obj_set_size(main_menu_scr, lv_pct(100), lv_pct(100));
    lv_obj_set_flex_flow(main_menu_scr, LV_FLEX_FLOW_COLUMN);

    lv_obj_t* btn;
    lv_obj_t* symbol;
    lv_obj_t* label;

    // INSERT PASSWORDS
    btn = lv_btn_create(main_menu_scr);
    symbol = lv_img_create(btn);
    lv_img_set_src(symbol, &insert_icon);
    lv_obj_set_size(symbol, 150, 150);
    lv_obj_center(symbol);
    lv_obj_set_width(btn, lv_pct(100));
    label = lv_label_create(btn);
    lv_label_set_text(label, "Insert");
    lv_obj_align_to(label, symbol, LV_ALIGN_BOTTOM_MID, 0, 30);
    lv_obj_add_event_cb(btn, insert_passwords_btn_event_cb, LV_EVENT_SHORT_CLICKED, NULL);
    lv_group_add_obj(main_menu_input_group, btn);
    lv_obj_set_style_bg_color(btn, lv_color_make(241, 73, 2), LV_STATE_FOCUSED);
    lv_obj_set_style_bg_color(btn, lv_color_make(120, 120, 120), LV_STATE_DEFAULT);

    // SHOW PASSWORDS
    btn = lv_btn_create(main_menu_scr);
    symbol = lv_img_create(btn);
    lv_img_set_src(symbol, &show_icon);
    lv_obj_set_size(symbol, 150, 150);
    lv_obj_center(symbol);
    lv_obj_set_width(btn, lv_pct(100));
    label = lv_label_create(btn);
    lv_label_set_text(label, "Show");
    lv_obj_align_to(label, symbol, LV_ALIGN_BOTTOM_MID, 0, 30);
    lv_obj_add_event_cb(btn, show_passwords_btn_event_cb, LV_EVENT_SHORT_CLICKED, NULL);
    lv_group_add_obj(main_menu_input_group, btn);
    lv_obj_set_style_bg_color(btn, lv_color_make(241, 73, 2), LV_STATE_FOCUSED);
    lv_obj_set_style_bg_color(btn, lv_color_make(120, 120, 120), LV_STATE_DEFAULT);

    // SETTINGS
    btn = lv_btn_create(main_menu_scr);
    symbol = lv_img_create(btn);
    lv_img_set_src(symbol, &settings_icon);
    lv_obj_set_size(symbol, 150, 150);
    lv_obj_center(symbol);
    lv_obj_set_width(btn, lv_pct(100));
    label = lv_label_create(btn);
    lv_label_set_text(label, "Settings");
    lv_obj_align_to(label, symbol, LV_ALIGN_BOTTOM_MID, 0, 30);
    lv_obj_add_event_cb(btn, settings_btn_event_cb, LV_EVENT_SHORT_CLICKED, NULL);
    lv_group_add_obj(main_menu_input_group, btn);
    lv_obj_set_style_bg_color(btn, lv_color_make(241, 73, 2), LV_STATE_FOCUSED);
    lv_obj_set_style_bg_color(btn, lv_color_make(120, 120, 120), LV_STATE_DEFAULT);

    // GENERATE
    btn = lv_btn_create(main_menu_scr);
    symbol = lv_img_create(btn);
    lv_img_set_src(symbol, &generate_icon);
    lv_obj_set_size(symbol, 150, 150);
    lv_obj_center(symbol);
    lv_obj_set_width(btn, lv_pct(100));
    label = lv_label_create(btn);
    lv_label_set_text(label, "Generate Password");
    lv_obj_align_to(label, symbol, LV_ALIGN_BOTTOM_MID, 0, 30);
    lv_obj_add_event_cb(btn, generate_btn_event_cb, LV_EVENT_SHORT_CLICKED, NULL);
    lv_group_add_obj(main_menu_input_group, btn);
    lv_obj_set_style_bg_color(btn, lv_color_make(241, 73, 2), LV_STATE_FOCUSED);
    lv_obj_set_style_bg_color(btn, lv_color_make(120, 120, 120), LV_STATE_DEFAULT);
}

void main_menu_screen_load() {
    lv_indev_set_group(rotary_indev, main_menu_input_group);
    lv_scr_load(main_menu_scr);
}

static void insert_passwords_btn_event_cb(lv_event_t* e) {
    ESP_LOGI(TAG, "INSERT PASS");
    insert_passwords_screen_load();
}

static void show_passwords_btn_event_cb(lv_event_t* e) {
    ESP_LOGI(TAG, "SHOW_PASS");
    show_passwords_screen_load();
}
static void settings_btn_event_cb(lv_event_t* e) {
    ESP_LOGI(TAG, "SETTINGS");
    settings_screen_load();
}

static void generate_btn_event_cb(lv_event_t* e) {}