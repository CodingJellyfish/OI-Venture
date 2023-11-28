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

#ifndef HEADER_WINDOW_FILE_H
#define HEADER_WINDOW_FILE_H

#include "system/vector.h"

#include <flecs.h>
#include <sokol_fetch.h>

#ifdef WINDOW_FILE_C
    #define ECS_META_IMPL IMPL
#else
    #define ECS_META_IMPL EXTERN
#endif

ECS_STRUCT(kh_file_t, {
    vector_uint8_t buffer;
ECS_PRIVATE
    sfetch_handle_t    _handle;
});

kh_file_t *kh_file_init(char *filename);

void KhWindowFileImport(ecs_world_t *world);

#endif
