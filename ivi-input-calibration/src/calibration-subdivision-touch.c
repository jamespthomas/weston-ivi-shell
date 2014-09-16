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
#include "calibration-subdivision-touch.h"

int
calibration_subdivision_touch_parse_entry(
                struct calibration_subdivision_touch *touch_sub,
                const char *key, const char *val)
{
    int retval = 0;
    if (strcmp(key, "display offset x") == 0) {
        char *endptr;
        uint32_t tmp_off = strtol(val, &endptr, 10);
        if (endptr == val) {
            weston_log("%s: Failed to convert '%s' to int\n",
                       __FUNCTION__, val);
        } else {
            touch_sub->display_offset_x = tmp_off;
            retval = 1;
        }
    } else if (strcmp(key, "display offset y") == 0) {
        char *endptr;
        uint32_t tmp_off = strtol(val, &endptr, 10);
        if (endptr == val) {
            weston_log("%s: Failed to convert '%s' to int\n",
                       __FUNCTION__, val);
        } else {
            touch_sub->display_offset_y = tmp_off;
            retval = 1;
        }
    }

    return retval;
}

void
calibration_subdivision_touch_handle_down(
                struct calibration_subdivision_touch *touch_sub,
                struct weston_seat *seat, int is_pointer, uint32_t button,
                uint32_t time, wl_fixed_t x, wl_fixed_t y)
{
    struct calibration_subdivision *sub = &touch_sub->subdivision;
    int slot = is_pointer ? CALIBRATION_POINTER_SLOT : button;
    if (!calibration_subdivision_get_slot_pressed(sub, button)) {
        if (is_pointer) {
            struct weston_pointer_grab *grab = &seat->pointer->default_grab;
            grab->interface->button(grab, time, button,
                                    WL_POINTER_BUTTON_STATE_PRESSED);
        } else {
            struct weston_touch_grab *grab = &seat->touch->default_grab;
            wl_fixed_t sx, sy;

            /* If there's no appropriate view, sx and sy would be junk data */
            if (seat->touch->focus)
                weston_view_from_global_fixed(seat->touch->focus,
                                              x, y, &sx, &sy);
            grab->interface->down(grab, time, button, sx, sy);
        }
        calibration_subdivision_set_slot_pressed(sub, slot, 1);
    }
}

void
calibration_subdivision_touch_handle_up(
                struct calibration_subdivision_touch *touch_sub,
                struct weston_seat *seat, int is_pointer, uint32_t button,
                uint32_t time, wl_fixed_t x, wl_fixed_t y)
{
    struct calibration_subdivision *sub = &touch_sub->subdivision;
    int slot = is_pointer ? CALIBRATION_POINTER_SLOT : button;
    if (calibration_subdivision_get_slot_pressed(sub, slot)) {
        if (is_pointer) {
            struct weston_pointer_grab *grab = &seat->pointer->default_grab;
            grab->interface->button(grab, time, button,
                                    WL_POINTER_BUTTON_STATE_RELEASED);
        } else {
            struct weston_touch_grab *grab = &seat->touch->default_grab;
            grab->interface->up(grab, time, button);
        }
        calibration_subdivision_set_slot_pressed(sub, slot, 0);
    }
}

void
calibration_subdivision_touch_handle_motion(
                struct calibration_subdivision_touch *touch_sub,
                struct weston_seat *seat, int is_pointer, uint32_t button,
                uint32_t time, wl_fixed_t x, wl_fixed_t y)
{
    if (is_pointer) {
        struct weston_pointer_grab *grab = &seat->pointer->default_grab;
        grab->interface->motion(grab, time, x, y);
    } else {
        struct weston_touch_grab *grab = &seat->touch->default_grab;
        wl_fixed_t sx, sy;

        /* If there's no appropriate view, sx and sy would be junk data */
        if (seat->touch->focus)
            weston_view_from_global_fixed(seat->touch->focus, x, y, &sx, &sy);
        grab->interface->motion(grab, time, button, sx, sy);
    }
}

void
calibration_subdivision_touch_handle_enter(
                struct calibration_subdivision_touch *touch_sub,
                struct weston_seat *seat, int is_pointer, uint32_t button,
                uint32_t time, wl_fixed_t x, wl_fixed_t y)
{
    struct calibration_subdivision *sub = &touch_sub->subdivision;
    int slot = is_pointer ? CALIBRATION_POINTER_SLOT : button;
    if (!calibration_subdivision_get_slot_pressed(sub, slot)) {
        if (is_pointer) {
            struct weston_pointer_grab *grab = &seat->pointer->default_grab;
            grab->interface->button(grab, time, button,
                                    WL_POINTER_BUTTON_STATE_PRESSED);
        } else {
            struct weston_touch_grab *grab = &seat->touch->default_grab;
            wl_fixed_t sx, sy;

            /* If there's no appropriate view, sx and sy would be junk data */
            if (seat->touch->focus)
                weston_view_from_global_fixed(seat->touch->focus,
                                              x, y, &sx, &sy);
            grab->interface->down(grab, time, button, sx, sy);
        }
        calibration_subdivision_set_slot_pressed(sub, slot, 1);
    }
}

void calibration_subdivision_touch_handle_leave(
                struct calibration_subdivision_touch *touch_sub,
                struct weston_seat *seat, int is_pointer, uint32_t button,
                uint32_t time, wl_fixed_t x, wl_fixed_t y)
{
    struct calibration_subdivision *sub = &touch_sub->subdivision;
    int slot = is_pointer ? CALIBRATION_POINTER_SLOT : button;
    if (calibration_subdivision_get_slot_pressed(sub, slot)) {
        if (is_pointer) {
            struct weston_pointer_grab *grab = &seat->pointer->default_grab;
            grab->interface->button(grab, time, button,
                                    WL_POINTER_BUTTON_STATE_RELEASED);
        } else {
            struct weston_touch_grab *grab = &seat->touch->default_grab;
            grab->interface->up(grab, time, button);
        }
        calibration_subdivision_set_slot_pressed(sub, slot, 0);
    }
}
