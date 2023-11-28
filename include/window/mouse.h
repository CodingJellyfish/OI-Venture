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

#ifndef HEADER_WINDOW_MOUSE_H
#define HEADER_WINDOW_MOUSE_H

#include <flecs.h>

#ifdef WINDOW_MOUSE_C
    #define ECS_META_IMPL IMPL
#else
    #define ECS_META_IMPL EXTERN
#endif

#ifndef KH_MAX_KEY_QUEUE
    #define KH_MAX_KEY_QUEUE 8
#endif

ECS_ENUM(kh_mouse_t, {
    KH_MOUSE_LEFT,
    KH_MOUSE_RIGHT,
    KH_MOUSE_MIDDLE,
    KH_MOUSE_COUNT
});

ECS_STRUCT(kh_mouse_context_t, {
    bool mouse_down_cur[KH_KEY_COUNT];
    bool mouse_down_pre[KH_KEY_COUNT];
    kh_key_t key_queue[KH_MAX_KEY_QUEUE];
    uint32_t char_queue[KH_MAX_KEY_QUEUE];
    uint32_t key_queue_front;
    uint32_t key_queue_back;
    uint32_t char_queue_front;
    uint32_t char_queue_back;
});

static inline bool kh_is_key_pressed(kh_keyboard_context_t *ctx, kh_key_t key)
{
    return ctx->key_down_cur[key] && !ctx->key_down_pre[key];
}

static inline bool kh_is_key_released(kh_keyboard_context_t *ctx, kh_key_t key)
{
    return !ctx->key_down_cur[key] && ctx->key_down_pre[key];
}

static inline bool kh_is_key_down(kh_keyboard_context_t *ctx, kh_key_t key)
{
    return ctx->key_down_cur[key];
}

static inline bool kh_is_key_up(kh_keyboard_context_t *ctx, kh_key_t key)
{
    return !ctx->key_down_cur[key];
}

static inline kh_key_t kh_get_key_pressed(kh_keyboard_context_t *ctx)
{
    if (ctx->key_queue_front == ctx->key_queue_back) {
        return KH_KEY_NONE;
    }
    return ctx->key_queue[ctx->key_queue_front++];
}

static inline uint32_t kh_get_char_pressed(kh_keyboard_context_t *ctx)
{
    if (ctx->char_queue_front == ctx->char_queue_back) {
        return 0;
    }
    return ctx->char_queue[ctx->char_queue_front++];
}

void KhWindowMouseImport(ecs_world_t *world);

#endif