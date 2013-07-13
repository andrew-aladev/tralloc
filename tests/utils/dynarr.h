// This file is part of talloc2. Copyright (C) 2013 Andrew Aladjev aladjev.andrew@gmail.com
// talloc2 is free software: you can redistribute it and/or modify it under the terms of the GNU General Public License as published by the Free Software Foundation, either version 3 of the License, or (at your option) any later version.
// talloc2 is distributed in the hope that it will be useful, but WITHOUT ANY WARRANTY; without even the implied warranty of MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU General Public License for more details.
// You should have received a copy of the GNU General Public License along with talloc2. If not, see <http://www.gnu.org/licenses/>.

#include <talloc2/utils/dynarr.h>

bool test_dynarr ( void * ctx )
{
    size_t a, b;
    talloc_dynarr * arr = talloc_dynarr_new ( ctx, 0 );
    if ( arr != NULL ) {
        return false;
    }
    arr = talloc_dynarr_new ( ctx, 3 );
    if ( arr == NULL ) {
        return false;
    }
    if (
        arr->current_capacity != 3 || talloc_dynarr_push ( arr, &a ) != 0 ||
        arr->current_capacity != 3 || talloc_dynarr_insert_after  ( arr, 0, &b ) != 0 ||
        arr->current_capacity != 3 || talloc_dynarr_insert_before ( arr, 1, &a ) != 0 ||
        arr->current_capacity != 3 || talloc_dynarr_push ( arr, &a ) != 0 ||
        arr->current_capacity != 6 || talloc_dynarr_push ( arr, &a ) != 0 ||
        arr->current_capacity != 6 || talloc_dynarr_push ( arr, &a ) != 0 ||
        arr->current_capacity != 6 || talloc_dynarr_insert_before ( arr, 5, &b ) != 0 ||
        arr->current_capacity != 9 || talloc_dynarr_push ( arr, &b ) != 0 ||
        arr->current_capacity != 9 || talloc_dynarr_push ( arr, &b ) != 0 ||
        arr->current_capacity != 9
    ) {
        talloc_free ( arr );
        return false;
    }

    talloc_dynarr_set ( arr, 3, &b );
    talloc_dynarr_set ( arr, 2, &a );

    if (
        talloc_dynarr_get_length ( arr ) != 9 ||
        talloc_dynarr_get ( arr, 0 ) != &a ||
        talloc_dynarr_get ( arr, 1 ) != &a ||
        talloc_dynarr_get ( arr, 2 ) != &a ||
        talloc_dynarr_get ( arr, 3 ) != &b ||
        talloc_dynarr_get ( arr, 4 ) != &a ||
        talloc_dynarr_get ( arr, 5 ) != &b ||
        talloc_dynarr_get ( arr, 6 ) != &a ||
        talloc_dynarr_get ( arr, 7 ) != &b ||
        talloc_dynarr_get ( arr, 8 ) != &b
    ) {
        talloc_free ( arr );
        return false;
    }

    if (
        talloc_dynarr_delete ( arr, 2 ) != 0 ||
        talloc_dynarr_delete ( arr, 5 ) != 0
    ) {
        talloc_free ( arr );
        return false;
    }

    if (
        talloc_dynarr_get_length ( arr ) != 7 ||
        talloc_dynarr_get ( arr, 0 ) != &a ||
        talloc_dynarr_get ( arr, 1 ) != &a ||
        talloc_dynarr_get ( arr, 2 ) != &b ||
        talloc_dynarr_get ( arr, 3 ) != &a ||
        talloc_dynarr_get ( arr, 4 ) != &b ||
        talloc_dynarr_get ( arr, 5 ) != &b ||
        talloc_dynarr_get ( arr, 6 ) != &b
    ) {
        talloc_free ( arr );
        return false;
    }

    if (
        arr->current_capacity != 9 || talloc_dynarr_pop ( arr ) != 0 ||
        arr->current_capacity != 6 || talloc_dynarr_pop ( arr ) != 0 ||
        arr->current_capacity != 6 || talloc_dynarr_pop ( arr ) != 0 ||
        arr->current_capacity != 6 || talloc_dynarr_pop ( arr ) != 0 ||
        arr->current_capacity != 3 || talloc_dynarr_pop ( arr ) != 0 ||
        arr->current_capacity != 3 ||
        talloc_dynarr_get_length ( arr ) != 2
    ) {
        talloc_free ( arr );
        return false;
    }

    if (
        talloc_dynarr_grow_and_set ( arr, 3, &a ) != 0 ||
        talloc_dynarr_grow_and_set ( arr, 6, &b ) != 0
    ) {
        talloc_free ( arr );
        return false;
    }

    if (
        talloc_dynarr_get_length ( arr ) != 7 ||
        arr->current_capacity != 9 ||
        talloc_dynarr_get ( arr, 3 ) != &a ||
        talloc_dynarr_get ( arr, 6 ) != &b
    ) {
        talloc_free ( arr );
        return false;
    }

    if ( talloc_free ( arr ) != 0 ) {
        return false;
    }
    return true;
}