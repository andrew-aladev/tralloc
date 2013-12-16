// This file is part of tralloc. Copyright (C) 2013 Andrew Aladjev aladjev.andrew@gmail.com
// tralloc is free software: you can redistribute it and/or modify it under the terms of the GNU General Public License as published by the Free Software Foundation, either version 3 of the License, or (at your option) any later version.
// tralloc is distributed in the hope that it will be useful, but WITHOUT ANY WARRANTY; without even the implied warranty of MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU General Public License for more details.
// You should have received a copy of the GNU General Public License along with tralloc. If not, see <http://www.gnu.org/licenses/>.

#include "dynarr.h"
#include <tralloc/utils/dynarr.h>

bool test_dynarr ( const tralloc_context * root )
{
    size_t a = 12345, b = 56789;
    tralloc_dynarr * arr = tralloc_dynarr_new ( root, 0 );
    if ( arr != NULL ) {
        return false;
    }
    arr = tralloc_dynarr_new ( root, 3 );
    if ( arr == NULL ) {
        return false;
    }
    if (
        arr->current_capacity != 3 || tralloc_dynarr_push          ( arr, &a )    != 0 ||
        arr->current_capacity != 3 || tralloc_dynarr_insert_after  ( arr, 0, &b ) != 0 ||
        arr->current_capacity != 3 || tralloc_dynarr_insert_before ( arr, 1, &a ) != 0 ||
        arr->current_capacity != 3 || tralloc_dynarr_push          ( arr, &a )    != 0 ||
        arr->current_capacity != 6 || tralloc_dynarr_push          ( arr, &a )    != 0 ||
        arr->current_capacity != 6 || tralloc_dynarr_push          ( arr, &a )    != 0 ||
        arr->current_capacity != 6 || tralloc_dynarr_insert_before ( arr, 5, &b ) != 0 ||
        arr->current_capacity != 9 || tralloc_dynarr_push          ( arr, &b )    != 0 ||
        arr->current_capacity != 9 || tralloc_dynarr_push          ( arr, &b )    != 0 ||
        arr->current_capacity != 9
    ) {
        tralloc_free ( arr );
        return false;
    }

    tralloc_dynarr_set ( arr, 3, &b );
    tralloc_dynarr_set ( arr, 2, &a );

    if (
        tralloc_dynarr_get_length ( arr )    != 9  ||
        tralloc_dynarr_get        ( arr, 0 ) != &a ||
        tralloc_dynarr_get        ( arr, 1 ) != &a ||
        tralloc_dynarr_get        ( arr, 2 ) != &a ||
        tralloc_dynarr_get        ( arr, 3 ) != &b ||
        tralloc_dynarr_get        ( arr, 4 ) != &a ||
        tralloc_dynarr_get        ( arr, 5 ) != &b ||
        tralloc_dynarr_get        ( arr, 6 ) != &a ||
        tralloc_dynarr_get        ( arr, 7 ) != &b ||
        tralloc_dynarr_get        ( arr, 8 ) != &b
    ) {
        tralloc_free ( arr );
        return false;
    }

    if (
        tralloc_dynarr_delete ( arr, 2 ) != 0 ||
        tralloc_dynarr_delete ( arr, 5 ) != 0
    ) {
        tralloc_free ( arr );
        return false;
    }

    if (
        tralloc_dynarr_get_length ( arr ) != 7 ||

        tralloc_dynarr_get ( arr, 0 ) != &a ||
        tralloc_dynarr_get ( arr, 1 ) != &a ||
        tralloc_dynarr_get ( arr, 2 ) != &b ||
        tralloc_dynarr_get ( arr, 3 ) != &a ||
        tralloc_dynarr_get ( arr, 4 ) != &b ||
        tralloc_dynarr_get ( arr, 5 ) != &b ||
        tralloc_dynarr_get ( arr, 6 ) != &b
    ) {
        tralloc_free ( arr );
        return false;
    }

    if (
        arr->current_capacity != 9 || tralloc_dynarr_pop ( arr ) != 0 ||
        arr->current_capacity != 6 || tralloc_dynarr_pop ( arr ) != 0 ||
        arr->current_capacity != 6 || tralloc_dynarr_pop ( arr ) != 0 ||
        arr->current_capacity != 6 || tralloc_dynarr_pop ( arr ) != 0 ||
        arr->current_capacity != 3 || tralloc_dynarr_pop ( arr ) != 0 ||
        arr->current_capacity != 3 ||
        tralloc_dynarr_get_length ( arr ) != 2
    ) {
        tralloc_free ( arr );
        return false;
    }

    if (
        tralloc_dynarr_grow_and_set ( arr, 3, &a ) != 0 ||
        tralloc_dynarr_grow_and_set ( arr, 6, &b ) != 0
    ) {
        tralloc_free ( arr );
        return false;
    }

    if (
        tralloc_dynarr_get_length ( arr ) != 7 ||
        arr->current_capacity != 9 ||
        tralloc_dynarr_get ( arr, 3 ) != &a ||
        tralloc_dynarr_get ( arr, 6 ) != &b
    ) {
        tralloc_free ( arr );
        return false;
    }

    if ( tralloc_free ( arr ) != 0 ) {
        return false;
    }
    return true;
}