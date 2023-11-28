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

#ifndef HEADER_WINDOW_KEYBOARD_H
#define HEADER_WINDOW_KEYBOARD_H

#include <flecs.h>

#ifdef WINDOW_KEYBOARD_C
    #define ECS_META_IMPL IMPL
#else
    #define ECS_META_IMPL EXTERN
#endif

#ifndef KH_MAX_KEY_QUEUE
    #define KH_MAX_KEY_QUEUE 8
#endif

ECS_ENUM(kh_key_t, {
    KH_KEY_NONE,
    KH_KEY_SPACE,
    KH_KEY_APOSTROPHE,  /* ' */
    KH_KEY_COMMA,  /* , */
    KH_KEY_MINUS,  /* - */
    KH_KEY_PERIOD,  /* . */
    KH_KEY_SLASH,  /* / */
    KH_KEY_0,
    KH_KEY_1,
    KH_KEY_2,
    KH_KEY_3,
    KH_KEY_4,
    KH_KEY_5,
    KH_KEY_6,
    KH_KEY_7,
    KH_KEY_8,
    KH_KEY_9,
    KH_KEY_SEMICOLON,  /* ; */
    KH_KEY_EQUAL,  /* = */
    KH_KEY_A,
    KH_KEY_B,
    KH_KEY_C,
    KH_KEY_D,
    KH_KEY_E,
    KH_KEY_F,
    KH_KEY_G,
    KH_KEY_H,
    KH_KEY_I,
    KH_KEY_J,
    KH_KEY_K,
    KH_KEY_L,
    KH_KEY_M,
    KH_KEY_N,
    KH_KEY_O,
    KH_KEY_P,
    KH_KEY_Q,
    KH_KEY_R,
    KH_KEY_S,
    KH_KEY_T,
    KH_KEY_U,
    KH_KEY_V,
    KH_KEY_W,
    KH_KEY_X,
    KH_KEY_Y,
    KH_KEY_Z,
    KH_KEY_LEFT_BRACKET,  /* [ */
    KH_KEY_BACKSLASH,  /* \ */
    KH_KEY_RIGHT_BRACKET,  /* ] */
    KH_KEY_GRAVE_ACCENT,  /* ` */
    KH_KEY_WORLD_1, /* non-US #1 */
    KH_KEY_WORLD_2, /* non-US #2 */
    KH_KEY_ESCAPE,
    KH_KEY_ENTER,
    KH_KEY_TAB,
    KH_KEY_BACKSPACE,
    KH_KEY_INSERT,
    KH_KEY_DELETE,
    KH_KEY_RIGHT,
    KH_KEY_LEFT,
    KH_KEY_DOWN,
    KH_KEY_UP,
    KH_KEY_PAGE_UP,
    KH_KEY_PAGE_DOWN,
    KH_KEY_HOME,
    KH_KEY_END,
    KH_KEY_CAPS_LOCK,
    KH_KEY_SCROLL_LOCK,
    KH_KEY_NUM_LOCK,
    KH_KEY_PRINT_SCREEN,
    KH_KEY_PAUSE,
    KH_KEY_F1,
    KH_KEY_F2,
    KH_KEY_F3,
    KH_KEY_F4,
    KH_KEY_F5,
    KH_KEY_F6,
    KH_KEY_F7,
    KH_KEY_F8,
    KH_KEY_F9,
    KH_KEY_F10,
    KH_KEY_F11,
    KH_KEY_F12,
    KH_KEY_F13,
    KH_KEY_F14,
    KH_KEY_F15,
    KH_KEY_F16,
    KH_KEY_F17,
    KH_KEY_F18,
    KH_KEY_F19,
    KH_KEY_F20,
    KH_KEY_F21,
    KH_KEY_F22,
    KH_KEY_F23,
    KH_KEY_F24,
    KH_KEY_F25,
    KH_KEY_KP_0,
    KH_KEY_KP_1,
    KH_KEY_KP_2,
    KH_KEY_KP_3,
    KH_KEY_KP_4,
    KH_KEY_KP_5,
    KH_KEY_KP_6,
    KH_KEY_KP_7,
    KH_KEY_KP_8,
    KH_KEY_KP_9,
    KH_KEY_KP_DECIMAL,
    KH_KEY_KP_DIVIDE,
    KH_KEY_KP_MULTIPLY,
    KH_KEY_KP_SUBTRACT,
    KH_KEY_KP_ADD,
    KH_KEY_KP_ENTER,
    KH_KEY_KP_EQUAL,
    KH_KEY_LEFT_SHIFT,
    KH_KEY_LEFT_CONTROL,
    KH_KEY_LEFT_ALT,
    KH_KEY_LEFT_SUPER,
    KH_KEY_RIGHT_SHIFT,
    KH_KEY_RIGHT_CONTROL,
    KH_KEY_RIGHT_ALT,
    KH_KEY_RIGHT_SUPER,
    KH_KEY_MENU,
    KH_KEY_COUNT
});

ECS_STRUCT(kh_keyboard_context_t, {
    bool key_down_cur[KH_KEY_COUNT];
    bool key_down_pre[KH_KEY_COUNT];
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

void KhWindowKeyboardImport(ecs_world_t *world);

#endif