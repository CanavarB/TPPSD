#include "salt_table.h"
#include "bootloader_random.h"
#include "esp_log.h"
#include "esp_random.h"
#include <string.h>

#define SALT_BUFFER_MAX_SIZE (uint32_t)1 << 15
#define SALT_BUFFER_MIN_SIZE (uint32_t)1 << 10

static const char* TAG = "SALT_TABLE";

salt_table_handle_t salt_table_init(const char* label) {
    ESP_LOGI(TAG, "============SALT TABLE============");

    const esp_partition_t* partition = esp_partition_get(
        esp_partition_find(ESP_PARTITION_TYPE_DATA, ESP_PARTITION_SUBTYPE_ANY, label));

    assert(partition != NULL);
    assert(partition->size >= SALT_BUFFER_MAX_SIZE);
    assert(partition->size % SALT_BUFFER_MIN_SIZE == 0);

    return partition;
}

esp_err_t salt_table_generate_random_table(salt_table_handle_t salt_table_handle) {
    if (salt_table_handle == NULL)
        return ESP_FAIL;
    ESP_ERROR_CHECK(esp_partition_erase_range(salt_table_handle, 0, salt_table_handle->size));
    ESP_LOGI(TAG, "Partition erased");

    bootloader_random_enable();

    uint32_t buffer_size = SALT_BUFFER_MAX_SIZE;
    uint8_t* buf = malloc(buffer_size);
    while (buf == NULL && buffer_size >= SALT_BUFFER_MIN_SIZE) {
        buffer_size = buffer_size >> 1;
        buf = malloc(buffer_size);
    }
    assert(buf != NULL);
    // ESP_LOGI(TAG, "Buffer size %" PRId32, buffer_size);

    for (uint32_t i = 0; i <= salt_table_handle->size - buffer_size; i += buffer_size) {
        esp_fill_random(buf, buffer_size);
        // ESP_LOGI(TAG, "Index:%" PRId32, i);
        // ESP_LOG_BUFFER_HEX(TAG, &buf, buffer_size);
        ESP_ERROR_CHECK(esp_partition_write(salt_table_handle, i, buf, buffer_size));
    }
    free(buf);
    bootloader_random_disable();
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