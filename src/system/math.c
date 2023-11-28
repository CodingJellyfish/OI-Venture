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

#define SYSTEM_MATH_C

#include "system/math.h"

void KhSystemMathImport(ecs_world_t *world)
{
    ECS_MODULE(world, KhSystemMath);

    ECS_META_COMPONENT(world, kh_vec2_t);
    ECS_META_COMPONENT(world, kh_rect_t);
    ECS_META_COMPONENT(world, kh_transform_t);
}