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

#ifndef HEADER_ASSETS_H
#define HEADER_ASSETS_H

#include <flecs.h>

#include <raylib.h>

#include "utils/array.h"

DEF_ARRAY(Vector2)

typedef ARRAY(Vector2) vertex_array;

extern ECS_COMPONENT_DECLARE(Font);
extern ECS_COMPONENT_DECLARE(Image);
extern ECS_COMPONENT_DECLARE(Texture2D);
extern ECS_COMPONENT_DECLARE(RenderTexture2D);
extern ECS_COMPONENT_DECLARE(vertex_array);

void AssetsModuleImport(ecs_world_t *ecs);

#endif