#include "TPPSD_nvs.h"

#include "esp_log.h"
#include "nvs_flash.h"
#include "product_config.h"

typedef uint8_t* dev_passwd;

static const char* TAG = "NVS";

size_t DEVICE_PASSWORD_LEN = 32;
static nvs_handle_t tppsd_nvs_handle;

esp_err_t nvs_init() {
    ESP_LOGI(TAG, "============NVS============");

    esp_err_t ret = nvs_flash_init();
    ESP_ERROR_CHECK(ret);
    ret = nvs_open(NVS_NAMESPACE, NVS_READWRITE, &tppsd_nvs_handle);
    if (ret != ESP_OK) {
        printf("Error (%s) opening NVS handle!\n", esp_err_to_name(ret));
    }
    return ret;
}

esp_err_t nvs_set_dev_passwd(uint8_t* read_buff) {
    return nvs_set_blob(tppsd_nvs_handle, NVS_KEY_DEVICE_PASSWORD, read_buff, DEVICE_PASSWORD_LEN);
}

esp_err_t nvs_get_dev_passwd(uint8_t* read_buff) {
    return nvs_get_blob(tppsd_nvs_handle, NVS_KEY_DEVICE_PASSWORD, read_buff, &DEVICE_PASSWORD_LEN);
}

esp_err_t nvs_set_stn_brig(PANEL_BRIGHTNESS_T* brightness) {
    return nvs_set_blob(tppsd_nvs_handle, NVS_KEY_SETTINGS_LAST_BRIGHNESS, brightness,
                        sizeof(PANEL_BRIGHTNESS_T));
}

esp_err_t nvs_get_stn_brig(PANEL_BRIGHTNESS_T* brightness) {
    size_t size = sizeof(PANEL_BRIGHTNESS_T);
    esp_err_t err =
        nvs_get_blob(tppsd_nvs_handle, NVS_KEY_SETTINGS_LAST_BRIGHNESS, brightness, &size);
    if (err == ESP_ERR_NVS_NOT_FOUND) {
        *brightness = PANEL_BRIGHTNESS_MAX;
        nvs_set_stn_brig(brightness);
    }
    return err;
}

esp_err_t nvs_save() { return nvs_commit(tppsd_nvs_handle); }

esp_err_t nvs_reset() {
    esp_err_t ret = nvs_flash_erase();
    ESP_ERROR_CHECK(ret);
    ret = nvs_init();
    return ret;
}
