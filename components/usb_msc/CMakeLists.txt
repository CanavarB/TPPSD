set(requires fatfs wear_levelling usb esp_tinyusb)
idf_component_register(
                    SRCS "usb_msc.c"
                    INCLUDE_DIRS "include"
                    REQUIRES "${requires}"
                    ) 
