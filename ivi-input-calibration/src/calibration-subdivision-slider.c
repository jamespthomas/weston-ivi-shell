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

#include <string.h>

#include <weston/compositor.h>

#include "calibration-subdivision-slider.h"

int
calibration_subdivision_slider_parse_entry(
                struct calibration_subdivision_slider *slider_sub,
                const char *key, const char *val)
{
    int retval = 0;
    if (strcmp(key, "orientation") == 0) {
        if (strcmp(val, "horizontal") == 0) {
            slider_sub->orientation = CALIBRATION_SLIDER_HORIZONTAL;
            retval = 1;
        } else if (strcmp(val, "vertical") == 0) {
            slider_sub->orientation = CALIBRATION_SLIDER_VERTICAL;
            retval = 1;
        } else {
            weston_log("%s: Unexpected slider orientation '%s'\n",
                       __FUNCTION__, val);
        }
    } else if (strcmp(key, "minimum value") == 0) {
        char *endptr;
        float tmp_min = strtof(val, &endptr);
        if (endptr == val) {
            weston_log("%s: Failed to convert '%s' to float\n",
                       __FUNCTION__, val);
        } else {
            slider_sub->minimum_value = tmp_min;
            retval = 1;
        }
    } else if (strcmp(key, "maximum value") == 0) {
        char *endptr;
        float tmp_max = strtof(val, &endptr);
        if (endptr == val) {
            weston_log("%s: Failed to convert '%s' to float\n",
                       __FUNCTION__, val);
        } else {
            slider_sub->maximum_value = tmp_max;
            retval = 1;
        }
    }

    return retval;
}

void
calibration_subdivision_slider_handle_down(
                struct calibration_subdivision_slider *slider_sub,
                struct weston_seat *seat, int is_pointer, uint32_t button,
                uint32_t time, wl_fixed_t x, wl_fixed_t y)
{
}

void
calibration_subdivision_slider_handle_up(
                struct calibration_subdivision_slider *slider_sub,
                struct weston_seat *seat, int is_pointer, uint32_t button,
                uint32_t time, wl_fixed_t x, wl_fixed_t y)
{
}

void
calibration_subdivision_slider_handle_motion(
                struct calibration_subdivision_slider *slider_sub,
                struct weston_seat *seat, int is_pointer, uint32_t button,
                uint32_t time, wl_fixed_t x, wl_fixed_t y)
{
}

void
calibration_subdivision_slider_handle_enter(
                struct calibration_subdivision_slider *slider_sub,
                struct weston_seat *seat, int is_pointer, uint32_t button,
                uint32_t time, wl_fixed_t x, wl_fixed_t y)
{
}

void calibration_subdivision_slider_handle_leave(
                struct calibration_subdivision_slider *slider_sub,
                struct weston_seat *seat, int is_pointer, uint32_t button,
                uint32_t time, wl_fixed_t x, wl_fixed_t y)
{
}
