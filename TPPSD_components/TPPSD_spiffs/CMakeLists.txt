set(requires main spiffs)
idf_component_register(SRCS "TPPSD_spiffs.c"
                    INCLUDE_DIRS "include"
                    REQUIRES "${requires}")
