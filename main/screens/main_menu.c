#include "esp_check.h"
#include "esp_log.h"
#include "lvgl.h"
#include "screens/insert_passwords.h"
#include "screens/register.h"
#include "screens/show_passwords.h"
// #include "tasks/fetch_passwords_task.h"

static const char* TAG = "MAIN_SCREEN";

extern lv_indev_t* rotary_indev;

lv_group_t* main_menu_input_group;

lv_obj_t* cont_col;
lv_obj_t* main_menu_scr;

lv_obj_t* insert_passwords_btn;
lv_obj_t* show_passwords_btn;
lv_obj_t* settings_btn;
lv_obj_t* read_csv_btn;
lv_obj_t* register_btn;

static void insert_passwords_btn_event_cb(lv_event_t* e);
static void show_passwords_btn_event_cb(lv_event_t* e);
static void settings_btn_event_cb(lv_event_t* e);
static void read_csv_btn_event_cb(lv_event_t* e);
static void register_btn_event_cb(lv_event_t* e);

void main_menu_screen_init() {
    main_menu_scr = lv_obj_create(NULL);
    main_menu_input_group = lv_group_create();

    cont_col = lv_obj_create(main_menu_scr);

    lv_obj_set_size(cont_col, lv_pct(100), lv_pct(100));
    lv_obj_center(cont_col);
    lv_obj_set_flex_flow(cont_col, LV_FLEX_FLOW_COLUMN);

    insert_passwords_btn = lv_btn_create(cont_col);
    lv_obj_set_width(insert_passwords_btn, lv_pct(100));
    lv_obj_t* insert_passwords_label = lv_label_create(insert_passwords_btn);
    lv_label_set_text(insert_passwords_label, "Insert Passwords");
    lv_obj_center(insert_passwords_label);

    lv_obj_t* show_passwords_label;
    show_passwords_btn = lv_btn_create(cont_col);
    lv_obj_set_width(show_passwords_btn, lv_pct(100));
    show_passwords_label = lv_label_create(show_passwords_btn);
    lv_label_set_text(show_passwords_label, "Show Passwords");
    lv_obj_center(show_passwords_label);

    lv_obj_t* settings_label;
    settings_btn = lv_btn_create(cont_col);
    lv_obj_set_width(settings_btn, lv_pct(100));
    settings_label = lv_label_create(settings_btn);
    lv_label_set_text(settings_label, "Settings");
    lv_obj_center(settings_label);

    lv_obj_t* read_csv_label;
    read_csv_btn = lv_btn_create(cont_col);
    lv_obj_set_width(read_csv_btn, lv_pct(100));
    read_csv_label = lv_label_create(read_csv_btn);
    lv_label_set_text(read_csv_label, "Read CSV");
    lv_obj_center(read_csv_label);

    lv_obj_t* register_label;
    register_btn = lv_btn_create(cont_col);
    lv_obj_set_width(register_btn, lv_pct(100));
    register_label = lv_label_create(register_btn);
    lv_label_set_text(register_label, "Register Screen");
    lv_obj_center(register_label);

    lv_obj_add_event_cb(insert_passwords_btn, insert_passwords_btn_event_cb,
                        LV_EVENT_ALL, NULL);
    lv_obj_add_event_cb(show_passwords_btn, show_passwords_btn_event_cb,
                        LV_EVENT_CLICKED, NULL);
    lv_obj_add_event_cb(settings_btn, settings_btn_event_cb, LV_EVENT_CLICKED,
                        NULL);
    lv_obj_add_event_cb(read_csv_btn, read_csv_btn_event_cb, LV_EVENT_CLICKED,
                        NULL);
    lv_obj_add_event_cb(register_btn, register_btn_event_cb, LV_EVENT_ALL,
                        NULL);

    lv_group_add_obj(main_menu_input_group, insert_passwords_btn);
    lv_group_add_obj(main_menu_input_group, show_passwords_btn);
    lv_group_add_obj(main_menu_input_group, settings_btn);
    lv_group_add_obj(main_menu_input_group, read_csv_btn);
    lv_group_add_obj(main_menu_input_group, register_btn);
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

static void read_csv_btn_event_cb(lv_event_t* e) {
    // start_fetch_passwords_task();
}

static void register_btn_event_cb(lv_event_t* e) {
    lv_event_code_t code = lv_event_get_code(e);

    switch (code) {
    case LV_EVENT_SHORT_CLICKED:
        register_screen_load();
        break;
    case LV_EVENT_LONG_PRESSED:
        ESP_LOGI(TAG, "LONG PRESSED REGISTER SCREEN");
    default:
        break;
    }
}