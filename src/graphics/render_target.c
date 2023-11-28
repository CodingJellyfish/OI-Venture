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

#define GRAPHICS_RENDER_TARGET_C

#include "graphics/render_target.h"

void kh_render_target_begin(kh_render_context_t *context, kh_render_target_t *target)
{
    sg_pass_action pass_action = {0};
    sg_begin_pass(target->_render_pass, &pass_action);
}

void kh_render_target_end(kh_render_context_t *context)
{
    sg_end_pass();
}

ECS_DTOR(kh_render_target_t, ptr, {
    sg_destroy_image(ptr->_render_target);
    if (ptr->has_depth) {
        sg_destroy_image(ptr->_render_depth);
    }
    sg_destroy_pass(ptr->_render_pass);
});

ECS_ON_SET(kh_render_target_t, ptr, {
    sg_image_desc fb_image_desc;
    memset(&fb_image_desc, 0, sizeof(sg_image_desc));

    fb_image_desc.render_target = true;
    fb_image_desc.width = ptr->width;
    fb_image_desc.height = ptr->height;
    fb_image_desc.min_filter = SG_FILTER_LINEAR;
    fb_image_desc.mag_filter = SG_FILTER_LINEAR;
    fb_image_desc.wrap_u = SG_WRAP_CLAMP_TO_EDGE;
    fb_image_desc.wrap_v = SG_WRAP_CLAMP_TO_EDGE;

    ptr->_render_target = sg_make_image(&fb_image_desc);

    if (ptr->has_depth) {
        sg_image_desc fb_depth_image_desc;
        memset(&fb_depth_image_desc, 0, sizeof(sg_depth_image_desc));

        fb_depth_image_desc.render_target = true;
        fb_depth_image_desc.width = ptr->width;
        fb_depth_image_desc.height = ptr->height;
        fb_depth_image_desc.pixel_format = sapp_depth_format();
        ptr->_render_depth = sg_make_image(&fb_depth_image_desc);
    }

    sg_pass_desc pass_desc;
    memset(&pass_desc, 0, sizeof(sg_pass_desc));
    pass_desc.color_attachments[0].image = ptr->_render_target;
    pass_desc.depth_stencil_attachment.image = ptr->_render_depth;
    ptr->_render_pass = sg_make_pass(&pass_desc);

#ifdef KH_DEBUG
    if(sg_query_image_state(ptr->_render_target) != SG_RESOURCESTATE_VALID) {
        fprintf(stderr, "kh_render_target_t: Failed to create frame buffer image\n");
        exit(-1);
    }
    if(ptr->has_depth && sg_query_image_state(ptr->_render_depth) != SG_RESOURCESTATE_VALID) {
        fprintf(stderr, "kh_render_target_t: Failed to create frame buffer depth image\n");
        exit(-1);
    }
    if(sg_query_pass_state(ptr->_render_pass) != SG_RESOURCESTATE_VALID) {
        fprintf(stderr, "Failed to create frame buffer pass\n");
        exit(-1);
    }
#endif
});

void KhGraphicsRenderTargetImport(ecs_world_t *world)
{
    ECS_MODULE(world, KhGraphicsRenderTarget);

    ECS_META_COMPONENT(world, kh_render_target_t);

    ecs_set_hooks(world, kh_render_target_t, {
        .dtor   = ecs_dtor(kh_render_target_t),
        .on_set = ecs_on_set(kh_render_target_t)
    });
}

