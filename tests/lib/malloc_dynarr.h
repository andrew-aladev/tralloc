// This file is part of tralloc. Copyright (C) 2013 Andrew Aladjev aladjev.andrew@gmail.com
// tralloc is free software: you can redistribute it and/or modify it under the terms of the GNU General Public License as published by the Free Software Foundation, either version 3 of the License, or (at your option) any later version.
// tralloc is distributed in the hope that it will be useful, but WITHOUT ANY WARRANTY; without even the implied warranty of MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU General Public License for more details.
// You should have received a copy of the GNU General Public License along with tralloc. If not, see <http://www.gnu.org/licenses/>.

#ifndef TRALLOC_UTILS_MALLOC_DYNARR_H
#define TRALLOC_UTILS_MALLOC_DYNARR_H

#include <tralloc/macro.h>
#include <tralloc/types.h>
#include <stdio.h>
#include <stdlib.h>

#undef _TRALLOC_INLINE
#ifdef _TRALLOC_UTILS_MALLOC_DYNARR_INCLUDED_FROM_OBJECT
#    define _TRALLOC_INLINE _TRALLOC_INLINE_IN_OBJECT
#else
#    define _TRALLOC_INLINE _TRALLOC_INLINE_IN_HEADER
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

malloc_dynarr * malloc_dynarr_new ( size_t capacity );

_TRALLOC_INLINE
void malloc_dynarr_set_free_item ( malloc_dynarr * arr, free_item free_item )
{
    arr->free_item = free_item;
}

uint8_t malloc_dynarr_grow   ( malloc_dynarr * arr );
uint8_t malloc_dynarr_append ( malloc_dynarr * arr, void * pointer );
uint8_t malloc_dynarr_clear  ( malloc_dynarr * arr );

_TRALLOC_INLINE
void malloc_dynarr_set ( malloc_dynarr * arr, size_t position, void * pointer )
{
    arr->data[position] = pointer;
}

_TRALLOC_INLINE
void * malloc_dynarr_get ( malloc_dynarr * arr, size_t position )
{
    return arr->data[position];
}

_TRALLOC_INLINE
size_t malloc_dynarr_get_length ( malloc_dynarr * arr )
{
    return arr->length;
}

void malloc_dynarr_free ( malloc_dynarr * arr );


#endif
