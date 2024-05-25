#include "insert_passwords_screen.h"
#include "fetch_passwords_task.h"
#include "main_menu_screen.h"

static const char* TAG = "INSERT_PASSWORDS_SCREEN";

LV_IMG_DECLARE(usb);

lv_obj_t* insert_passwords_scr;
lv_group_t* insert_passwords_input_group;

static void cancel_btn_event_cb(lv_event_t* e);

void insert_passwords_screen_init() {
    insert_passwords_scr = lv_obj_create(NULL);
    insert_passwords_input_group = lv_group_create();

    lv_obj_t* usb_symbol = lv_img_create(insert_passwords_scr);
    lv_img_set_src(usb_symbol, &usb);
    lv_obj_remove_style_all(usb_symbol);
    lv_obj_set_size(usb_symbol, 150, 150);
    lv_obj_align(usb_symbol, LV_ALIGN_TOP_MID, 0, 50);

    lv_obj_t* cancel_btn = lv_btn_create(insert_passwords_scr);
    lv_obj_align(cancel_btn, LV_ALIGN_BOTTOM_MID, 0, -20);

    lv_obj_t* cancel_label = lv_label_create(cancel_btn);
    lv_label_set_text(cancel_label, "Cancel");
    lv_obj_center(cancel_label);

    lv_obj_add_event_cb(cancel_btn, cancel_btn_event_cb, LV_EVENT_CLICKED, NULL);
    lv_group_add_obj(insert_passwords_input_group, cancel_btn);
}

void insert_passwords_screen_load() {

    lv_indev_set_group(rotary_indev, insert_passwords_input_group);
    lv_scr_load(insert_passwords_scr);
    usb_msc_start(&usb_msc);
}

static void cancel_btn_event_cb(lv_event_t* e) {
    ESP_LOGI(TAG, "CANCEL PRESSED");
    usb_msc_stop(&usb_msc);
    start_fetch_passwords_task();
    main_menu_screen_load();
}