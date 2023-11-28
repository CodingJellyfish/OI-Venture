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

#ifndef HEADER_GRAPHICS_RENDER_CONTEXT_H
#define HEADER_GRAPHICS_RENDER_CONTEXT_H

#include <flecs.h>
#include <sokol_gfx.h>

#ifdef GRAPHICS_RENDER_CONTEXT_C
    #define ECS_META_IMPL IMPL
#else
    #define ECS_META_IMPL EXTERN
#endif

#ifndef KH_RENDER_CONTEXT_MAX_VERTICES
    #define KH_RENDER_CONTEXT_DEFAULT_VERTEX_SIZE (8 * sizeof(float))
#endif

#ifndef KH_RENDER_CONTEXT_MAX_VERTICES
    #define KH_RENDER_CONTEXT_MAX_VERTICES 65536
#endif


ECS_STRUCT(kh_render_context_t, {
ECS_PRIVATE
    sg_buffer _vertex_buffer;
    sg_buffer _index_buffer;
});

void KhGraphicsRenderContextImport(ecs_world_t *world);

#endif
