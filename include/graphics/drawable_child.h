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

#ifndef HEADER_GRAPHICS_DRAWABLE_CHILD_H
#define HEADER_GRAPHICS_DRAWABLE_CHILD_H

#include "graphics/drawable.h"
#include "system/math.h"
#include "system/vector.h"

#include <flecs.h>
#include <sokol_gfx.h>

#ifdef GRAPHICS_DRAWABLE_CHILD_C
    #define ECS_META_IMPL IMPL
#else
    #define ECS_META_IMPL EXTERN
#endif

ECS_STRUCT(kh_drawable_child_t, {
    vector_float    vertex_buffer_slice;
    vector_uint16_t index_buffer_slice;
});

extern ECS_TAG_DECLARE(KhDrawableChildDirty);

void KhGraphicsDrawableChildImport(ecs_world_t *world);

#endif