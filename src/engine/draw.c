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

#include "engine/draw.h"
#include "utils/gfx.h"
#include "utils/tools.h"

#include <math.h>

ECS_COMPONENT_DECLARE(draw_canvas_t);
ECS_COMPONENT_DECLARE(draw_color_t);
ECS_COMPONENT_DECLARE(draw_custom_t);
ECS_COMPONENT_DECLARE(draw_font_t);
ECS_COMPONENT_DECLARE(draw_mode_t);
ECS_COMPONENT_DECLARE(draw_priority_t);
ECS_COMPONENT_DECLARE(draw_shape_t);
ECS_COMPONENT_DECLARE(draw_sprite_t);
ECS_COMPONENT_DECLARE(draw_text_t);
ECS_COMPONENT_DECLARE(draw_texture_t);

ECS_DECLARE(Draw);

static void begin_draw(ecs_iter_t *it)
{   
    BeginDrawing();

    init_config_t *conf = ecs_field(it, init_config_t, 1);
    ClearBackground(conf->background);
}

static void end_draw(ecs_iter_t *it)
{
    init_config_t *conf = ecs_field(it, init_config_t, 1);
    if (conf->draw_fps) {
        DrawFPS(5, 5);
    }
    EndDrawing();
}

static inline Color calc_color(draw_color_t color, Vector2 pos, int ind)
{
    switch (color.type) {
    case COLOR_PLAIN:
        return color.color;
    case COLOR_FADE:
        switch (ind) {
            case 0:
                return color.color;
            case 1:
                return color.color2;
            case 2:
                return color.color3;
            case 3:
                return color.color4;
            default:
                return color.color;
        }
    case COLOR_FADE_LINEAR:
    {
        float t = ((pos.x - color.linear.start.x) * (color.linear.end.x - color.linear.start.x)
                 + (pos.y - color.linear.start.y) * (color.linear.end.y - color.linear.start.y))
                / ((color.linear.end.x - color.linear.start.x) * (color.linear.end.x - color.linear.start.x)
                 + (color.linear.end.y - color.linear.start.y) * (color.linear.end.y - color.linear.start.y));
        t = t > 0.0f ? t < 1.0f ? t : 1.0f : 0.0f;
        return (Color) {
            .r = t * color.color2.r + (1.0f - t) * color.color.r,
            .g = t * color.color2.g + (1.0f - t) * color.color.g,
            .b = t * color.color2.b + (1.0f - t) * color.color.b,
            .a = t * color.color2.a + (1.0f - t) * color.color.a
        };
    }
    break;
    case COLOR_FADE_RADIAL:
    {
        float dis = sqrtf((pos.x - color.radial.center.x) * (pos.x - color.radial.center.x)
                        + (pos.y - color.radial.center.y) * (pos.y - color.radial.center.y));
        float t = (dis - color.radial.inner) / (color.radial.outer - color.radial.inner);
        t = t > 0.0f ? t < 1.0f ? t : 1.0f : 0.0f;
        return (Color) {
            .r = t * color.color2.r + (1.0f - t) * color.color.r,
            .g = t * color.color2.g + (1.0f - t) * color.color.g,
            .b = t * color.color2.b + (1.0f - t) * color.color.b,
            .a = t * color.color2.a + (1.0f - t) * color.color.a
        };
    }
    break;
    }
}

static inline void check_mode(bool has, draw_mode_t *col, int ind)
{
    if (has) {
        if (col[ind].has_blend) {
            BeginBlendMode(col[ind].blend);
        } else {
            EndBlendMode();
        }
        if (col[ind].has_scissor) {
            BeginScissorMode(col[ind].scissor.x, col[ind].scissor.y, col[ind].scissor.width, col[ind].scissor.height);
        } else {
            EndScissorMode();
        }
        if (col[ind].has_shader) {
            BeginShaderMode(col[ind].shader);
        } else {
            EndShaderMode();
        }
    }
}

static inline void check_transform(bool has, transform_t *col, bool *flip, int ind)
{
    if (has) {
        rlLoadIdentity();
        rlTranslatef(col[ind]._t.x, col[ind]._t.y, 0.0f);
        if (col[ind]._r) {
            rlRotatef(col[ind]._r, col[ind]._o.x, col[ind]._o.y, 0.0f);
        }
        *flip = col[ind]._s.x * col[ind]._s.y < 0.0f;
        rlScalef(col[ind]._s.x, col[ind]._s.y, 1.0f);
        rlTranslatef(-col[ind]._o.x, -col[ind]._o.y, 0.0f);
    } else {
        *flip = false;
    }
}

static inline void check_tex(bool has, Texture2D *tex, Rectangle *rec, draw_texture_t *col, int ind)
{
    if (has) {
        *tex = col[ind].texture;
        *rec = (Rectangle) {
            .x = 0.0f,
            .y = 0.0f,
            .width = col[ind].texture.width,
            .height = col[ind].texture.height
        };
    }
}

