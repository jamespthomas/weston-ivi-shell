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

#ifndef CALIBRATION_UTIL_H_INCLUDED
#define CALIBRATION_UTIL_H_INCLUDED

#include <wayland-util.h>

#include <weston/compositor.h>

#define CALIBRATION_POINTER_SLOT -1

void
calibration_strip_whitespace(char *str);

int
calibration_split_line(const char *line, char *key, char *val);

int
parse_key_code(const char *str);

#endif
