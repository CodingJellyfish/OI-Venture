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

#ifndef HEADER_SYSTEM_MATH_H
#define HEADER_SYSTEM_MATH_H

#include "system/vector.h"

#include <flecs.h>

#include <math.h>

#ifdef SYSTEM_MATH_C
    #define ECS_META_IMPL IMPL
#else
    #define ECS_META_IMPL EXTERN
#endif

#ifndef KH_EPS
    #define KH_EPS 1e-6
#endif

ECS_STRUCT(kh_vec2_t, {
    float x;
    float y;
});

ECS_STRUCT(kh_rect_t, {
    float x;
    float y;
    float w;
    float h;
});

ECS_STRUCT(kh_transform_t, {
    float elm[3][2];
});

const kh_vec2_t kh_vec2_zero = { 0, 0 };
const kh_vec2_t kh_vec2_one  = { 1, 1 };

const kh_transform_t kh_transform_identity = { { { 1, 0 }, { 0, 1 }, { 0, 0 } }};

static inline float kh_clampf(float a, float min, float max)
{
    return fminf(max, fmaxf(min, a));
}

static inline float kh_lerpf(float a, float b, float amount)
{
    return a + amount * (b - a);
}

static inline float kh_normf(float a, float min, float max)
{
    return (a - min) / (max - min);
}

static inline float kh_remapf(float a, float from_min, float from_max, float to_min, float to_max)
{
    return (a - from_min) / (from_max - from_min) * (to_max - to_min) + to_min;
}

static inline float kh_wrapf(float a, float min, float max)
{
    return a - floorf((a - min) / (max - min)) * (max - min);
}

static inline bool kh_equalf(float a, float b)
{
    return fabsf(a - b) <= KH_EPS * fmaxf(1.0f, fmaxf(fabsf(a), fabsf(b)));
}

static inline kh_vec2_t kh_vec2_neg(kh_vec2_t a)
{
    return (kh_vec2_t) { -a.x, -a.y };
}

static inline kh_vec2_t kh_vec2_add(kh_vec2_t a, kh_vec2_t b)
{
    return (kh_vec2_t) { a.x + b.x, a.y + b.y };
}

static inline kh_vec2_t kh_vec2_addf(kh_vec2_t a, float b)
{
    return (kh_vec2_t) { a.x + b, a.y + b };
}

static inline kh_vec2_t kh_vec2_sub(kh_vec2_t a, kh_vec2_t b)
{
    return (kh_vec2_t) { a.x - b.x, a.y - b.y };
}

static inline kh_vec2_t kh_vec2_subf(kh_vec2_t a, float b)
{
    return (kh_vec2_t) { a.x - b, a.y - b };
}

static inline kh_vec2_t kh_vec2_mul(kh_vec2_t a, kh_vec2_t b)
{
    return (kh_vec2_t) { a.x * b.x, a.y * b.y };
}

static inline kh_vec2_t kh_vec2_mulf(kh_vec2_t a, float b)
{
    return (kh_vec2_t) { a.x * b, a.y * b };
}

static inline kh_vec2_t kh_vec2_div(kh_vec2_t a, kh_vec2_t b)
{
    return (kh_vec2_t) { a.x / b.x, a.y / b.y };
}

static inline kh_vec2_t kh_vec2_divf(kh_vec2_t a, float b)
{
    return (kh_vec2_t) { a.x / b, a.y / b };
}

static inline float kh_vec2_dot(kh_vec2_t a, kh_vec2_t b)
{
    return a.x * b.x + a.y * b.y;
}

static inline float kh_vec2_det(kh_vec2_t a, kh_vec2_t b)
{
    return a.x * b.y - a.y * b.x;
}

static inline kh_vec2_t kh_vec2_transform(kh_vec2_t a, kh_transform_t b)
{
    return (kh_vec2_t) {
        a.x * b.elm[0][0] + a.y * b.elm[1][0] + b.elm[2][0],
        a.x * b.elm[0][1] + a.y * b.elm[1][1] + b.elm[2][1]
    };
}

static inline float kh_vec2_len_square(kh_vec2_t a)
{
    return kh_vec2_dot(a, a);
}

static inline float kh_vec2_len(kh_vec2_t a)
{
    return sqrtf(kh_vec2_len_square(a));
}

static inline float kh_vec2_dis_square(kh_vec2_t a, kh_vec2_t b)
{
    return kh_vec2_len_square(kh_vec2_sub(a, b));
}

