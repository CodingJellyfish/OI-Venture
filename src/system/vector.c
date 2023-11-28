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

#define SYSTEM_VECTOR_C

#include "system/vector.h"

KH_VECTOR_META_DECLARE(bool, ecs_id(ecs_bool_t));
KH_VECTOR_META_DECLARE(char, ecs_id(ecs_char_t));
KH_VECTOR_META_DECLARE(uint8_t, ecs_id(ecs_u8_t));
KH_VECTOR_META_DECLARE(uint16_t, ecs_id(ecs_u16_t));
KH_VECTOR_META_DECLARE(uint32_t, ecs_id(ecs_u32_t));
KH_VECTOR_META_DECLARE(uint64_t, ecs_id(ecs_u64_t));
KH_VECTOR_META_DECLARE(uintptr_t, ecs_id(ecs_uptr_t));
KH_VECTOR_META_DECLARE(int8_t, ecs_id(ecs_i8_t));
KH_VECTOR_META_DECLARE(int16_t, ecs_id(ecs_i16_t));
KH_VECTOR_META_DECLARE(int32_t, ecs_id(ecs_i32_t));
KH_VECTOR_META_DECLARE(int64_t, ecs_id(ecs_i64_t));
KH_VECTOR_META_DECLARE(intptr_t, ecs_id(ecs_iptr_t));
KH_VECTOR_META_DECLARE(float, ecs_id(ecs_f32_t));
KH_VECTOR_META_DECLARE(double, ecs_id(ecs_f64_t));
KH_VECTOR_META_DECLARE(vector_char, ecs_id(ecs_string_t));

void KhSystemVectorImport(ecs_world_t *world)
{
    kh_vector_meta_define(bool, world);
    kh_vector_meta_define(char, world);
    kh_vector_meta_define(uint8_t, world);
    kh_vector_meta_define(uint16_t, world);
    kh_vector_meta_define(uint32_t, world);
    kh_vector_meta_define(uint64_t, world);
    kh_vector_meta_define(uintptr_t, world);
    kh_vector_meta_define(int8_t, world);
    kh_vector_meta_define(int16_t, world);
    kh_vector_meta_define(int32_t, world);
    kh_vector_meta_define(int64_t, world);
    kh_vector_meta_define(intptr_t, world);
    kh_vector_meta_define(float, world);
    kh_vector_meta_define(double, world);
    kh_vector_meta_define(vector_char, world);
}