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
