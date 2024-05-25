#include "lvgl_task.h"

#include "esp_dma_utils.h"
#include "esp_timer.h"
#include "freertos/FreeRTOS.h"
// SCREENS
#include "insert_passwords_screen.h"
#include "main_menu_screen.h"
#include "register_screen.h"
#include "settings_screen.h"
#include "show_passwords_screen.h"

static const char* TAG = "LVGL_TASK";

// PRIVATE
static TaskHandle_t lvgl_task_handle;

static uint16_t* lvgl_draw_buffer;
static lv_disp_draw_buf_t disp_buf;
static esp_timer_handle_t lvgl_tick_timer;
static lv_indev_drv_t indev_drv;
lv_indev_t* rotary_indev;
static SemaphoreHandle_t lvgl_mux = NULL;

static void lvgl_flush_cb(lv_disp_drv_t* drv, const lv_area_t* area, lv_color_t* color_map);
static void increase_lvgl_tick(void* arg);
static bool lvgl_lock(int timeout_ms);
static void encoder_read(lv_indev_drv_t* drv, lv_indev_data_t* data);

void lvgl_task(void* arg) {

    ESP_LOGI(TAG, "Starting LVGL task");
    uint32_t task_delay_ms = LVGL_TASK_MAX_DELAY_MS;

    while (true) {
        // Lock the mutex due to the LVGL APIs are not thread-safe
        if (lvgl_lock(-1)) {
            task_delay_ms = lv_timer_handler();
            // Release the mutex
            xSemaphoreGiveRecursive(lvgl_mux);
        }
        if (task_delay_ms > LVGL_TASK_MAX_DELAY_MS) {
            task_delay_ms = LVGL_TASK_MAX_DELAY_MS;
        } else if (task_delay_ms < LVGL_TASK_MIN_DELAY_MS) {
            task_delay_ms = LVGL_TASK_MIN_DELAY_MS;
        }
        vTaskDelay(pdMS_TO_TICKS(task_delay_ms));
    }
}

// PUBLIC
lv_disp_drv_t disp_drv;

void start_lvgl_task() {
    //  LVGL INIT
    ESP_LOGI(TAG, "Initialize LVGL library");
    lv_init();

    lvgl_draw_buffer =
        (uint16_t*)heap_caps_malloc(DISPLAY_BUFFER_SIZE * sizeof(uint16_t), MALLOC_CAP_DMA);
    assert(lvgl_draw_buffer);

    // initialize LVGL draw buffers
    lv_disp_draw_buf_init(&disp_buf, lvgl_draw_buffer, NULL, DISPLAY_BUFFER_SIZE);

    ESP_LOGI(TAG, "Register display driver to LVGL");
    lv_disp_drv_init(&disp_drv);
    disp_drv.hor_res = AMOLED_HEIGHT;
    disp_drv.ver_res = AMOLED_WIDTH;
    disp_drv.flush_cb = lvgl_flush_cb;
    disp_drv.draw_buf = &disp_buf;
    disp_drv.rotated = LV_DISP_ROT_90;
    lv_disp_drv_register(&disp_drv);

    ESP_LOGI(TAG, "Install LVGL tick timer");
    // Tick interface for LVGL (using esp_timer to generate 2ms periodic event)
    const esp_timer_create_args_t lvgl_tick_timer_args = {.callback = &increase_lvgl_tick,
                                                          .arg = NULL,
                                                          .dispatch_method = ESP_TIMER_TASK,
                                                          .name = "lvgl_tick",
                                                          .skip_unhandled_events = false};

    ESP_ERROR_CHECK(esp_timer_create(&lvgl_tick_timer_args, &lvgl_tick_timer));
    ESP_ERROR_CHECK(esp_timer_start_periodic(lvgl_tick_timer, LVGL_TICK_PERIOD_MS * 1000));

    lv_indev_drv_init(&indev_drv);
    indev_drv.type = LV_INDEV_TYPE_ENCODER;
    indev_drv.read_cb = encoder_read;
    indev_drv.long_press_time = 1000;
    rotary_indev = lv_indev_drv_register(&indev_drv);

    lvgl_mux = xSemaphoreCreateRecursiveMutex();
    assert(lvgl_mux);

    ESP_LOGI(TAG, "Display LVGL");

    if (lvgl_lock(-1)) {
        register_screen_init();
        main_menu_screen_init();
        insert_passwords_screen_init();
        show_passwords_screen_init();
        settings_screen_init();

        register_screen_load();
        // register_screen_init();
        // main_menu_screen_load();
        /*settings_screen_load();
        main_menu_screen_load();
        insert_passwords_screen_load();
        show_passwords_screen_load(); */
        xSemaphoreGiveRecursive(lvgl_mux);
    }

    ESP_LOGI(TAG, "Create LVGL task");
    xTaskCreatePinnedToCore(lvgl_task, "LVGL", LVGL_TASK_STACK_SIZE, NULL, tskIDLE_PRIORITY,
                            &lvgl_task_handle, APP_CPU_NUM);
}
void finish_lvgl_task() {
    lv_deinit();
    esp_timer_stop(lvgl_tick_timer);
    esp_timer_delete(lvgl_tick_timer);
    free(lvgl_draw_buffer);
    vTaskDelete(lvgl_task_handle);
}

static void lvgl_flush_cb(lv_disp_drv_t* drv, const lv_area_t* area, lv_color_t* color_map) {

    esp_lcd_panel_draw_bitmap(panel.esp_lcd_panel_handle, area->x1, area->y1, area->x2 + 1,
                              area->y2 + 1, (uint16_t*)color_map);
    lv_disp_flush_ready(&disp_drv);
}
static void increase_lvgl_tick(void* arg) {
    /* Tell LVGL how many milliseconds has elapsed */
    lv_tick_inc(LVGL_TICK_PERIOD_MS);
}
static bool lvgl_lock(int timeout_ms) {
    // Convert timeout in milliseconds to FreeRTOS ticks
    // If `timeout_ms` is set to -1, the program will block until the condition
    // is met

    TickType_t timeout_ticks;
    if (timeout_ms == -1) {
        timeout_ticks = portMAX_DELAY;
    } else {
        timeout_ticks = pdMS_TO_TICKS(timeout_ms);
    }
    return xSemaphoreTakeRecursive(lvgl_mux, timeout_ticks) == pdTRUE;
}
static void encoder_read(lv_indev_drv_t* drv, lv_indev_data_t* data) {

    data->key = LV_KEY_ENTER;

    static uint16_t last_position;
    static rotary_encoder_event_t event;
    rotary_encoder_get_event(&rotary_encoder, &event);

    if (event.button_event == BUTTON_PRESS_DOWN || event.button_event == BUTTON_LONG_PRESS_START ||
        event.button_event == BUTTON_LONG_PRESS_HOLD) {
        data->state = LV_INDEV_STATE_PRESSED;
    } else {
        data->state = LV_INDEV_STATE_RELEASED;
        data->enc_diff = event.knob_count - last_position;
        last_position = event.knob_count;
    }
}
