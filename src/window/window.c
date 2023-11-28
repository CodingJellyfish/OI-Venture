/*  A competitive programming style dungeon game
 *  Copyright (C) 2022-2023 CodingJellyfish
 *
 *  This program is free software: you can redistribute it and/or modify
 *  it under the terms of the GNU Affero General Public License as published by
 *  the Free Software Foundation, either version 3 of the License, or
 *  (at your option) any later version.
 *
 *  This program is distributed in the hope that it will be useful,
 *  but WITHOUT ANY WARRANTY; without even the implied warranty of
 *  MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 *  GNU Affero General Public License for more details.
 *
 *  You should have received a copy of the GNU Affero General Public License
 *  along with this program.  If not, see <https://www.gnu.org/licenses/>.
 */

#define WINDOW_C

#include "window/window.h"

#include <sokol_app.h>

typedef struct sokol_app_desc_t {
    ecs_world_t *world;
    ecs_app_desc_t app;
} sokol_app_desc_t;

static void kh_init(void *ptr)
{
    sokol_app_desc_t *desc = (sokol_app_desc_t*) ptr;
}

static void kh_frame(void *ptr)
{
    sokol_app_desc_t *desc = (sokol_app_desc_t*) ptr;

    ecs_app_run_frame(desc->world, &desc->app);
}

static void kh_cleanup(void *ptr)
{
    sokol_app_desc_t *desc = (sokol_app_desc_t*) ptr;

    ecs_fini(desc->world);
    ecs_os_free(desc);
}

static void kh_event(const sapp_event* event, void *ptr)
{
    sokol_app_desc_t *desc = (sokol_app_desc_t*) ptr;
}

sapp_desc sokol_main(int argc, char* argv[])
{
    sokol_app_desc_t *app_desc = (sokol_app_desc_t*) ecs_os_malloc(sizeof(sokol_app_desc_t));

    *app_desc = (sokol_app_desc_t) {
        .world = ecs_init_w_args(argc, argv),
        .app = {
            .target_fps = KH_DEFAULT_FPS,
            .enable_rest = true,
            .threads = KH_DEFAULT_THREADS
        }
    };

    return (sapp_desc) {
        .width = KH_DEFAULT_WINDOW_WIDTH,
        .height = KH_DEFAULT_WINDOW_HEIGHT,
        .user_data = app_desc,
        .init_userdata_cb = kh_init,
        .frame_userdata_cb = kh_frame,
        .cleanup_userdata_cb = kh_cleanup,
        .event_userdata_cb = kh_event
    };
}