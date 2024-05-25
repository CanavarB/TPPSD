#include "register_screen.h"

static const char* TAG = "REGISTER_SCREEN";
extern lv_indev_t* rotary_indev;

lv_obj_t* register_scr;
lv_group_t* register_input_group;

lv_obj_t* roller;

static void roller_event_handler(lv_event_t* e);

void register_screen_init() {

    register_scr = lv_obj_create(NULL);
    register_input_group = lv_group_create();

    roller = lv_roller_create(register_scr);
    lv_roller_set_options(roller,
                          "1\n"
                          "2\n"
                          "3\n"
                          "4\n"
                          "5\n"
                          "6\n"
                          "7\n"
                          "8\n"
                          "9\n"
                          "0",
                          LV_ROLLER_MODE_NORMAL);

    lv_obj_set_size(roller, lv_pct(100), lv_pct(80));

    lv_obj_align(roller, LV_ALIGN_BOTTOM_MID, 0, 0);

    lv_obj_add_event_cb(roller, roller_event_handler, LV_EVENT_ALL, NULL);
    lv_group_add_obj(register_input_group, roller);
    lv_roller_set_selected(roller, 0, LV_ANIM_ON);
}

void register_screen_load() {

    lv_indev_set_group(rotary_indev, register_input_group);
    lv_scr_load(register_scr);
}

static void roller_event_handler(lv_event_t* e) {
    lv_event_code_t code = lv_event_get_code(e);
    lv_obj_t* obj = lv_event_get_target(e);
    if (code == LV_EVENT_VALUE_CHANGED) {
        ESP_LOGE(TAG, "XXXXXXXXXXXXX");
        char buf[32];
        lv_roller_get_selected_str(obj, buf, sizeof(buf));
    }
}