static inline void check_canvas(bool has, Texture2D *tex, Rectangle *rec, draw_canvas_t *col, int ind)
{
    if (has) {
        *tex = col[ind].canvas.texture;
        *rec = (Rectangle) {
            .x = 0.0f,
            .y = 0.0f,
            .width = col[ind].canvas.texture.width,
            .height = col[ind].canvas.texture.height
        };
    }
}

static inline void check_sprite(bool has, Rectangle *rec, Vector2 *ori, draw_sprite_t *col, int ind)
{
    if (has) {
        *rec = (Rectangle) {
            .x = col[ind].rect.width / col[ind].framecnt
            * (col[ind].framecur / col[ind].frametime),
            .y = col[ind].rect.y,
            .width = col[ind].rect.width / col[ind].framecnt,
            .height = col[ind].rect.height
        };
        *ori = col[ind].origin;
    }
}

static inline void check_custom(bool has, draw_entity_info_t *info, draw_custom_t *col, int ind)
{
    if (has) {
        col[ind].func(col[ind].arg, info);
    }
}

static void draw_text(draw_text_t text, draw_font_t font, draw_color_t color)
{
    DrawTextEx(
        font.font,
        text.str,
        text.origin,
        text.size, 
        text.space,
        color.color
    );
}

static void draw_shape(draw_shape_t shape, draw_color_t color, Texture2D tex, Rectangle texrect, Vector2 texorigin, bool flip)
{
    int segments = shape.segments ? shape.segments : DEFAULT_SEGMENTS;

    switch (shape.type) {
    case DRAW_BEZIER:
    case DRAW_BEZIER_TEXFILL:
    {
        Vector2 previous = { 0 }, previous2 = { 0 };
        Vector2 current = { 0 }, current2 = { 0 };

        for (int j = 0; j <= segments; j++)
        {
            float t = 1.0f / segments * j;
            float fac1 = powf(1.0f - t, 2.0f);
            float fac2 = 2.0f * (1.0f - t) * t;
            float fac3 = powf(t, 2.0f);

            float vec1x = fac1 * (shape.vertex.point[0].y - shape.vertex.point[2].y)
                        + fac2 * (shape.vertex.point[2].y - shape.vertex.point[3].y)
                        + fac3 * (shape.vertex.point[3].y - shape.vertex.point[1].y);
            float vec1y = fac1 * (shape.vertex.point[2].x - shape.vertex.point[0].x)
                        + fac2 * (shape.vertex.point[3].x - shape.vertex.point[2].x)
                        + fac3 * (shape.vertex.point[1].x - shape.vertex.point[3].x);
            float veclen = sqrtf(vec1x * vec1x + vec1y * vec1y) / shape.thickness * 2.0f;
            vec1x /= veclen;
            vec1y /= veclen;
            
            float a = powf(1.0f - t, 3.0f);
            float b = 3.0f * powf(1.0f - t, 2.0f) * t;
            float c = 3.0f * (1.0f - t) * powf(t, 2.0f);
            float d = powf(t, 3.0f);
            current.y = a * shape.vertex.point[0].y
                        + b * shape.vertex.point[2].y
                        + c * shape.vertex.point[3].y
                        + d * shape.vertex.point[1].y;
            current.x = a * shape.vertex.point[0].x
                        + b * shape.vertex.point[2].x
                        + c * shape.vertex.point[3].x
                        + d * shape.vertex.point[1].x;

            current2 = current;
            current.x += vec1x;
            current.y += vec1y;
            current2.x -= vec1x;
            current2.y -= vec1y;

            if (j)
            {              
                draw_quad(
                    previous2, previous, current, current2,
                    tex, texrect, shape.type & 1 ? NULL : &texorigin, 
                    calc_color(color, previous2, 0),
                    calc_color(color, previous, 1),
                    calc_color(color, current, 2),
                    calc_color(color, current2, 3),
                    flip
                );
            }
            previous = current;
            previous2 = current2;
        }
    }
    break;   
    case DRAW_BEZIER_QUAD:
    case DRAW_BEZIER_QUAD_TEXFILL:
    {
        Vector2 previous = { 0 }, previous2 = { 0 };
        Vector2 current = { 0 }, current2 = { 0 };

        for (int j = 0; j <= segments; j++)
        {
            float t = 1.0f / segments * j;
            float fac1 = 1.0f - t;
            float fac2 = t;

            float vec1x = fac1 * (shape.vertex.point[0].y - shape.vertex.point[2].y)
                        + fac2 * (shape.vertex.point[2].y - shape.vertex.point[1].y);
            float vec1y = fac1 * (shape.vertex.point[2].x - shape.vertex.point[0].x)
                        + fac2 * (shape.vertex.point[1].x - shape.vertex.point[2].x);
            float veclen = sqrtf(vec1x * vec1x + vec1y * vec1y) / shape.thickness * 2.0f;
            vec1x /= veclen;
            vec1y /= veclen;
            
            float a = powf(1.0f - t, 2.0f);
            float b = 2.0f * (1.0f - t) * t;
            float c = powf(t, 2.0f);
            current.y = a * shape.vertex.point[0].y
                        + b * shape.vertex.point[2].y
                        + c * shape.vertex.point[1].y;
            current.x = a * shape.vertex.point[0].x
                        + b * shape.vertex.point[2].x
                        + c * shape.vertex.point[1].x;

            current2 = current;
            current.x += vec1x;
            current.y += vec1y;
            current2.x -= vec1x;
            current2.y -= vec1y;

            if (j)
            {              
                draw_quad(
                    previous2, previous, current, current2,
                    tex, texrect, shape.type & 1 ? NULL : &texorigin, 
                    calc_color(color, previous2, 0),
                    calc_color(color, previous, 1),
                    calc_color(color, current, 2),
                    calc_color(color, current2, 3),
                    flip
                );
            }
            previous = current;
            previous2 = current2;
        }
    }
    break; 
    case DRAW_BEZIER_QUAD_FILL:
    case DRAW_BEZIER_QUAD_FILL_TEXFILL:
    {
        Vector2 from = shape.vertex.point[0], to = shape.vertex.point[1], ctrl = shape.vertex.point[2];
        if ((ctrl.x - from.x) * (to.y - from.y) - (ctrl.y - from.y) * (to.x - from.x) > 0) {
            Vector2 tmp = from;
            from = to;
            to = tmp;
        }
        Vector2 previous = { 0 }, previous2 = { 0 };
        Vector2 current = { 0 };

        for (int j = 0; j <= segments; j++)
        {
            float t = 1.0f / segments * j;
            
            float a = powf(1.0f - t, 2.0f);
            float b = 2.0f * (1.0f - t) * t;
            float c = powf(t, 2.0f);
            current.y = a * from.y + b * ctrl.y + c * to.y;
            current.x = a * from.x + b * ctrl.x + c * to.x;

            if (j && !(j & 1)) {              
                draw_quad(
                    previous2, previous, current, shape.vertex.point[0],
                    tex, texrect, shape.type & 1 ? NULL : &texorigin, 
                    calc_color(color, previous2, 0),
                    calc_color(color, previous, 1),
                    calc_color(color, current, 2),
                    calc_color(color, shape.vertex.point[0], 3),
                    flip
                );
            } else if (j == segments) {
                draw_quad(
                    previous, previous, current, shape.vertex.point[0],
                    tex, texrect, shape.type & 1 ? NULL : &texorigin, 
                    calc_color(color, previous, 0),
                    calc_color(color, previous, 1),
                    calc_color(color, current, 2),
                    calc_color(color, shape.vertex.point[0], 3),
                    flip
                );
            }
            previous2 = previous;
            previous = current;
        }
    }
    break; 
    case DRAW_BEZIER_QUAD_FILL_CTRL:
    case DRAW_BEZIER_QUAD_FILL_CTRL_TEXFILL:
    {
        Vector2 from = shape.vertex.point[0], to = shape.vertex.point[1], ctrl = shape.vertex.point[2];
        if ((ctrl.x - from.x) * (to.y - from.y) - (ctrl.y - from.y) * (to.x - from.x) < 0) {
            Vector2 tmp = from;
            from = to;
            to = tmp;
        }
        Vector2 previous = { 0 }, previous2 = { 0 };
        Vector2 current = { 0 };

        for (int j = 0; j <= segments; j++)
        {
            float t = 1.0f / segments * j;
            
            float a = powf(1.0f - t, 2.0f);
            float b = 2.0f * (1.0f - t) * t;
            float c = powf(t, 2.0f);
            current.y = a * from.y + b * ctrl.y + c * to.y;
            current.x = a * from.x + b * ctrl.x + c * to.x;

            if (j && !(j & 1)) {              
                draw_quad(
                    previous2, previous, current, ctrl,
                    tex, texrect, shape.type & 1 ? NULL : &texorigin, 
                    calc_color(color, previous2, 0),
                    calc_color(color, previous, 1),
                    calc_color(color, current, 2),
                    calc_color(color, ctrl, 3),
                    flip
                );
            } else if (j == segments) {
                draw_quad(
                    previous, previous, current, ctrl,
                    tex, texrect, shape.type & 1 ? NULL : &texorigin, 
                    calc_color(color, previous, 0),
                    calc_color(color, previous, 1),
                    calc_color(color, current, 2),
                    calc_color(color, ctrl, 3),
                    flip
                );
            }
            previous2 = previous;
            previous = current;
        }
    }
    break; 
    case DRAW_LINE_2:
    case DRAW_LINE_2_TEXFILL:
    {
        float vec1x = shape.vertex.point[2].y - shape.vertex.point[3].y;
        float vec1y = shape.vertex.point[3].x - shape.vertex.point[2].x;
        float veclen = sqrtf(vec1x * vec1x + vec1y * vec1y) / shape.thickness * 2.0f;
        vec1x /= veclen;
        vec1y /= veclen;
        Vector2 x1 = shape.vertex.point[2];
        x1.x -= vec1x, x1.y -= vec1y;
        Vector2 x2 = shape.vertex.point[2];
        x2.x += vec1x, x2.y += vec1y;
        Vector2 x3 = shape.vertex.point[3];
        x3.x += vec1x, x3.y += vec1y;
        Vector2 x4 = shape.vertex.point[3];
        x4.x -= vec1x, x4.y -= vec1y;

        draw_quad(
            x1, x2, x3, x4,
            tex, texrect, shape.type & 1 ? NULL : &texorigin, 
            calc_color(color, x1, 0),
            calc_color(color, x2, 1),
            calc_color(color, x3, 2),
            calc_color(color, x4, 3),
            flip
        );
    }
    case DRAW_LINE:
    case DRAW_LINE_TEXFILL:
    {
        float vec1x = shape.vertex.point[0].y - shape.vertex.point[1].y;
        float vec1y = shape.vertex.point[1].x - shape.vertex.point[0].x;
        float veclen = sqrtf(vec1x * vec1x + vec1y * vec1y) / shape.thickness * 2.0f;
        vec1x /= veclen;
        vec1y /= veclen;
        Vector2 x1 = shape.vertex.point[0];
        x1.x -= vec1x, x1.y -= vec1y;
        Vector2 x2 = shape.vertex.point[0];
        x2.x += vec1x, x2.y += vec1y;
        Vector2 x3 = shape.vertex.point[1];
        x3.x += vec1x, x3.y += vec1y;
        Vector2 x4 = shape.vertex.point[1];
        x4.x -= vec1x, x4.y -= vec1y;

        draw_quad(
            x1, x2, x3, x4,
            tex, texrect, shape.type & 1 ? NULL : &texorigin, 
            calc_color(color, x1, 0),
            calc_color(color, x2, 1),
            calc_color(color, x3, 2),
            calc_color(color, x4, 3),
            flip
        );
    }
    break;
    case DRAW_QUAD:
    case DRAW_QUAD_TEXFILL:
    {
        draw_quad(
            shape.vertex.point[0],
            shape.vertex.point[1],
            shape.vertex.point[2],
            shape.vertex.point[3],
            tex, texrect, shape.type & 1 ? NULL : &texorigin, 
            calc_color(color, shape.vertex.point[0], 0),
            calc_color(color, shape.vertex.point[1], 1),
            calc_color(color, shape.vertex.point[2], 2),
            calc_color(color, shape.vertex.point[3], 3),
            flip
        );
    }
    break;
    case DRAW_RECT_2:
    case DRAW_RECT_2_TEXFILL:
    {
        Vector2 x1 = (Vector2) {
            shape.rect.rect2.x,
            shape.rect.rect2.y
        };
        Vector2 x2 = (Vector2) {
            shape.rect.rect2.x,
            shape.rect.rect2.y + shape.rect.rect2.height
        };
        Vector2 x3 = (Vector2) {
            shape.rect.rect2.x + shape.rect.rect2.width,
            shape.rect.rect2.y + shape.rect.rect2.height
        };
        Vector2 x4 = (Vector2) {
            shape.rect.rect2.x + shape.rect.rect2.width,
            shape.rect.rect2.y
        };

        draw_quad(
            x1, x2, x3, x4,
            tex, texrect, shape.type & 1 ? NULL : &texorigin, 
            calc_color(color, x1, 0),
            calc_color(color, x2, 1),
            calc_color(color, x3, 2),
            calc_color(color, x4, 3),
            flip
        );
    }
    case DRAW_RECT:
    case DRAW_RECT_TEXFILL:
    {
        Vector2 x1 = (Vector2) {
            shape.rect.rect.x,
            shape.rect.rect.y
        };
        Vector2 x2 = (Vector2) {
            shape.rect.rect.x,
            shape.rect.rect.y + shape.rect.rect.height
        };
        Vector2 x3 = (Vector2) {
            shape.rect.rect.x + shape.rect.rect.width,
            shape.rect.rect.y + shape.rect.rect.height
        };
        Vector2 x4 = (Vector2) {
            shape.rect.rect.x + shape.rect.rect.width,
            shape.rect.rect.y
        };

        draw_quad(
            x1, x2, x3, x4,
            tex, texrect, shape.type & 1 ? NULL : &texorigin, 
            calc_color(color, x1, 0),
            calc_color(color, x2, 1),
            calc_color(color, x3, 2),
            calc_color(color, x4, 3),
            flip
        );
    }
    break;
    case DRAW_RECT_2_OUTLINE:
    case DRAW_RECT_2_OUTLINE_TEXFILL:
    {
        Vector2 x11 = (Vector2) {
            shape.rect.rect2.x - shape.thickness,
            shape.rect.rect2.y - shape.thickness
        };
        Vector2 x12 = (Vector2) {
            shape.rect.rect2.x + shape.thickness,
            shape.rect.rect2.y + shape.thickness
        };
        Vector2 x21 = (Vector2) {
            shape.rect.rect2.x - shape.thickness,
            shape.rect.rect2.y + shape.rect.rect2.height + shape.thickness
        };
        Vector2 x22 = (Vector2) {
            shape.rect.rect2.x + shape.thickness,
            shape.rect.rect2.y + shape.rect.rect2.height - shape.thickness
        };
        Vector2 x31 = (Vector2) {
            shape.rect.rect2.x + shape.rect.rect2.width + shape.thickness,
            shape.rect.rect2.y + shape.rect.rect2.height + shape.thickness
        };
        Vector2 x32 = (Vector2) {
            shape.rect.rect2.x + shape.rect.rect2.width - shape.thickness,
            shape.rect.rect2.y + shape.rect.rect2.height - shape.thickness
        };
        Vector2 x41 = (Vector2) {
            shape.rect.rect2.x + shape.rect.rect2.width + shape.thickness,
            shape.rect.rect2.y - shape.thickness
        };
        Vector2 x42 = (Vector2) {
            shape.rect.rect2.x + shape.rect.rect2.width - shape.thickness,
            shape.rect.rect2.y + shape.thickness
        };

        draw_quad(
            x11, x21, x22, x12,
            tex, texrect, shape.type & 1 ? NULL : &texorigin, 
            calc_color(color, x11, 0),
            calc_color(color, x21, 1),
            calc_color(color, x22, 2),
            calc_color(color, x12, 3),
            flip
        );
        draw_quad(
            x21, x31, x32, x22,
            tex, texrect, shape.type & 1 ? NULL : &texorigin, 
            calc_color(color, x21, 0),
            calc_color(color, x31, 1),
            calc_color(color, x32, 2),
            calc_color(color, x22, 3),
            flip
        );
        draw_quad(
            x31, x41, x42, x32,
            tex, texrect, shape.type & 1 ? NULL : &texorigin, 
            calc_color(color, x31, 0),
            calc_color(color, x41, 1),
            calc_color(color, x42, 2),
            calc_color(color, x32, 3),
            flip
        );
        draw_quad(
            x41, x11, x12, x42,
            tex, texrect, shape.type & 1 ? NULL : &texorigin, 
            calc_color(color, x41, 0),
            calc_color(color, x11, 1),
            calc_color(color, x12, 2),
            calc_color(color, x42, 3),
            flip
        );
    }
    case DRAW_RECT_OUTLINE:
    case DRAW_RECT_OUTLINE_TEXFILL:
    {
        Vector2 x11 = (Vector2) {
            shape.rect.rect.x - shape.thickness,
            shape.rect.rect.y - shape.thickness
        };
        Vector2 x12 = (Vector2) {
            shape.rect.rect.x + shape.thickness,
            shape.rect.rect.y + shape.thickness
        };
        Vector2 x21 = (Vector2) {
            shape.rect.rect.x - shape.thickness,
            shape.rect.rect.y + shape.rect.rect.height + shape.thickness
        };
        Vector2 x22 = (Vector2) {
            shape.rect.rect.x + shape.thickness,
            shape.rect.rect.y + shape.rect.rect.height - shape.thickness
        };
        Vector2 x31 = (Vector2) {
            shape.rect.rect.x + shape.rect.rect.width + shape.thickness,
            shape.rect.rect.y + shape.rect.rect.height + shape.thickness
        };
        Vector2 x32 = (Vector2) {
            shape.rect.rect.x + shape.rect.rect.width - shape.thickness,
            shape.rect.rect.y + shape.rect.rect.height - shape.thickness
        };
        Vector2 x41 = (Vector2) {
            shape.rect.rect.x + shape.rect.rect.width + shape.thickness,
            shape.rect.rect.y - shape.thickness
        };
        Vector2 x42 = (Vector2) {
            shape.rect.rect.x + shape.rect.rect.width - shape.thickness,
            shape.rect.rect.y + shape.thickness
        };

        draw_quad(
            x11, x21, x22, x12,
            tex, texrect, shape.type & 1 ? NULL : &texorigin, 
            calc_color(color, x11, 0),
            calc_color(color, x21, 1),
            calc_color(color, x22, 2),
            calc_color(color, x12, 3),
            flip
        );
        draw_quad(
            x21, x31, x32, x22,
            tex, texrect, shape.type & 1 ? NULL : &texorigin, 
            calc_color(color, x21, 0),
            calc_color(color, x31, 1),
            calc_color(color, x32, 2),
            calc_color(color, x22, 3),
            flip
        );
        draw_quad(
            x31, x41, x42, x32,
            tex, texrect, shape.type & 1 ? NULL : &texorigin, 
            calc_color(color, x31, 0),
            calc_color(color, x41, 1),
            calc_color(color, x42, 2),
            calc_color(color, x32, 3),
            flip
        );
        draw_quad(
            x41, x11, x12, x42,
            tex, texrect, shape.type & 1 ? NULL : &texorigin, 
            calc_color(color, x41, 0),
            calc_color(color, x11, 1),
            calc_color(color, x12, 2),
            calc_color(color, x42, 3),
            flip
        );
    }
    break;
    case DRAW_TRIANGLE:
    case DRAW_TRIANGLE_TEXFILL:
    {
        draw_quad(
            shape.vertex.point[0],
            shape.vertex.point[1],
            shape.vertex.point[2],
            shape.vertex.point[2],
            tex, texrect, shape.type & 1 ? NULL : &texorigin, 
            calc_color(color, shape.vertex.point[0], 0),
            calc_color(color, shape.vertex.point[1], 1),
            calc_color(color, shape.vertex.point[2], 2),
            calc_color(color, shape.vertex.point[2], 2),
            flip
        );
    }
    break;
    }
}