static inline float kh_vec2_dis(kh_vec2_t a, kh_vec2_t b)
{
    return kh_vec2_len(kh_vec2_sub(a, b));
}

static inline float kh_vec2_angle(kh_vec2_t a, kh_vec2_t b)
{
    return -atan2f(kh_vec2_det(a, b), kh_vec2_dot(a, b));
}

static inline kh_vec2_t kh_vec2_norm(kh_vec2_t a)
{
    float len = kh_vec2_len(a);
    return len > 0 ? kh_vec2_divf(a, len) : kh_vec2_zero;
}

static inline kh_vec2_t kh_vec2_lerp(kh_vec2_t a, kh_vec2_t b, float amount)
{
    return kh_vec2_add(a, kh_vec2_mulf(kh_vec2_sub(b, a), amount));
}

static inline kh_vec2_t kh_vec2_reflect(kh_vec2_t a, kh_vec2_t normal)
{
    return kh_vec2_sub(a, kh_vec2_mulf(normal, kh_vec2_dot(a, normal) * 2.0f));
}

static inline kh_vec2_t kh_vec2_rotate(kh_vec2_t a, float radian)
{
    kh_vec2_t b = { cosf(radian), sinf(radian) };
    return (kh_vec2_t) { a.x * b.x - a.y * b.y, a.x * b.y + b.x * a.y };
}

static inline kh_vec2_t kh_vec2_move_towards(kh_vec2_t a, kh_vec2_t b, float max)
{
    kh_vec2_t d = kh_vec2_sub(a, b);
    float len = kh_vec2_len_square(d);
    return len == 0 || (max > 0 && len < max * max) ? b : kh_vec2_add(a, kh_vec2_divf(d, sqrtf(len) / max));
}

static inline kh_vec2_t kh_vec2_inv(kh_vec2_t a)
{
    return kh_vec2_div(kh_vec2_one, a);
}

static inline kh_vec2_t kh_vec2_clamp(kh_vec2_t a, kh_vec2_t min, kh_vec2_t max)
{
    return (kh_vec2_t) { kh_clampf(a.x, min.x, max.x), kh_clampf(a.y, min.y, max.y) };
}

static inline kh_vec2_t kh_vec2_clampf(kh_vec2_t a, float min, float max)
{
    float len = kh_vec2_len_square(a);
    if (len == 0) {
        return kh_vec2_zero;
    } else if (len < min * min) {
        return kh_vec2_mulf(a, min / sqrtf(len));
    } else if (len > max * max) {
        return kh_vec2_mulf(a, max / sqrtf(len));
    } else {
        return a;
    }
}

static inline bool kh_vec2_equal(kh_vec2_t a, kh_vec2_t b)
{
    return kh_equalf(a.x, b.x) && kh_equalf(a.y, b.y);
}

static inline kh_transform_t kh_transform_mul(kh_transform_t a, kh_transform_t b)
{
    return (kh_transform_t) {{{
            a.elm[0][0] * b.elm[0][0] + a.elm[0][1] * b.elm[1][0],
            a.elm[0][0] * b.elm[0][1] + a.elm[0][1] * b.elm[1][1]
        }, {
            a.elm[1][0] * b.elm[0][0] + a.elm[1][1] * b.elm[1][0],
            a.elm[1][0] * b.elm[0][1] + a.elm[1][1] * b.elm[1][1]
        }, {
            a.elm[2][0] * b.elm[0][0] + a.elm[2][1] * b.elm[1][0] + b.elm[2][0],
            a.elm[2][0] * b.elm[0][1] + a.elm[2][1] * b.elm[1][1] + b.elm[2][1]
    }}};
}

static inline kh_transform_t kh_transform_inv(kh_transform_t a)
{
    float det = a.elm[0][0] * a.elm[1][1] - a.elm[1][0] * a.elm[0][1];
    return (kh_transform_t) {{{
            a.elm[1][1] / det,
            -a.elm[0][1] / det
        }, {
            -a.elm[1][0] / det,
            a.elm[0][0] / det
        }, {
            (a.elm[1][0] * a.elm[2][1] - a.elm[1][1] * a.elm[2][0]) / det,
            (a.elm[0][1] * a.elm[2][0] - a.elm[0][0] * a.elm[2][1]) / det
    }}};
}

void KhSystemMathImport(ecs_world_t *world);

#endif