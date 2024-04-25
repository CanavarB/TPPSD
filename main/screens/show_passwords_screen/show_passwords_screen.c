#include "show_passwords_screen.h"

static const char* TAG = "SHOW_PASSWORDS_SCREEN";

extern lv_indev_t* rotary_indev;

lv_group_t* show_passwords_input_group;
lv_obj_t* show_passwords_scr;

static lv_obj_t* cont_col;

void create_name_buttons();

void show_passwords_screen_init() {
    show_passwords_scr = lv_obj_create(NULL);
    show_passwords_input_group = lv_group_create();

    cont_col = lv_obj_create(show_passwords_scr);
    lv_obj_set_size(cont_col, lv_pct(100), lv_pct(100));
    lv_obj_center(cont_col);
    lv_obj_set_flex_flow(cont_col, LV_FLEX_FLOW_COLUMN);
}
void show_passwords_screen_load() {
    lv_indev_set_group(rotary_indev, show_passwords_input_group);
    // create_name_buttons();
    lv_scr_load(show_passwords_scr);
}
