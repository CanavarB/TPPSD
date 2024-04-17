set(requires esp_lcd)
idf_component_register(SRCS "i80_st7789_panel.c"
                    INCLUDE_DIRS "include"
                    REQUIRES "${requires}")
