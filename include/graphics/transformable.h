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

#ifndef HEADER_GRAPHICS_TRANSFORMABLE_H
#define HEADER_GRAPHICS_TRANSFORMABLE_H

#include "system/math.h"

#include <flecs.h>

#ifdef GRAPHICS_TRANSFORMABLE_C
    #define ECS_META_IMPL IMPL
#else
    #define ECS_META_IMPL EXTERN
#endif

ECS_STRUCT(kh_transformable_t, {
    kh_vec2_t      origin;          
    kh_vec2_t      position;        
    float          rotation;        
    kh_vec2_t      scale;
ECS_PRIVATE
    kh_transform_t _transform;     
    kh_transform_t _inv_transform;
    bool           _transform_clean;   
    bool           _inv_transform_clean; 
});

static inline void kh_transformable_set_position(kh_transformable_t *transformable, kh_vec2_t position)
{
    transformable->_transform_dirty = true;
    transformable->_inv_transform_dirty = true;
    transformable->position = position;
}

static inline void kh_transformable_set_rotation(kh_transformable_t *transformable, float radian)
{
    transformable->_transform_dirty = true;
    transformable->_inv_transform_dirty = true;
    transformable->rotation = radian;
}

static inline void kh_transformable_set_scale(kh_transformable_t *transformable, kh_vec2_t factors)
{
    transformable->_transform_dirty = true;
    transformable->_inv_transform_dirty = true;
    transformable->scale = factors;
}

static inline void kh_transformable_set_origin(kh_transformable_t *transformable, kh_vec2_t origin)
{
    transformable->_transform_dirty = true;
    transformable->_inv_transform_dirty = true;
    transformable->origin = origin;
}

static inline void kh_transformable_move(kh_transformable_t *transformable, kh_vec2_t offset)
{
    transformable->_transform_dirty = true;
    transformable->_inv_transform_dirty = true;
    transformable->position = kh_vec2_add(transformable->position, offset);
}

static inline void kh_transformable_rotate(kh_transformable_t *transformable, float radian)
{
    transformable->_transform_dirty = true;
    transformable->_inv_transform_dirty = true;
    transformable->rotation += radian;
}

static inline void kh_transformable_scale(kh_transformable_t *transformable, kh_vec2_t factors)
{
    transformable->_transform_dirty = true;
    transformable->_inv_transform_dirty = true;
    transformable->scale = kh_vec2_mul(transformable->scale, factors);
}

static inline kh_transform_t kh_transformable_get_transform(kh_transformable_t *transformable)
{
    if (transformable->_transform_dirty) {

        float co  = cosf(transformable->rotation);
        float si  = sinf(transformable->rotation);
        float sxc = transformable->scale.x * co;
        float syc = transformable->scale.y * co;
        float sxs = transformable->scale.x * si;
        float sys = transformable->scale.y * si;
        float tx  = transformable->position.x - transformable->origin.x * sxc - transformable->origin.y * sys;
        float ty  = transformable->position.x + transformable->origin.x * sxs - transformable->origin.y * syc;

        transformable->_transform = (kh_transform_t) {{
            { sxc, -sxs }, { sys, syc }, { tx, ty }
        }};
        transformable->_transform_dirty = false;
    }
    return transformable->_transform;
}

static inline kh_transform_t kh_transformable_get_inv_transform(kh_transformable_t *transformable)
{
    if (transformable->_inv_transform_dirty) {

        transformable->_inv_transform = kh_transform_inv(kh_transformable_get_transform(transformable));
        transformable->_inv_transform_dirty = false;
    }
    return transformable->_transform;
}

void KhGraphicsTransformableImport(ecs_world_t *world);

#endif