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

#ifndef HEADER_WINDOW_H
#define HEADER_WINDOW_H

#include <flecs.h>

#ifndef KH_DEFAULT_WINDOW_WIDTH
    #define KH_DEFAULT_WINDOW_WIDTH 1024
#endif

#ifndef KH_DEFAULT_WINDOW_HEIGHT
    #define KH_DEFAULT_WINDOW_HEIGHT 768
#endif

#ifndef KH_DEFAULT_FPS
    #define KH_DEFAULT_FPS 60
#endif

#ifndef KH_DEFAULT_THREADS
    #define KH_DEFAULT_THREADS 1
#endif

ECS_STRUCT(kh_window_t, {
    uint32_t width;
    uint32_t height;
});

#endif