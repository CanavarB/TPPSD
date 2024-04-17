set(requires spiffs)
idf_component_register(SRCS "enc_storage.c"
                    INCLUDE_DIRS "include"
                    REQUIRES "${requires}")
