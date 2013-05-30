// This file is part of talloc. Copyright (C) 2013 Andrew Aladjev aladjev.andrew@gmail.com
// talloc is free software: you can redistribute it and/or modify it under the terms of the GNU General Lesser Public License as published by the Free Software Foundation, either version 3 of the License, or (at your option) any later version.
// talloc is distributed in the hope that it will be useful, but WITHOUT ANY WARRANTY; without even the implied warranty of MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU General Lesser Public License for more details.
// You should have received a copy of the GNU General Lesser Public License along with talloc. If not, see <http://www.gnu.org/licenses/>.

#ifndef TALLOC_UTILS_DYNARR_H
#define TALLOC_UTILS_DYNARR_H

#include <stdio.h>
#include <stdlib.h>
#include <stdint.h>
#include <stddef.h>
#include <stdbool.h>

typedef struct talloc_dynarr_t {
    size_t length;
    size_t start_capacity;
    size_t current_capacity;
    void ** data;
} talloc_dynarr;

extern inline
talloc_dynarr * talloc_dynarr_new ( size_t capacity )
{
    if ( capacity == 0 ) {
        return NULL;
    }
    
    talloc_dynarr * arr = malloc ( sizeof ( talloc_dynarr ) );
    if ( arr == NULL ) {
        return NULL;
    }

    arr->start_capacity = arr->current_capacity = capacity;
    void ** data = malloc ( arr->current_capacity * sizeof ( uintptr_t ) );
    if ( data == NULL ) {
        free ( arr );
        return NULL;
    }
    arr->data = data;
    arr->length = 0;

    return arr;
}

extern inline
uint8_t talloc_dynarr_grow ( talloc_dynarr * arr )
{
    // linear growth
    arr->current_capacity = arr->current_capacity + arr->start_capacity;
    void ** reallocated_data = realloc ( arr->data, arr->current_capacity * sizeof ( uintptr_t ) );
    if ( reallocated_data == NULL ) {
        return 1;
    }
    arr->data = reallocated_data;
    return 0;
}

extern inline
uint8_t talloc_dynarr_append ( talloc_dynarr * arr, void * pointer )
{
    size_t index = arr->length;
    arr->length++;
    if ( arr->length > arr->current_capacity ) {
        if ( talloc_dynarr_grow ( arr ) != 0 ) {
            return 1;
        }
    }
    arr->data[index] = pointer;
    return 0;
}

extern inline
void talloc_dynarr_set ( talloc_dynarr * arr, size_t position, void * pointer )
{
    arr->data[position] = pointer;
}

extern inline
void * talloc_dynarr_get ( talloc_dynarr * arr, size_t position )
{
    return arr->data[position];
}

extern inline
size_t talloc_dynarr_get_length ( talloc_dynarr * arr )
{
    return arr->length;
}

extern inline
void talloc_dynarr_free ( talloc_dynarr * arr )
{
    if ( arr != NULL ) {
        free ( arr->data );
        free ( arr );
    }
}

#endif

