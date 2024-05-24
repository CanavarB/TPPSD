#include "main_menu_screen.h"

// SCREENS
#include "insert_passwords_screen.h"
#include "register_screen.h"
#include "show_passwords_screen.h"
// #include "tasks/fetch_passwords_task.h"

static const char* TAG = "MAIN_SCREEN";

extern lv_indev_t* rotary_indev;

lv_group_t* main_menu_input_group;

lv_obj_t* main_menu_scr;

static void insert_passwords_btn_event_cb(lv_event_t* e);
static void show_passwords_btn_event_cb(lv_event_t* e);
static void settings_btn_event_cb(lv_event_t* e);
static void generate_btn_event_cb(lv_event_t* e);

void main_menu_screen_init() {
    LV_IMG_DECLARE(insert_icon);
    LV_IMG_DECLARE(show_icon);
    LV_IMG_DECLARE(settings_icon);
    LV_IMG_DECLARE(generate_icon);

    main_menu_scr = lv_obj_create(NULL);
    main_menu_input_group = lv_group_create();
    lv_obj_set_size(main_menu_scr, lv_pct(100), lv_pct(100));

    lv_obj_set_flex_flow(main_menu_scr, LV_FLEX_FLOW_COLUMN);

    lv_obj_t* insert_passwords_btn = lv_btn_create(main_menu_scr);
    lv_obj_t* insert_symbol = lv_img_create(insert_passwords_btn);
    lv_img_set_src(insert_symbol, &insert_icon);
    lv_obj_set_size(insert_symbol, 150, 150);
    lv_obj_center(insert_symbol);
    lv_obj_set_width(insert_passwords_btn, lv_pct(100));
    lv_obj_t* insert_passwords_label = lv_label_create(insert_passwords_btn);
    lv_label_set_text(insert_passwords_label, "Insert");
    lv_obj_align_to(insert_passwords_label, insert_symbol, LV_ALIGN_BOTTOM_MID, 0, 30);

    lv_obj_t* show_passwords_label;
    lv_obj_t* show_passwords_btn = lv_btn_create(main_menu_scr);
    lv_obj_t* show_symbol = lv_img_create(show_passwords_btn);
    lv_img_set_src(show_symbol, &show_icon);
    lv_obj_set_size(show_symbol, 150, 150);
    lv_obj_center(show_symbol);
    lv_obj_set_width(show_passwords_btn, lv_pct(100));
    show_passwords_label = lv_label_create(show_passwords_btn);
    lv_label_set_text(show_passwords_label, "Show");
    lv_obj_align_to(show_passwords_label, show_symbol, LV_ALIGN_BOTTOM_MID, 0, 30);

    lv_obj_t* settings_label;
    lv_obj_t* settings_btn = lv_btn_create(main_menu_scr);
    lv_obj_t* settings_symbol = lv_img_create(settings_btn);
    lv_img_set_src(settings_symbol, &settings_icon);
    lv_obj_set_size(settings_symbol, 150, 150);
    lv_obj_center(settings_symbol);
    lv_obj_set_width(settings_btn, lv_pct(100));
    settings_label = lv_label_create(settings_btn);
    lv_label_set_text(settings_label, "Settings");
    lv_obj_align_to(settings_label, settings_symbol, LV_ALIGN_BOTTOM_MID, 0, 30);

    lv_obj_t* generate_label;
    lv_obj_t* generate_btn = lv_btn_create(main_menu_scr);
    lv_obj_t* generate_symbol = lv_img_create(generate_btn);
    lv_img_set_src(generate_symbol, &generate_icon);
    lv_obj_set_size(generate_symbol, 150, 150);
    lv_obj_center(generate_symbol);
    lv_obj_set_width(generate_btn, lv_pct(100));
    generate_label = lv_label_create(generate_btn);
    lv_label_set_text(generate_label, "Generate Password");
    lv_obj_align_to(generate_label, generate_symbol, LV_ALIGN_BOTTOM_MID, 0, 30);

    lv_obj_add_event_cb(insert_passwords_btn, insert_passwords_btn_event_cb, LV_EVENT_ALL, NULL);
    lv_obj_add_event_cb(show_passwords_btn, show_passwords_btn_event_cb, LV_EVENT_CLICKED, NULL);
    lv_obj_add_event_cb(settings_btn, settings_btn_event_cb, LV_EVENT_CLICKED, NULL);
    lv_obj_add_event_cb(generate_btn, generate_btn_event_cb, LV_EVENT_CLICKED, NULL);

    lv_group_add_obj(main_menu_input_group, insert_passwords_btn);
    lv_group_add_obj(main_menu_input_group, show_passwords_btn);
    lv_group_add_obj(main_menu_input_group, settings_btn);
    lv_group_add_obj(main_menu_input_group, generate_btn);
}

void main_menu_screen_load() {
    lv_indev_set_group(rotary_indev, main_menu_input_group);
    lv_scr_load(main_menu_scr);
}

static void insert_passwords_btn_event_cb(lv_event_t* e) {
    lv_event_code_t code = lv_event_get_code(e);
    switch (code) {
    case LV_EVENT_RELEASED:
        ESP_LOGI(TAG, "INSERT_PASS ");
        insert_passwords_screen_load();
        break;
    default:
        break;
    }
}

static void show_passwords_btn_event_cb(lv_event_t* e) {
    ESP_LOGI(TAG, "SHOW_PASS");
    show_passwords_screen_load();
}
static void settings_btn_event_cb(lv_event_t* e) { ESP_LOGI(TAG, "SETTINGS"); }

static void generate_btn_event_cb(lv_event_t* e) {
    // start_fetch_passwords_task();
}