static void do_draw(ecs_iter_t *it)
{
    unsigned has = 0;
    unsigned has_arr = 0;
    unsigned has_single = 0;

#define CHECK_COL(T, X, N) \
    T *X = NULL; \
    const unsigned has_##X = 1u << (N - 1); \
    if (ecs_field_is_set(it, N)) { \
        X = ecs_field(it, T, N); \
        has |= has_##X; \
        bool y = ecs_field_is_self(it, N); \
        has_arr |= y * has_##X; \
        has_single |= (!y) * has_##X; \
    }

    CHECK_COL(draw_canvas_t,        canvas_col,    1)
    CHECK_COL(draw_color_t,         color_col,     2)
    CHECK_COL(draw_custom_t,        custom_col,    3)
    CHECK_COL(draw_font_t,          font_col,      4)
    CHECK_COL(draw_mode_t,          mode_col,      5)
    CHECK_COL(draw_shape_t,         shape_col,     6)
    CHECK_COL(draw_sprite_t,        sprite_col,    7)
    CHECK_COL(draw_text_t,          text_col,      8)
    CHECK_COL(draw_texture_t,       texture_col,   9)
    CHECK_COL(transform_t,          transform_col, 10)

    draw_entity_info_t info = {
        .color = { .type = COLOR_PLAIN, .color = WHITE },
        .flip = false,
        .font = { GetFontDefault() },
        .tex = { 1, 1, 1, 1, 7 },
        .texrec = { 0.0f, 0.0f, 1.0f, 1.0f },
        .texorigin = { 0.0f, 0.0f }
    };

    if (has & has_transform_col) {
        rlPushMatrix();
    }
    check_transform(has_single & has_transform_col, transform_col, &info.flip, 0);
    check_mode(has_single & has_mode_col, mode_col, 0);
    check_canvas(has_single & has_canvas_col, &info.tex, &info.texrec, canvas_col, 0);
    check_tex(has_single & has_texture_col, &info.tex, &info.texrec, texture_col, 0);
    check_sprite(has_single & has_sprite_col, &info.texrec, &info.texorigin, sprite_col, 0);
    if (has_single & has_font_col) {
        info.font = font_col[0];
    }
    if (has_single & has_color_col) {
        info.color = color_col[0];
    }
    
    for (int i = 0; i < it->count; i++) {
        check_transform(has_arr & has_transform_col, transform_col, &info.flip, i);
        check_mode(has_arr & has_mode_col, mode_col, i);
        check_canvas(has_arr & has_canvas_col, &info.tex, &info.texrec, canvas_col, i);
        check_tex(has_arr & has_texture_col, &info.tex, &info.texrec, texture_col, i);
        check_sprite(has_arr & has_sprite_col, &info.texrec, &info.texorigin, sprite_col, i);
        if (has_arr & has_font_col) {
            info.font = font_col[i];
        }
        if (has_arr & has_color_col) {
            info.color = color_col[i];
        }

        if (has & has_text_col) {
            draw_text(
                has_arr & has_text_col ? text_col[i] : text_col[0],
                info.font,
                info.color
            );
        }
        if (has & has_shape_col) {
            draw_shape(
                has_arr & has_shape_col ? shape_col[i] : shape_col[0],
                info.color,
                info.tex,
                info.texrec,
                info.texorigin,
                info.flip
            );
        } else if (has & (has_texture_col | has_canvas_col)) {
            draw_shape(
                (draw_shape_t) {
                    .type = DRAW_RECT,
                    .rect.rect = {
                        .x = info.texorigin.x,
                        .y = info.texorigin.y,
                        .width = info.texrec.width,
                        .height = info.texrec.height
                    }
                },
                info.color,
                info.tex,
                info.texrec,
                info.texorigin,
                info.flip
            );
            check_custom(has_single & has_custom_col, &info, custom_col, 0);
            check_custom(has_arr & has_custom_col, &info, custom_col, i);
        }
    }

    if (has & has_mode_col) {
        EndBlendMode();
        EndScissorMode();
        EndShaderMode();
    }
    if (has & has_transform_col) {
        rlPopMatrix();
    }
#undef CHECK_COL
}

