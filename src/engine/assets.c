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

#include "engine/assets.h"

IMPL_ARRAY(Vector2)

ECS_COMPONENT_DECLARE(Font);
ECS_COMPONENT_DECLARE(Image);
ECS_COMPONENT_DECLARE(Texture2D);
ECS_COMPONENT_DECLARE(RenderTexture2D);
ECS_COMPONENT_DECLARE(vertex_array);

static void remove_canvas(ecs_iter_t *it)
{
    RenderTexture2D *canvas_col = ecs_field(it, RenderTexture2D, 1);
    for (int i = 0; i < it->count; i++) {
        UnloadRenderTexture(canvas_col[i]);
    }
}

static void remove_font(ecs_iter_t *it)
{
    Font *font_col = ecs_field(it, Font, 1);
    for (int i = 0; i < it->count; i++) {
        UnloadFont(font_col[i]);
    }
}

static void remove_image(ecs_iter_t *it)
{
    Image *font_col = ecs_field(it, Image, 1);
    for (int i = 0; i < it->count; i++) {
        UnloadImage(font_col[i]);
    }
}

static void remove_texture(ecs_iter_t *it)
{
    Texture2D *texture_col = ecs_field(it, Texture2D, 1);
    for (int i = 0; i < it->count; i++) {
        UnloadTexture(texture_col[i]);
    }
}

static void remove_vertex_array(ecs_iter_t *it)
{
    vertex_array *arr_col = ecs_field(it, vertex_array, 1);
    for (int i = 0; i < it->count; i++) {
        free(arr_col[i].n);
    }
}


void AssetsModuleImport(ecs_world_t *ecs)
{
    ECS_MODULE(ecs, AssetsModule);

    ECS_COMPONENT_DEFINE(ecs, Font);
    ECS_COMPONENT_DEFINE(ecs, Image);
    ECS_COMPONENT_DEFINE(ecs, Texture2D);
    ECS_COMPONENT_DEFINE(ecs, RenderTexture2D);
    ECS_COMPONENT_DEFINE(ecs, vertex_array);

    ecs_observer_init(ecs, &(ecs_observer_desc_t) {
        .filter.terms = { { ecs_id(Font) } },
        .events = { EcsOnRemove },
        .callback = remove_font
    });
    ecs_observer_init(ecs, &(ecs_observer_desc_t) {
        .filter.terms = { { ecs_id(Image) } },
        .events = { EcsOnRemove },
        .callback = remove_image
    });
    ecs_observer_init(ecs, &(ecs_observer_desc_t) {
        .filter.terms = { { ecs_id(Texture2D) } },
        .events = { EcsOnRemove },
        .callback = remove_texture
    });
    ecs_observer_init(ecs, &(ecs_observer_desc_t) {
        .filter.terms = { { ecs_id(RenderTexture2D) } },
        .events = { EcsOnRemove },
        .callback = remove_canvas
    });
    ecs_observer_init(ecs, &(ecs_observer_desc_t) {
        .filter.terms = { { ecs_id(vertex_array) } },
        .events = { EcsOnRemove },
        .callback = remove_vertex_array
    });
}
