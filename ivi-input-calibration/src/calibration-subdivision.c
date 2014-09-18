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

#include <stdlib.h>
#include <string.h>

#include <weston/compositor.h>

#include "calibration-subdivision.h"
#include "calibration-subdivision-button.h"
#include "calibration-subdivision-slider.h"
#include "calibration-subdivision-touch.h"
#include "calibration-util.h"

void
calibration_subdivision_set_slot_pressed(struct calibration_subdivision *sub,
                                         int slot, int pressed)
{
    struct calibration_slot_pressed_map *map;
    int found_slot = 0;
    wl_list_for_each(map, &sub->slot_pressed_list, link) {
        if (map->slot == slot) {
            map->pressed = pressed;
            found_slot = 1;
            break;
        }
    }
    if (!found_slot) {
        map = calloc(1, sizeof *map);
        if (map == NULL) {
            weston_log("%s: Failed to allocate memory for slot-pressed "
                       "mapping\n", __FUNCTION__);
            return;
        }
        map->slot = slot;
        map->pressed = pressed;
        wl_list_insert(&sub->slot_pressed_list, &map->link);
    }
}

int
calibration_subdivision_get_slot_pressed(struct calibration_subdivision *sub,
                                         int slot)
{
    struct calibration_slot_pressed_map *map;

    wl_list_for_each(map, &sub->slot_pressed_list, link) {
        if (map->slot == slot)
            return map->pressed;
    }
    return 0;
}

int
calibration_subdivision_parse(struct calibration_subdivision *sub,
                              FILE *stream)
{
    char *buf;
    char key[BUFSIZ], val[BUFSIZ];
    int retval = 0;
    size_t n;
    fpos_t pos;

    buf = calloc(1, BUFSIZ);
    if (buf == NULL) {
        weston_log("%s: Failed to allocate memory for read buffer\n",
                   __FUNCTION__);
        return retval;
    }

    if (fgetpos(stream, &pos) < 0) {
        weston_log("%s: Failed to get stream position\n", __FUNCTION__);
        free(buf);
        return retval;
    }

    wl_list_init(&sub->slot_pressed_list);

    while (getline(&buf, &n, stream) != -1) {
        if (!calibration_split_line(buf, key, val)) {
            weston_log("%s: Failed to split line\n", __FUNCTION__);
            break;
        }

        if (key[0] == '-') { /* Found start of the next entry */
            if (fsetpos(stream, &pos) < 0) {
                weston_log("%s: Failed to set stream position\n",
                           __FUNCTION__);
            }
            retval = 1;
            break;
        }

        if (calibration_subdivision_parse_entry(sub, key, val)) {
            retval = 1;
        } else if (sub->type == CALIBRATION_SUBDIVISION_TYPE_BUTTON) {
            struct calibration_subdivision_button *button_sub =
                wl_container_of(sub, button_sub, subdivision);
            retval = calibration_subdivision_button_parse_entry(button_sub,
                                                                key, val);
        } else if (sub->type == CALIBRATION_SUBDIVISION_TYPE_SLIDER) {
            struct calibration_subdivision_slider *slider_sub =
                wl_container_of(sub, slider_sub, subdivision);
            retval = calibration_subdivision_slider_parse_entry(slider_sub,
                                                                key, val);

        } else if (sub->type == CALIBRATION_SUBDIVISION_TYPE_TOUCH) {

        } else {
            weston_log("%s: Unexpected subdivision type %d\n",
                       __FUNCTION__, sub->type);
            break;
        }

        if (fgetpos(stream, &pos) < 0) {
            weston_log("%s: Failed to get stream position\n", __FUNCTION__);
            break;
        }
    }
    
    free(buf);
    return retval;
}

int
calibration_subdivision_parse_entry(struct calibration_subdivision *sub,
                                    const char *key, const char *val)
{
    if (strcmp(key, "name") == 0) {
        char *tmpname = strdup(val);
        if (tmpname == NULL) {
            weston_log("%s: Could not duplicate string %s\n",
                       __FUNCTION__, val);
            return 0;
        }
        sub->name = tmpname;
    } else if (strcmp(key, "id") == 0) {
        char *endptr;
        uint32_t tmpid = strtol(val, &endptr, 10);
        if (endptr == val) {
            weston_log("%s: Could not convert '%s' to int\n",
                       __FUNCTION__, val);
            return 0;
        }
        sub->id = tmpid;
    } else if (strcmp(key, "top left x") == 0) {
        char *endptr;
        uint32_t tmp_pos = strtol(val, &endptr, 10);
        if (endptr == val) {
            weston_log("%s: Could not convert '%s' to int\n",
                       __FUNCTION__, val);
            return 0;
        }
        sub->top_left_x = tmp_pos;
    } else if (strcmp(key, "top left y") == 0) {
        char *endptr;
        uint32_t tmp_pos = strtol(val, &endptr, 10);
        if (endptr == val) {
            weston_log("%s: Could not convert '%s' to int\n",
                       __FUNCTION__, val);
            return 0;
        }
        sub->top_left_y = tmp_pos;
    } else if (strcmp(key, "bottom right x") == 0) {
        char *endptr;
        uint32_t tmp_pos = strtol(val, &endptr, 10);
        if (endptr == val) {
            weston_log("%s: Could not convert '%s' to int\n",
                       __FUNCTION__, val);
            return 0;
        }
        sub->bottom_right_x = tmp_pos;
    } else if (strcmp(key, "bottom right y") == 0) {
        char *endptr;
        uint32_t tmp_pos = strtol(val, &endptr, 10);
        if (endptr == val) {
            weston_log("%s: Could not convert '%s' to int\n",
                       __FUNCTION__, val);
            return 0;
        }
        sub->bottom_right_y = tmp_pos;
    } else {
        return 0;
    }

    return 1;
}

