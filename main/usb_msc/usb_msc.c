#include "driver/gpio.h"
#include "esp_check.h"
#include "esp_log.h"
#include "esp_partition.h"
#include "product_config.h"
#include "tinyusb.h"
#include "tusb_msc_storage.h"

static const char* TAG = "USB_MSC";

#define EPNUM_MSC 1
#define TUSB_DESC_TOTAL_LEN (TUD_CONFIG_DESC_LEN + TUD_MSC_DESC_LEN)

enum { ITF_NUM_MSC = 0, ITF_NUM_TOTAL };

enum {
    EDPT_CTRL_OUT = 0x00,
    EDPT_CTRL_IN = 0x80,

    EDPT_MSC_OUT = 0x01,
    EDPT_MSC_IN = 0x81,
};
static uint8_t const desc_configuration[] = {
    // Config number, interface count, string index, total length, attribute,
    // power in mA
    TUD_CONFIG_DESCRIPTOR(1, ITF_NUM_TOTAL, 0, TUSB_DESC_TOTAL_LEN,
                          TUSB_DESC_CONFIG_ATT_REMOTE_WAKEUP, 100),

    // Interface number, string index, EP Out & EP In address, EP size
    TUD_MSC_DESCRIPTOR(ITF_NUM_MSC, 0, EDPT_MSC_OUT, EDPT_MSC_IN,
                       TUD_OPT_HIGH_SPEED ? 512 : 64),
};
static tusb_desc_device_t descriptor_config = {
    .bLength = sizeof(descriptor_config),
    .bDescriptorType = TUSB_DESC_DEVICE,
    .bcdUSB = 0x0200,
    .bDeviceClass = TUSB_CLASS_MISC,
    .bDeviceSubClass = MISC_SUBCLASS_COMMON,
    .bDeviceProtocol = MISC_PROTOCOL_IAD,
    .bMaxPacketSize0 = CFG_TUD_ENDPOINT0_SIZE,
    .idVendor = 0x303A,
    .idProduct = 0x4002,
    .bcdDevice = 0x100,
    .iManufacturer = 0x01,
    .iProduct = 0x02,
    .iSerialNumber = 0x03,
    .bNumConfigurations = 0x01};

static char const* string_desc_arr[] = {
    (const char[]){0x09, 0x04}, // 0: is supported language is English (0x0409)
    "TinyUSB",                  // 1: Manufacturer
    "TinyUSB Device",           // 2: Product
    "123456",                   // 3: Serials
    "Example MSC",              // 4. MSC
};

static const tinyusb_config_t tusb_cfg = {
    .device_descriptor = &descriptor_config,
    .string_descriptor = string_desc_arr,
    .string_descriptor_count =
        sizeof(string_desc_arr) / sizeof(string_desc_arr[0]),
    .external_phy = false,
    .configuration_descriptor = desc_configuration,
};

static void storage_mount_changed_cb(tinyusb_msc_event_t* event) {
    ESP_LOGI(TAG, "Storage mounted to application: %s",
             event->mount_changed_data.is_mounted ? "Yes" : "No");
}

static wl_handle_t wl_handle;
const esp_partition_t* data_partition;

esp_err_t usb_msc_init() {
    ESP_LOGI(TAG, "Initializing storage...");
    wl_handle = WL_INVALID_HANDLE;

    ESP_LOGI(TAG, "Initializing wear levelling");
    data_partition = esp_partition_get(
        esp_partition_find(ESP_PARTITION_TYPE_DATA,
                           ESP_PARTITION_SUBTYPE_DATA_FAT, USB_MSC_LABEL));

    if (data_partition == NULL) {
        ESP_LOGE(TAG,
                 "Failed to find FATFS partition. Check the partition table.");
        return ESP_ERR_NOT_FOUND;
    }
    ESP_ERROR_CHECK(wl_mount(data_partition, &wl_handle));

    const tinyusb_msc_spiflash_config_t config_spi = {
        .wl_handle = wl_handle,
        .callback_mount_changed = storage_mount_changed_cb,
        .mount_config.max_files = 5,
    };
    ESP_ERROR_CHECK(tinyusb_msc_storage_init_spiflash(&config_spi));
    ESP_ERROR_CHECK(tinyusb_msc_register_callback(
        TINYUSB_MSC_EVENT_MOUNT_CHANGED, storage_mount_changed_cb));

    ESP_LOGI(TAG, "Mount storage...");
    ESP_ERROR_CHECK(tinyusb_msc_storage_mount(USB_MSC_MOUNT_PATH));

    ESP_LOGI(TAG, "USB MSC initialization DONE");

    return ESP_OK;
}

esp_err_t usb_msc_start() {

    ESP_LOGI(TAG, "USB MSC Starting");

    return tinyusb_driver_install(&tusb_cfg);
}

esp_err_t usb_msc_stop() {
    ESP_LOGI(TAG, "USB MSC Stoping");
    return tinyusb_driver_uninstall();
}