/*
 * Copyright 2014 Codethink Ltd
 *
 * Permission to use, copy, modify, distribute, and sell this software and
 * its documentation for any purpose is hereby granted without fee, provided
 * that the above copyright notice appear in all copies and that both that
 * copyright notice and this permission notice appear in supporting
 * documentation, and that the name of the copyright holders not be used in
 * advertising or publicity pertaining to distribution of the software
 * without specific, written prior permission.  The copyright holders make
 * no representations about the suitability of this software for any
 * purpose.  It is provided "as is" without express or implied warranty.
 *
 * THE COPYRIGHT HOLDERS DISCLAIM ALL WARRANTIES WITH REGARD TO THIS
 * SOFTWARE, INCLUDING ALL IMPLIED WARRANTIES OF MERCHANTABILITY AND
 * FITNESS, IN NO EVENT SHALL THE COPYRIGHT HOLDERS BE LIABLE FOR ANY
 * SPECIAL, INDIRECT OR CONSEQUENTIAL DAMAGES OR ANY DAMAGES WHATSOEVER
 * RESULTING FROM LOSS OF USE, DATA OR PROFITS, WHETHER IN AN ACTION OF
 * CONTRACT, NEGLIGENCE OR OTHER TORTIOUS ACTION, ARISING OUT OF OR IN
 * CONNECTION WITH THE USE OR PERFORMANCE OF THIS SOFTWARE.
 */

#include <getopt.h>

#include <wayland-util.h>
#include <weston/compositor.h>

#include "calibration-input-configuration.h"
#include "calibration-subdivision-button.h"
#include "calibration-subdivision-slider.h"
#include "calibration-subdivision-touch.h"

struct calibration_seat_context {
    struct wl_listener destroy_listener;
    struct wl_listener updated_caps_listener;
    struct wl_list link;

    struct weston_seat *seat;

    struct weston_pointer_grab p_grab;
    struct weston_keyboard_grab k_grab;
    struct weston_touch_grab t_grab;
};

struct input_calibration_context {
    struct wl_listener seat_create_listener;
    struct calibration_input_configuration configuration;

    struct wl_list seat_context_list;
};

static struct input_calibration_context g_ctx;



static void
calibration_pointer_grab_focus(struct weston_pointer_grab *grab)
{
    grab->pointer->default_grab.interface->focus(grab);
}

static void
calibration_pointer_grab_motion(struct weston_pointer_grab *grab,
                                uint32_t time, wl_fixed_t x, wl_fixed_t y)
{
    grab->pointer->default_grab.interface->motion(grab, time, x, y);
}

static void
calibration_pointer_grab_button(struct weston_pointer_grab *grab,
                                uint32_t time, uint32_t button, uint32_t state)
{
    grab->pointer->default_grab.interface->button(grab, time, button, state);
}

static void
calibration_pointer_grab_cancel(struct weston_pointer_grab *grab)
{
    /* Any cleanup that needs to be done */
    grab->pointer->default_grab.interface->cancel(grab);
}

struct weston_pointer_grab_interface calibration_pointer_grab_interface = {
    calibration_pointer_grab_focus,
    calibration_pointer_grab_motion,
    calibration_pointer_grab_button,
    calibration_pointer_grab_cancel
};



static void
calibration_keyboard_grab_key(struct weston_keyboard_grab *grab, uint32_t time,
                              uint32_t key, uint32_t state)
{
    grab->keyboard->default_grab.interface->key(grab, time, key, state);
}

static void
calibration_keyboard_grab_modifiers(struct weston_keyboard_grab *grab,
                                    uint32_t serial, uint32_t mods_depressed,
                                    uint32_t mods_latched, uint32_t mods_locked,
                                    uint32_t group)
{
    grab->keyboard->default_grab.interface->modifiers(grab, serial,
                    mods_depressed, mods_latched, mods_locked, group);
}

static void
calibration_keyboard_grab_cancel(struct weston_keyboard_grab *grab)
{
    grab->keyboard->default_grab.interface->cancel(grab);
}

