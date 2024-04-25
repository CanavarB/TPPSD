#include "rotary_encoder.h"
#include "esp_check.h"
#include "esp_log.h"

static const char* TAG = "ROTARY_ENCODER";

esp_err_t button_init(rotary_encoder_handle_t* rotary_encoder_handle, gpio_num_t button_pin);
esp_err_t knob_init(rotary_encoder_handle_t* rotary_encoder_handle, gpio_num_t encoder_pin_a,
                    gpio_num_t encoder_pin_b);

rotary_encoder_handle_t rotary_encoder_init(gpio_num_t encoder_pin_a, gpio_num_t encoder_pin_b,
                                            gpio_num_t button_pin) {
    ESP_LOGI(TAG, "============ROTARY ENCODER============");

    ESP_ERROR_CHECK(gpio_install_isr_service(0));
    rotary_encoder_handle_t rotary_encoder_handle;
    if (button_pin != -1)
        ESP_ERROR_CHECK(button_init(&rotary_encoder_handle, button_pin));
    if (encoder_pin_a != -1 && encoder_pin_b != -1)
        ESP_ERROR_CHECK(knob_init(&rotary_encoder_handle, encoder_pin_a, encoder_pin_b));
    return rotary_encoder_handle;
}

esp_err_t button_init(rotary_encoder_handle_t* rotary_encoder_handle, gpio_num_t button_pin) {
    button_config_t gpio_btn_cfg = {
        .type = BUTTON_TYPE_GPIO,
        .long_press_time = CONFIG_BUTTON_LONG_PRESS_TIME_MS,
        .short_press_time = CONFIG_BUTTON_SHORT_PRESS_TIME_MS,
        .gpio_button_config =
            {
                .gpio_num = (int32_t)button_pin,
                .active_level = 0,
            },
    };

    button_handle_t button_handle = iot_button_create(&gpio_btn_cfg);
    if (button_handle == NULL) {
        ESP_LOGE(TAG, "Button create failed");
        return ESP_FAIL;
    }

    rotary_encoder_handle->button_handle = button_handle;
    return ESP_OK;
}

esp_err_t knob_init(rotary_encoder_handle_t* rotary_encoder_handle, gpio_num_t encoder_pin_a,
                    gpio_num_t encoder_pin_b) {
    knob_config_t cfg = {
        .default_direction = 0,
        .gpio_encoder_a = (uint8_t)encoder_pin_a,
        .gpio_encoder_b = (uint8_t)encoder_pin_b,
    };

    knob_handle_t knob_handle = iot_knob_create(&cfg);

    if (knob_handle == NULL) {
        ESP_LOGE(TAG, "knob create failed");
        return ESP_FAIL;
    }
    rotary_encoder_handle->knob_handle = knob_handle;
    return ESP_OK;
}

void rotary_encoder_get_event(rotary_encoder_handle_t* rotary_encoder_handle,
                              rotary_encoder_event_t* event) {
    if (rotary_encoder_handle->knob_handle != NULL) {
        event->knob_event = iot_knob_get_event(rotary_encoder_handle->knob_handle);
        event->knob_count = iot_knob_get_count_value(rotary_encoder_handle->knob_handle);
    }
    if (rotary_encoder_handle->button_handle != NULL) {
        event->button_event = iot_button_get_event(rotary_encoder_handle->button_handle);
    }
}