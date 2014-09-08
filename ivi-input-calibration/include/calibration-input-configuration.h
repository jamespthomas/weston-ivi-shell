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

#ifndef CALIBRATION_INPUT_CONFIGURATION_H_INCLUDED
#define CALIBRATION_INPUT_CONFIGURATION_H_INCLUDED

#include <wayland-util.h>

#include "calibration-subdivision.h"
#include "calibration-input-calibration.h"

struct device_name {
    struct wl_list link;
    char *device_name;
};

struct calibration_input_configuration {
    char *filename;
    struct wl_list device_names;
    struct wl_list subdivisions;
    struct calibration_input_calibration calibration;
};

int
calibration_input_configuration_init(
                struct calibration_input_configuration *conf,
                const char *path);
void calibration_input_configuration_destroy(
                struct calibration_input_configuration *conf);
int
calibration_input_configuration_parse(
                struct calibration_input_configuration *conf,
                const char *path);


#endif
