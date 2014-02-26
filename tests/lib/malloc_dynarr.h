// This file is part of tralloc. Copyright (C) 2013 Andrew Aladjev aladjev.andrew@gmail.com
// tralloc is free software: you can redistribute it and/or modify it under the terms of the GNU General Public License as published by the Free Software Foundation, either version 3 of the License, or (at your option) any later version.
// tralloc is distributed in the hope that it will be useful, but WITHOUT ANY WARRANTY; without even the implied warranty of MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU General Public License for more details.
// You should have received a copy of the GNU General Public License along with tralloc. If not, see <http://www.gnu.org/licenses/>.

#ifndef TRALLOC_UTILS_MALLOC_DYNARR_H
#define TRALLOC_UTILS_MALLOC_DYNARR_H

#include <tralloc/macro.h>
#include <stdio.h>
#include <stdlib.h>
#include <stdint.h>
#include <stddef.h>
#include <stdbool.h>

#undef INLINE
#ifdef TRALLOC_UTILS_MALLOC_DYNARR_INCLUDED_FROM_OBJECT
#    define INLINE INLINE_IN_OBJECT
#else
#    define INLINE INLINE_IN_HEADER
#endif

// this was implemented to store history of tralloc events

typedef void ( * free_item ) ( void * item );

typedef struct malloc_dynarr_t {
    size_t length;
    size_t start_capacity;
    size_t current_capacity;
    void ** data;
    free_item free_item;
} malloc_dynarr;

INLINE
malloc_dynarr * malloc_dynarr_new ( size_t capacity )
{
    if ( capacity == 0 ) {
        return NULL;
    }

    malloc_dynarr * arr = malloc ( sizeof ( malloc_dynarr ) );
    if ( arr == NULL ) {
        return NULL;
    }

    arr->start_capacity = arr->current_capacity = capacity;
    void ** data = malloc ( arr->current_capacity * sizeof ( uintptr_t ) );
    if ( data == NULL ) {
        free ( arr );
        return NULL;
    }
    arr->data      = data;
    arr->length    = 0;
    arr->free_item = NULL;

    return arr;
}

INLINE
void malloc_dynarr_set_free_item ( malloc_dynarr * arr, free_item free_item )
{
    arr->free_item = free_item;
}

INLINE
uint8_t malloc_dynarr_grow ( malloc_dynarr * arr )
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

INLINE
uint8_t malloc_dynarr_append ( malloc_dynarr * arr, void * pointer )
{
    size_t index = arr->length;
    arr->length++;
    if ( arr->length > arr->current_capacity ) {
        if ( malloc_dynarr_grow ( arr ) != 0 ) {
            return 1;
        }
    }
    arr->data[index] = pointer;
    return 0;
}

INLINE
uint8_t malloc_dynarr_clear ( malloc_dynarr * arr )
{
    if ( arr == NULL ) {
        return 1;
    }
    free_item free_item = arr->free_item;
    if ( free_item != NULL ) {
        for ( size_t index = 0; index < arr->length; index ++ ) {
            free_item ( arr->data[index] );
        }
    }
    free ( arr->data );

    arr->current_capacity = arr->start_capacity;
    void ** data = malloc ( arr->current_capacity * sizeof ( uintptr_t ) );
    if ( data == NULL ) {
        return 2;
    }
    arr->data   = data;
    arr->length = 0;
    return 0;
}

INLINE
void malloc_dynarr_set ( malloc_dynarr * arr, size_t position, void * pointer )
{
    arr->data[position] = pointer;
}

INLINE
void * malloc_dynarr_get ( malloc_dynarr * arr, size_t position )
{
    return arr->data[position];
}

INLINE
size_t malloc_dynarr_get_length ( malloc_dynarr * arr )
{
    return arr->length;
}

INLINE
void malloc_dynarr_free ( malloc_dynarr * arr )
{
    if ( arr == NULL ) {
        return;
    }
    free_item free_item = arr->free_item;
    if ( free_item != NULL ) {
        for ( size_t index = 0; index < arr->length; index ++ ) {
            free_item ( arr->data[index] );
        }
    }
    free ( arr->data );
    free ( arr );
}

#endif
