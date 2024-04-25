set(requires main nvs_flash)
idf_component_register(SRCS "TPPSD_nvs.c"
                    INCLUDE_DIRS "include"
                    REQUIRES "${requires}")