struct weston_keyboard_grab_interface calibration_keyboard_grab_interface = {
    calibration_keyboard_grab_key,
    calibration_keyboard_grab_modifiers,
    calibration_keyboard_grab_cancel
};



static void
calibration_touch_grab_down(struct weston_touch_grab *grab, uint32_t time,
                            int touch_id, wl_fixed_t sx, wl_fixed_t sy)
{
    grab->touch->default_grab.interface->down(grab, time, touch_id, sx, sy);
}

static void
calibration_touch_grab_up(struct weston_touch_grab *grab, uint32_t time,
                          int touch_id)
{
    grab->touch->default_grab.interface->up(grab, time, touch_id);
}

static void
calibration_touch_grab_motion(struct weston_touch_grab *grab, uint32_t time,
                              int touch_id, wl_fixed_t sx, wl_fixed_t sy)
{
    grab->touch->default_grab.interface->motion(grab, time, touch_id, sx, sy);
}

static void
calibration_touch_grab_frame(struct weston_touch_grab *grab)
{
    grab->touch->default_grab.interface->frame(grab);
}

static void
calibration_touch_grab_cancel(struct weston_touch_grab *grab)
{
    grab->touch->default_grab.interface->frame(grab);
}

struct weston_touch_grab_interface calibration_touch_grab_interface = {
    calibration_touch_grab_down,
    calibration_touch_grab_up,
    calibration_touch_grab_motion,
    calibration_touch_grab_frame,
    calibration_touch_grab_cancel
};


static void
grab_pointer(struct calibration_seat_context *ctx_seat)
{
    if (ctx_seat == NULL)
        weston_log("%s: ctx_seat is NULL\n", __FUNCTION__);
    else if (ctx_seat->seat == NULL)
        weston_log("%s: ctx_seat->seat is NULL\n", __FUNCTION__);
    else if (ctx_seat->seat->pointer == NULL)
        weston_log("%s: ctx_seat->seat->pointer is NULL\n", __FUNCTION__);
    else
        weston_pointer_start_grab(ctx_seat->seat->pointer,
                                  &ctx_seat->p_grab);
}

static void
grab_keyboard(struct calibration_seat_context *ctx_seat)
{
    if (ctx_seat == NULL)
        weston_log("%s: ctx_seat is NULL\n", __FUNCTION__);
    else if (ctx_seat->seat == NULL)
        weston_log("%s: ctx_seat->seat is NULL\n", __FUNCTION__);
    else if (ctx_seat->seat->keyboard == NULL)
        weston_log("%s: ctx_seat->seat->keyboard is NULL\n", __FUNCTION__);
    else
        weston_keyboard_start_grab(ctx_seat->seat->keyboard,
                                   &ctx_seat->k_grab);
}

static void
grab_touch(struct calibration_seat_context *ctx_seat)
{
    if (ctx_seat == NULL)
        weston_log("%s: ctx_seat is NULL\n", __FUNCTION__);
    else if (ctx_seat->seat == NULL)
        weston_log("%s: ctx_seat->seat is NULL\n", __FUNCTION__);
    else if (ctx_seat->seat->touch == NULL)
        weston_log("%s: ctx_seat->seat->touch is NULL\n", __FUNCTION__);
    else
        weston_touch_start_grab(ctx_seat->seat->touch,
                                &ctx_seat->t_grab);

}

static struct calibration_seat_context*
get_seat_ctx_from_seat(struct weston_seat *seat)
{
    struct calibration_seat_context *seat_ctx;
    wl_list_for_each(seat_ctx, &g_ctx.seat_context_list, link) {
        if (seat_ctx->seat == seat)
            return seat_ctx;
    }

    return NULL;
}

static void
handle_seat_updated_caps(struct wl_listener *listener, void *data)
{
    struct weston_seat *seat = data;
    struct calibration_seat_context *seat_ctx = get_seat_ctx_from_seat(seat);

    if (seat->pointer != NULL
        && seat->pointer != seat_ctx->p_grab.pointer) {
        grab_pointer(seat_ctx);
    }

    if (seat->keyboard != NULL
        && seat->keyboard != seat_ctx->k_grab.keyboard) {
        grab_keyboard(seat_ctx);
    }

    if (seat->touch != NULL
        && seat->touch != seat_ctx->t_grab.touch) {
        grab_touch(seat_ctx);
    }
}

