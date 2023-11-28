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

#ifndef HEADER_GRAPHICS_BLEND_MODE_H
#define HEADER_GRAPHICS_BLEND_MODE_H

#include "system/math.h"
#include "system/vector.h"

#include <flecs.h>
#include <sokol_gfx.h>

#ifdef GRAPHICS_BLEND_MODE_C
    #define ECS_META_IMPL IMPL
#else
    #define ECS_META_IMPL EXTERN
#endif

ECS_ENUM(kh_blend_factor_t, {
    KH_BLEND_FACTOR_ZERO,
    KH_BLEND_FACTOR_ONE,
    KH_BLEND_FACTOR_SRC_COLOR,
    KH_BLEND_FACTOR_ONE_MINUS_SRC_COLOR,
    KH_BLEND_FACTOR_DST_COLOR,
    KH_BLEND_FACTOR_ONE_MINUS_DST_COLOR,
    KH_BLEND_FACTOR_SRC_ALPHA,
    KH_BLEND_FACTOR_ONE_MINUS_SRC_ALPHA,
    KH_BLEND_FACTOR_DST_ALPHA,
    KH_BLEND_FACTOR_ONE_MINUS_DST_ALPHA
});

ECS_ENUM(kh_blend_equation_t, {
    KH_BLEND_EQUATION_ADD,
    KH_BLEND_EQUATION_SUB,
    KH_BLEND_EQUATION_REV_SUB,
    KH_BLEND_EQUATION_MIN,
    KH_BLEND_EQUATION_MAX
});

ECS_STRUCT(kh_blend_mode_t, {
    kh_blend_factor_t color_src_factor;
    kh_blend_factor_t color_dst_factor;
    kh_blend_equation_t color_equation;
    kh_blend_factor_t alpha_src_factor;
    kh_blend_factor_t alpha_dst_factor;
    kh_blend_equation_t alpha_equation;
});

void KhGraphicsBlendModeImport(ecs_world_t *world);

#endif