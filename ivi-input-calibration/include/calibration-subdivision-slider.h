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

#ifndef CALIBRATION_SUBDIVISION_SLIDER_H_INCLUDED
#define CALIBRATION_SUBDIVISION_SLIDER_H_INCLUDED

#include <wayland-util.h>

#include "calibration-subdivision.h"

enum calibration_slider_orientation {
    CALIBRATION_SLIDER_UNKNOWN = 0,
    CALIBRATION_SLIDER_HORIZONTAL,
    CALIBRATION_SLIDER_VERTICAL
};

struct calibration_subdivision_slider {
    struct calibration_subdivision subdivision;
    enum calibration_slider_orientation orientation;
    float minimum_value;
    float maximum_value;
};

int
calibration_subdivision_slider_parse_entry(
                struct calibration_subdivision_slider *slider_sub,
                const char *key, const char *val);

#endif
