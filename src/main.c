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

#include <flecs.h>

//#define DEBUG

#include <raylib.h>

#include <math.h>

#define MAXROW 1000
#define MAXCOL 1000
#define EPS 1e-5f

int len = 128;
int padding = 24;
int padding2 = 20;
int thick = 4;
int height = 32;

Color ground = ORANGE;
Color ground_fill = {32, 20, 0, 255};
Color wall = BLUE;
Color wall_fill = {0, 15, 31, 255};


int map[MAXROW + 5][MAXCOL + 5];
ecs_entity_t map_draw[MAXROW + 5][MAXCOL + 5];

typedef struct {
    int x, y;
} map_location_t;

typedef struct {
    int x, y;
    int speed, timer;
} map_to_location_t;

ECS_COMPONENT_DECLARE(map_location_t);
ECS_COMPONENT_DECLARE(map_to_location_t);

/*ARRAY(Vector2) gen_circle_sector(Vector2 center, float radius, float angle, float angle2)
{
    int cnt = radius / 4.0f + 1;

    ARRAY(Vector2) ret = new_array_Vector2(0);

    float stepLength = (angle2 - angle) / (float)cnt;

    for (int i = 0; i <= cnt; i++)
    {
        push_array_Vector2(&ret, (Vector2) {
            .x = center.x + sinf(DEG2RAD * angle) * radius, 
            .y = center.y + cosf(DEG2RAD * angle) * radius
        });
        angle += stepLength;
    }

    return ret;
}

ecs_entity_t gen_arc_ent(ecs_world_t *ecs, Vector2 center, float radius, float angle, float angle2, Color color, float layer)
{
    ecs_entity_t line = ecs_new_id(ecs);
    ecs_add(ecs, line, Draw);
    ecs_set(ecs, line, draw_color_t, { color });
    ecs_set(ecs, line, draw_shape_t, { .type = DRAW_LINE_STRIP, .vertex.arr = gen_circle_sector(center, radius, angle, angle2) });
    ecs_set(ecs, line, draw_priority_t, { layer });
    return line;
}

ecs_entity_t gen_pizza_ent(ecs_world_t *ecs, Vector2 center, float radius, float angle, float angle2, Color color, float layer)
{
    ARRAY(Vector2) fanbuf = gen_circle_sector(center, radius, angle, angle2);
    push_array_Vector2(&fanbuf, center);

    ecs_entity_t fan = ecs_new_id(ecs);
    ecs_add(ecs, fan, Draw);
    ecs_set(ecs, fan, draw_color_t, { color });
    ecs_set(ecs, fan, draw_shape_t, { .type = DRAW_TRIANGLE_FAN, .vertex.arr = fanbuf });
    ecs_set(ecs, fan, draw_priority_t, { layer + EPS });
    return fan;
}*/

ecs_entity_t gen_line_ent(ecs_world_t *ecs, float x1, float y1, float x2, float y2, Color color, float layer)
{
    ecs_entity_t line = ecs_new_id(ecs);
    ecs_add(ecs, line, Draw);
    ecs_set(ecs, line, draw_color_t, { .type = COLOR_PLAIN, .color = color });
    ecs_set(ecs, line, draw_shape_t, { .type = DRAW_LINE, .thickness = 10, .vertex.point = { 
        {x1, y1},
        {x2, y2}
    }});
    ecs_set(ecs, line, draw_priority_t, { layer });
    return line;
}