static void do_draw_3d(ecs_iter_t *it)
{
}

static int cmp(ecs_entity_t e1, const void *v1, ecs_entity_t e2, const void *v2)
{
    const draw_priority_t *p1 = v1, *p2 = v2;
    return (p1->priority > p2->priority) - (p1->priority < p2->priority);
}

static void init_canvas_system(ecs_iter_t *it)
{
    draw_canvas_t *canvas_col = ecs_field(it, draw_canvas_t, 1);
    for (int i = it->count - 1; ~i; i--) {
        if (canvas_col[i].is_3d) {
            canvas_col[i].draw_system = ecs_system_init(it->world, &(ecs_system_desc_t) {
                .query = {
                    .filter.terms = {
                        { ecs_pair(Draw, it->entities[i]) }
                    },
                },
                .callback = do_draw_3d
            });
        } else {
            canvas_col[i].draw_system = ecs_system_init(it->world, &(ecs_system_desc_t) {
                .query = { 
                    .filter = {
                        .terms = {
                            { ecs_id(draw_canvas_t),         .oper = EcsOptional, .inout = EcsIn },
                            { ecs_id(draw_color_t),          .oper = EcsOptional, .inout = EcsIn },
                            { ecs_id(draw_custom_t),         .oper = EcsOptional, .inout = EcsIn },
                            { ecs_id(draw_font_t),           .oper = EcsOptional, .inout = EcsIn },
                            { ecs_id(draw_mode_t),           .oper = EcsOptional, .inout = EcsIn },
                            { ecs_id(draw_shape_t),          .oper = EcsOptional, .inout = EcsIn },
                            { ecs_id(draw_sprite_t),         .oper = EcsOptional, .inout = EcsIn },
                            { ecs_id(draw_text_t),           .oper = EcsOptional, .inout = EcsIn },
                            { ecs_id(draw_texture_t),        .oper = EcsOptional, .inout = EcsIn },     
                            { ecs_id(transform_t),           .oper = EcsOptional, .inout = EcsIn },
                            { ecs_pair(Draw, it->entities[i]) },
                            { ecs_id(draw_priority_t), .inout = EcsIn }
                        },
                        .instanced = true
                    },
                    .order_by_component = ecs_id(draw_priority_t),
                    .order_by = cmp,
                },
                .callback = do_draw
            });
        }
    }
}

