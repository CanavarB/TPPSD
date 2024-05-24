#include "usb_keyboard_task.h"

static const char* TAG = "USB_KEYBOARD_TASK";
static TaskHandle_t usb_keyboard_task_handle;

void usb_keyboard_task() {}

void start_usb_keyboard_task() {

    ESP_LOGI(TAG, "Create USB_KEYBOARD task");
    xTaskCreatePinnedToCore(usb_keyboard_task, "USB_KEYBOARD", USB_KEYBOARD_TASK_STACK_SIZE, NULL,
                            tskIDLE_PRIORITY, &usb_keyboard_task_handle, PRO_CPU_NUM);
}

void finish_usb_keyboard_task() {}