static void
handle_seat_destroy(struct wl_listener *listener, void *data)
{
    struct calibration_seat_context *ctx =
        container_of(listener, struct calibration_seat_context,
                     destroy_listener);

    /* Free up any pointer grabs */
    if (ctx->p_grab.pointer != NULL) {
        calibration_pointer_grab_cancel(&ctx->p_grab);
    }

    /* Free up any keyboard grabs */
    if (ctx->k_grab.keyboard != NULL) {
        calibration_keyboard_grab_cancel(&ctx->k_grab);
    }

    /* Free up any touch grabs */
    if (ctx->t_grab.touch != NULL) {
        calibration_touch_grab_cancel(&ctx->t_grab);
    }

    /* Remove self from any lists */
    wl_list_remove(&ctx->link);
    free(ctx);
}

static void
handle_seat_create(struct wl_listener *listener, void *data)
{
    struct weston_seat *seat = data;
    struct calibration_seat_context *seat_ctx = calloc(1, sizeof *seat_ctx);

    if (seat_ctx == NULL) {
        weston_log("%s: Failed to create seat context\n", __FUNCTION__);
        return;
    }

    seat_ctx->seat = seat;
    seat_ctx->p_grab.interface = &calibration_pointer_grab_interface;
    seat_ctx->k_grab.interface = &calibration_keyboard_grab_interface;
    seat_ctx->t_grab.interface = &calibration_touch_grab_interface;

    seat_ctx->destroy_listener.notify = &handle_seat_destroy;
    wl_signal_add(&seat->destroy_signal, &seat_ctx->destroy_listener);
    
    seat_ctx->updated_caps_listener.notify = &handle_seat_updated_caps;
    wl_signal_add(&seat->updated_caps_signal, &seat_ctx->updated_caps_listener);

    wl_list_insert(&g_ctx.seat_context_list, &seat_ctx->link);
}

static void
init_context(struct weston_compositor *ec, const char *config_path)
{
    wl_list_init(&g_ctx.seat_context_list);

    /* Set up seat creation listener */
    g_ctx.seat_create_listener.notify = &handle_seat_create;
    wl_signal_add(&ec->seat_created_signal, &g_ctx.seat_create_listener);

    /* Set up configuration */
    if (config_path != NULL) {
        calibration_input_configuration_init(&g_ctx.configuration, config_path);
        calibration_input_configuration_parse(&g_ctx.configuration, config_path);
    }
}

static void
strip_args(int *argc, char *argv[], int index, int count)
{
    for (; index != *argc; index++) {
        if (index >= *argc)

            argv[index - count] = NULL;
        else
            argv[index - count] = argv[index];
    }
}

WL_EXPORT int
module_init(struct weston_compositor *ec, int *argc, char *argv[])
{
    struct weston_seat *seat;
    int getopt_ret;
    char *input_configuration_path = NULL;
    int swallowed_args = 0;

    /* Using getopt because weston's libshared is not exported */
    struct option long_opts[] = {
        { "input-configuration", required_argument, 0, 'c' },
        {0, 0, 0, 0}
    };


    while ((getopt_ret = getopt_long(*argc, argv, "", long_opts, NULL)) != -1) {
        if (getopt_ret == 'c') {
            input_configuration_path = optarg;

            /* If optarg is not a separate element of argv */
            if (optarg != argv[optind - 1]) {
                /* Remove 1 element from argv, decrement argc by 1 */
                strip_args(argc, argv, optind, 1);
                swallowed_args++;
            } else {
                /* Remove 2 elements from argv, decrement argc by 2 */
                strip_args(argc, argv, optind, 2);
                swallowed_args += 2;
            }
        }
    }

    *argc -= swallowed_args;

    init_context(ec, input_configuration_path);

    wl_list_for_each(seat, &ec->seat_list, link) {
        handle_seat_create(NULL, seat);
        handle_seat_updated_caps(NULL, seat);
    }

    return 0;
}
