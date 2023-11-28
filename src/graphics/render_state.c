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

#define GRAPHICS_RENDER_STATE_C

#include "graphics/default_shader.h"
#include "graphics/render_state.h"
#include "graphics/sokol.h"

typedef struct set_default_state_ctx_t {
    kh_render_state_t state;
} set_default_state_ctx_t;

void set_default_state(ecs_iter_t *it)
{
    kh_render_state_t *state = ecs_field(it, kh_render_state_t, 1);
    set_default_state_ctx_t *ctx  = (set_default_state_ctx_t*) it->ctx;

    for (int i = 0; i < it->count; i++) {
        state[i] = ctx->state;
    }
}

void set_default_state_ctx_free(void *_ctx)
{
    set_default_state_ctx_t *ctx  = (set_default_state_ctx_t*) _ctx;

    kh_destroy_shader(&ctx->state.shader);
    kh_destroy_texture(&ctx->state.texture);

    ecs_os_free(ctx);
}

void KhGraphicsRenderStateImport(ecs_world_t *world)
{
    ECS_MODULE(world, KhGraphicsRenderState);

    set_default_state_ctx_t *ctx = ecs_os_malloc_t(set_default_state_ctx_t);

    ctx->state.blend_mode = (kh_blend_mode_t) {

    };
    ctx->state.shader = kh_create_shader(kh_sokol_shader_conv(default2d_shader_desc));
    ctx->state.texture = kh_create_default_texture();

    ecs_observer(world, {
        .filter.expr = "kh_render_state_t",
        .events = { EcsOnAdd },
        .callback = set_default_state,
        .ctx = ctx,
        .ctx_free = set_default_state_ctx_free
    });
}

