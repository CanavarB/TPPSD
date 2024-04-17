#include "salt_table.h"

static const char* TAG = "SALT_TABLE";

salt_table_handle_t salt_table_init(const char* label) {
    const esp_partition_t* partition = esp_partition_get(
        esp_partition_find(ESP_PARTITION_TYPE_DATA, ESP_PARTITION_SUBTYPE_ANY, label));

    assert(partition != NULL);

    return partition;
}

esp_err_t salt_table_generate_random_table(salt_table_handle_t salt_table_handle) {
    if (salt_table_handle == NULL)
        return ESP_FAIL;
    ESP_ERROR_CHECK(esp_partition_erase_range(salt_table_handle, 0, salt_table_handle->size));
    ESP_LOGI(TAG, "Partition erased");

    uint8_t* buf = malloc(1024);
    for (uint32_t i = 0; i <= salt_table_handle->size - 1024; i += 1024) {
        esp_fill_random(buf, 1024);

        // ESP_LOG_BUFFER_HEX(TAG, &salt_item, sizeof(salt_item));
        ESP_ERROR_CHECK(esp_partition_write(salt_table_handle, i, buf, 1024));
    }
    free(buf);
    ESP_LOGI(TAG, "SALT_TABLE partition initialized.");
    return ESP_OK;
}

esp_err_t salt_table_get_item(salt_table_handle_t salt_table_handle, unsigned int index,
                              uint8_t* salt) {
    if (salt_table_handle == NULL)
        return ESP_FAIL;
    return esp_partition_read(salt_table_handle, index, salt, sizeof(uint8_t));
}

esp_err_t salt_table_get_item_index_list(salt_table_handle_t salt_table_handle,
                                         uint32_t* index_list, uint8_t index_list_size) {
    if (index_list == NULL) {
        ESP_LOGE(TAG, "No index list");
        return ESP_ERR_INVALID_ARG;
    }
    uint32_t rand;
    bool isDiff;

    for (int8_t i = 0; i < index_list_size; i++) {
        isDiff = false;
        while (!isDiff) {
            isDiff = true;
            rand = esp_random() % salt_table_handle->size;
            for (int8_t j = i - 1; j >= 0; j--) {
                if (index_list[j] == rand) {
                    isDiff = false;
                    break;
                }
            }
        }
        index_list[i] = rand;
    }
    return ESP_OK;
}