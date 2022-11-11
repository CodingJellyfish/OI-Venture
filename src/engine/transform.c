// An adventure story featuring a couple of Chinese programming geniuses.
// Copyright (C) 2022 CodingJellyfish

// This program is free software: you can redistribute it and/or modify
// it under the terms of the GNU General Public License as published by
// the Free Software Foundation, either version 3 of the License, or
// (at your option) any later version.

// This program is distributed in the hope that it will be useful,
// but WITHOUT ANY WARRANTY; without even the implied warranty of
// MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
// GNU General Public License for more details.

// You should have received a copy of the GNU General Public License
// along with this program.  If not, see <https://www.gnu.org/licenses/>.

#include "engine/transform.h"

#include <math.h>

ECS_COMPONENT_DECLARE(transform_t);

ECS_DECLARE(TransformChildOf);

static void check_transform_avaliable(ecs_iter_t *it)
{
    transform_t *transform_col = ecs_field(it, transform_t, 1);
    for (int i = 0; i < it->count; i++) {
        if (transform_col[i].scale.x == 0.0f) {
            transform_col[i].scale.x = 1.0f;
        }
        if (transform_col[i].scale.y == 0.0f) {
            transform_col[i].scale.y = 1.0f;
        }
    }
}

static void calc_transform(ecs_iter_t *it)
{
    transform_t *transform_col = ecs_field(it, transform_t, 1);

    /*if (ecs_field_is_set(it, 2)) {
        transform_t *transform_parent_col = ecs_field(it, transform_t, 2);
        for (int i = 0; i < it->count; i++) {
            transform_col[i]._o = transform_parent_col[i]._t;
            transform_col[i]._o.x += transform_col[i].origin.x;
            transform_col[i]._o.y += transform_col[i].origin.y;

            transform_col[i]._t = transform_parent_col[i]._t;
            transform_col[i]._t.x += transform_col[i].translation.x
                                   * transform_col[i].scale.x
                                   * cosf(DEG2RAD * transform_parent_col[i]._r);
            transform_col[i]._t.x += transform_col[i].translation.y
                                   * transform_col[i].scale.y
                                   * sinf(DEG2RAD * transform_parent_col[i]._r);
            transform_col[i]._t.y += transform_col[i].translation.y
                                   * transform_col[i].scale.y
                                   * cosf(DEG2RAD * transform_parent_col[i]._r);
            transform_col[i]._t.y += transform_col[i].translation.x
                                   * transform_col[i].scale.x
                                   * sinf(DEG2RAD * transform_parent_col[i]._r);
            
            transform_col[i]._r = transform_parent_col[i]._r;
            transform_col[i]._r += transform_col[i].rotation;

            transform_col[i]._s = transform_parent_col[i]._s;
            transform_col[i]._s.x *= transform_col[i].scale.x;
            transform_col[i]._s.y *= transform_col[i].scale.y;
        }
    } else {*/
        for (int i = 0; i < it->count; i++) {
            transform_col[i]._t = transform_col[i].translation;
            
            transform_col[i]._r = transform_col[i].rotation;

            transform_col[i]._s = transform_col[i].scale;

            transform_col[i]._o = transform_col[i].origin;
        }
    //}
}

Vector2 do_translation(Vector2 point, Vector2 tran)
{
    point.x += tran.x;
    point.y += tran.y;
    return point;
}

Vector2 do_rotation(Vector2 point, Vector2 origin, float angle)
{
    Vector2 ret = origin;
    if (angle) {
        ret.x += (point.x - origin.x) * cosf(DEG2RAD * angle);
        ret.x += (point.y - origin.y) * sinf(DEG2RAD * angle);
        ret.y += (point.x - origin.x) * sinf(DEG2RAD * angle);
        ret.y += (point.y - origin.y) * cosf(DEG2RAD * angle);
    }
    return ret;
}

Vector2 do_scale(Vector2 point, Vector2 origin, Vector2 scale)
{
    Vector2 ret = origin;
    ret.x += (point.x - origin.x) * scale.x;
    ret.y += (point.y - origin.y) * scale.y;
    return ret;
}

Vector2 do_transform(Vector2 point, transform_t *tran)
{
    Vector2 ret = point;
    ret = do_scale(point, tran->_o, tran->_s);
    ret = do_rotation(point, tran->_o, tran->_r);
    ret = do_translation(point, tran->_t);
    return ret;
}

void TransformModuleImport(ecs_world_t *ecs)
{
    ECS_MODULE(ecs, TransformModule);

    ECS_COMPONENT_DEFINE(ecs, transform_t);

    ECS_TAG_DEFINE(ecs, TransformChildOf);

    ecs_add_id(ecs, TransformChildOf, EcsAcyclic);

    ecs_observer_init(ecs, &(ecs_observer_desc_t) {
        .filter.terms = { { ecs_id(transform_t) } },
        .events = { EcsOnSet },
        .callback = check_transform_avaliable
    });
    ecs_system_init(ecs, &(ecs_system_desc_t) {
        .entity = ecs_entity(ecs, {
            .add = { ecs_dependson(EcsPreUpdate) }
        }),
        .query.filter.terms = {
            { ecs_id(transform_t) },
            { ecs_id(transform_t), .src = { .flags = EcsSelf | EcsUp, .trav = TransformChildOf }, .oper = EcsOptional }
        },
        .callback = calc_transform
    });
}
