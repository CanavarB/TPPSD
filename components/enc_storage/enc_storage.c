#include "enc_storage.h"

static const char* TAG = "ENC_STORAGE";

esp_err_t enc_storage_init(const char* base_path, const char* label) {
    ESP_LOGI(TAG, "============ENC STORAGE============");

    esp_vfs_spiffs_conf_t spiffs_conf;
    spiffs_conf.base_path = base_path;
    spiffs_conf.partition_label = label;
    spiffs_conf.max_files = 5;
    spiffs_conf.format_if_mount_failed = true;

    esp_err_t ret = esp_vfs_spiffs_register(&spiffs_conf);

    if (ret != ESP_OK) {
        if (ret == ESP_FAIL) {
            ESP_LOGE(TAG, "Failed to mount or format filesystem");
        } else if (ret == ESP_ERR_NOT_FOUND) {
            ESP_LOGE(TAG, "Failed to find SPIFFS partition");
        } else {
            ESP_LOGE(TAG, "Failed to initialize SPIFFS (%s)", esp_err_to_name(ret));
        }
        return ret;
    }
    ESP_LOGI(TAG, "SPIFFS partition initialized.");
    return ESP_OK;
}