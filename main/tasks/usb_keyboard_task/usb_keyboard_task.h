#pragma once
#include "main.h"

#define USB_KEYBOARD_TASK_STACK_SIZE (int)(4 * 1024)
#define USB_KEYBOARD_TASK_PRIORITY 2

void start_usb_keyboard_task();
void finish_usb_keyboard_task();