set(SCREENS_DIR "screens/")
set(TASKS_DIR "tasks/")
set(IMAGES_DIR "images/")


FILE(GLOB_RECURSE TASKS_SOURCE ${TASKS_DIR}*.c)
FILE(GLOB_RECURSE SCREEN_SOURCE ${SCREENS_DIR}*.c)
FILE(GLOB_RECURSE IMAGES_SOURCE ${IMAGES_DIR}*.c)

FILE(GLOB TASKS_INCLUDE_DIRS ${TASKS_DIR}*_task)
FILE(GLOB SCREEN_INCLUDE_DIRS ${SCREENS_DIR}*_screen)

set(srcs 
    "main.c"
    ${TASKS_SOURCE}
    ${SCREEN_SOURCE}
    ${IMAGES_SOURCE}
    )

set(includes
    "."
    ${TASKS_INCLUDE_DIRS}
    ${SCREEN_INCLUDE_DIRS}
    )

set(requires
    rotary_encoder
    aes_256_cbc
    salt_table
    i80_st7789_panel
    usb_msc
    usb_keyboard
    TPPSD_nvs
    TPPSD_spiffs
    mbedtls
    )

idf_component_register(
    SRCS "${srcs}"
    INCLUDE_DIRS "${includes}"
    REQUIRES "${requires}")
