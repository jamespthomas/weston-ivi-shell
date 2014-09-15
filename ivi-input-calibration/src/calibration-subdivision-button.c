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

#include "calibration-util.h"
#include "calibration-subdivision.h"
#include "calibration-subdivision-button.h"

int
calibration_subdivision_button_parse_entry(
                struct calibration_subdivision_button *button_sub,
                const char *key, const char *val)
{
    int retval = 0;
    if (strcmp(key, "key code") == 0) {
        char *endptr;
        uint32_t tmp_code = strtol(val, &endptr, 10);
        if (endptr != val) {
            button_sub->key_code = tmp_code;
            retval = 1;
        } else if ((tmp_code = parse_key_code(val)) >= 0) {
            button_sub->key_code = tmp_code;
            retval = 1;
        } else {
            weston_log("%s: Failed to parse '%s' into a valid key "
                       "code\n", __FUNCTION__, val);
        }

    } else if (strcmp(key, "minimum repetition interval") == 0) {
        char *endptr;
        float tmp_interval = strtof(val, &endptr);
        if (endptr == val) {
            weston_log("%s: Failed to convert %s to float\n",
                       __FUNCTION__, val);
        } else {
            button_sub->minimum_repetition_interval = tmp_interval;
            retval = 1;
        }
    }

    return retval;
}

void
calibration_subdivision_button_handle_down(
                struct calibration_subdivision_button *button_sub,
                struct weston_seat *seat, int is_pointer, uint32_t button,
                uint32_t time, wl_fixed_t x, wl_fixed_t y)
{
    struct calibration_subdivision *sub = &button_sub->subdivision;
    int slot = is_pointer ? CALIBRATION_POINTER_SLOT : button;
    if (!calibration_subdivision_get_slot_pressed(sub, slot)) {

        if (time >= button_sub->last_time
                   + button_sub->minimum_repetition_interval * 1000) {
            struct weston_keyboard_grab *grab = &seat->keyboard->default_grab;

            grab->interface->key(grab, time, button_sub->key_code,
                                 WL_KEYBOARD_KEY_STATE_PRESSED);
        }
        calibration_subdivision_set_slot_pressed(sub, slot, 1);
    }
}

void
calibration_subdivision_button_handle_up(
                struct calibration_subdivision_button *button_sub,
                struct weston_seat *seat, int is_pointer, uint32_t button,
                uint32_t time, wl_fixed_t x, wl_fixed_t y)
{
    struct calibration_subdivision *sub = &button_sub->subdivision;
    int slot = is_pointer ? CALIBRATION_POINTER_SLOT : button;
    if (calibration_subdivision_get_slot_pressed(sub, slot)) {
        if ( time >= button_sub->last_time
                     + button_sub->minimum_repetition_interval * 1000) {
            struct weston_keyboard_grab *grab = &seat->keyboard->default_grab;

            grab->interface->key(grab, time, button_sub->key_code,
                                 WL_KEYBOARD_KEY_STATE_RELEASED);
            button_sub->last_time = time;
        }
        calibration_subdivision_set_slot_pressed(sub, slot, 0);
    }
}

void
calibration_subdivision_button_handle_motion(
                struct calibration_subdivision_button *button_sub,
                struct weston_seat *seat, int is_pointer, uint32_t button,
                uint32_t time, wl_fixed_t x, wl_fixed_t y)
{
}

void
calibration_subdivision_button_handle_enter(
                struct calibration_subdivision_button *button_sub,
                struct weston_seat *seat, int is_pointer, uint32_t button,
                uint32_t time, wl_fixed_t x, wl_fixed_t y)
{
}

void calibration_subdivision_button_handle_leave(
                struct calibration_subdivision_button *button_sub,
                struct weston_seat *seat, int is_pointer, uint32_t button,
                uint32_t time, wl_fixed_t x, wl_fixed_t y)
{
    struct calibration_subdivision *sub = &button_sub->subdivision;
    int slot = is_pointer ? CALIBRATION_POINTER_SLOT : button;
    if (calibration_subdivision_get_slot_pressed(sub, slot)) {
        struct weston_keyboard_grab *grab = &seat->keyboard->default_grab;
        /* Indicate a 'cancel', which is different from a release */
        grab->interface->key(grab, time, button_sub->key_code,
                             WL_KEYBOARD_KEY_STATE_PRESSED);
        calibration_subdivision_set_slot_pressed(sub, slot, 0);
    }
}
