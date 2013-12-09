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

// Function creates new dynamic array with capacity and attaches it to ctx.
// Function returns pointer to talloc_dynarr or NULL if error occurred.
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

// Function inserts data before index position to arr.
// Function returns zero or non-zero value if error occurred.
uint8_t talloc_dynarr_insert_before ( talloc_dynarr * arr, size_t index, void * data );

// Function inserts data after index position to arr.
// Function returns zero or non-zero value if error occurred.
inline
uint8_t talloc_dynarr_insert_after ( talloc_dynarr * arr, size_t index, void * data )
{
    return talloc_dynarr_insert_before ( arr, index + 1, data );
}

// Function grows arr to index + 1 position and sets data to index.
// Function returns zero or non-zero value if error occurred.
uint8_t talloc_dynarr_grow_and_set ( talloc_dynarr * arr, size_t index, void * data );

// Function sets data to index in arr.
inline
void talloc_dynarr_set ( talloc_dynarr * arr, size_t index, void * data )
{
    arr->data[index] = data;
}

// Function returns data from index in arr.
inline
void * talloc_dynarr_get ( talloc_dynarr * arr, size_t index )
{
    return arr->data[index];
}

// Function returns length from arr.
inline
size_t talloc_dynarr_get_length ( talloc_dynarr * arr )
{
    return arr->length;
}

// Function pushes data to arr.
// Function returns zero or non-zero value if error occurred.
uint8_t talloc_dynarr_push ( talloc_dynarr * arr, void * data );

// Function deletes first data from arr.
// Function returns zero or non-zero value if error occurred.
uint8_t talloc_dynarr_pop ( talloc_dynarr * arr );

// Function deletes data from index in arr.
// Function returns zero or non-zero value if error occurred.
uint8_t talloc_dynarr_delete ( talloc_dynarr * arr, size_t index );

#endif
