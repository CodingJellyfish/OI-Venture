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

#define GRAPHICS_TEXTURE_C

#include "graphics/texture.h"

kh_texture_t kh_create_default_texture(void)
{
    uint32_t pixels[4];
    memset(pixels, 0xFF, sizeof(pixels));

    sg_image tex = sg_make_image(&(sg_image_desc) {
        .type = SG_IMAGETYPE_2D,
        .width = 2,
        .height = 2,
        .pixel_format = SG_PIXELFORMAT_RGBA8,
        .data.subimage[0][0].ptr = pixels,
        .data.subimage[0][0].size = sizeof(pixels),
        .label = "sgp-white-texture"
    });

#ifdef KH_DEBUG
    if(sg_query_image_state(ret) != SG_RESOURCESTATE_VALID) {

        return;
    }
#endif

    return (kh_texture_t) {
        ._texture = tex
    };
}

void kh_destroy_texture(kh_texture_t *tex)
{
    sg_destroy_image(tex->_texture);
}

void KhGraphicsTextureImport(ecs_world_t *world)
{
    ECS_MODULE(world, KhGraphicsTexture);
}

