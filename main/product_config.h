/**
 * @file      product_pins.h
 * @author    Lewis He (lewishe@outlook.com)
 * @license   MIT
 * @copyright Copyright (c) 2024  Shenzhen Xinyuan Electronic Technology Co.,
 * Ltd
 * @date      2024-01-07
 *
 */

#pragma once

#include <sdkconfig.h>

#define ROT_ENC_A_GPIO (gpio_num_t)(21)
#define ROT_ENC_B_GPIO (gpio_num_t)(17)
#define ROT_ENC_BUTTON_GPIO (gpio_num_t)(16)

#define USB_MSC_LABEL "msc"
#define USB_MSC_MOUNT_PATH "/msc"
#define PASSWD_FILE_NAME "/passwords.csv"
#define PASSWD_FILE_PATH USB_MSC_MOUNT_PATH PASSWD_FILE_NAME

#define STORAGE_LABEL "storage"
#define STORAGE_MOUNT_PATH "/spiffs"

#define SALT_TABLE_LABEL "salt_table"
#define SALT_TABLE_SIZE (1 * 1024 * 1024)
#define SALT_SIZE (32)
#define SALT_NUM (SALT_TABLE_SIZE / SALT_SIZE)

#define NVS_DEVICE_PASSWORD_KEY "device_passwd"
#define NVS_IS_REGISTERED_KEY "is_registered"

#define BOARD_NONE_PIN (-1)

#define BOARD_POWERON (gpio_num_t)(15)

#define LCD_PIXEL_CLOCK_HZ (10 * 1000 * 1000)

#define BOARD_TFT_BL (gpio_num_t)(38)
#define BOARD_TFT_DATA0 (gpio_num_t)(39)
#define BOARD_TFT_DATA1 (gpio_num_t)(40)
#define BOARD_TFT_DATA2 (gpio_num_t)(41)
#define BOARD_TFT_DATA3 (gpio_num_t)(42)
#define BOARD_TFT_DATA4 (gpio_num_t)(45)
#define BOARD_TFT_DATA5 (gpio_num_t)(46)
#define BOARD_TFT_DATA6 (gpio_num_t)(47)
#define BOARD_TFT_DATA7 (gpio_num_t)(48)
#define BOARD_TFT_RST (gpio_num_t)(5)
#define BOARD_TFT_CS (gpio_num_t)(6)
#define BOARD_TFT_DC (gpio_num_t)(7)
#define BOARD_TFT_WR (gpio_num_t)(8)
#define BOARD_TFT_RD (gpio_num_t)(9)
#define BOARD_I2C_SCL (gpio_num_t)(17)
#define BOARD_I2C_SDA (gpio_num_t)(18)

#define AMOLED_WIDTH (170)
#define AMOLED_HEIGHT (320)

#define DISPLAY_BUFFER_SIZE (AMOLED_WIDTH * AMOLED_HEIGHT)
