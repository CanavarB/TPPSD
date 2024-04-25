#pragma once
#include <inttypes.h>

typedef struct {
    uint8_t* data;
    uint32_t size;
} aes_in_out_t;

void aes_356_cbc_init(void);
void aes_256_cbc_encrypt(const aes_in_out_t* input, const uint8_t* key, const uint8_t* iv,
                         aes_in_out_t* output);

void aes_256_cbc_decrypt(const aes_in_out_t* input, const uint8_t* key, const uint8_t* iv,
                         aes_in_out_t* output);