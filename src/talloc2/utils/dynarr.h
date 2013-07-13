// This file is part of talloc2. Copyright (C) 2013 Andrew Aladjev aladjev.andrew@gmail.com
// talloc2 is free software: you can redistribute it and/or modify it under the terms of the GNU General Lesser Public License as published by the Free Software Foundation, either version 3 of the License, or (at your option) any later version.
// talloc2 is distributed in the hope that it will be useful, but WITHOUT ANY WARRANTY; without even the implied warranty of MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU General Lesser Public License for more details.
// You should have received a copy of the GNU General Lesser Public License along with talloc2. If not, see <http://www.gnu.org/licenses/>.

#ifndef TALLOC_UTILS_DYNARR_H
#define TALLOC_UTILS_DYNARR_H

#include "../tree.h"

typedef struct talloc_dynarr_t {
    size_t length;
    size_t start_capacity;
    size_t current_capacity;
    void ** data;
} talloc_dynarr;

inline
talloc_dynarr * talloc_dynarr_new ( void * ctx, size_t capacity )
{
    if ( capacity == 0 ) {
        return NULL;
    }

    talloc_dynarr * arr = talloc ( ctx, sizeof ( talloc_dynarr ) );
    if ( arr == NULL ) {
        return NULL;
    }

    arr->start_capacity = arr->current_capacity = capacity;
    void ** data = talloc ( arr, capacity * sizeof ( uintptr_t ) );
    if ( data == NULL ) {
        talloc_free ( arr );
        return NULL;
    }
    arr->data = data;
    arr->length = 0;

    return arr;
}

uint8_t talloc_dynarr_push   ( talloc_dynarr * arr, void * data );
uint8_t talloc_dynarr_pop    ( talloc_dynarr * arr );
uint8_t talloc_dynarr_delete ( talloc_dynarr * arr, size_t index );

uint8_t talloc_dynarr_insert_before ( talloc_dynarr * arr, size_t index, void * data );
inline
uint8_t talloc_dynarr_insert_after ( talloc_dynarr * arr, size_t index, void * data )
{
    return talloc_dynarr_insert_before ( arr, index + 1, data );
}

inline
void talloc_dynarr_set ( talloc_dynarr * arr, size_t position, void * pointer )
{
    arr->data[position] = pointer;
}

inline
void * talloc_dynarr_get ( talloc_dynarr * arr, size_t position )
{
    return arr->data[position];
}

inline
size_t talloc_dynarr_get_length ( talloc_dynarr * arr )
{
    return arr->length;
}

#endif
