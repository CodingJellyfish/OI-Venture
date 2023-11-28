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

#define GRAPHICS_RENDER_CONTEXT_C

#include "graphics/render_context.h"

ECS_ON_ADD(kh_render_context_t, ptr, {
    sg_desc sgdesc = { .context = sapp_sgcontext() };
    sg_setup(&sgdesc);

    sg_buffer_desc vertex_buf_desc;
    memset(&vertex_buf_desc, 0, sizeof(sg_buffer_desc));
    vertex_buf_desc.size = KH_RENDER_CONTEXT_DEFAULT_VERTEX_SIZE * KH_RENDER_CONTEXT_MAX_VERTICES;
    vertex_buf_desc.type = SG_BUFFERTYPE_VERTEXBUFFER;
    vertex_buf_desc.usage = SG_USAGE_STREAM;

    ptr->_vertex_buffer = sg_make_buffer(&vertex_buf_desc);

    sg_buffer_desc index_buf_desc;
    memset(&vertex_buf_desc, 0, sizeof(sg_buffer_desc));
    index_buf_desc.size = KH_RENDER_CONTEXT_DEFAULT_VERTEX_SIZE * KH_RENDER_CONTEXT_MAX_VERTICES;
    index_buf_desc.type = SG_BUFFERTYPE_INDEX_BUFFER;
    index_buf_desc.usage = SG_USAGE_STREAM;

    ptr->_index_buffer = sg_make_buffer(&index_buf_desc);

#ifdef KH_DEBUG
    if (!sg_isvalid()) {
        fprintf(stderr, "Failed to create Sokol GFX context!\n");
        exit(-1);
    }
    if (sg_query_buffer_state(ptr->_vertex_buffer) != SG_RESOURCESTATE_VALID) {
        sgp_shutdown();
        return;
    }
    if (sg_query_buffer_state(ptr->_index_buffer) != SG_RESOURCESTATE_VALID) {
        sgp_shutdown();
        return;
    }
#endif
});

ECS_ON_REMOVE(kh_render_context_t, ptr, {
    if (ptr->_vertex_buffer.id != SG_INVALID_ID) {
        sg_destroy_buffer(ptr->_vertex_buffer);
    }
    if (ptr->_index_buffer.id != SG_INVALID_ID) {
        sg_destroy_buffer(ptr->_index_buffer);
    }
});

void KhGraphicsRenderContextImport(ecs_world_t *world)
{
    ECS_MODULE(world, KhGraphicsContext);

    ECS_META_COMPONENT(world, kh_render_context_t);

    ecs_set_hooks(world, kh_render_context_t, {
        .on_add = ecs_on_add(kh_render_context_t),
        .on_remove = ecs_on_remove(kh_render_context_t)
    });

    ecs_singleton_add(world, kh_render_context_t);
}

