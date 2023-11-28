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

#ifndef HEADER_GRAPHICS_SOKOL_H
#define HEADER_GRAPHICS_SOKOL_H

#include "graphics/shader.h"
#include "system/vector.h"

#include <flecs.h>
#include <sokol_gfx.h>

static inline kh_shader_desc_t* kh_sokol_shader_conv
    (const sg_shader_desc*(*desc)(sg_backend))
{
    static kh_shader_desc_t ret;
    ret._sokol_desc = *desc(sg_query_backend());
    return &ret;
}

#endif