#include "show_passwords_screen.h"
#include "fetch_passwords_task.h"
#include <dirent.h>
#include <errno.h>

static const char* TAG = "SHOW_PASSWORDS_SCREEN";

extern lv_indev_t* rotary_indev;

lv_group_t* show_passwords_input_group;
lv_obj_t* show_passwords_scr;

static void change_event_cb(lv_event_t* e);

void show_passwords_screen_init() {
    show_passwords_scr = lv_obj_create(NULL);
    show_passwords_input_group = lv_group_create();

    lv_obj_t* table = lv_table_create(show_passwords_scr);

    lv_obj_set_size(table, lv_pct(100), lv_pct(100));
    lv_obj_center(table);
    lv_table_set_col_width(table, 0, 150);
    lv_table_set_col_cnt(table, 1);

    struct dirent* d;
    DIR* dh = opendir(SPIFFS_BASE_PATH);
    if (!dh) {
        if (errno == ENOENT) {
            // If the directory is not found
            ESP_LOGE(TAG, "Directory doesn't exist %s", SPIFFS_BASE_PATH);
        } else {
            // If the directory is not readable then throw error and exit
            ESP_LOGE(TAG, "Unable to read directory %s", SPIFFS_BASE_PATH);
        }
        return;
    }

    uint8_t i = 0;
    while ((d = readdir(dh)) != NULL) {
        if (strstr(d->d_name, "Password")) {
            lv_table_set_cell_value_fmt(table, i, 0, "%s", d->d_name);
            i++;
        }
    }

    lv_obj_add_event_cb(table, change_event_cb, LV_EVENT_CLICKED, NULL);

    lv_group_add_obj(show_passwords_input_group, table);
}
void show_passwords_screen_load() {
    // start_fetch_passwords_task();
    lv_indev_set_group(rotary_indev, show_passwords_input_group);

    lv_scr_load(show_passwords_scr);
}

static void change_event_cb(lv_event_t* e) {
    ESP_LOGI(TAG, "AAA");
    lv_obj_t* obj = lv_event_get_target(e);
    uint16_t col;
    uint16_t row;
    lv_table_get_selected_cell(obj, &row, &col);
    FILE* password_file = fopen(strcat("/spiffs/", lv_table_get_cell_value(obj, row, col)), "r");
    if (password_file == NULL) {
        ESP_LOGE(TAG, "Unable to read: %s", lv_table_get_cell_value(obj, row, col));
    }

    char* password = malloc(65);

    fgets(password, 64, password_file);
    ESP_LOGI(TAG, "%s", password);
    usb_keyboard_send(&usb_keyboard, password);
    fclose(password_file);
    free(password);
}