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

#ifndef HEADER_GRAPHICS_TEXTURE_H
#define HEADER_GRAPHICS_TEXTURE_H

#include "system/math.h"
#include "system/vector.h"

#include <flecs.h>
#include <sokol_gfx.h>

#ifdef GRAPHICS_TEXTURE_C
    #define ECS_META_IMPL IMPL
#else
    #define ECS_META_IMPL EXTERN
#endif

ECS_STRUCT(kh_texture_t, {
ECS_PRIVATE
    sg_image _texture;
});

kh_texture_t kh_create_default_texture(void);

void kh_destroy_texture(kh_texture_t *tex);

void KhGraphicsTextureImport(ecs_world_t *world);

#endif