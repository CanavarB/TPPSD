set(requires mbedtls)
idf_component_register(SRCS "aes_256_cbc.c"
                    INCLUDE_DIRS "include"
                    REQUIRES "${requires}")