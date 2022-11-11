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

#ifndef HEADER_DRAW_H
#define HEADER_DRAW_H

#include <flecs.h>

#include <raylib.h>

#include "engine/transform.h"
#include "engine/window.h"
#include "utils/array.h"

#define DEFAULT_SEGMENTS 30

typedef enum {
    COLOR_PLAIN,
    COLOR_FADE,
    COLOR_FADE_LINEAR,
    COLOR_FADE_RADIAL
} draw_type_color_t;

typedef struct {
    draw_type_color_t type;
    Color color;
    Color color2;
    union {
        struct {
            Color color3;
            Color color4;
        };
        struct {
            Vector2 start;
            Vector2 end;
        } linear;
        struct {
            Vector2 center;
            float inner;
            float outer;
        } radial;
    };
} draw_color_t;

typedef struct {
    Font font;
} draw_font_t;

typedef struct {
    draw_font_t font;
    Texture2D tex;
    Rectangle texrec;
    Vector2 texorigin;
    draw_color_t color;
    bool flip;
} draw_entity_info_t;

typedef enum {
    DRAW_BEZIER,
    DRAW_BEZIER_TEXFILL,
    DRAW_BEZIER_QUAD,
    DRAW_BEZIER_QUAD_TEXFILL,
    DRAW_BEZIER_QUAD_FILL,
    DRAW_BEZIER_QUAD_FILL_TEXFILL,
    DRAW_BEZIER_QUAD_FILL_CTRL,
    DRAW_BEZIER_QUAD_FILL_CTRL_TEXFILL,
    DRAW_ELLIPSE,
    DRAW_ELLIPSE_TEXFILL,
    DRAW_ELLIPSE_FILL,
    DRAW_ELLIPSE_FILL_TEXFILL,
    DRAW_PATH,
    DRAW_PATH_TEXFILL,
    DRAW_PATH_FILL,
    DRAW_PATH_FILL_TEXFILL,
    DRAW_LINE,
    DRAW_LINE_TEXFILL,
    DRAW_LINE_2,
    DRAW_LINE_2_TEXFILL,
    DRAW_LINE_STRIP_2,
    DRAW_LINE_STRIP_2_TEXFILL,
    DRAW_LINE_STRIP_3,
    DRAW_LINE_STRIP_3_TEXFILL,
    DRAW_QUAD,
    DRAW_QUAD_TEXFILL,
    DRAW_QUAD_OUTLINE,
    DRAW_QUAD_OUTLINE_TEXFILL,
    DRAW_RECT,
    DRAW_RECT_TEXFILL,
    DRAW_RECT_OUTLINE,
    DRAW_RECT_OUTLINE_TEXFILL,
    DRAW_RECT_2,
    DRAW_RECT_2_TEXFILL,
    DRAW_RECT_2_OUTLINE,
    DRAW_RECT_2_OUTLINE_TEXFILL,
    DRAW_TRIANGLE,
    DRAW_TRIANGLE_TEXFILL,
    DRAW_TRIANGLE_OUTLINE,
    DRAW_TRIANGLE_OUTLINE_TEXFILL,
} draw_type_shape_t;

typedef struct {
    bool is_3d;
    bool has_camera;
    bool has_vr;
    Color background;
    ecs_entity_t draw_system;
    RenderTexture2D canvas;
    union {
        Camera2D two;
        Camera3D three;
    } camera;
    VrStereoConfig vr;
} draw_canvas_t;

typedef struct {
    void *arg;
    void (*func)(void*, draw_entity_info_t*);
} draw_custom_t;

typedef struct {
    Shader    shader;
    int       blend;
    Rectangle scissor;
    bool      has_shader;
    bool      has_blend;
    bool      has_scissor;
} draw_mode_t;

typedef struct {
    float priority;
} draw_priority_t;

typedef struct {
    draw_type_shape_t type;
    short segments;
    float thickness;
    union {
        struct {
            Vector2 start;
            Vector2 end;
            Vector2 radius;
            float angle;
            bool sweep;
            bool largearc;
        } ellipse;
        struct {
            Rectangle rect;
            Rectangle rect2;
        } rect;
        struct {
            Vector2 point[4];
        } vertex;
    };
} draw_shape_t;

typedef struct {
    Rectangle rect;
    Vector2 origin;
    int framecnt;
    int frametime;
    int framecur;
} draw_sprite_t;

typedef struct {
    char *str;
    Vector2 origin;
    float size;
    float space;
} draw_text_t;

typedef struct {
    Texture2D texture;
} draw_texture_t;

extern ECS_COMPONENT_DECLARE(draw_canvas_t);
extern ECS_COMPONENT_DECLARE(draw_color_t);
extern ECS_COMPONENT_DECLARE(draw_custom_t);
extern ECS_COMPONENT_DECLARE(draw_font_t);
extern ECS_COMPONENT_DECLARE(draw_mode_t);
extern ECS_COMPONENT_DECLARE(draw_priority_t);
extern ECS_COMPONENT_DECLARE(draw_shape_t);
extern ECS_COMPONENT_DECLARE(draw_sprite_t);
extern ECS_COMPONENT_DECLARE(draw_text_t);
extern ECS_COMPONENT_DECLARE(draw_texture_t);

extern ECS_DECLARE(Draw);

void DrawModuleImport(ecs_world_t *ecs);

#endif