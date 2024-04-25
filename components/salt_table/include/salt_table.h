#pragma once
#include "esp_partition.h"

typedef const esp_partition_t* salt_table_handle_t;

salt_table_handle_t salt_table_init(const char* label);
esp_err_t salt_table_generate_random_table(salt_table_handle_t salt_table_handle);
esp_err_t salt_table_get_item(salt_table_handle_t salt_table_handle, unsigned int index,
                              uint8_t* salt);
esp_err_t salt_table_get_item_index_list(salt_table_handle_t salt_table_handle,
                                         uint32_t* index_list, uint8_t index_list_size);