uint32_t
calibration_subdivision_get_width(struct calibration_subdivision *sub)
{
    return sub->bottom_right_x - sub->top_left_x;
}

uint32_t
calibration_subdivision_get_height(struct calibration_subdivision *sub)
{
    return sub->bottom_right_y - sub->top_left_y;
}

struct calibration_subdivision*
calibration_subdivision_get(struct wl_list *subdivision_list, float x, float y)
{
    struct calibration_subdivision *sub;
    wl_list_for_each(sub, subdivision_list, link) {
        if (x >= sub->top_left_x && y >= sub->top_left_y
            && x < sub->bottom_right_x && y < sub->bottom_right_y) {
            return sub;
        }
    }
    return NULL;
}

void
calibration_subdivision_handle_down(struct calibration_subdivision *sub,
                                    struct weston_seat *seat,
                                    int is_pointer, uint32_t button,
                                    uint32_t time,
                                    wl_fixed_t x, wl_fixed_t y)
{
    if (sub == NULL) {
        weston_log("%s: subdivision is NULL\n", __FUNCTION__);
        return;
    } else if (seat == NULL) {
        weston_log("%s: seat is NULL\n", __FUNCTION__);
        return;
    }

    if (sub->type == CALIBRATION_SUBDIVISION_TYPE_BUTTON) {
        struct calibration_subdivision_button *button_sub =
            wl_container_of(sub, button_sub, subdivision);
        calibration_subdivision_button_handle_down(button_sub, seat,
                                                   is_pointer, button,
                                                   time, x, y);
    } else if (sub->type == CALIBRATION_SUBDIVISION_TYPE_SLIDER) {
        struct calibration_subdivision_slider *slider_sub =
            wl_container_of(sub, slider_sub, subdivision);
        calibration_subdivision_slider_handle_down(slider_sub, seat,
                                                   is_pointer, button,
                                                   time, x, y);

    } else if (sub->type == CALIBRATION_SUBDIVISION_TYPE_TOUCH) {
        struct calibration_subdivision_touch *touch_sub =
            wl_container_of(sub, touch_sub, subdivision);
        calibration_subdivision_touch_handle_down(touch_sub, seat,
                                                  is_pointer, button,
                                                  time, x, y);
    } else {
        weston_log("%s: subdivision has unexpected type %d\n",
                   __FUNCTION__, sub->type);
    }
}

void
calibration_subdivision_handle_up(struct calibration_subdivision *sub,
                                  struct weston_seat *seat,
                                  int is_pointer, uint32_t button,
                                  uint32_t time,
                                  wl_fixed_t x, wl_fixed_t y)
{
    if (sub == NULL) {
        weston_log("%s: subdivision is NULL\n", __FUNCTION__);
        return;
    } else if (seat == NULL) {
        weston_log("%s: seat is NULL\n", __FUNCTION__);
        return;
    }

    if (sub->type == CALIBRATION_SUBDIVISION_TYPE_BUTTON) {
        struct calibration_subdivision_button *button_sub =
            wl_container_of(sub, button_sub, subdivision);
        calibration_subdivision_button_handle_up(button_sub, seat,
                                                 is_pointer, button,
                                                 time, x, y);
    } else if (sub->type == CALIBRATION_SUBDIVISION_TYPE_SLIDER) {
        struct calibration_subdivision_slider *slider_sub =
            wl_container_of(sub, slider_sub, subdivision);
        calibration_subdivision_slider_handle_up(slider_sub, seat,
                                                 is_pointer, button,
                                                 time, x, y);

    } else if (sub->type == CALIBRATION_SUBDIVISION_TYPE_TOUCH) {
        struct calibration_subdivision_touch *touch_sub =
            wl_container_of(sub, touch_sub, subdivision);
        calibration_subdivision_touch_handle_up(touch_sub, seat,
                                                is_pointer, button,
                                                time, x, y);
    } else {
        weston_log("%s: subdivision has unexpected type %d\n",
                   __FUNCTION__, sub->type);
    }
}

