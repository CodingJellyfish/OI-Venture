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

#define WINDOW_KEYBOARD_C

#include "window/keyboard.h"

ECS_SYSTEM_DECLARE(keyboard_progress);

void keyboard_progress(ecs_iter_t *it)
{
    kh_keyboard_context_t *ctx = ecs_field(it, kh_keyboard_context_t, 1);

    ecs_os_memcpy_n(ctx->key_down_pre, ctx->key_down_cur, bool, KH_KEY_COUNT);
    
    ctx->key_queue_front = 0;
    ctx->key_queue_back = 0;
    ctx->char_queue_front = 0;
    ctx->char_queue_back = 0;
}

void KhWindowKeyboardImport(ecs_world_t *world)
{
    ECS_MODULE(world, KhWindowKeyboard);

    ECS_META_COMPONENT(world, kh_keyboard_context_t);

    ECS_SYSTEM_DEFINE(world, keyboard_progress, EcsPreFrame,
        kh_keyboard_context_t($)
    );
}

