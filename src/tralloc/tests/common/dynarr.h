// This file is part of tralloc. Copyright (C) 2013 Andrew Aladjev aladjev.andrew@gmail.com
// tralloc is free software: you can redistribute it and/or modify it under the terms of the GNU General Public License as published by the Free Software Foundation, either version 3 of the License, or (at your option) any later version.
// tralloc is distributed in the hope that it will be useful, but WITHOUT ANY WARRANTY; without even the implied warranty of MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU General Public License for more details.
// You should have received a copy of the GNU General Public License along with tralloc. If not, see <http://www.gnu.org/licenses/>.

#ifndef TRALLOC_TESTS_COMMON_DYNARR_H
#define TRALLOC_TESTS_COMMON_DYNARR_H

#include <tralloc/types.h>
#include <tralloc/macro.h>

#undef _TRALLOC_INLINE
#ifdef _TRALLOC_INCLUDED_FROM_TESTS_COMMON_DYNARR_C
#    define _TRALLOC_INLINE _TRALLOC_INLINE_IN_OBJECT
#else
#    define _TRALLOC_INLINE _TRALLOC_INLINE_IN_HEADER
#endif


// this was implemented to store history of tralloc events

typedef void ( * dynarr_free_item ) ( void * item );

typedef struct dynarr_t {
    size_t length;
    size_t start_capacity;
    size_t current_capacity;
    void ** data;
    dynarr_free_item free_item;
} dynarr;

dynarr * dynarr_new ( size_t capacity );

_TRALLOC_INLINE
void dynarr_set_free_item ( dynarr * arr, dynarr_free_item free_item )
{
    arr->free_item = free_item;
}

uint8_t dynarr_grow   ( dynarr * arr );
uint8_t dynarr_append ( dynarr * arr, void * pointer );
uint8_t dynarr_clear  ( dynarr * arr );

_TRALLOC_INLINE
void dynarr_set ( dynarr * arr, size_t position, void * pointer )
{
    arr->data[position] = pointer;
}

_TRALLOC_INLINE
void * dynarr_get ( dynarr * arr, size_t position )
{
    return arr->data[position];
}

_TRALLOC_INLINE
size_t dynarr_get_length ( dynarr * arr )
{
    return arr->length;
}

void dynarr_free ( dynarr * arr );


#endif
