#include "register_screen.h"
#include "main_menu_screen.h"
#include "mbedtls/sha256.h"
#include "sha/sha_parallel_engine.h"
#include <string.h>

static const char* TAG = "REGISTER_SCREEN";
extern lv_indev_t* rotary_indev;

lv_obj_t* register_scr;
lv_group_t* register_input_group;
lv_obj_t* roller;
lv_obj_t* passwd_label;

typedef struct {
    char buf[3];
    int8_t dir;
} prev_status;

prev_status prev;

typedef struct {
    char password[24];
    int8_t index;
} password_input;

password_input passwd_input;

static void roller_event_handler(lv_event_t* e);

void register_screen_init() {

    register_scr = lv_obj_create(NULL);
    register_input_group = lv_group_create();
    static lv_style_t style;
    lv_style_init(&style);
    passwd_label = lv_label_create(register_scr);
    lv_label_set_text(passwd_label, "Password?");
    lv_label_set_long_mode(passwd_label, LV_LABEL_LONG_CLIP);
    lv_obj_set_size(passwd_label, lv_pct(100), lv_pct(80));
    lv_obj_align(passwd_label, LV_ALIGN_TOP_MID, 0, 0);

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
}

void register_screen_load() {
    strcpy(prev.buf, "5\n");
    prev.dir = 0;
    passwd_input.index = 0;
    memset(passwd_input.password, 0, 24);
    lv_roller_set_selected(roller, 4, LV_ANIM_ON);

    lv_indev_set_group(rotary_indev, register_input_group);
    lv_scr_load(register_scr);
}

static void roller_event_handler(lv_event_t* e) {
    lv_event_code_t code = lv_event_get_code(e);
    // ESP_LOGI(TAG, "%d", code);
    lv_obj_t* obj = lv_event_get_target(e);
    if (code == LV_EVENT_KEY) {
        char buf[32];
        lv_roller_get_selected_str(obj, buf, sizeof(buf));
        int cmp = strcmp(buf, prev.buf);
        if (cmp < 0) {
            if (prev.dir > 0) {
                strcpy(passwd_input.password + passwd_input.index, prev.buf);
                passwd_input.index++;
                lv_label_set_text(passwd_label, prev.buf);
                ESP_LOGI(TAG, "%s", prev.buf);
            }
            prev.dir = -1;

        } else if (cmp > 0) {
            if (prev.dir < 0) {
                strcpy(passwd_input.password + passwd_input.index, prev.buf);
                passwd_input.index++;
                lv_label_set_text(passwd_label, prev.buf);
                ESP_LOGI(TAG, "%s", prev.buf);
            }
            prev.dir = 1;
        }
        strcpy(prev.buf, buf);

    } else if (code == LV_EVENT_CLICKED) {
        ESP_LOGI(TAG, "DEFOCUS");
        char buf[32];
        lv_roller_get_selected_str(obj, buf, sizeof(buf));
        strcpy(passwd_input.password + passwd_input.index, buf);
        passwd_input.index++;
        uint8_t hash[32];
        esp_sha(SHA2_256, (uint8_t*)passwd_input.password, 24, hash);
        uint8_t hash2[32];
        nvs_get_dev_passwd(hash2);
        ESP_LOG_BUFFER_HEX(TAG, hash, 32);
        ESP_LOGI(TAG, "%s", passwd_input.password);
        if (memcmp(hash, hash2, 32) == 0) {
            main_menu_screen_load();
        } else {
            ESP_LOGE(TAG, "WRONG PASSWD");
            lv_label_set_text(passwd_label, "Wrong Password!");
            register_screen_load();
        }
    }
}