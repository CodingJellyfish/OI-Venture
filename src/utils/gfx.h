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

#ifndef HEADER_GFX_H
#define HEADER_GFX_H

#include <raylib.h>
#include <rlgl.h>

void draw_quad(
    Vector2 a, Vector2 b, Vector2 c, Vector2 d, 
    Texture2D tex, Rectangle texrec, Vector2 *origin,
    Color color, Color color2, Color color3, Color color4, bool flip
)
{
    rlSetTexture(tex.id);
    rlBegin(RL_QUADS);
    if (flip) {
        if (!origin) {
            rlColor4ub(color.r, color.g, color.b, color.a);
            rlTexCoord2f(texrec.x / tex.width, texrec.y / tex.height);
            rlVertex2f(a.x, a.y);
            rlColor4ub(color4.r, color4.g, color4.b, color4.a);
            rlTexCoord2f(texrec.x / tex.width, (texrec.y + texrec.height) / tex.height);
            rlVertex2f(d.x, d.y);
            rlColor4ub(color3.r, color3.g, color3.b, color3.a);
            rlTexCoord2f((texrec.x + texrec.width) / tex.width, (texrec.y + texrec.height) / tex.height);
            rlVertex2f(c.x, c.y);
            rlColor4ub(color2.r, color2.g, color2.b, color2.a);
            rlTexCoord2f((texrec.x + texrec.width) / tex.width, texrec.y / texrec.height);
            rlVertex2f(b.x, b.y);
        } else {
            rlColor4ub(color.r, color.g, color.b, color.a);
            rlTexCoord2f((texrec.x + a.x - origin->x) / tex.width, (texrec.y + a.y - origin->y) / tex.height);
            rlVertex2f(a.x, a.y);
            rlColor4ub(color4.r, color4.g, color4.b, color4.a);
            rlTexCoord2f((texrec.x + d.x - origin->x) / tex.width, (texrec.y + d.y - origin->y) / tex.height);
            rlVertex2f(d.x, d.y);
            rlColor4ub(color3.r, color3.g, color3.b, color3.a);
            rlTexCoord2f((texrec.x + c.x - origin->x) / tex.width, (texrec.y + c.y - origin->y) / tex.height);
            rlVertex2f(c.x, c.y);
            rlColor4ub(color2.r, color2.g, color2.b, color2.a);
            rlTexCoord2f((texrec.x + b.x - origin->x) / tex.width, (texrec.y + b.y - origin->y) / tex.height);
            rlVertex2f(b.x, b.y);
        }
    } else {
        if (!origin) {
            rlColor4ub(color.r, color.g, color.b, color.a);
            rlTexCoord2f(texrec.x / tex.width, texrec.y / tex.height);
            rlVertex2f(a.x, a.y);
            rlColor4ub(color2.r, color2.g, color2.b, color2.a);
            rlTexCoord2f((texrec.x + texrec.width) / tex.width, texrec.y / texrec.height);
            rlVertex2f(b.x, b.y);
            rlColor4ub(color3.r, color3.g, color3.b, color3.a);
            rlTexCoord2f((texrec.x + texrec.width) / tex.width, (texrec.y + texrec.height) / tex.height);
            rlVertex2f(c.x, c.y);
            rlColor4ub(color4.r, color4.g, color4.b, color4.a);
            rlTexCoord2f(texrec.x / tex.width, (texrec.y + texrec.height) / tex.height);
            rlVertex2f(d.x, d.y);
        } else {
            rlColor4ub(color.r, color.g, color.b, color.a);
            rlTexCoord2f((texrec.x + a.x - origin->x) / tex.width, (texrec.y + a.y - origin->y) / tex.height);
            rlVertex2f(a.x, a.y);
            rlColor4ub(color2.r, color2.g, color2.b, color2.a);
            rlTexCoord2f((texrec.x + b.x - origin->x) / tex.width, (texrec.y + b.y - origin->y) / tex.height);
            rlVertex2f(b.x, b.y);
            rlColor4ub(color3.r, color3.g, color3.b, color3.a);
            rlTexCoord2f((texrec.x + c.x - origin->x) / tex.width, (texrec.y + c.y - origin->y) / tex.height);
            rlVertex2f(c.x, c.y);
            rlColor4ub(color4.r, color4.g, color4.b, color4.a);
            rlTexCoord2f((texrec.x + d.x - origin->x) / tex.width, (texrec.y + d.y - origin->y) / tex.height);
            rlVertex2f(d.x, d.y);
        }
    }
    rlEnd();
}

#endif