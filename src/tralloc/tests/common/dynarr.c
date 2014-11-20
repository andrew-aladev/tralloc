// This file is part of tralloc. Copyright (C) 2013 Andrew Aladjev aladjev.andrew@gmail.com
// tralloc is free software: you can redistribute it and/or modify it under the terms of the GNU General Public License as published by the Free Software Foundation, either version 3 of the License, or (at your option) any later version.
// tralloc is distributed in the hope that it will be useful, but WITHOUT ANY WARRANTY; without even the implied warranty of MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU General Public License for more details.
// You should have received a copy of the GNU General Public License along with tralloc. If not, see <http://www.gnu.org/licenses/>.

#define _TRALLOC_INCLUDED_FROM_TESTS_COMMON_DYNARR_C
#include <tralloc/tests/common/dynarr.h>

#include <stdlib.h>


dynarr * dynarr_new ( size_t capacity )
{
    if ( capacity == 0 ) {
        return NULL;
    }

    dynarr * arr = malloc ( sizeof ( dynarr ) );
    if ( arr == NULL ) {
        return NULL;
    }

    arr->start_capacity = arr->current_capacity = capacity;
    void ** data = malloc ( arr->current_capacity * sizeof ( void * ) );
    if ( data == NULL ) {
        free ( arr );
        return NULL;
    }
    arr->data      = data;
    arr->length    = 0;
    arr->free_item = NULL;

    return arr;
}

uint8_t dynarr_grow ( dynarr * arr )
{
    // linear growth
    arr->current_capacity = arr->current_capacity + arr->start_capacity;
    void ** reallocated_data = realloc ( arr->data, arr->current_capacity * sizeof ( void * ) );
    if ( reallocated_data == NULL ) {
        return 1;
    }
    arr->data = reallocated_data;
    return 0;
}

uint8_t dynarr_append ( dynarr * arr, void * pointer )
{
    size_t index = arr->length;
    arr->length++;
    if ( arr->length > arr->current_capacity ) {
        if ( dynarr_grow ( arr ) != 0 ) {
            return 1;
        }
    }
    arr->data[index] = pointer;
    return 0;
}

uint8_t dynarr_clear ( dynarr * arr )
{
    if ( arr == NULL ) {
        return 1;
    }
    dynarr_free_item free_item = arr->free_item;
    size_t index;
    if ( free_item != NULL ) {
        for ( index = 0; index < arr->length; index ++ ) {
            free_item ( arr->data[index] );
        }
    }
    free ( arr->data );

    arr->current_capacity = arr->start_capacity;
    void ** data = malloc ( arr->current_capacity * sizeof ( void * ) );
    if ( data == NULL ) {
        return 2;
    }
    arr->data   = data;
    arr->length = 0;
    return 0;
}

void dynarr_free ( dynarr * arr )
{
    if ( arr == NULL ) {
        return;
    }
    dynarr_free_item free_item = arr->free_item;
    size_t index;
    if ( free_item != NULL ) {
        for ( index = 0; index < arr->length; index ++ ) {
            free_item ( arr->data[index] );
        }
    }
    free ( arr->data );
    free ( arr );
}
