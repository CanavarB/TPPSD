#pragma once
#include "esp_log.h"
#include "esp_partition.h"
#include "esp_random.h"
#include <string.h>

typedef const esp_partition_t* salt_table_handle_t;

salt_table_handle_t salt_table_init(const char* label);
esp_err_t salt_table_generate_random_table(salt_table_handle_t salt_table_handle);
esp_err_t salt_table_get_item(salt_table_handle_t salt_table_handle, unsigned int index,
                              uint8_t* salt);