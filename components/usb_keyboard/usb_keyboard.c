#include "usb_keyboard.h"
#include "class/hid/hid_device.h"
#include "esp_log.h"
#include "esp_private/usb_phy.h"
#include <stdio.h>
#include <string.h>

static const char* TAG = "USB_MSC";
esp_err_t usb_keyboard_start(usb_keyboard_handle_t* usb_keyboard_handle);
esp_err_t usb_keyboard_stop(usb_keyboard_handle_t* usb_keyboard_handle);

#define TUSB_DESC_TOTAL_LEN (TUD_CONFIG_DESC_LEN + CFG_TUD_HID * TUD_HID_DESC_LEN)

const uint8_t hid_report_descriptor[] = {
    TUD_HID_REPORT_DESC_KEYBOARD(HID_REPORT_ID(HID_ITF_PROTOCOL_KEYBOARD))};

const char* hid_string_descriptor[5] = {
    // array of pointer to string descriptors
    (char[]){0x09, 0x04},    // 0: is supported language is English (0x0409)
    "TinyUSB",               // 1: Manufacturer
    "TinyUSB Device",        // 2: Product
    "123456",                // 3: Serials, should use chip ID
    "Example HID interface", // 4: HID
};

static const uint8_t hid_configuration_descriptor[] = {
    // Configuration number, interface count, string index, total length, attribute, power in mA
    TUD_CONFIG_DESCRIPTOR(1, 1, 0, TUSB_DESC_TOTAL_LEN, TUSB_DESC_CONFIG_ATT_REMOTE_WAKEUP, 100),

    // Interface number, string index, boot protocol, report descriptor len, EP In address, size &
    // polling interval
    TUD_HID_DESCRIPTOR(0, 4, false, sizeof(hid_report_descriptor), 0x81, 16, 10),
};

uint16_t tud_hid_get_report_cb(uint8_t instance, uint8_t report_id, hid_report_type_t report_type,
                               uint8_t* buffer, uint16_t reqlen) {
    (void)instance;
    (void)report_id;
    (void)report_type;
    (void)buffer;
    (void)reqlen;

    return 0;
}
uint8_t const* tud_hid_descriptor_report_cb(uint8_t instance) {
    // We use only one interface and one HID report descriptor, so we can ignore parameter
    // 'instance'
    return hid_report_descriptor;
}
void tud_hid_set_report_cb(uint8_t instance, uint8_t report_id, hid_report_type_t report_type,
                           uint8_t const* buffer, uint16_t bufsize) {}
usb_keyboard_handle_t usb_keyboard_init() {
    ESP_LOGI(TAG, "Initializing keyboard");
    const tinyusb_config_t tusb_cfg = {
        .device_descriptor = NULL,
        .string_descriptor = hid_string_descriptor,
        .string_descriptor_count = sizeof(hid_string_descriptor) / sizeof(hid_string_descriptor[0]),
        .external_phy = false,
        .configuration_descriptor = hid_configuration_descriptor,
    };

    return tusb_cfg;
}

esp_err_t usb_keyboard_send(usb_keyboard_handle_t* usb_keyboard_handle, char* text) {
    esp_err_t ret = usb_keyboard_start(usb_keyboard_handle);
    while (!tud_mounted()) {
        vTaskDelay(pdMS_TO_TICKS(10));
    }

    vTaskDelay(pdMS_TO_TICKS(150));
    /* if (ret != ESP_OK) {
        return ret;
    } */

    uint8_t keycode[6] = {0};
    uint8_t modifier = 0;
    for (uint8_t i = 0; text[i] != '\0'; i++) {

        if (text[i] >= 0x41 && text[i] <= 0x5A) { // UPPER CASE
            keycode[0] = text[i] - 0x3D;
            modifier = 2;
        } else if (text[i] >= 0x61 && text[i] <= 0x7A) { // LOWERCASE
            keycode[0] = text[i] - 0x5D;
            modifier = 0;
        } else if (text[i] >= 0x30 && text[i] <= 0x39) { // NUMBER
            keycode[0] = text[i] - 0x13;
            modifier = 0;
        }

        tud_hid_keyboard_report(HID_ITF_PROTOCOL_KEYBOARD, modifier, keycode);
        vTaskDelay(pdMS_TO_TICKS(50));
        tud_hid_keyboard_report(HID_ITF_PROTOCOL_KEYBOARD, 0, NULL);
        vTaskDelay(pdMS_TO_TICKS(50));
    }

    /* return usb_keyboard_stop(usb_keyboard_handle); */
    return ESP_OK;
}

esp_err_t usb_keyboard_start(usb_keyboard_handle_t* usb_keyboard_handle) {
    return tinyusb_driver_install(usb_keyboard_handle);
}

esp_err_t usb_keyboard_stop(usb_keyboard_handle_t* usb_keyboard_handle) {
    tinyusb_driver_uninstall();

    usb_phy_config_t phy_conf = {
        .controller = USB_PHY_CTRL_SERIAL_JTAG,
    };
    usb_phy_handle_t jtag_phy;
    return usb_new_phy(&phy_conf, &jtag_phy);
    return ESP_OK;
}
