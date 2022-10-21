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

#ifndef HEADER_WINDOW_H
#define HEADER_WINDOW_H

#include <flecs.h>

#include <raylib.h>

typedef struct {
    int width;
    int height;
    char *title;
    bool draw_fps;
    Color background;
} init_config_t;

extern ECS_COMPONENT_DECLARE(init_config_t);

void WindowModuleImport(ecs_world_t *ecs);

#endif