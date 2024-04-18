#pragma once
#include "driver/gpio.h"
#include "esp_check.h"
#include "esp_log.h"
#include "esp_partition.h"
#include "tinyusb.h"
#include "tusb_msc_storage.h"

typedef tinyusb_config_t usb_msc_handle_t;

usb_msc_handle_t usb_msc_init(const char* LABEL, const char* MOUNT_PATH);
esp_err_t usb_msc_start(usb_msc_handle_t*);
esp_err_t usb_msc_stop(usb_msc_handle_t*);
