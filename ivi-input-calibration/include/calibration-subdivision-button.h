/***************************************************************************
 *
 * Copyright (c) 2014 Codethink Limited
 *
 *
 * Licensed under the Apache License, Version 2.0 (the "License");
 * you may not use this file except in compliance with the License.
 * You may obtain a copy of the License at
 *
 *      http://www.apache.org/licenses/LICENSE-2.0
 *
 * Unless required by applicable law or agreed to in writing, software
 * distributed under the License is distributed on an "AS IS" BASIS,
 * WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
 * See the License for the specific language governing permissions and
 * limitations under the License.
 *
 ****************************************************************************/

#ifndef CALIBRATION_SUBDIVISION_BUTTON_H_INCLUDED
#define CALIBRATION_SUBDIVISION_BUTTON_H_INCLUDED

#include <wayland-util.h>

#include "calibration-subdivision.h"

struct calibration_subdivision_button {
    /* Parsed calibration */
    struct calibration_subdivision subdivision;
    uint32_t key_code;
    float minimum_repetition_interval;

    /* State */
    uint32_t last_time;
};

int
calibration_subdivision_button_parse_entry(
                struct calibration_subdivision_button *button_sub,
                const char *key, const char *val);

void
calibration_subdivision_button_handle_down(
                struct calibration_subdivision_button *button_sub,
                struct weston_seat *seat, int is_pointer, uint32_t button,
                uint32_t time, wl_fixed_t x, wl_fixed_t y);

void
calibration_subdivision_button_handle_up(
                struct calibration_subdivision_button *button_sub,
                struct weston_seat *seat, int is_pointer, uint32_t button,
                uint32_t time, wl_fixed_t x, wl_fixed_t y);

void
calibration_subdivision_button_handle_motion(
                struct calibration_subdivision_button *button_sub,
                struct weston_seat *seat, int is_pointer, uint32_t button,
                uint32_t time, wl_fixed_t x, wl_fixed_t y);

void
calibration_subdivision_button_handle_enter(
                struct calibration_subdivision_button *button_sub,
                struct weston_seat *seat, int is_pointer, uint32_t button,
                uint32_t time, wl_fixed_t x, wl_fixed_t y);

void calibration_subdivision_button_handle_leave(
                struct calibration_subdivision_button *button_sub,
                struct weston_seat *seat, int is_pointer, uint32_t button,
                uint32_t time, wl_fixed_t x, wl_fixed_t y);
#endif
