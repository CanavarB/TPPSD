#pragma once
#include "esp_err.h"
#include "tinyusb.h"

typedef tinyusb_config_t usb_keyboard_handle_t;

usb_keyboard_handle_t usb_keyboard_init();
esp_err_t usb_keyboard_send(usb_keyboard_handle_t* usb_keyboard_handle, char* text);