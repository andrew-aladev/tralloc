// This file is part of talloc2. Copyright (C) 2013 Andrew Aladjev aladjev.andrew@gmail.com
// talloc2 is free software: you can redistribute it and/or modify it under the terms of the GNU General Lesser Public License as published by the Free Software Foundation, either version 3 of the License, or (at your option) any later version.
// talloc2 is distributed in the hope that it will be useful, but WITHOUT ANY WARRANTY; without even the implied warranty of MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU General Lesser Public License for more details.
// You should have received a copy of the GNU General Lesser Public License along with talloc2. If not, see <http://www.gnu.org/licenses/>.

#include "dynarr.h"

extern inline
talloc_dynarr * talloc_dynarr_new ( void * ctx, size_t capacity );

static inline
uint8_t talloc_dynarr_grow ( talloc_dynarr * arr )
{
    size_t capacity = arr->current_capacity;
    if ( capacity >= arr->length ) {
        return 0;
    }

    capacity += arr->start_capacity;
    void ** data = talloc_realloc ( arr->data, capacity * sizeof ( uintptr_t ) );
    if ( data == NULL ) {
        return 1;
    }
    arr->data             = data;
    arr->current_capacity = capacity;
    return 0;
}

uint8_t talloc_dynarr_push ( talloc_dynarr * arr, void * data )
{
    size_t index = arr->length;
    arr->length++;
    if ( talloc_dynarr_grow ( arr ) != 0 ) {
        return 1;
    }
    arr->data[index] = data;
    return 0;
}

static inline
uint8_t talloc_dynarr_reduction ( talloc_dynarr * arr )
{
    size_t capacity       = arr->current_capacity;
    size_t start_capacity = arr->start_capacity;
    if ( capacity < arr->length + start_capacity ) {
        return 0;
    }

    capacity -= start_capacity;
    void ** data = talloc_realloc ( arr->data, capacity * sizeof ( uintptr_t ) );
    if ( data == NULL ) {
        return 1;
    }
    arr->data             = data;
    arr->current_capacity = capacity;
    return 0;
}

uint8_t talloc_dynarr_pop ( talloc_dynarr * arr )
{
    arr->length--;
    if ( talloc_dynarr_reduction ( arr ) != 0 ) {
        return 1;
    }
    return 0;
}

extern inline
void talloc_dynarr_set ( talloc_dynarr * arr, size_t position, void * pointer );

extern inline
void * talloc_dynarr_get ( talloc_dynarr * arr, size_t position );

extern inline
size_t talloc_dynarr_get_length ( talloc_dynarr * arr );
