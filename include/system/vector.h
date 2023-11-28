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

#ifndef HEADER_SYSTEM_VECTOR_H
#define HEADER_SYSTEM_VECTOR_H

#include <flecs.h>

#define kh_vector_name(T) vector_##T

#define _kh_vector_init_name(T)             _kh_vector_ ## T ## _init
#define _kh_vector_clone_name(T)            _kh_vector_ ## T ## _clone
#define _kh_vector_resize_name(T)           _kh_vector_ ## T ## _resize
#define _kh_vector_push_name(T)             _kh_vector_ ## T ## _push
#define _kh_vector_pushvec_name(T)          _kh_vector_ ## T ## _pushvec
#define _kh_vector_insert_name(T)           _kh_vector_ ## T ## _insert
#define _kh_vector_insertvec_name(T)        _kh_vector_ ## T ## _insertvec
#define _kh_vector_pop_name(T)              _kh_vector_ ## T ## _pop
#define _kh_vector_erase_name(T)            _kh_vector_ ## T ## _erase
#define _kh_vector_check_name(T)            _kh_vector_ ## T ## _check
#define _kh_vector_free_name(T)             _kh_vector_ ## T ## _free
#define _kh_vector_component_define_name(T) _kh_vector_ ## T ## _component_define

#define _kh_vector_meta_serialize_name(T)      _kh_vector_ ## T ## _meta_serialize
#define _kh_vector_meta_count_name(T)          _kh_vector_ ## T ## _meta_count
#define _kh_vector_meta_resize_name(T)         _kh_vector_ ## T ## _meta_resize
#define _kh_vector_meta_ensure_element_name(T) _kh_vector_ ## T ## _meta_ensure_element
#define _kh_vector_meta_component_name(T)      _kh_vector_ ## T ## _meta_define


#define kh_vector_init(T, COUNT, ...) _kh_vector_init_name(T)(COUNT, __VA_ARGS__)

#define kh_vector_init_empty(T) _kh_vector_init_name(T)(0)

#define kh_vector_clone(T, DST) _kh_vector_clone_name(T)(DST)

#define kh_vector_resize(T, DST, LEN) _kh_vector_resize_name(T)(DST, LEN)

#define kh_vector_push(T, DST, ELM) _kh_vector_push_name(T)(DST, ELM)

#define kh_vector_pushvec(T, DST, SRC) _kh_vector_pushvec_name(T)(DST, SRC)

#define kh_vector_insert(T, DST, ELM, POS) _kh_vector_insert_name(T)(DST, ELM, POS)

#define kh_vector_insertvec(T, DST, SRC, POS) _kh_vector_insertvec_name(T)(DST, SRC, POS)

#define kh_vector_pop(T, DST) _kh_vector_pop_name(T)(DST)

#define kh_vector_erase(T, DST, POS) _kh_vector_erase_name(T)(DST, POS)

#define kh_vector_check(T, DST) _kh_vector_check_name(T)(DST)

#define kh_vector_free(T, DST) _kh_vector_free_name(T)(DST)

#define kh_vector_component_define(T, WORLD) _kh_vector_component_define_name(T)(WORLD)

#define kh_vector_meta_define(T, WORLD) _kh_vector_meta_component_name(T)(WORLD)


#define kh_vector_size(VEC) (((uint32_t*) VEC)[-1])
#define _kh_vector_end(VEC) (((uint32_t*) VEC)[-2])

#define _KH_VALUE_COUNT 2


