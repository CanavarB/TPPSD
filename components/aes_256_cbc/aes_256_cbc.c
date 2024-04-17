#include "aes_256_cbc.h"

static const char* TAG = "AES_256_CBC";

static esp_aes_context* ctxAES;

void aes_356_cbc_init(void) {
    ESP_LOGI(TAG, "============AES 256 CBC============");

    esp_aes_init(ctxAES);
}

void aes_256_cbc_encrypt(const aes_in_out_t* input, const uint8_t* key, const uint8_t* iv,
                         aes_in_out_t* output) {
    uint8_t iv_copy[16] = {0};
    memcpy(iv_copy, iv, 16);

    uint8_t padded_input_len = ((input->size / 16 + 1) * 16);
    int mod16 = input->size % 16;
    uint8_t* padded_input = (uint8_t*)malloc(padded_input_len);
    if (!padded_input) {
        ESP_LOGE(TAG, "Failed to allocate memory");
        exit(-1);
    }

    memcpy(padded_input, input->data, input->size);
    uint8_t pkc5_value = (16 - mod16);
    for (size_t i = input->size; i < padded_input_len; i++) {
        padded_input[i] = pkc5_value;
    }

    output->size = padded_input_len;
    output->data = (uint8_t*)malloc(output->size);
    if (!output->data) {
        ESP_LOGE(TAG, "Failed to allocate memory");
        exit(-1);
    }
    esp_aes_setkey(ctxAES, key, 256);
    esp_aes_crypt_cbc(ctxAES, ESP_AES_ENCRYPT, output->size, iv_copy, (unsigned char*)padded_input,
                      output->data);

    free(padded_input);
}

void aes_256_cbc_decrypt(const aes_in_out_t* input, const uint8_t* key, const uint8_t* iv,
                         aes_in_out_t* output) {
    uint8_t iv_copy[16] = {0};
    memcpy(iv_copy, iv, 16);

    esp_aes_setkey(ctxAES, key, 256);
    output->size = input->size;
    output->data = (uint8_t*)malloc(output->size);
    if (!output->data) {
        ESP_LOGE(TAG, "Failed to allocate memory");
        exit(-1);
    }
    esp_aes_crypt_cbc(ctxAES, MBEDTLS_AES_DECRYPT, output->size, iv_copy, input->data,
                      output->data);

    uint8_t padded_output_len = output->size - output->data[output->size - 1];
    uint8_t* padded_output = (uint8_t*)malloc(padded_output_len);
    if (!padded_output) {
        ESP_LOGE(TAG, "Failed to allocate memory");
        exit(-1);
    }
    memcpy(padded_output, output->data, padded_output_len);

    output->size = padded_output_len;
    free(output->data);
    output->data = padded_output;
}
