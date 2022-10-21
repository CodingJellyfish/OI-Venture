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

#ifndef HEADER_ARRAY_H
#define HEADER_ARRAY_H

#include <stdarg.h>

#define ARRAY(T) _array_##T

#define DEF_ARRAY(T) \
\
typedef struct { \
    T *n; \
    int len; \
    int edge; \
} ARRAY(T); \
\
ARRAY(T) new_array_##T(int size, ...); \
\
void push_array_##T(ARRAY(T) *target, T elm);

#define IMPL_ARRAY(T) \
ARRAY(T) new_array_##T(int size, ...) { \
    if (size) { \
        T *ptr = (T*) malloc(sizeof(T) * size); \
        va_list valist; \
        va_start(valist, size); \
        for (int i = 0; i < size; i++) { \
            ptr[i] = va_arg(valist, T); \
        } \
        va_end(valist); \
        return (ARRAY(T)) { ptr, size, size }; \
    } else { \
        return (ARRAY(T)) { (T*) malloc(sizeof(T)), 0, 1 }; \
    } \
} \
\
void push_array_##T(ARRAY(T) *target, T elm) { \
    if (target->len == target->edge) { \
        target->n = (T*) realloc(target->n, sizeof(T) * (target->edge *= 2)); \
    } \
    target->n[target->len++] = elm; \
} \

#endif