#define KH_VECTOR_EXTERN(T) \
    \
    typedef T* vector_##T; \
    extern ECS_COMPONENT_DECLARE(vector_##T); \
    \
    vector_##T _kh_vector_init_name(T)(uint32_t count, ...); \
    \
    vector_##T _kh_vector_clone_name(T)(vector_##T dst); \
    \
    void _kh_vector_resize_name(T)(vector_##T dst, uint32_t len); \
    \
    void _kh_vector_push_name(T)(vector_##T dst, T elm); \
    \
    void _kh_vector_pushvec_name(T)(vector_##T dst, vector_##T src); \
    \
    void _kh_vector_insert_name(T)(vector_##T dst, T elm, uint32_t pos); \
    \
    void _kh_vector_insertvec_name(T)(vector_##T dst, vector_##T src, uint32_t pos); \
    \
    void _kh_vector_pop_name(T)(vector_##T dst); \
    \
    void _kh_vector_erase_name(T)(vector_##T dst, uint32_t pos); \
    \
    bool _kh_vector_check_name(T)(vector_##T dst); \
    \
    void _kh_vector_free_name(T)(vector_##T dst); \
    \
    void _kh_vector_component_define_name(T)(ecs_world_t *world); \


#define KH_VECTOR_DECLARE(T) \
    \
    vector_##T _kh_vector_init_name(T)(uint32_t count, ...) \
    { \
        uint32_t arr_len = count ? count : 1; \
        uint32_t *ptr = (uint32_t*) ecs_os_malloc(_KH_VALUE_COUNT * sizeof(uint32_t) + arr_len * sizeof(T)); \
        ptr += _KH_VALUE_COUNT; \
        vector_##T ret = (vector_##T) ptr; \
        va_list list; \
        va_start(list, count); \
        for (uint32_t i = 0; i < count; i++) { \
            ret[i] = va_arg(list, T); \
        } \
        va_end(list); \
        kh_vector_size(ret) = 0; \
        _kh_vector_end(ret) = arr_len; \
        return ret; \
    } \
    \
    vector_##T _kh_vector_clone_name(T)(vector_##T dst) \
    { \
        vector_##T ret = _kh_vector_init_name(T)(0); \
        _kh_vector_resize_name(T)(ret, kh_vector_size(dst)); \
        ecs_os_memcpy_n(ret, dst, T, kh_vector_size(dst)); \
        return ret; \
    } \
    \
    void _kh_vector_resize_name(T)(vector_##T dst, uint32_t len) \
    { \
        bool flag = false; \
        while (_kh_vector_end(dst) < len) { \
            flag = true; \
            _kh_vector_end(dst) <<= 1; \
        } \
        while (_kh_vector_end(dst) >= len * 2) { \
            flag = true; \
            _kh_vector_end(dst) >>= 1; \
        } \
        if (flag) { \
            uint32_t *ptr = (uint32_t*) dst; \
            ptr -= _KH_VALUE_COUNT; \
            ptr = (uint32_t*) ecs_os_realloc(ptr, _KH_VALUE_COUNT * sizeof(uint32_t) + _kh_vector_end(dst) * sizeof(T)); \
        } \
    } \
    \
    void _kh_vector_push_name(T)(vector_##T dst, T elm) \
    { \
        if (_kh_vector_end(dst) == kh_vector_size(dst)) { \
            _kh_vector_resize_name(T)(dst, kh_vector_size(dst) + 1); \
        } \
        dst[kh_vector_size(dst)++] = elm; \
    } \
    \
    void _kh_vector_pushvec_name(T)(vector_##T dst, vector_##T src) \
    { \
        _kh_vector_resize_name(T)(dst, kh_vector_size(dst) + kh_vector_size(src)); \
        ecs_os_memcpy_n(dst + kh_vector_size(dst), src, T, kh_vector_size(src)); \
    } \
    \
    void _kh_vector_insert_name(T)(vector_##T dst, T elm, uint32_t pos) \
    { \
        if (_kh_vector_end(dst) == kh_vector_size(dst)) { \
            _kh_vector_resize_name(T)(dst, kh_vector_size(dst) + 1); \
        } \
        ecs_os_memmove(dst + pos + 1, dst + pos, (kh_vector_size(dst) - pos) * sizeof(T)); \
        dst[pos] = elm; \
    } \
    \
    void _kh_vector_insertvec_name(T)(vector_##T dst, vector_##T src, uint32_t pos) \
    { \
        _kh_vector_resize_name(T)(dst, kh_vector_size(dst) + kh_vector_size(src)); \
        ecs_os_memmove(dst + pos + kh_vector_size(src), dst + pos, (kh_vector_size(dst) - pos) * sizeof(T)); \
        ecs_os_memcpy_n(dst + pos, src, T, kh_vector_size(src)); \
    } \
    \
    void _kh_vector_pop_name(T)(vector_##T dst) \
    { \
        kh_vector_size(dst)--; \
    } \
    \
    void _kh_vector_erase_name(T)(vector_##T dst, uint32_t pos) \
    { \
        kh_vector_size(dst)--; \
        ecs_os_memmove(dst + pos, dst + pos + 1, (kh_vector_size(dst) - pos) * sizeof(T)); \
    } \
    \
    bool _kh_vector_check_name(T)(vector_##T dst) \
    { \
        return _kh_vector_end(dst); \
    } \
    \
    void _kh_vector_free_name(T)(vector_##T dst) \
    { \
        while (kh_vector_size(dst)) { \
            _kh_vector_pop_name(T)(dst); \
        } \
        uint32_t *ptr = (uint32_t*) dst; \
        ecs_os_free(ptr - _KH_VALUE_COUNT); \
    } \
    \
    void _kh_vector_component_define_name(T)(ecs_world_t *world) \
    { \
        ECS_COMPONENT_DEFINE(world, vector_##T); \
    }


#define KH_VECTOR_META_EXTERN(T) \
    \
    typedef T* vector_##T; \
    extern ECS_COMPONENT_DECLARE(vector_##T); \
    \
    vector_##T _kh_vector_init_name(T)(uint32_t count, ...); \
    \
    vector_##T _kh_vector_clone_name(T)(vector_##T dst); \
    \
    void _kh_vector_resize_name(T)(vector_##T dst, uint32_t len); \
    \
    void _kh_vector_push_name(T)(vector_##T dst, T elm); \
    \
    void _kh_vector_pushvec_name(T)(vector_##T dst, vector_##T src); \
    \
    void _kh_vector_insert_name(T)(vector_##T dst, T elm, uint32_t pos); \
    \
    void _kh_vector_insertvec_name(T)(vector_##T dst, vector_##T src, uint32_t pos); \
    \
    void _kh_vector_pop_name(T)(vector_##T dst); \
    \
    void _kh_vector_erase_name(T)(vector_##T dst, uint32_t pos); \
    \
    bool _kh_vector_check_name(T)(vector_##T dst); \
    \
    void _kh_vector_free_name(T)(vector_##T dst); \
    \
    void _kh_vector_component_define_name(T)(ecs_world_t *world); \
    \
    void _kh_vector_meta_component_name(T)(ecs_world_t *ecs); \


#define KH_VECTOR_META_DECLARE(T, ID) \
    \
    vector_##T _kh_vector_init_name(T)(uint32_t count, ...) \
    { \
        uint32_t arr_len = count ? count : 1; \
        uint32_t *ptr = (uint32_t*) ecs_os_malloc(_KH_VALUE_COUNT * sizeof(uint32_t) + arr_len * sizeof(T)); \
        ptr += _KH_VALUE_COUNT; \
        vector_##T ret = (vector_##T) ptr; \
        va_list list; \
        va_start(list, count); \
        for (uint32_t i = 0; i < count; i++) { \
            ret[i] = va_arg(list, T); \
        } \
        va_end(list); \
        kh_vector_size(ret) = 0; \
        _kh_vector_end(ret) = arr_len; \
        return ret; \
    } \
    \
    vector_##T _kh_vector_clone_name(T)(vector_##T dst) \
    { \
        vector_##T ret = _kh_vector_init_name(T)(0); \
        _kh_vector_resize_name(T)(ret, kh_vector_size(dst)); \
        ecs_os_memcpy_n(ret, dst, T, kh_vector_size(dst)); \
        return ret; \
    } \
    \
    void _kh_vector_resize_name(T)(vector_##T dst, uint32_t len) \
    { \
        bool flag = false; \
        while (_kh_vector_end(dst) < len) { \
            flag = true; \
            _kh_vector_end(dst) <<= 1; \
        } \
        while (_kh_vector_end(dst) >= len * 2) { \
            flag = true; \
            _kh_vector_end(dst) >>= 1; \
        } \
        if (flag) { \
            uint32_t *ptr = (uint32_t*) dst; \
            ptr -= _KH_VALUE_COUNT; \
            ptr = (uint32_t*) ecs_os_realloc(ptr, _KH_VALUE_COUNT * sizeof(uint32_t) + _kh_vector_end(dst) * sizeof(T)); \
        } \
    } \
    \
    void _kh_vector_push_name(T)(vector_##T dst, T elm) \
    { \
        if (_kh_vector_end(dst) == kh_vector_size(dst)) { \
            _kh_vector_resize_name(T)(dst, kh_vector_size(dst) + 1); \
        } \
        dst[kh_vector_size(dst)++] = elm; \
    } \
    \
    void _kh_vector_pushvec_name(T)(vector_##T dst, vector_##T src) \
    { \
        _kh_vector_resize_name(T)(dst, kh_vector_size(dst) + kh_vector_size(src)); \
        ecs_os_memcpy_n(dst + kh_vector_size(dst), src, T, kh_vector_size(src)); \
    } \
    \
    void _kh_vector_insert_name(T)(vector_##T dst, T elm, uint32_t pos) \
    { \
        if (_kh_vector_end(dst) == kh_vector_size(dst)) { \
            _kh_vector_resize_name(T)(dst, kh_vector_size(dst) + 1); \
        } \
        ecs_os_memmove(dst + pos + 1, dst + pos, (kh_vector_size(dst) - pos) * sizeof(T)); \
        dst[pos] = elm; \
    } \
    \
    void _kh_vector_insertvec_name(T)(vector_##T dst, vector_##T src, uint32_t pos) \
    { \
        _kh_vector_resize_name(T)(dst, kh_vector_size(dst) + kh_vector_size(src)); \
        ecs_os_memmove(dst + pos + kh_vector_size(src), dst + pos, (kh_vector_size(dst) - pos) * sizeof(T)); \
        ecs_os_memcpy_n(dst + pos, src, T, kh_vector_size(src)); \
    } \
    \
    void _kh_vector_pop_name(T)(vector_##T dst) \
    { \
        kh_vector_size(dst)--; \
    } \
    \
    void _kh_vector_erase_name(T)(vector_##T dst, uint32_t pos) \
    { \
        kh_vector_size(dst)--; \
        ecs_os_memmove(dst + pos, dst + pos + 1, (kh_vector_size(dst) - pos) * sizeof(T)); \
    } \
    \
    bool _kh_vector_check_name(T)(vector_##T dst) \
    { \
        return _kh_vector_end(dst); \
    } \
    \
    void _kh_vector_free_name(T)(vector_##T dst) \
    { \
        while (kh_vector_size(dst)) { \
            _kh_vector_pop_name(T)(dst); \
        } \
        uint32_t *ptr = (uint32_t*) dst; \
        ecs_os_free(ptr - _KH_VALUE_COUNT); \
    } \
    \
    void _kh_vector_component_define_name(T)(ecs_world_t *world) \
    { \
        ECS_COMPONENT_DEFINE(world, vector_##T); \
    }
    \
    static int _kh_vector_meta_serialize_name(T)(const ecs_serializer_t *ser, const void *ptr) { \
        const vector_##T *data = ptr; \
        for (size_t i = 0; i < kh_vector_size(*data); i++) { \
            ser->value(ser, ID, &(*data)[i]); \
        } \
        return 0; \
    } \
    \
    static size_t _kh_vector_meta_count_name(T)(const void *ptr) { \
        const vector_##T *data = ptr; \
        return kh_vector_size(*data); \
    } \
    \
    static void _kh_vector_meta_resize_name(T)(void *ptr, size_t size) { \
        vector_##T *data = ptr; \
        while (size < kh_vector_size(*data)) { \
            _kh_vector_pop_name(T)(*data); \
        } \
        if (size > kh_vector_size(*data)) { \
            kh_vector_size(*data) = size; \
        } \
        _kh_vector_resize_name(T)(*data, size); \
    } \
    \
    static void* _kh_vector_meta_ensure_element_name(T)(void *ptr, size_t index) { \
        vector_##T *data = ptr; \
        if (kh_vector_size(*data) <= index) { \
            kh_vector_size(*data) = index + 1; \
            kh_vector_resize(T, *data, index + 1); \
        } \
        return &(*data)[index]; \
    } \
    \
    void _kh_vector_meta_component_name(T)(ecs_world_t *ecs) \
    { \
        ECS_COMPONENT_DEFINE(ecs, vector_##T); \
        ecs_opaque(ecs, { \
            .entity = ecs_id(vector_##T), \
            .type = { \
                .as_type = ecs_vector(ecs, { .type = ID }), \
                .serialize = _kh_vector_meta_serialize_name(T), \
                .count = _kh_vector_meta_count_name(T), \
                .resize = _kh_vector_meta_resize_name(T), \
                .ensure_element = _kh_vector_meta_ensure_element_name(T) \
            } \
        }); \
    }

KH_VECTOR_META_EXTERN(bool);
KH_VECTOR_META_EXTERN(char);
KH_VECTOR_META_EXTERN(uint8_t);
KH_VECTOR_META_EXTERN(uint16_t);
KH_VECTOR_META_EXTERN(uint32_t);
KH_VECTOR_META_EXTERN(uint64_t);
KH_VECTOR_META_EXTERN(uintptr_t);
KH_VECTOR_META_EXTERN(int8_t);
KH_VECTOR_META_EXTERN(int16_t);
KH_VECTOR_META_EXTERN(int32_t);
KH_VECTOR_META_EXTERN(int64_t);
KH_VECTOR_META_EXTERN(intptr_t);
KH_VECTOR_META_EXTERN(float);
KH_VECTOR_META_EXTERN(double);
KH_VECTOR_META_EXTERN(vector_char);

void KhSystemVectorImport(ecs_world_t *world);

#endif