int main(int argc, char *argv[])
{
    ecs_world_t *ecs = ecs_init_w_args(argc, argv);

    ECS_COMPONENT_DEFINE(ecs, map_location_t);
    ECS_COMPONENT_DEFINE(ecs, map_to_location_t);

#ifdef DEBUG
    ecs_singleton_set(ecs, EcsRest, {0});

    ECS_IMPORT(ecs, FlecsMonitor);
#endif
    ECS_IMPORT(ecs, DrawModule);

    ecs_singleton_set(ecs, init_config_t, {
        .width = 2560,
        .height = 1440,
        .title = "OI-Venture",
        .draw_fps = true
    });
    ecs_entity_t background = ecs_new_id(ecs);
    ecs_add(ecs, background, Draw);
    ecs_set(ecs, background, draw_color_t, { .type = COLOR_PLAIN, .color = BLACK });
    ecs_set(ecs, background, draw_priority_t, { -20 });

    Image jaywalk = LoadImage("tex/jaywalk.png");
    ImageColorInvert(&jaywalk);

    ecs_entity_t jay = ecs_new_id(ecs);
    ecs_add(ecs, jay, Draw);
    ecs_add_id(ecs, jay, EcsPrefab);
    ecs_set(ecs, jay, draw_color_t, { .type = COLOR_PLAIN, .color = BEIGE });
    ecs_set(ecs, jay, draw_priority_t, { 3 });
    ecs_set(ecs, jay, draw_texture_t, { LoadTextureFromImage(jaywalk) });
    ecs_set(ecs, jay, draw_sprite_t, {
        .origin = { 190.0f, 200.0f },
        .rect = { .width = 3072, .height = 128 },
        .framecnt = 24,
        .frametime = 1
    });
    for (int i = 0; i < 1; i++) {
        ecs_entity_t k = ecs_new_w_pair(ecs, EcsIsA, jay);
        //ecs_entity_t k = ecs_new_id(ecs);
        /*ecs_add(ecs, k, Draw);
        ecs_set(ecs, k, draw_color_t, { BEIGE });
        ecs_set(ecs, k, draw_priority_t, { 3 });*/
        /*ecs_set(ecs, k, draw_sprite_t, {
            .origin = { 190.0f, 200.0f },
            .rect = { .width = 3072, .height = 128 },
            .framecnt = 24,
            .frametime = 1
        });*/
        ecs_set(ecs, k, draw_shape_t, {
            .type = DRAW_BEZIER,
            .thickness = 10.0f,
            .vertex = { {
                { rand() % 1000, rand() % 1000 },
                { rand() % 1000, rand() % 1000 },
                { rand() % 1000, rand() % 1000 },
                { rand() % 1000, rand() % 1000 }
            } }
        });
    }
    ecs_set(ecs, jay, map_location_t, { .x = 50, .y = 50 });

    /*int row = 100, col = 100;

    for (int i = 0; i <= row + 1; i++) {
        for (int j = 0; j <= col + 1; j++) {
            map[i][j] = GetRandomValue(0, i && i <= row && j && j <= col ? 2 : 0);
        }
    }
    for (int i = 0; i <= row + 1; i++) {
        for (int j = 0; j <= col + 1; j++) {
            map_draw[i][j] = ecs_new_w_id(ecs, Draw);
            ecs_set(ecs, map_draw[i][j], draw_priority_t, { -10 });
            //ecs_set(ecs, map_draw[i][j], transform_t, { .translation = { 1.0f * len * j, 1.0f * len * i } });

            /*if (map[i][j] >= 1) {
                float w = 1.0f * len, h = 1.0f * len;

                ecs_entity_t rect = ecs_new_w_pair(ecs, EcsIsA, map_draw[i][j]);
                ecs_add(ecs, rect, Draw);
                ecs_set(ecs, rect, draw_color_t, { ground_fill });
                ARRAY(Vector2) buf = new_array_Vector2(0);

                if (map[i - 1][j - 1] >= 1 || map[i][j - 1] >= 1 || map[i - 1][j] >= 1) {
                    push_array_Vector2(&buf, (Vector2){ 0, 0 });
                } else {
                    push_array_Vector2(&buf, (Vector2){ padding, 0 });
                    push_array_Vector2(&buf, (Vector2){ 0, padding });
                }
                if (map[i + 1][j - 1] >= 1 || map[i][j - 1] >= 1 || map[i + 1][j] >= 1) {
                    push_array_Vector2(&buf, (Vector2){ 0, h });
                } else {
                    push_array_Vector2(&buf, (Vector2){ 0, h - padding });
                    push_array_Vector2(&buf, (Vector2){ padding, h });
                }
                if (map[i + 1][j + 1] >= 1 || map[i][j + 1] >= 1 || map[i + 1][j] >= 1) {
                    push_array_Vector2(&buf, (Vector2){ w, h });
                } else {
                    push_array_Vector2(&buf, (Vector2){ w - padding, h });
                    push_array_Vector2(&buf, (Vector2){ w, h - padding });
                }
                if (map[i - 1][j + 1] >= 1 || map[i][j + 1] >= 1 || map[i - 1][j] >= 1) {
                    push_array_Vector2(&buf, (Vector2){ w, 0 });
                } else {
                    push_array_Vector2(&buf, (Vector2){ w, padding });
                    push_array_Vector2(&buf, (Vector2){ w - padding, 0 });
                }
                ecs_set(ecs, rect, draw_shape_t, { .type = DRAW_TRIANGLE_FAN, .vertex.arr = buf });
            }
            if (j != col + 1 && ((map[i][j] >= 1 && map[i][j + 1] < 1)
                              || (map[i][j] < 1 && map[i][j + 1] >= 1))) {
                gen_line_ent(ecs, len * (j + 1), len * i + padding, len * (j + 1), len * (i + 1) - padding, ground, -10);
            }
            if (i != col + 1 && ((map[i][j] >= 1 && map[i + 1][j] < 1)
                              || (map[i][j] < 1 && map[i + 1][j] >= 1))) {
                gen_line_ent(ecs, len * j + padding, len * (i + 1), len * (j + 1) - padding, len * (i + 1), ground, -10);
            } 
            if (i <= row && j <= col) {
                int sum = (map[i][j] >= 1) + (map[i + 1][j] >= 1) + (map[i][j + 1] >= 1) + (map[i + 1][j + 1] >= 1);
                
                if (sum == 2 && map[i + 1][j] < 1 && map[i + 1][j + 1] < 1)
                {
                    gen_line_ent(ecs, len * (j + 1) - padding, len * (i + 1), len * (j + 1) + padding, len * (i + 1), ground, -10);
                }
                else if (sum == 2 && map[i][j] < 1 && map[i][j + 1] < 1)
                {
                    gen_line_ent(ecs, len * (j + 1) - padding, len * (i + 1), len * (j + 1) + padding, len * (i + 1), ground, -10);
                }
                else if (sum == 2 && map[i][j + 1] < 1 && map[i + 1][j + 1] < 1)
                {
                    gen_line_ent(ecs, len * (j + 1), len * (i + 1) - padding, len * (j + 1), len * (i + 1) + padding, ground, -10);
                }
                else if (sum == 2 && map[i][j] < 1 && map[i + 1][j] < 1)
                {
                    gen_line_ent(ecs, len * (j + 1), len * (i + 1) - padding, len * (j + 1), len * (i + 1) + padding, ground, -10);
                }
                /*else if (sum != 0)
                {
                    if (sum != 4)
                    {
                        if ((map[i][j] < 1) ^ (sum == 1))
                        {
                            gen_arc_ent(ecs, (Vector2){ 1.0f * len * (j + 1) - padding, 1.0f * len * (i + 1) - padding }, padding, 0.0f, 90.0f, ground, -10);
                        }
                        if ((map[i + 1][j] < 1) ^ (sum == 1))
                        {
                            gen_arc_ent(ecs, (Vector2){ 1.0f * len * (j + 1) - padding, 1.0f * len * (i + 1) + padding }, padding, 90.0f, 180.0f, ground, -10);
                        }
                        if ((map[i + 1][j + 1] < 1) ^ (sum == 1))
                        {
                            gen_arc_ent(ecs, (Vector2){ 1.0f * len * (j + 1) + padding, 1.0f * len * (i + 1) + padding }, padding, 180.0f, 270.0f, ground, -10);
                        }
                        if ((map[i][j + 1] < 1) ^ (sum == 1))
                        {
                            gen_arc_ent(ecs, (Vector2){ 1.0f * len * (j + 1) + padding, 1.0f * len * (i + 1) - padding }, padding, 270.0f, 360.0f, ground, -10);
                        }
                    }
                }
            }
        }
    }*/

#if defined(PLATFORM_WEB)
    emscripten_set_main_loop(ecs_progress(ecs, 0), 60, 1);
#elif defined(DEBUG)
    ecs_set_target_fps(ecs, 60);

    ecs_app_run(ecs, &(ecs_app_desc_t) {
        .enable_rest = true
    });
#else
    ecs_set_target_fps(ecs, 60);

    while (ecs_progress(ecs, 0)) {}
#endif
    return ecs_fini(ecs);
}