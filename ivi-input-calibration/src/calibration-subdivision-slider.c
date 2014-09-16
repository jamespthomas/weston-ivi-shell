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

#include "calibration-util.h"
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

static void
send_slider_axis(struct calibration_subdivision_slider *slider_sub,
                 struct weston_seat *seat, uint32_t time,
                 wl_fixed_t x, wl_fixed_t y)
{
    float axis_value, slider_position;
    if (slider_sub->orientation == CALIBRATION_SLIDER_HORIZONTAL) {
        uint32_t min = slider_sub->subdivision.top_left_x;
        uint32_t max = slider_sub->subdivision.bottom_right_x;
        slider_position = (wl_fixed_to_double(x) - min) / (float) (max - min);
    } else if (slider_sub->orientation == CALIBRATION_SLIDER_VERTICAL) {
        uint32_t min = slider_sub->subdivision.top_left_y;
        uint32_t max = slider_sub->subdivision.bottom_right_y;
        /* Vertical sliders are maximum-value at the top,
         * minimum-value at the bottom */
        slider_position = 1.0f - ((wl_fixed_to_double(y) - min)
                                 / (float) (max - min));
    } else {
        weston_log("%s: Slider %s has unknown orientation '%d'\n",
                   __FUNCTION__, slider_sub->subdivision.name,
                   slider_sub->orientation);
	return;
    }
    axis_value = slider_position * (slider_sub->maximum_value
                                    - slider_sub->minimum_value);
    notify_axis(seat, time, slider_sub->subdivision.id,
                wl_fixed_from_double(axis_value));
}

void
calibration_subdivision_slider_handle_down(
                struct calibration_subdivision_slider *slider_sub,
                struct weston_seat *seat, int is_pointer, uint32_t button,
                uint32_t time, wl_fixed_t x, wl_fixed_t y)
{
    struct calibration_subdivision *sub = &slider_sub->subdivision;
    int slot = is_pointer ? CALIBRATION_POINTER_SLOT : button;
    if (!calibration_subdivision_get_slot_pressed(sub, slot)) {
        struct weston_pointer_grab *grab = &seat->pointer->default_grab;
        grab->interface->button(grab, time, sub->id,
                                WL_POINTER_BUTTON_STATE_PRESSED);

        send_slider_axis(slider_sub, seat, time, x, y);
        calibration_subdivision_set_slot_pressed(sub, slot, 1);
    }
}

void
calibration_subdivision_slider_handle_up(
                struct calibration_subdivision_slider *slider_sub,
                struct weston_seat *seat, int is_pointer, uint32_t button,
                uint32_t time, wl_fixed_t x, wl_fixed_t y)
{
    struct calibration_subdivision *sub = &slider_sub->subdivision;
    int slot = is_pointer ? CALIBRATION_POINTER_SLOT : button;
    if (calibration_subdivision_get_slot_pressed(sub, slot)) {
        struct weston_pointer_grab *grab = &seat->pointer->default_grab;
        grab->interface->button(grab, time, sub->id,
                                WL_POINTER_BUTTON_STATE_RELEASED);
        calibration_subdivision_set_slot_pressed(sub, slot, 0);
    }
}

void
calibration_subdivision_slider_handle_motion(
                struct calibration_subdivision_slider *slider_sub,
                struct weston_seat *seat, int is_pointer, uint32_t button,
                uint32_t time, wl_fixed_t x, wl_fixed_t y)
{
    send_slider_axis(slider_sub, seat, time, x, y);
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
    struct calibration_subdivision *sub = &slider_sub->subdivision;
    int slot = is_pointer ? CALIBRATION_POINTER_SLOT : button;
    if (calibration_subdivision_get_slot_pressed(sub, slot)) {
        struct weston_pointer_grab *grab = &seat->pointer->default_grab;
        grab->interface->button(grab, time, sub->id, 
                                WL_POINTER_BUTTON_STATE_RELEASED);
        calibration_subdivision_set_slot_pressed(sub, slot, 0);
    }
}
