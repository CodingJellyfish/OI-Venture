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
        .draw_fps = true,
        .background = BLACK
    });

    Image jaywalk = LoadImage("tex/jaywalk.png");
    ImageColorInvert(&jaywalk);

    ecs_entity_t jay = ecs_new_id(ecs);
    ecs_add(ecs, jay, Draw);
    ecs_set(ecs, jay, draw_color_t, {
        .type = COLOR_PLAIN,
        .color = LIME,
    });
    ecs_set(ecs, jay, draw_priority_t, { 3 });
    ecs_set(ecs, jay, draw_texture_t, { LoadTextureFromImage(jaywalk) });
    ecs_set(ecs, jay, draw_sprite_t, {
        .origin = { 1190.0f, 1200.0f },
        .rect = { .width = 3072, .height = 128 },
        .framecnt = 24
    });
    ecs_set(ecs, jay, map_location_t, { .x = 50, .y = 50 });

    int row = 30, col = 30;

    for (int i = 0; i <= row + 1; i++) {
        for (int j = 0; j <= col + 1; j++) {
            map[i][j] = GetRandomValue(0, i && i <= row && j && j <= col ? 2 : 0);
        }
    }
    for (int i = 0; i <= row; i++) {
        for (int j = 0; j <= col; j++) {
            map_draw[i][j] = ecs_new_w_id(ecs, Draw);
            ecs_set(ecs, map_draw[i][j], transform_t, { .translation = { len * j, len * i } });
            ecs_set(ecs, map_draw[i][j], map_location_t, { .x = j, .y = i });

            ecs_entity_t back = ecs_new_w_pair(ecs, EcsIsA, map_draw[i][j]);
            ecs_set(ecs, back, draw_priority_t, { -10 });
            ecs_set(ecs, back, draw_color_t, {
                .type = COLOR_PLAIN,
                .color = ground_fill
            });
            ecs_add_id(ecs, back, EcsPrefab);

            ecs_entity_t fore = ecs_new_w_pair(ecs, EcsIsA, map_draw[i][j]);
            ecs_set(ecs, fore, draw_priority_t, { -10 + EPS });
            ecs_set(ecs, fore, draw_color_t, {
                .type = COLOR_PLAIN,
                .color = ground
            });
            ecs_add_id(ecs, fore, EcsPrefab);

            if (((map[i][j] >= 1 && map[i][j + 1] < 1) || (map[i][j] < 1 && map[i][j + 1] >= 1))
             && ((map[i][j] >= 1 && map[i + 1][j] < 1) || (map[i][j] < 1 && map[i + 1][j] >= 1))) {
                ecs_entity_t ent = ecs_new_w_pair(ecs, EcsIsA, fore);
                ecs_set(ecs, ent, draw_shape_t, {
                    .type = DRAW_LINE_2,
                    .thickness = 2.0f,
                    .vertex = { {
                        { len, padding },
                        { len, len - padding },
                        { padding, len },
                        { len - padding, len }
                    } }
                });
            } else if ((map[i][j] >= 1 && map[i][j + 1] < 1) || (map[i][j] < 1 && map[i][j + 1] >= 1)) {
                ecs_entity_t ent = ecs_new_w_pair(ecs, EcsIsA, fore);
                ecs_set(ecs, ent, draw_shape_t, {
                    .type = DRAW_LINE,
                    .thickness = 2.0f,
                    .vertex = { {
                        { len, padding },
                        { len, len - padding }
                    } }
                });
            } else if ((map[i][j] >= 1 && map[i + 1][j] < 1) || (map[i][j] < 1 && map[i + 1][j] >= 1)) {
                ecs_entity_t ent = ecs_new_w_pair(ecs, EcsIsA, fore);
                ecs_set(ecs, ent, draw_shape_t, {
                    .type = DRAW_LINE,
                    .thickness = 2.0f,
                    .vertex = { {
                        { padding, len },
                        { len - padding, len }
                    } }
                });
            } 
            if (i <= row && j <= col) {
                int sum = (map[i][j] >= 1) + (map[i + 1][j] >= 1) + (map[i][j + 1] >= 1) + (map[i + 1][j + 1] >= 1);
                
                if (sum == 2 && ((map[i + 1][j] < 1 && map[i + 1][j + 1] < 1) || (map[i][j] < 1 && map[i][j + 1] < 1))
                    && ((map[i][j + 1] < 1 && map[i + 1][j + 1] < 1) || (map[i][j] < 1 && map[i + 1][j] < 1))) {
                    ecs_entity_t ent = ecs_new_w_pair(ecs, EcsIsA, fore);
                    ecs_set(ecs, ent, draw_shape_t, {
                        .type = DRAW_LINE_2,
                        .thickness = 2.0f,
                        .vertex = { {
                            { len, len - padding },
                            { len, len + padding },
                            { len - padding, len },
                            { len + padding, len }
                        } }
                    });
                } else if (sum == 2 && ((map[i + 1][j] < 1 && map[i + 1][j + 1] < 1) || (map[i][j] < 1 && map[i][j + 1] < 1))) {
                    ecs_entity_t ent = ecs_new_w_pair(ecs, EcsIsA, fore);
                    ecs_set(ecs, ent, draw_shape_t, {
                        .type = DRAW_LINE,
                        .thickness = 2.0f,
                        .vertex = { {
                            { len - padding, len },
                            { len + padding, len }
                        } }
                    });
                } else if (sum == 2 && ((map[i][j + 1] < 1 && map[i + 1][j + 1] < 1) || (map[i][j] < 1 && map[i + 1][j] < 1))) {
                    ecs_entity_t ent = ecs_new_w_pair(ecs, EcsIsA, fore);
                    ecs_set(ecs, ent, draw_shape_t, {
                        .type = DRAW_LINE,
                        .thickness = 2.0f,
                        .vertex = { {
                            { len, len - padding },
                            { len, len + padding }
                        } }
                    });
                } else if (sum != 0 && sum != 4) {
                    if ((map[i][j] < 1) ^ (sum == 1)) {
                        ecs_entity_t ent = ecs_new_w_pair(ecs, EcsIsA, fore);
                        ecs_set(ecs, ent, draw_shape_t, {
                            .type = DRAW_BEZIER_QUAD,
                            .thickness = 2.0f,
                            .vertex = { {
                                { len, len - padding },
                                { len - padding, len },
                                { len, len }
                            } }
                        });
                    }
                    if ((map[i + 1][j] < 1) ^ (sum == 1)) {
                        ecs_entity_t ent = ecs_new_w_pair(ecs, EcsIsA, fore);
                        ecs_set(ecs, ent, draw_shape_t, {
                            .type = DRAW_BEZIER_QUAD,
                            .thickness = 2.0f,
                            .vertex = { {
                                { len, len + padding },
                                { len - padding, len },
                                { len, len }
                            } }
                        });
                    }
                    if ((map[i + 1][j + 1] < 1) ^ (sum == 1)) {
                        ecs_entity_t ent = ecs_new_w_pair(ecs, EcsIsA, fore);
                        ecs_set(ecs, ent, draw_shape_t, {
                            .type = DRAW_BEZIER_QUAD,
                            .thickness = 2.0f,
                            .vertex = { {
                                { len, len + padding },
                                { len + padding, len },
                                { len, len }
                            } }
                        });
                    }
                    if ((map[i][j + 1] < 1) ^ (sum == 1)) {
                        ecs_entity_t ent = ecs_new_w_pair(ecs, EcsIsA, fore);
                        ecs_set(ecs, ent, draw_shape_t, {
                            .type = DRAW_BEZIER_QUAD,
                            .thickness = 2.0f,
                            .vertex = { {
                                { len, len - padding },
                                { len + padding, len },
                                { len, len }
                            } }
                        });
                    }
                }
            }
        }
    }

#if defined(PLATFORM_WEB)
    emscripten_set_main_loop(ecs_progress(ecs, 0), 60, 1);
#elif defined(DEBUG)
    ecs_set_target_fps(ecs, 60);

    ecs_app_run(ecs, &(ecs_app_desc_t) {
        .enable_rest = true
    });
#else
    //ecs_set_target_fps(ecs, 60);

    while (ecs_progress(ecs, 0)) {}
#endif
    return ecs_fini(ecs);
}