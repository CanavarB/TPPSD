#pragma once
#include "lvgl.h"
#include "main.h"

#define LVGL_TASK_STACK_SIZE (int)(4 * 1024)
#define LVGL_TASK_PRIORITY 2
#define LVGL_TICK_PERIOD_MS 2
#define LVGL_TASK_MAX_DELAY_MS 500
#define LVGL_TASK_MIN_DELAY_MS 1

extern lv_disp_drv_t disp_drv;
extern lv_indev_t* rotary_indev;

void start_lvgl_task();
void finish_lvgl_task();
