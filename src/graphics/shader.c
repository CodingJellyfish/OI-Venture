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

#define GRAPHICS_SHADER_C

#include "graphics/shader.h"

#include "graphics/default_shader.h"

kh_shader_t kh_create_shader(kh_shader_desc_t *desc)
{
    sg_shader_desc shader_desc = desc->_sokol_desc;

    if (desc->vs_code) {
        shader_desc.vs.source = desc->vs_code;
    }
    if (desc->fs_code) {
        shader_desc.fs.source = desc->fs_code;
    }

    sg_shader shader = sg_make_shader(&desc->_sokol_desc);

    return (kh_shader_t) {
        ._shader = shader
    };
}

void kh_destroy_shader(kh_shader_t *shader)
{
    sg_destroy_shader(shader->_shader);
}

void KhGraphicsShaderImport(ecs_world_t *world)
{
    ECS_MODULE(world, KhGraphicsShader);
}

