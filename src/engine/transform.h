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

#ifndef HEADER_TRANSFORM_H
#define HEADER_TRANSFORM_H

#include <flecs.h>

#include <raylib.h>

typedef struct {
    Vector2 origin, _o;
    Vector2 translation, _t;
    float   rotation, _r;
    Vector2 scale, _s;
} transform_t;

extern ECS_COMPONENT_DECLARE(transform_t);

extern ECS_DECLARE(TransformChildOf);

Vector2 do_translation(Vector2 point, Vector2 tran);

Vector2 do_rotation(Vector2 point, Vector2 origin, float angle);

Vector2 do_scale(Vector2 point, Vector2 origin, Vector2 scale);

Vector2 do_transform(Vector2 point, transform_t *tran);

void TransformModuleImport(ecs_world_t *ecs);

#endif