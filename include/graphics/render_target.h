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

#ifndef HEADER_GRAPHICS_RENDER_TARGET_H
#define HEADER_GRAPHICS_RENDER_TARGET_H

#include "graphics/render_context.h"
#include "graphics/texture.h"
#include "system/vector.h"

#include <flecs.h>
#include <sokol_gfx.h>

#ifdef GRAPHICS_RENDER_TARGET_C
    #define ECS_META_IMPL IMPL
#else
    #define ECS_META_IMPL EXTERN
#endif

ECS_STRUCT(kh_render_target_t, {
    uint32_t width;
    uint32_t height;
    bool     has_depth;
ECS_PRIVATE
    sg_image _render_depth;
    sg_image _render_target;
    sg_pass  _render_pass;
});

static inline void kh_render_target_get_texture(kh_render_target_t *target, kh_texture_t *tex)
{
    *tex = (kh_texture_t) { target->_render_target };
}

void kh_render_target_begin(kh_render_context_t *context, kh_render_target_t *target);

void kh_render_target_end(kh_render_context_t *context);

void KhGraphicsRenderTargetImport(ecs_world_t *world);

#endif