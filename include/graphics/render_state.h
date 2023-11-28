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

#ifndef HEADER_GRAPHICS_RENDER_STATE_H
#define HEADER_GRAPHICS_RENDER_STATE_H

#include "graphics/blend_mode.h"
#include "graphics/shader.h"
#include "graphics/texture.h"
#include "system/math.h"
#include "system/vector.h"

#include <flecs.h>
#include <sokol_gfx.h>

#ifdef GRAPHICS_RENDER_STATE_C
    #define ECS_META_IMPL IMPL
#else
    #define ECS_META_IMPL EXTERN
#endif

ECS_STRUCT(kh_render_state_t, {
    kh_blend_mode_t blend_mode;
    kh_shader_t     shader;
    kh_texture_t    texture;
});

void KhGraphicsRenderStateImport(ecs_world_t *world);

#endif