void
calibration_subdivision_handle_motion(struct calibration_subdivision *sub,
                                      struct weston_seat *seat,
                                      int is_pointer, uint32_t button,
                                      uint32_t time,
                                      wl_fixed_t x, wl_fixed_t y)
{
    if (sub == NULL) {
        weston_log("%s: subdivision is NULL\n", __FUNCTION__);
        return;
    } else if (seat == NULL) {
        weston_log("%s: seat is NULL\n", __FUNCTION__);
        return;
    }

    if (sub->type == CALIBRATION_SUBDIVISION_TYPE_BUTTON) {
        struct calibration_subdivision_button *button_sub =
            wl_container_of(sub, button_sub, subdivision);
        calibration_subdivision_button_handle_motion(button_sub, seat,
                                                   is_pointer, button,
                                                   time, x, y);
    } else if (sub->type == CALIBRATION_SUBDIVISION_TYPE_SLIDER) {
        struct calibration_subdivision_slider *slider_sub =
            wl_container_of(sub, slider_sub, subdivision);
        calibration_subdivision_slider_handle_motion(slider_sub, seat,
                                                     is_pointer, button,
                                                     time, x, y);

    } else if (sub->type == CALIBRATION_SUBDIVISION_TYPE_TOUCH) {
        struct calibration_subdivision_touch *touch_sub =
            wl_container_of(sub, touch_sub, subdivision);
        calibration_subdivision_touch_handle_motion(touch_sub, seat,
                                                    is_pointer, button,
                                                    time, x, y);
    } else {
        weston_log("%s: subdivision has unexpected type %d\n",
                   __FUNCTION__, sub->type);
    }
}

void
calibration_subdivision_handle_enter(struct calibration_subdivision *sub,
                                     struct weston_seat *seat,
                                     int is_pointer, uint32_t button,
                                     uint32_t time,
                                     wl_fixed_t x, wl_fixed_t y)
{
    if (sub == NULL) {
        weston_log("%s: subdivision is NULL\n", __FUNCTION__);
        return;
    } else if (seat == NULL) {
        weston_log("%s: seat is NULL\n", __FUNCTION__);
        return;
    }

    if (sub->type == CALIBRATION_SUBDIVISION_TYPE_BUTTON) {
        struct calibration_subdivision_button *button_sub = 
            wl_container_of(sub, button_sub, subdivision);
        calibration_subdivision_button_handle_enter(button_sub, seat,
                                                    is_pointer, button,
                                                    time, x, y);
    } else if (sub->type == CALIBRATION_SUBDIVISION_TYPE_SLIDER) {
        struct calibration_subdivision_slider *slider_sub =
            wl_container_of(sub, slider_sub, subdivision);
        calibration_subdivision_slider_handle_enter(slider_sub, seat,
                                                    is_pointer, button,
                                                    time, x, y);
    } else if (sub->type == CALIBRATION_SUBDIVISION_TYPE_TOUCH) {
        struct calibration_subdivision_touch *touch_sub =
            wl_container_of(sub, touch_sub, subdivision);
        calibration_subdivision_touch_handle_enter(touch_sub, seat,
                                                   is_pointer, button,
                                                   time, x, y);
    } else {
        weston_log("%s: subdivision has unexpected type %d\n",
                   __FUNCTION__, sub->type);
    }
}

void
calibration_subdivision_handle_leave(struct calibration_subdivision *sub,
                                     struct weston_seat *seat,
                                     int is_pointer, uint32_t button,
                                     uint32_t time,
                                     wl_fixed_t x, wl_fixed_t y)
{
    if (sub == NULL) {
        weston_log("%s: subdivision is NULL\n", __FUNCTION__);
        return;
    } else if (seat == NULL) {
        weston_log("%s: seat is NULL\n", __FUNCTION__);
        return;
    }

    if (sub->type == CALIBRATION_SUBDIVISION_TYPE_BUTTON) {
        struct calibration_subdivision_button *button_sub = 
            wl_container_of(sub, button_sub, subdivision);
        calibration_subdivision_button_handle_leave(button_sub, seat,
                                                    is_pointer, button,
                                                    time, x, y);
    } else if (sub->type == CALIBRATION_SUBDIVISION_TYPE_SLIDER) {
        struct calibration_subdivision_slider *slider_sub =
            wl_container_of(sub, slider_sub, subdivision);
        calibration_subdivision_slider_handle_leave(slider_sub, seat,
                                                    is_pointer, button,
                                                    time, x, y);
    } else if (sub->type == CALIBRATION_SUBDIVISION_TYPE_TOUCH) {
        struct calibration_subdivision_touch *touch_sub =
            wl_container_of(sub, touch_sub, subdivision);
        calibration_subdivision_touch_handle_leave(touch_sub, seat,
                                                   is_pointer, button,
                                                   time, x, y);
    } else {
        weston_log("%s: subdivision has unexpected type %d\n",
                   __FUNCTION__, sub->type);
    }
}
