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

#include "calibration-util.h"
#include "calibration-input-calibration.h"
#include "calibration-subdivision-button.h"
#include "calibration-subdivision-slider.h"
#include "calibration-subdivision-touch.h"

#include "calibration-input-configuration.h"

int
calibration_input_configuration_init(
                struct calibration_input_configuration *conf,
                const char *path)
{
    wl_list_init(&conf->device_names);
    wl_list_init(&conf->subdivisions);

    if (conf->filename != NULL) {
        weston_log("%s: conf->filename is not NULL. conf may have alread "
                   "been initialized!\n", __FUNCTION__);
        return 0;
    }

    conf->filename = strdup(path);
    if (conf->filename == NULL) {
        weston_log("%s: Could not allocate memory for conf->filename\n",
                   __FUNCTION__);
        return 0;
    }
    return 1;
}

void
calibration_input_configuration_destroy(
                struct calibration_input_configuration *conf)
{
    if (conf->filename != NULL)
        free(conf->filename);
}

int
calibration_input_configuration_parse(
                struct calibration_input_configuration *conf,
                const char *path)
{
    FILE *file; 
    char key[BUFSIZ], val[BUFSIZ];
    int retval = 0;
    int inSubdivisions = 0;
    char *buf;
    size_t n;
    struct calibration_subdivision_button *button_sub = NULL;
    struct calibration_subdivision_slider *slider_sub = NULL;
    struct calibration_subdivision_touch *touch_sub = NULL;

    errno = 0;

    buf = calloc(1, BUFSIZ);

    if (buf == NULL) {
        weston_log("%s: Failed to allocate memory for read buffer\n",
                   __FUNCTION__);
        goto exit;
    }

    file = fopen(path, "r");

    if (file == NULL) {
        weston_log("%s: Failed to read file %s: %s\n",
                   __FUNCTION__, path, strerror(errno));
        goto exit_buf;
    }

    while (getline(&buf, &n, file) != -1) {
        /* subdivisions is an exception to the expectation that all keys
         * have values */
        if (strncmp(buf, "subdivisions:", strlen("subdivisions:")) == 0) {
            inSubdivisions = 1;
            continue;
        }

        if (!calibration_split_line(buf, key, val)) {
            weston_log("%s: Failed to split line\n", __FUNCTION__);
            goto exit_file;
        }

        if (strcmp(key, "device name") == 0) {
            struct device_name *devname =  calloc(1, sizeof *devname);
            devname->device_name = strdup(val);
            if (devname == NULL) {
                weston_log("%s: Failed to allocate memory for device name\n",
                           __FUNCTION__);
                goto exit_file;
            }
            wl_list_insert(&conf->device_names, &devname->link);

        } else if (strcmp(key, "calibration file") == 0) {
            /* Parse calibration file */
            calibration_input_calibration_parse(&conf->calibration, val);

        } else if (key[0] == '-') { /* List entry starts with a '-' */
            if (!inSubdivisions) {
                weston_log("%s: List entry found when not in 'subdivisions'\n",
                           __FUNCTION__);
                goto exit_file;
            }

            if (strcmp(key, "- type") != 0) {
                weston_log("%s: Subdivision does not start with 'type' "
                           "(received key '%s')\n",
                           __FUNCTION__, key);
                goto exit_file;
            }

            if (strcmp(val, "button") == 0) {
                button_sub = calloc(1, sizeof *button_sub);
                if (button_sub == NULL) {
                    weston_log("%s: Failed to allocate memory for button "
                               "subdivision\n", __FUNCTION__);
                    goto exit_file;
                }
                button_sub->subdivision.type =
                    CALIBRATION_SUBDIVISION_TYPE_BUTTON;
                if (!calibration_subdivision_parse(&button_sub->subdivision,
                                                   file)) {
                    weston_log("%s: Failed to parse button subdivision\n",
                               __FUNCTION__);
                    free(button_sub);
                    goto exit_file;
                }
                wl_list_insert(&conf->subdivisions,
                               &button_sub->subdivision.link);
            } else if (strcmp(val, "slider") == 0) {
                slider_sub = calloc(1, sizeof *slider_sub);
                if (slider_sub == NULL) {
                    weston_log("%s: Failed to allocate memory for slider "
                               "subdivision\n", __FUNCTION__);
                    goto exit_file;
                }
                slider_sub->subdivision.type =
                    CALIBRATION_SUBDIVISION_TYPE_SLIDER;
                if (!calibration_subdivision_parse(&slider_sub->subdivision,
                                                   file)) {
                    weston_log("%s: Failed to parse slider subdivision\n",
                               __FUNCTION__);
                    free(slider_sub);
                    goto exit_file;
                }
                wl_list_insert(&conf->subdivisions,
                               &slider_sub->subdivision.link);
            } else if (strcmp(val, "touch") == 0) {
                touch_sub = calloc(1, sizeof *touch_sub);
                if (touch_sub == NULL) {
                    weston_log("%s: Failed to allocate memory for touch "
                               "subdivision\n", __FUNCTION__);
                    goto exit_file;
                }
                touch_sub->subdivision.type =
                    CALIBRATION_SUBDIVISION_TYPE_TOUCH;
                if (!calibration_subdivision_parse(&touch_sub->subdivision,
                                                   file)) {
                    weston_log("%s: Failed to parse touch subdivision\n",
                               __FUNCTION__);
                    free(touch_sub);
                    goto exit_file;
                }
                wl_list_insert(&conf->subdivisions,
                               &touch_sub->subdivision.link);
            } else {
                weston_log("%s: Invalid subdivision type '%s'\n",
                           __FUNCTION__, val);
                goto exit_file;
            }

        } else {
            weston_log("%s: Unknown key '%s'\n", __FUNCTION__, key);
            goto exit_file;
        }
    }

    if (errno != 0) {
        weston_log("%s: Error while reading line from file %s: %s\n",
                   __FUNCTION__, path, strerror(errno));
    }

    retval = 1;

exit_file:
    fclose(file);
exit_buf:
    free(buf);
exit:
    return retval;
}
