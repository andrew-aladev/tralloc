// This file is part of tralloc. Copyright (C) 2013 Andrew Aladjev aladjev.andrew@gmail.com
// tralloc is free software: you can redistribute it and/or modify it under the terms of the GNU General Lesser Public License as published by the Free Software Foundation, either version 3 of the License, or (at your option) any later version.
// tralloc is distributed in the hope that it will be useful, but WITHOUT ANY WARRANTY; without even the implied warranty of MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU General Lesser Public License for more details.
// You should have received a copy of the GNU General Lesser Public License along with tralloc. If not, see <http://www.gnu.org/licenses/>.

#ifndef TRALLOC_UTILS_DYNARR_H
#define TRALLOC_UTILS_DYNARR_H

#include "../tree.h"

typedef struct tralloc_dynarr_t {
    size_t length;
    size_t start_capacity;
    size_t current_capacity;
    tralloc_context ** data;
} tralloc_dynarr;

// Function creates new dynamic array with capacity and attaches it to ctx.
// Function returns pointer to tralloc_dynarr or NULL if error occurred.
inline
tralloc_dynarr * tralloc_dynarr_new ( const tralloc_context * ctx, size_t capacity )
{
    if ( capacity == 0 ) {
        return NULL;
    }

    tralloc_dynarr * arr = tralloc ( ctx, sizeof ( tralloc_dynarr ) );
    if ( arr == NULL ) {
        return NULL;
    }

    arr->start_capacity = arr->current_capacity = capacity;
    tralloc_context ** data = tralloc ( arr, capacity * sizeof ( uintptr_t ) );
    if ( data == NULL ) {
        tralloc_free ( arr );
        return NULL;
    }
    arr->data = data;
    arr->length = 0;

    return arr;
}

// Function inserts data before index position to arr.
// Function returns zero or non-zero value if error occurred.
uint8_t tralloc_dynarr_insert_before ( tralloc_dynarr * arr, size_t index, void * data );

// Function inserts data after index position to arr.
// Function returns zero or non-zero value if error occurred.
inline
uint8_t tralloc_dynarr_insert_after ( tralloc_dynarr * arr, size_t index, void * data )
{
    return tralloc_dynarr_insert_before ( arr, index + 1, data );
}

// Function grows arr to index + 1 position and sets data to index.
// Function returns zero or non-zero value if error occurred.
uint8_t tralloc_dynarr_grow_and_set ( tralloc_dynarr * arr, size_t index, void * data );

// Function sets data to index in arr.
inline
void tralloc_dynarr_set ( tralloc_dynarr * arr, size_t index, void * data )
{
    arr->data[index] = data;
}

// Function returns data from index in arr.
inline
void * tralloc_dynarr_get ( const tralloc_dynarr * arr, size_t index )
{
    return arr->data[index];
}

// Function returns length from arr.
inline
size_t tralloc_dynarr_get_length ( const tralloc_dynarr * arr )
{
    return arr->length;
}

// Function pushes data to arr.
// Function returns zero or non-zero value if error occurred.
uint8_t tralloc_dynarr_push ( tralloc_dynarr * arr, void * data );

// Function deletes first data from arr.
// Function returns zero or non-zero value if error occurred.
uint8_t tralloc_dynarr_pop ( tralloc_dynarr * arr );

// Function deletes data from index in arr.
// Function returns zero or non-zero value if error occurred.
uint8_t tralloc_dynarr_delete ( tralloc_dynarr * arr, size_t index );

#endif
