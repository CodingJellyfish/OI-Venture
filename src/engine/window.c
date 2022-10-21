// An adventure story featuring a couple of Chinese programming geniuses.
// Copyright (C) 2022 CodingJellyfish

// This program is free software: you can redistribute it and/or modify
// it under the terms of the GNU General Public License as published by
// the Free Software Foundation, either version 3 of the License, or
// (at your option) any later version.

// This program is distributed in the hope that it will be useful,
// but WITHOUT ANY WARRANTY; without even the implied warranty of
// MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
// GNU General Public License for more details.

// You should have received a copy of the GNU General Public License
// along with this program.  If not, see <https://www.gnu.org/licenses/>.

#include "engine/window.h"

ECS_COMPONENT_DECLARE(init_config_t);

static void init_window(ecs_iter_t *it)
{
    init_config_t *conf = ecs_field(it, init_config_t, 1);

    InitWindow(conf->width, conf->height, conf->title);
}

static void quit_window(ecs_iter_t *it)
{
    if (WindowShouldClose()) {
        ecs_quit(it->real_world);
    }
}

static void close_window(ecs_iter_t *it)
{
    CloseWindow();
}

void WindowModuleImport(ecs_world_t *ecs)
{
    ECS_MODULE(ecs, WindowModule);

    ECS_COMPONENT_DEFINE(ecs, init_config_t);

    ecs_entity_t
        initwindow = ecs_observer_init(ecs, &(ecs_observer_desc_t) {
            .filter.terms = { { ecs_id(init_config_t), .src = ecs_id(init_config_t) } },
            .events = { EcsOnSet },
            .callback = init_window
        }),
        quitwindow = ecs_system_init(ecs, &(ecs_system_desc_t) {
            .entity = ecs_entity(ecs, {
                .add = { ecs_dependson(EcsPostUpdate) }
            }),
            .query.filter.terms = {
                { ecs_id(init_config_t), .src = ecs_id(init_config_t) }
            },
            .callback = quit_window,
            .no_staging = true
        }),
        closewindow = ecs_observer_init(ecs, &(ecs_observer_desc_t) {
            .filter.terms = { { ecs_id(init_config_t), .src = ecs_id(init_config_t) } },
            .events = { EcsOnDelete },
            .callback = close_window
        });
}
