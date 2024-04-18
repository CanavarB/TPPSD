#include "usb_msc.h"
#include "esp_private/usb_phy.h"

static const char* TAG = "USB_MSC";

#define TUSB_DESC_TOTAL_LEN (TUD_CONFIG_DESC_LEN + TUD_MSC_DESC_LEN)
enum { ITF_NUM_MSC = 0, ITF_NUM_TOTAL };
enum {
    EDPT_CTRL_OUT = 0x00,
    EDPT_CTRL_IN = 0x80,

    EDPT_MSC_OUT = 0x01,
    EDPT_MSC_IN = 0x81,
};
static tusb_desc_device_t descriptor_config = {
    .bLength = sizeof(descriptor_config),
    .bDescriptorType = TUSB_DESC_DEVICE,
    .bcdUSB = 0x0200,
    .bDeviceClass = TUSB_CLASS_MISC,
    .bDeviceSubClass = MISC_SUBCLASS_COMMON,
    .bDeviceProtocol = MISC_PROTOCOL_IAD,
    .bMaxPacketSize0 = CFG_TUD_ENDPOINT0_SIZE,
    .idVendor = 0x303A, // This is Espressif VID. This needs to be changed
                        // according to Users / Customers
    .idProduct = 0x4002,
    .bcdDevice = 0x100,
    .iManufacturer = 0x01,
    .iProduct = 0x02,
    .iSerialNumber = 0x03,
    .bNumConfigurations = 0x01};

static char const* string_desc_arr[] = {
    (const char[]){0x09, 0x04}, // 0: is supported language is English (0x0409)
    "CB",                       // 1: Manufacturer
    "TPPSD",                    // 2: Product
    "1",                        // 3: Serials
    "TPPSD_MSC",                // 4. MSC
};

static uint8_t const desc_configuration[] = {
    // Config number, interface count, string index, total length, attribute,
    // power in mA
    TUD_CONFIG_DESCRIPTOR(1, ITF_NUM_TOTAL, 0, TUSB_DESC_TOTAL_LEN,
                          TUSB_DESC_CONFIG_ATT_REMOTE_WAKEUP, 100),

    // Interface number, string index, EP Out & EP In address, EP size
    TUD_MSC_DESCRIPTOR(ITF_NUM_MSC, 0, EDPT_MSC_OUT, EDPT_MSC_IN, TUD_OPT_HIGH_SPEED ? 512 : 64),
};

usb_msc_handle_t usb_msc_init(const char* LABEL, const char* MOUNT_PATH) {
    ESP_LOGI(TAG, "Initializing storage...");
    wl_handle_t wl_handle = WL_INVALID_HANDLE;

    ESP_LOGI(TAG, "Initializing wear levelling");
    const esp_partition_t* data_partition = esp_partition_get(
        esp_partition_find(ESP_PARTITION_TYPE_DATA, ESP_PARTITION_SUBTYPE_DATA_FAT, LABEL));

    assert(data_partition);

    ESP_ERROR_CHECK(wl_mount(data_partition, &wl_handle));

    const tinyusb_msc_spiflash_config_t config_spi = {
        .wl_handle = wl_handle,
        //.callback_mount_changed = storage_mount_changed_cb,
        .mount_config.max_files = 5,
    };
    ESP_ERROR_CHECK(tinyusb_msc_storage_init_spiflash(&config_spi));

    ESP_LOGI(TAG, "Mount storage...");
    ESP_ERROR_CHECK(tinyusb_msc_storage_mount(MOUNT_PATH));

    ESP_LOGI(TAG, "USB MSC initialization DONE");
    tinyusb_config_t tusb_cfg;

    tusb_cfg.device_descriptor = &descriptor_config;
    tusb_cfg.string_descriptor = string_desc_arr;
    tusb_cfg.string_descriptor_count = sizeof(string_desc_arr) / sizeof(string_desc_arr[0]);
    tusb_cfg.external_phy = false;
    tusb_cfg.configuration_descriptor = desc_configuration;

    return tusb_cfg;
}

esp_err_t usb_msc_start(usb_msc_handle_t* usb_msc_handle) {
    return tinyusb_driver_install(usb_msc_handle);
}
esp_err_t usb_msc_stop(usb_msc_handle_t* usb_msc_handle) {
    tinyusb_driver_uninstall();

    usb_phy_config_t phy_conf = {
        .controller = USB_PHY_CTRL_SERIAL_JTAG,
    };
    usb_phy_handle_t jtag_phy;
    return usb_new_phy(&phy_conf, &jtag_phy);
}
