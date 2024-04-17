#pragma once
#include "esp_log.h"
#include "esp_spiffs.h"

esp_err_t enc_storage_init(const char* base_path, const char* label);