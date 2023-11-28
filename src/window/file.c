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

#define WINDOW_FILE_C

#include "window/file.h"

#include <stdio.h>

static void file_response_callback(const sfetch_response_t* response)
{
    if (response->fetched) {
        const void* data = 
        uint64_t data_size = response->data.size;


    }
    if (response->failed) {
        switch (response->error_code) {
        SFETCH_ERROR_FILE_NOT_FOUND: 
            break;
        SFETCH_ERROR_BUFFER_TOO_SMALL:
            vector_uint8_t buffer = response->buffer.ptr;
            buffer
            break;
        }
    }
}

ECS_ON_SET(kh_file_t, ptr, {

});

ECS_ON_REMOVE(kh_file_t, ptr, {

});

kh_file_t* kh_file_init(char *filename)
{
    static kh_file_t ret;

    FILE *filesize_test = fopen(filename, "rt");
    fseek(filesize_test, 0, SEEK_END);

    ret.buffer = kh_vector_init_empty(uint8_t);
    kh_vector_resize(uint8_t, ret.buffer, ftell(filesize_test));

    ret._handle = sfetch_send(&(sfetch_request_t){
        .path = filename,
        .callback = file_response_callback,
        .buffer.ptr = ret.buffer,
        .buffer.size = kh_vector_size(ret.buffer)
    });

    fclose(filesize_test);

    return &ret;
}

void KhWindowFileImport(ecs_world_t *world)
{
    ECS_MODULE(world, KhWindowFile);

    ECS_META_COMPONENT(world, kh_file_t);

    ecs_set_hooks(world, kh_file_t, {
        .on_set = ecs_on_set(kh_file_t),
    });
}

