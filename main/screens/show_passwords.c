#include "esp_check.h"
#include "esp_log.h"
#include "lvgl.h"
#include "product_config.h"
#include "screens/main_menu.h"
#include <dirent.h>
#include <errno.h>

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
    create_name_buttons();
    lv_scr_load(show_passwords_scr);
}

void create_name_buttons() {

    lv_obj_t* btn;
    lv_obj_t* label;
    struct dirent* d;
    DIR* dh = opendir(STORAGE_MOUNT_PATH);
    if (!dh) {
        if (errno == ENOENT) {
            // If the directory is not found
            ESP_LOGE(TAG, "Directory doesn't exist %s", STORAGE_MOUNT_PATH);
        } else {
            // If the directory is not readable then throw error and exit
            ESP_LOGE(TAG, "Unable to read directory %s", STORAGE_MOUNT_PATH);
        }
        return;
    }

    while ((d = readdir(dh)) != NULL) {
        ESP_LOGI(TAG, "%s\n", d->d_name);
        btn = lv_btn_create(cont_col);
        lv_obj_set_width(btn, lv_pct(100));
        label = lv_label_create(btn);
        lv_label_set_text(label, d->d_name);
        lv_obj_center(label);
        lv_group_add_obj(show_passwords_input_group, btn);
    }
}
