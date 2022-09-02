// OI-Venture - An adventure story featuring a couple of Chinese programming geniuses.
// Copyright (C) 2022  CodingJellyfish

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

#include <flecs.h>

#include <raylib-cpp.hpp>
 
int main()
{
    flecs::world world;

    raylib::Color textColor = raylib::Color::LightGray();
    raylib::Window window(800, 450, "raylib [core] example - basic window");
 
    SetTargetFPS(60);

    while (!window.ShouldClose()) 
    {
        BeginDrawing();
        {
            window.ClearBackground(RAYWHITE);
            textColor.DrawText("Congrats! You created your first window!", 190, 200, 20);
        }
        EndDrawing();
    }
    return 0;
}