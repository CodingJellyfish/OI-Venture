#include <flecs.h>

#include <raylib.h>
#include <rlgl.h>

#include <math.h>
#include <stdlib.h>

struct Jay {};

int map[305][305];
int len = 128;
int padding = 24;
int padding2 = 20;
int thick = 4;
int height = 32;

Color darkorange = {32, 20, 0, 255};
Color darkblue = {0, 15, 31, 255};

void DrawPizza(Vector2 center, float radius, float startAngle, float endAngle, Color color, Color color2)
{
    DrawCircleSector(center, radius, startAngle, endAngle, 20, color2);

    float stepLength = (endAngle - startAngle) / (float)20;
    float angle = startAngle;

    rlBegin(RL_LINES);

    for (int i = 0; i < 20; i++)
    {
        rlColor4ub(color.r, color.g, color.b, color.a);

        rlVertex2f(center.x + sinf(DEG2RAD*angle)*radius, center.y + cosf(DEG2RAD*angle)*radius);
        rlVertex2f(center.x + sinf(DEG2RAD*(angle + stepLength))*radius, center.y + cosf(DEG2RAD*(angle + stepLength))*radius);

        angle += stepLength;
    }

    rlEnd();
}

int main(void)
{
    /*flecs::world ecs;

    ecs.import<Window>();
    
    auto win = ecs.entity("Window")
        .set<Window::Title>({ "OI-Venture" })
        .set<Window::Size>({ 1024, 768 })
        .set<Window::Color>({ BLUE });

    ecs.import<Sprites>();

    auto jaytex = ecs.entity("Jay")
        .set<Sprites::Texture>({ LoadTexture("tex/jaywalk.png") });
    
    for (int i = 0; i < 2; i++)
    {
        auto jay = ecs.entity()
            .is_a(jaytex)
            .set<Sprites::Pos>({{ (float)(rand() % 800), (float)(rand() % 600) }})
            .set<Sprites::Rect>({{ 0.0f, 0.0f, 3072.0f, 128.0f }})
            .set<Sprites::Animated>({ 24, 1, (unsigned)rand() % 24 })
            .set<Sprites::Color>({ Paint::RayWhite })
            .add<Jay>();
    }

    ecs.set_target_fps(60);*/

    InitWindow(1024, 768, "Map");

    for (int i = 1; i <= 30; i++)
    {
        for (int j = 1; j <= 30; j++)
        {
            map[i][j] = rand() % 4;
            if (map[i][j] >= 3)
                map[i][j] = 0;
            printf("%d ", map[i][j]);
        }
        printf("\n");
    }
    
    while (!WindowShouldClose())
    {
        BeginDrawing();
        ClearBackground(BLACK);
        for (int i = 1; i <= 30; i++)
        {
            for (int j = 1; j <= 30; j++)
            {
                if (map[i][j] >= 1)
                {
                    DrawRectangleRounded((Rectangle){ 1.0f * len * j, 1.0f * len * i, 1.0f * len, 1.0f * len }, 2.0f * padding / len, 1, darkorange);
                }
            }
        }
        for (int i = 0; i <= 30; i++)
        {
            for (int j = 0; j < 30; j++)
            {
                if ((map[i][j] >= 1 && map[i][j + 1] < 1)
                || (map[i][j] < 1 && map[i][j + 1] >= 1))
                {
                    DrawLine(len * (j + 1), len * i + padding, len * (j + 1), len * (i + 1) - padding, ORANGE);
                }
            }
        }
        for (int i = 0; i < 30; i++)
        {
            for (int j = 0; j <= 30; j++)
            {
                if ((map[i][j] >= 1 && map[i + 1][j] < 1)
                 || (map[i][j] < 1 && map[i + 1][j] >= 1))
                {
                    DrawLine(len * j + padding, len * (i + 1), len * (j + 1) - padding, len * (i + 1), ORANGE);
                }
            }
        }
        for (int i = 0; i < 30; i++)
        {
            for (int j = 0; j < 30; j++)
            {
                int sum = (map[i][j] >= 1) + (map[i + 1][j] >= 1) + (map[i][j + 1] >= 1) + (map[i + 1][j + 1] >= 1);
                
                if (sum == 2 && map[i + 1][j] < 1 && map[i + 1][j + 1] < 1)
                {
                    DrawRectangle(len * (j + 1) - padding, len * (i + 1) - padding, 2 * padding, padding, darkorange);
                    DrawLine(len * (j + 1) - padding, len * (i + 1), len * (j + 1) + padding, len * (i + 1), ORANGE);
                }
                else if (sum == 2 && map[i][j] < 1 && map[i][j + 1] < 1)
                {
                    DrawRectangle(len * (j + 1) - padding, len * (i + 1), 2 * padding, padding, darkorange);
                    DrawLine(len * (j + 1) - padding, len * (i + 1), len * (j + 1) + padding, len * (i + 1), ORANGE);
                }
                else if (sum == 2 && map[i][j + 1] < 1 && map[i + 1][j + 1] < 1)
                {
                    DrawRectangle(len * (j + 1) - padding, len * (i + 1) - padding, padding, 2 * padding, darkorange);
                    DrawLine(len * (j + 1), len * (i + 1) - padding, len * (j + 1), len * (i + 1) + padding, ORANGE);
                }
                else if (sum == 2 && map[i][j] < 1 && map[i + 1][j] < 1)
                {
                    DrawRectangle(len * (j + 1), len * (i + 1) - padding, padding, 2 * padding, darkorange);
                    DrawLine(len * (j + 1), len * (i + 1) - padding, len * (j + 1), len * (i + 1) + padding, ORANGE);
                }
                else if (sum != 0)
                {
                    if (sum != 1)
                    {
                        DrawRectangle(len * (j + 1) - padding, len * (i + 1) - padding, 2 * padding, 2 * padding, darkorange);
                    }
                    if (sum != 4)
                    {
                        if ((map[i][j] < 1) ^ (sum == 1))
                        {
                            DrawPizza((Vector2){ 1.0f * len * (j + 1) - padding, 1.0f * len * (i + 1) - padding }, padding, 0.0f, 90.0f, ORANGE, sum != 1 ? BLACK : darkorange);
                        }
                        if ((map[i + 1][j] < 1) ^ (sum == 1))
                        {
                            DrawPizza((Vector2){ 1.0f * len * (j + 1) - padding, 1.0f * len * (i + 1) + padding }, padding, 90.0f, 180.0f, ORANGE, sum != 1 ? BLACK : darkorange);
                        }
                        if ((map[i + 1][j + 1] < 1) ^ (sum == 1))
                        {
                            DrawPizza((Vector2){ 1.0f * len * (j + 1) + padding, 1.0f * len * (i + 1) + padding }, padding, 180.0f, 270.0f, ORANGE, sum != 1 ? BLACK : darkorange);
                        }
                        if ((map[i][j + 1] < 1) ^ (sum == 1))
                        {
                            DrawPizza((Vector2){ 1.0f * len * (j + 1) + padding, 1.0f * len * (i + 1) - padding }, padding, 270.0f, 360.0f, ORANGE, sum != 1 ? BLACK : darkorange);
                        }
                    }
                }
            }
        }
        for (int i = 1; i <= 30; i++)
        {
            for (int j = 1; j <= 30; j++)
            {
                if (map[i][j] >= 2)
                {
                    int len2 = len - 2 * padding;
                    DrawRectangleRounded((Rectangle){ 1.0f * len * j + padding, 1.0f * len * i + padding, 1.0f * len2, 1.0f * len2 }, 2.0f * padding2 / len2, 1, darkblue);
                }
            }
        }
        for (int i = 0; i <= 30; i++)
        {
            for (int j = 0; j < 30; j++)
            {
                if (map[i][j] >= 2 && map[i][j + 1] >= 2)
                {
                    DrawRectangle(len * (j + 1) - padding - padding2, len * i + padding, 2 * padding + 2 * padding2, len - 2 * padding, darkblue);
                }
            }
        }
        for (int i = 0; i < 30; i++)
        {
            for (int j = 0; j <= 30; j++)
            {
                if (map[i][j] >= 2 && map[i + 1][j] >= 2)
                {
                    DrawRectangle(len * j + padding, len * (i + 1) - padding - padding2, len - 2 * padding, 2 * padding + 2 * padding2, darkblue);
                }
            }
        }
        for (int i = 0; i < 30; i++)
        {
            for (int j = 0; j < 30; j++)
            {
                int sum = (map[i][j] >= 2) + (map[i + 1][j] >= 2) + (map[i][j + 1] >= 2) + (map[i + 1][j + 1] >= 2);
                if (sum == 3)
                {
                    if (map[i][j] < 2)
                    {
                        DrawRectangle(len * (j + 1) + padding - padding2, len * (i + 1) + padding - padding2, padding2, padding2, darkblue);
                    }
                    else if (map[i + 1][j] < 2)
                    {
                        DrawRectangle(len * (j + 1) + padding - padding2, len * (i + 1) - padding, padding2, padding2, darkblue);
                    }
                    else if (map[i + 1][j + 1] < 2)
                    {
                        DrawRectangle(len * (j + 1) - padding, len * (i + 1) - padding, padding2, padding2, darkblue);
                    }
                    else if (map[i][j + 1] < 2)
                    {
                        DrawRectangle(len * (j + 1) - padding, len * (i + 1) + padding - padding2, padding2, padding2, darkblue);
                    }
                }
                else if (sum == 4)
                {
                    DrawRectangle(len * (j + 1) - padding, len * (i + 1) - padding, 2 * padding, 2 * padding, darkblue);
                }
            }
        }
        for (int i = 0; i <= 30; i++)
        {
            for (int j = 0; j < 30; j++)
            {
                if (map[i][j] >= 2 && map[i][j + 1] < 2)
                {
                    DrawLine(len * (j + 1) - padding, len * i + padding + padding2, len * (j + 1) - padding, len * (i + 1) - (padding + padding2), BLUE);
                }
                if (map[i][j] < 2 && map[i][j + 1] >= 2)
                {
                    DrawLine(len * (j + 1) + padding, len * i + padding + padding2, len * (j + 1) + padding, len * (i + 1) - (padding + padding2), BLUE);
                }
            }
        }
        for (int i = 0; i < 30; i++)
        {
            for (int j = 0; j <= 30; j++)
            {
                if (map[i][j] >= 2 && map[i + 1][j] < 2)
                {
                    DrawLine(len * j + (padding + padding2), len * (i + 1) - padding, len * (j + 1) - (padding + padding2), len * (i + 1) - padding, BLUE);
                }
                if (map[i][j] < 2 && map[i + 1][j] >= 2)
                {
                    DrawLine(len * j + (padding + padding2), len * (i + 1) + padding, len * (j + 1) - (padding + padding2), len * (i + 1) + padding, BLUE);
                }
            }
        }
        for (int i = 0; i < 30; i++)
        {
            for (int j = 0; j < 30; j++)
            {
                int sum = (map[i][j] >= 2) + (map[i + 1][j] >= 2) + (map[i][j + 1] >= 2) + (map[i + 1][j + 1] >= 2);
                
                if (sum == 2 && map[i + 1][j] < 2 && map[i + 1][j + 1] < 2)
                {
                    DrawLine(len * (j + 1) - (padding + padding2), len * (i + 1) - padding, len * (j + 1) + (padding + padding2), len * (i + 1) - padding, BLUE);
                }
                else if (sum == 2 && map[i][j] < 2 && map[i][j + 1] < 2)
                {
                    DrawLine(len * (j + 1) - (padding + padding2), len * (i + 1) + padding, len * (j + 1) + (padding + padding2), len * (i + 1) + padding, BLUE);
                }
                else if (sum == 2 && map[i][j + 1] < 2 && map[i + 1][j + 1] < 2)
                {
                    DrawLine(len * (j + 1) - padding, len * (i + 1) - (padding + padding2), len * (j + 1) - padding, len * (i + 1) + (padding + padding2), BLUE);
                }
                else if (sum == 2 && map[i][j] < 2 && map[i + 1][j] < 2)
                {
                    DrawLine(len * (j + 1) + padding, len * (i + 1) - (padding + padding2), len * (j + 1) + padding, len * (i + 1) + (padding + padding2), BLUE);
                }
                else if (sum == 1 || sum == 2)
                {
                    if (map[i][j] >= 2)
                    {
                        DrawPizza((Vector2){ 1.0f * len * (j + 1) - (padding + padding2), 1.0f * len * (i + 1) - (padding + padding2) }, padding2, 0.0f, 90.0f, BLUE, darkblue);
                    }
                    if (map[i + 1][j] >= 2)
                    {
                        DrawPizza((Vector2){ 1.0f * len * (j + 1) - (padding + padding2), 1.0f * len * (i + 1) + (padding + padding2) }, padding2, 90.0f, 180.0f, BLUE, darkblue);
                    }
                    if (map[i + 1][j + 1] >= 2)
                    {
                        DrawPizza((Vector2){ 1.0f * len * (j + 1) + (padding + padding2), 1.0f * len * (i + 1) + (padding + padding2) }, padding2, 180.0f, 270.0f, BLUE, darkblue);
                    }
                    if (map[i][j + 1] >= 2)
                    {
                        DrawPizza((Vector2){ 1.0f * len * (j + 1) + (padding + padding2), 1.0f * len * (i + 1) - (padding + padding2) }, padding2, 270.0f, 360.0f, BLUE, darkblue);
                    }
                }
                else if (sum == 3)
                {
                    if (map[i][j] < 2)
                    {
                        DrawLine(len * (j + 1) - (padding + padding2), len * (i + 1) + padding, len * (j + 1) + padding - padding2, len * (i + 1) + padding, BLUE);
                        DrawLine(len * (j + 1) + padding, len * (i + 1) - (padding + padding2), len * (j + 1) + padding, len * (i + 1) + padding - padding2, BLUE);
                        DrawPizza((Vector2){ 1.0f * len * (j + 1) + padding - padding2, 1.0f * len * (i + 1) + padding - padding2 }, padding2, 0.0f, 90.0f, BLUE, darkorange);
                    }
                    if (map[i + 1][j] < 2)
                    {
                        DrawLine(len * (j + 1) - (padding + padding2), len * (i + 1) - padding, len * (j + 1) + padding - padding2, len * (i + 1) - padding, BLUE);
                        DrawLine(len * (j + 1) + padding, len * (i + 1) - padding + padding2, len * (j + 1) + padding, len * (i + 1) + (padding + padding2), BLUE);
                        DrawPizza((Vector2){ 1.0f * len * (j + 1) + padding - padding2, 1.0f * len * (i + 1) - padding + padding2 }, padding2, 90.0f, 180.0f, BLUE, darkorange);
                    }
                    if (map[i + 1][j + 1] < 2)
                    {
                        DrawLine(len * (j + 1) - padding + padding2, len * (i + 1) - padding, len * (j + 1) + (padding + padding2), len * (i + 1) - padding, BLUE);
                        DrawLine(len * (j + 1) - padding, len * (i + 1) - padding + padding2, len * (j + 1) - padding, len * (i + 1) + (padding + padding2), BLUE);
                        DrawPizza((Vector2){ 1.0f * len * (j + 1) - padding + padding2, 1.0f * len * (i + 1) - padding + padding2 }, padding2, 180.0f, 270.0f, BLUE, darkorange);
                    }
                    if (map[i][j + 1] < 2)
                    {
                        DrawLine(len * (j + 1) - padding + padding2, len * (i + 1) + padding, len * (j + 1) + (padding + padding2), len * (i + 1) + padding, BLUE);
                        DrawLine(len * (j + 1) - padding, len * (i + 1) - (padding + padding2), len * (j + 1) - padding, len * (i + 1) + padding - padding2, BLUE);
                        DrawPizza((Vector2){ 1.0f * len * (j + 1) - padding + padding2, 1.0f * len * (i + 1) + padding - padding2 }, padding2, 270.0f, 360.0f, BLUE, darkorange);
                    }
                }
            }
            for (int i = 0; i <= 30; i++)
            {
                for (int j = 0; j < 30; j++)
                {
                    if (map[i][j] >= 2 && map[i][j + 1] >= 2)
                    {
                        DrawRectangle(len * (j + 1) - padding - padding2, len * i + padding - height, 2 * padding + 2 * padding2, len - 2 * padding, darkblue);
                    }
                }
            }
            for (int i = 0; i < 30; i++)
            {
                for (int j = 0; j <= 30; j++)
                {
                    if (map[i][j] >= 2 && map[i + 1][j] >= 2)
                    {
                        DrawRectangle(len * j + padding, len * (i + 1) - padding - padding2 - height, len - 2 * padding, 2 * padding + 2 * padding2, darkblue);
                    }
                }
            }
            for (int i = 0; i < 30; i++)
            {
                for (int j = 0; j < 30; j++)
                {
                    int sum = (map[i][j] >= 2) + (map[i + 1][j] >= 2) + (map[i][j + 1] >= 2) + (map[i + 1][j + 1] >= 2);
                    if (sum == 3)
                    {
                        if (map[i][j] < 2)
                        {
                            DrawRectangle(len * (j + 1) + padding - padding2, len * (i + 1) + padding - padding2 - height, padding2, padding2, darkblue);
                        }
                        else if (map[i + 1][j] < 2)
                        {
                            DrawRectangle(len * (j + 1) + padding - padding2, len * (i + 1) - padding - height, padding2, padding2, darkblue);
                        }
                        else if (map[i + 1][j + 1] < 2)
                        {
                            DrawRectangle(len * (j + 1) - padding, len * (i + 1) - padding - height, padding2, padding2, darkblue);
                        }
                        else if (map[i][j + 1] < 2)
                        {
                            DrawRectangle(len * (j + 1) - padding, len * (i + 1) + padding - padding2 - height, padding2, padding2, darkblue);
                        }
                    }
                    else if (sum == 4)
                    {
                        DrawRectangle(len * (j + 1) - padding, len * (i + 1) - padding - height, 2 * padding, 2 * padding, darkblue);
                    }
                }
            }
            for (int i = 1; i <= 30; i++)
            {
                for (int j = 1; j <= 30; j++)
                {
                    if (map[i][j] >= 2)
                    {
                        int len2 = len - 2 * padding;
                        DrawRectangleRounded((Rectangle){ 1.0f * len * j + padding, 1.0f * len * i + padding - height, 1.0f * len2, 1.0f * len2 }, 2.0f * padding2 / len2, 1, darkblue);
                    }
                }
            }
            for (int i = 0; i <= 30; i++)
            {
                for (int j = 0; j < 30; j++)
                {
                    if (map[i][j] >= 2 && map[i][j + 1] < 2)
                    {
                        DrawLine(len * (j + 1) - padding, len * i + (padding + padding2) - height, len * (j + 1) - padding, len * (i + 1) - (padding + padding2) - height, BLUE);
                    }
                    if (map[i][j] < 2 && map[i][j + 1] >= 2)
                    {
                        DrawLine(len * (j + 1) + padding, len * i + (padding + padding2) - height, len * (j + 1) + padding, len * (i + 1) - (padding + padding2) - height, BLUE);
                    }
                }
            }
            for (int i = 0; i < 30; i++)
            {
                for (int j = 0; j <= 30; j++)
                {
                    if (map[i][j] >= 2 && map[i + 1][j] < 2)
                    {
                        DrawLine(len * j + (padding + padding2), len * (i + 1) - padding - height, len * (j + 1) - (padding + padding2), len * (i + 1) - padding - height, BLUE);
                    }
                    if (map[i][j] < 2 && map[i + 1][j] >= 2)
                    {
                        DrawLine(len * j + (padding + padding2), len * (i + 1) + padding - height, len * (j + 1) - (padding + padding2), len * (i + 1) + padding - height, BLUE);
                    }
                }
            }
            for (int i = 0; i < 30; i++)
            {
                for (int j = 0; j < 30; j++)
                {
                    int sum = (map[i][j] >= 2) + (map[i + 1][j] >= 2) + (map[i][j + 1] >= 2) + (map[i + 1][j + 1] >= 2);
                    
                    if (sum == 2 && map[i + 1][j] < 2 && map[i + 1][j + 1] < 2)
                    {
                        DrawLine(len * (j + 1) - (padding + padding2), len * (i + 1) - padding - height, len * (j + 1) + (padding + padding2), len * (i + 1) - padding - height, BLUE);
                    }
                    else if (sum == 2 && map[i][j] < 2 && map[i][j + 1] < 2)
                    {
                        DrawLine(len * (j + 1) - (padding + padding2), len * (i + 1) + padding - height, len * (j + 1) + (padding + padding2), len * (i + 1) + padding - height, BLUE);
                    }
                    else if (sum == 2 && map[i][j + 1] < 2 && map[i + 1][j + 1] < 2)
                    {
                        DrawLine(len * (j + 1) - padding, len * (i + 1) - (padding + padding2) - height, len * (j + 1) - padding, len * (i + 1) + (padding + padding2) - height, BLUE);
                    }
                    else if (sum == 2 && map[i][j] < 2 && map[i + 1][j] < 2)
                    {
                        DrawLine(len * (j + 1) + padding, len * (i + 1) - (padding + padding2) - height, len * (j + 1) + padding, len * (i + 1) + (padding + padding2) - height, BLUE);
                    }
                    else if (sum == 1 || sum == 2)
                    {
                        if (map[i][j] >= 2)
                        {
                            DrawPizza((Vector2){ 1.0f * len * (j + 1) - (padding + padding2), 1.0f * len * (i + 1) - (padding + padding2) - height }, padding2, 0.0f, 90.0f, BLUE, darkblue);
                        }
                        if (map[i + 1][j] >= 2)
                        {
                            DrawPizza((Vector2){ 1.0f * len * (j + 1) - (padding + padding2), 1.0f * len * (i + 1) + (padding + padding2) - height }, padding2, 90.0f, 180.0f, BLUE, darkblue);
                        }
                        if (map[i + 1][j + 1] >= 2)
                        {
                            DrawPizza((Vector2){ 1.0f * len * (j + 1) + (padding + padding2), 1.0f * len * (i + 1) + (padding + padding2) - height }, padding2, 180.0f, 270.0f, BLUE, darkblue);
                        }
                        if (map[i][j + 1] >= 2)
                        {
                            DrawPizza((Vector2){ 1.0f * len * (j + 1) + (padding + padding2), 1.0f * len * (i + 1) - (padding + padding2) - height }, padding2, 270.0f, 360.0f, BLUE, darkblue);
                        }
                    }
                    else if (sum == 3)
                    {
                        if (map[i][j] < 2)
                        {
                            DrawLine(len * (j + 1) - (padding + padding2), len * (i + 1) + padding - height, len * (j + 1) + padding - padding2, len * (i + 1) + padding - height, BLUE);
                            DrawLine(len * (j + 1) + padding, len * (i + 1) - (padding + padding2) - height, len * (j + 1) + padding, len * (i + 1) - height + padding - padding2, BLUE);
                            DrawPizza((Vector2){ 1.0f * len * (j + 1) + padding - padding2, 1.0f * len * (i + 1) - height + padding - padding2 }, padding2, 0.0f, 90.0f, BLUE, darkorange);
                        }
                        if (map[i + 1][j] < 2)
                        {
                            DrawLine(len * (j + 1) - (padding + padding2), len * (i + 1) - padding - height, len * (j + 1) + padding - padding2, len * (i + 1) - padding - height, BLUE);
                            DrawLine(len * (j + 1) + padding, len * (i + 1) - height - padding + padding2, len * (j + 1) + padding, len * (i + 1) + (padding + padding2) - height, BLUE);
                            DrawPizza((Vector2){ 1.0f * len * (j + 1) + padding - padding2, 1.0f * len * (i + 1) - height - padding + padding2 }, padding2, 90.0f, 180.0f, BLUE, darkblue);
                        }
                        if (map[i + 1][j + 1] < 2)
                        {
                            DrawLine(len * (j + 1) - padding + padding2, len * (i + 1) - padding - height, len * (j + 1) + (padding + padding2), len * (i + 1) - padding - height, BLUE);
                            DrawLine(len * (j + 1) - padding, len * (i + 1) - height - padding + padding2, len * (j + 1) - padding, len * (i + 1) + (padding + padding2) - height, BLUE);
                            DrawPizza((Vector2){ 1.0f * len * (j + 1) - padding + padding2, 1.0f * len * (i + 1) - height - padding + padding2 }, padding2, 180.0f, 270.0f, BLUE, darkblue);
                        }
                        if (map[i][j + 1] < 2)
                        {
                            DrawLine(len * (j + 1) - padding + padding2, len * (i + 1) + padding - height, len * (j + 1) + (padding + padding2), len * (i + 1) + padding - height, BLUE);
                            DrawLine(len * (j + 1) - padding, len * (i + 1) - (padding + padding2) - height, len * (j + 1) - padding, len * (i + 1) - height + padding - padding2, BLUE);
                            DrawPizza((Vector2){ 1.0f * len * (j + 1) - padding + padding2, 1.0f * len * (i + 1) - height + padding - padding2 }, padding2, 270.0f, 360.0f, BLUE, darkorange);
                        }
                    }
                }
            }
        }
        DrawFPS(5, 5);
        EndDrawing();
        // Main Loop
    }
    CloseWindow();

    return 0;
}