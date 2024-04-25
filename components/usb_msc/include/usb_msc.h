#pragma once
#include "esp_err.h"
#include "tinyusb.h"

typedef tinyusb_config_t usb_msc_handle_t;

usb_msc_handle_t usb_msc_init(const char* LABEL, const char* MOUNT_PATH);
esp_err_t usb_msc_start(usb_msc_handle_t*);
esp_err_t usb_msc_stop(usb_msc_handle_t*);
