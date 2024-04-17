set(requires esp_partition)

idf_component_register(SRCS "salt_table.c"
                    INCLUDE_DIRS "include"
                    REQUIRES "${requires}")
