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

#include <errno.h>
#include <stdio.h>
#include <string.h>

#include <weston/compositor.h>

#include "calibration-input-calibration.h"
#include "calibration-util.h"

int
calibration_input_calibration_parse(
                struct calibration_input_calibration *calibration,
                const char *path)
{
    FILE *file = NULL;
    char key[BUFSIZ], val[BUFSIZ];
    int retval = 1;
    char *buf;
    size_t n;

    buf = calloc(1, BUFSIZ);
    if (buf == NULL) {
        weston_log("%s: Failed to allocate memory for read buffer\n",
                   __FUNCTION__);
        retval = 0;
    } else if ((file = fopen(path, "r")) == NULL) {
        weston_log("%s: Failed to read file %s: %s\n",
                   __FUNCTION__, path, strerror(errno));
        retval = 0;
    } else {
        while (getline(&buf, &n, file) != -1) {
            if (!calibration_split_line(buf, key, val)) {
                weston_log("%s: Failed to split line '%s'\n",
                           __FUNCTION__, buf);
                retval = 0;
                break;
            }

            if (strcmp(key, "x coefficient one") == 0) {
                char *nptr;
                float tmp_coeff = strtof(val, &nptr);
                if (nptr == val) {
                    weston_log("%s: Failed to convert '%s' to float\n",
                               __FUNCTION__, val);
                    retval = 0;
                    break;
                }
                calibration->x1 = tmp_coeff;
            } else if (strcmp(key, "x coefficient two") == 0) {
                char *nptr;
                float tmp_coeff = strtof(val, &nptr);
                if (nptr == val) {
                    weston_log("%s: Failed to convert '%s' to float\n",
                               __FUNCTION__, val);
                    retval = 0;
                    break;
                }
                calibration->x2 = tmp_coeff;
            } else if (strcmp(key, "x coefficient three") == 0) {
                char *nptr;
                float tmp_coeff = strtof(val, &nptr);
                if (nptr == val) {
                    weston_log("%s: Failed to convert '%s' to float\n",
                               __FUNCTION__, val);
                    retval = 0;
                    break;
                }
                calibration->x3 = tmp_coeff;
            } else if (strcmp(key, "y coefficient one") == 0) {
                char *nptr;
                float tmp_coeff = strtof(val, &nptr);
                if (nptr == val) {
                    weston_log("%s: Failed to convert '%s' to float\n",
                               __FUNCTION__, val);
                    retval = 0;
                    break;
                }
                calibration->y1 = tmp_coeff;
            } else if (strcmp(key, "y coefficient two") == 0) {
                char *nptr;
                float tmp_coeff = strtof(val, &nptr);
                if (nptr == val) {
                    weston_log("%s: Failed to convert '%s' to float\n",
                               __FUNCTION__, val);
                    retval = 0;
                    break;
                }
                calibration->y2 = tmp_coeff;
            } else if (strcmp(key, "y coefficient three") == 0) {
                char *nptr;
                float tmp_coeff = strtof(val, &nptr);
                if (nptr == val) {
                    weston_log("%s: Failed to convert '%s' to float\n",
                               __FUNCTION__, val);
                    retval = 0;
                    break;
                }
                calibration->y3 = tmp_coeff;
            }
        }
    }

    if (file != NULL)
        fclose(file);
    if (buf != NULL)
        free(buf);
    return retval;
}