static void process_canvas(ecs_iter_t *it)
{
    draw_canvas_t *canvas_col = ecs_field(it, draw_canvas_t, 1);
    for (int i = it->count - 1; ~i; i--) {
        BeginTextureMode(canvas_col[i].canvas);
        ClearBackground(canvas_col[i].background);
        if (canvas_col[i].is_3d) {
            if (canvas_col[i].has_vr) {
                BeginVrStereoMode(canvas_col[i].vr);
            }
            if (canvas_col[i].has_camera) {
                BeginMode3D(canvas_col[i].camera.three);
            }
            ecs_run(it->world, canvas_col[i].draw_system, 0.0f, NULL);
            if (canvas_col[i].has_camera) {
                EndMode3D();
            }
            if (canvas_col[i].has_vr) {
                EndVrStereoMode();
            }
        } else {
            if (canvas_col[i].has_camera) {
                BeginMode2D(canvas_col[i].camera.two);
            }
            ecs_run(it->world, canvas_col[i].draw_system, 0.0f, NULL);
            if (canvas_col[i].has_camera) {
                EndMode2D();
            }
        }
        EndTextureMode();
    }
}

static void check_sprite_avaliable(ecs_iter_t *it)
{
    draw_sprite_t *sprite_col = ecs_field(it, draw_sprite_t, 1);
    for (int i = 0; i < it->count; i++) {
        if (sprite_col[i].frametime == 0) {
            sprite_col[i].frametime = 1;
        }
        if (sprite_col[i].framecnt == 0) {
            sprite_col[i].framecnt = 1;
        }
    }
}

