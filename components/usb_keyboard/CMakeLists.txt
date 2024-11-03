set(requires usb esp_tinyusb)
idf_component_register(
                    SRCS "usb_keyboard.c"
                    INCLUDE_DIRS "include"
                    REQUIRES "${requires}"
                    ) 