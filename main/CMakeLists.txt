set(requires
    fatfs
    wear_levelling
    esp_lcd
    rotary_encoder
    aes_256_cbc
    salt_table
    i80_st7789_panel
    enc_storage
    )

set(srcs 
    "main.c"
    "usb_msc/usb_msc.c"
    "screens/register.c"
    "screens/main_menu.c"
    "screens/insert_passwords.c"
    "screens/show_passwords.c"
    "images/usb.c"
    )

set(includes
    "."
    )

idf_component_register(
    SRCS "${srcs}"
    INCLUDE_DIRS "${includes}"
    REQUIRES "${requires}")
