#pragma once
#include "iot_button.h"
#include "iot_knob.h"

typedef struct {
    knob_event_t knob_event;
    int knob_count;
    button_event_t button_event;
} rotary_encoder_event_t;

typedef struct {
    button_handle_t button_handle;
    knob_handle_t knob_handle;
} rotary_encoder_handle_t;

rotary_encoder_handle_t rotary_encoder_init(gpio_num_t encoder_pin_a, gpio_num_t encoder_pin_b,
                                            gpio_num_t button_pin);
void rotary_encoder_get_event(rotary_encoder_handle_t* rotary_encoder_handle,
                              rotary_encoder_event_t* event);
