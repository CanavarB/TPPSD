set(srcs 
    "main.c"
    )

set(tasks
    
)

set(screens
    "screens/register.c"
    "screens/main_menu.c"
    "screens/insert_passwords.c"
    "screens/show_passwords.c"
)

set (images 
    "images/usb.c"
    )

list(APPEND srcs ${images} ${tasks} ${screens})

set(requires
    rotary_encoder
    aes_256_cbc
    salt_table
    i80_st7789_panel
    enc_storage
    usb_msc
    )

set(includes
    "."
    )

idf_component_register(
    SRCS "${srcs}"
    INCLUDE_DIRS "${includes}"
    REQUIRES "${requires}")
