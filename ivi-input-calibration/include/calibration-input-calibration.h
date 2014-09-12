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

#ifndef CALIBRATION_INPUT_CALIBRATION_H_INCLUDED
#define CALIBRATION_INPUT_CALIBRATION_H_INCLUDED

#include <wayland-util.h>

struct calibration_input_calibration {
    float x1;
    float x2;
    float x3;
    float y1;
    float y2;
    float y3;
};

int
calibration_input_calibration_parse(
                struct calibration_input_calibration *calibration,
                const char *path);

void
calibration_input_calibration_apply(
                struct calibration_input_calibration *calibration,
                wl_fixed_t xin, wl_fixed_t yin,
                wl_fixed_t *xout, wl_fixed_t *yout);
#endif
