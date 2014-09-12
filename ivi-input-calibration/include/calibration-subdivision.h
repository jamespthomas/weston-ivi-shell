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

#ifndef CALIBRATION_SUBDIVISION_H_INCLUDED
#define CALIBRATION_SUBDIVISION_H_INCLUDED

#include <wayland-util.h>

enum calibration_subdivision_type {
    CALIBRATION_SUBDIVISION_TYPE_UNKNOWN = 0,
    CALIBRATION_SUBDIVISION_TYPE_TOUCH,
    CALIBRATION_SUBDIVISION_TYPE_BUTTON,
    CALIBRATION_SUBDIVISION_TYPE_SLIDER,
};

struct calibration_subdivision {
    struct wl_list link;
    enum calibration_subdivision_type type;
    char *name;
    uint32_t id;
    uint32_t top_left_x;
    uint32_t top_left_y;
    uint32_t bottom_right_x;
    uint32_t bottom_right_y;
};

int
calibration_subdivision_parse(struct calibration_subdivision *sub,
                              FILE *stream);

int
calibration_subdivision_parse_entry(struct calibration_subdivision *sub,
                                    const char *key, const char *val);

uint32_t
calibration_subdivision_get_width(struct calibration_subdivision *sub);

uint32_t
calibration_subdivision_get_height(struct calibration_subdivision *sub);

struct calibration_subdivision*
calibration_subdivision_get(struct wl_list *subdivision_list, float x, float y);

void
calibration_subdivision_handle_down(struct calibration_subdivision *sub,
                                    struct weston_seat *seat,
                                    int is_pointer, uint32_t button,
                                    uint32_t time,
                                    wl_fixed_t x, wl_fixed_t y);

void
calibration_subdivision_handle_up(struct calibration_subdivision *sub,
                                  struct weston_seat *seat,
                                  int is_pointer, uint32_t button,
                                  uint32_t time,
                                  wl_fixed_t x, wl_fixed_t y);

void
calibration_subdivision_handle_motion(struct calibration_subdivision *sub,
                                      struct weston_seat *seat,
                                      int is_pointer, uint32_t button,
                                      uint32_t time,
                                      wl_fixed_t x, wl_fixed_t y);

#endif