static void update_sprite(ecs_iter_t *it)
{
    draw_sprite_t *sprite_col = ecs_field(it, draw_sprite_t, 1);
    for (int i = 0; i < it->count; i++) {
        sprite_col[i].framecur++;
        if (sprite_col[i].framecur == sprite_col[i].framecnt * sprite_col[i].frametime) {
            sprite_col[i].framecur = 0;
        }
    }
}

void DrawModuleImport(ecs_world_t *ecs)
{
    ECS_MODULE(ecs, DrawModule);

    ECS_IMPORT(ecs, TransformModule);
    ECS_IMPORT(ecs, WindowModule);

    ECS_COMPONENT_DEFINE(ecs, draw_canvas_t);
    ECS_COMPONENT_DEFINE(ecs, draw_color_t);
    ECS_COMPONENT_DEFINE(ecs, draw_custom_t);
    ECS_COMPONENT_DEFINE(ecs, draw_font_t);
    ECS_COMPONENT_DEFINE(ecs, draw_mode_t);
    ECS_COMPONENT_DEFINE(ecs, draw_priority_t);
    ECS_COMPONENT_DEFINE(ecs, draw_shape_t);
    ECS_COMPONENT_DEFINE(ecs, draw_sprite_t);
    ECS_COMPONENT_DEFINE(ecs, draw_text_t);
    ECS_COMPONENT_DEFINE(ecs, draw_texture_t);

    ECS_TAG_DEFINE(ecs, Draw);

    ecs_add_id(ecs, Draw, EcsAcyclic);

    ecs_singleton_add(ecs, draw_mode_t);

    ecs_entity_t
        predraw = ecs_entity_init(ecs, &(ecs_entity_desc_t) {
            .name = "PreDraw",
            .add = { EcsPhase }
        }),
        ondraw = ecs_entity_init(ecs, &(ecs_entity_desc_t) {
            .name = "OnDraw",
            .add = { EcsPhase }
        }),
        postdraw = ecs_entity_init(ecs, &(ecs_entity_desc_t) {
            .name = "PostDraw",
            .add = { EcsPhase }
        });

    ecs_add_pair(ecs, predraw, EcsDependsOn, EcsPostUpdate);
    ecs_add_pair(ecs, ondraw, EcsDependsOn, predraw);
    ecs_add_pair(ecs, postdraw, EcsDependsOn, ondraw);

    ecs_system_init(ecs, &(ecs_system_desc_t) {
        .entity = ecs_entity(ecs, {
            .add = { ecs_dependson(predraw) }
        }),
        .query.filter.terms = {
            { ecs_id(draw_canvas_t), .src = { .flags = EcsSelf | EcsUp, .trav = Draw } },
            { Draw }
        },
        .callback = process_canvas,
        .no_staging = true
    }),
    ecs_system_init(ecs, &(ecs_system_desc_t) {
        .entity = ecs_entity(ecs, {
            .add = { ecs_dependson(predraw) }
        }),
        .query.filter.terms = {
            { ecs_id(init_config_t), .src = { ecs_id(init_config_t) } }
        },
        .callback = begin_draw
    }),
    ecs_system_init(ecs, &(ecs_system_desc_t) {
        .entity = ecs_entity(ecs, {
            .add = { ecs_dependson(postdraw) }
        }),
        .query.filter.terms = {
            { ecs_id(init_config_t), .src = { ecs_id(init_config_t) } },
        },
        .callback = end_draw
    }),
    ecs_system_init(ecs, &(ecs_system_desc_t) {
        .entity = ecs_entity(ecs, {
            .add = { ecs_dependson(ondraw) }
        }),
        .query = { 
            .filter = {
                .terms = {
                    { ecs_id(draw_canvas_t),         .oper = EcsOptional, .inout = EcsIn },
                    { ecs_id(draw_color_t),          .oper = EcsOptional, .inout = EcsIn },
                    { ecs_id(draw_custom_t),         .oper = EcsOptional, .inout = EcsIn },
                    { ecs_id(draw_font_t),           .oper = EcsOptional, .inout = EcsIn },
                    { ecs_id(draw_mode_t),           .oper = EcsOptional, .inout = EcsIn },
                    { ecs_id(draw_shape_t),          .oper = EcsOptional, .inout = EcsIn },
                    { ecs_id(draw_sprite_t),         .oper = EcsOptional, .inout = EcsIn },
                    { ecs_id(draw_text_t),           .oper = EcsOptional, .inout = EcsIn },
                    { ecs_id(draw_texture_t),        .oper = EcsOptional, .inout = EcsIn },     
                    { ecs_id(transform_t),           .oper = EcsOptional, .inout = EcsIn },
                    { Draw },
                    { ecs_id(draw_priority_t), .inout = EcsIn }
                },
                .instanced = true
            },
            .order_by_component = ecs_id(draw_priority_t),
            .order_by = cmp,
        },
        .callback = do_draw
    }),
    ecs_system_init(ecs, &(ecs_system_desc_t) {
        .entity = ecs_entity(ecs, {
            .add = { ecs_dependson(ondraw) }
        }),
        .query.filter.terms = { { ecs_id(draw_sprite_t) } },
        .callback = update_sprite
    });
    ecs_observer_init(ecs, &(ecs_observer_desc_t) {
        .filter.terms = { { ecs_id(draw_sprite_t) } },
        .events = { EcsOnSet },
        .callback = check_sprite_avaliable
    });
    ecs_observer_init(ecs, &(ecs_observer_desc_t) {
        .filter.terms = { { ecs_id(draw_canvas_t) } },
        .events = { EcsOnSet },
        .callback = init_canvas_system
    });
}
