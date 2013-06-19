// This file is part of talloc2. Copyright (C) 2013 Andrew Aladjev aladjev.andrew@gmail.com
// talloc2 is free software: you can redistribute it and/or modify it under the terms of the GNU General Public License as published by the Free Software Foundation, either version 3 of the License, or (at your option) any later version.
// talloc2 is distributed in the hope that it will be useful, but WITHOUT ANY WARRANTY; without even the implied warranty of MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU General Public License for more details.
// You should have received a copy of the GNU General Public License along with talloc2. If not, see <http://www.gnu.org/licenses/>.

#include <stdbool.h>

#include <talloc2/tree.h>
#include <talloc2/helpers.h>

#ifdef TALLOC_UTILS_BUFFER
#include <talloc2/utils/buffer.h>
#endif

#ifdef TALLOC_UTILS_DYNARR
#include <talloc2/utils/dynarr.h>
#endif

#ifdef TALLOC_UTILS_LIST
#include <talloc2/utils/list.h>
#endif

#ifdef TALLOC_UTILS_BUFFER
bool test_buffer ( void * ctx )
{
    talloc_buffer * buffer = talloc_buffer_new ( ctx );
    if ( buffer == NULL ) {
        return false;
    }
    char * buf;

    if ( ( buf = talloc_buffer_get ( buffer, 10 ) ) == NULL ) {
        talloc_free ( buffer );
        return false;
    }
    buf[0] = '0';
    buf[1] = '1';
    buf[2] = '2';
    if ( talloc_buffer_cut ( buffer, 7 ) != 0 ) {
        talloc_free ( buffer );
        return false;
    }

    if ( ( buf = talloc_buffer_get ( buffer, 10 ) ) == NULL ) {
        talloc_free ( buffer );
        return false;
    }
    buf[0] = '3';
    buf[1] = '4';
    if ( talloc_buffer_cut ( buffer, 8 ) != 0 ) {
        talloc_free ( buffer );
        return false;
    }

    if ( ( buf = talloc_buffer_get ( buffer, 5 ) ) == NULL ) {
        talloc_free ( buffer );
        return false;
    }
    buf[0] = '5';
    if ( talloc_buffer_cut ( buffer, 4 ) != 0 ) {
        talloc_free ( buffer );
        return false;
    }

    if ( ( buf = talloc_buffer_get ( buffer, 10 ) ) == NULL ) {
        talloc_free ( buffer );
        return false;
    }
    buf[0] = '6';
    buf[1] = '7';
    buf[2] = '8';
    buf[3] = '9';
    if ( talloc_buffer_cut ( buffer, 6 ) != 0 ) {
        talloc_free ( buffer );
        return false;
    }

    if (
        buffer->length != 10 ||
        strncmp ( buffer->buf, "0123456789", 10 ) != 0
    ) {
        talloc_free ( buffer );
        return false;
    }
    return true;
}
#endif

#ifdef TALLOC_UTILS_DYNARR
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
        arr->current_capacity != 3 || talloc_dynarr_push ( arr, &a ) != 0 ||
        arr->current_capacity != 3 || talloc_dynarr_push ( arr, &b ) != 0 ||
        arr->current_capacity != 3 || talloc_dynarr_push ( arr, &a ) != 0 ||
        arr->current_capacity != 6 || talloc_dynarr_push ( arr, &a ) != 0 ||
        arr->current_capacity != 6 || talloc_dynarr_push ( arr, &b ) != 0 ||
        arr->current_capacity != 6 || talloc_dynarr_push ( arr, &a ) != 0 ||
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
        arr->current_capacity != 9 || talloc_dynarr_pop ( arr ) != 0 ||
        arr->current_capacity != 9 || talloc_dynarr_pop ( arr ) != 0 ||
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

    if ( talloc_free ( arr ) != 0 ) {
        return false;
    }
    return true;
}
#endif

#ifdef TALLOC_UTILS_LIST
bool test_list ( void * ctx )
{
    size_t a, b;
    talloc_list * list = talloc_list_new ( ctx );
    char * str_1       = talloc_strdup ( list, "str_1" );
    char * str_2       = talloc_strdup ( list, "str_2" );
    if ( list == NULL || str_1 == NULL || str_2 == NULL ) {
        return false;
    }

    if (
        talloc_list_push       ( list, str_1, true )  != 0 ||
        talloc_list_push       ( list, str_1, true )  != 0 ||
        talloc_list_push       ( list, str_1, true )  != 0 ||
        talloc_list_push       ( list, &a, false )    != 0 ||
        talloc_list_push       ( list, &b, false )    != 0 ||
        talloc_list_unshift    ( list, str_2, true )  != 0 ||
        talloc_list_unshift    ( list, str_2, true )  != 0 ||
        talloc_list_unshift    ( list, str_2, true )  != 0 ||
        talloc_list_get_length ( list ) != 8
    ) {
        talloc_free ( list );
        return false;
    }

    if ( talloc_free ( list ) != 0 ) {
        return false;
    }

    return true;
}
#endif

int main ()
{
    void * ctx = talloc_new ( NULL );
    if ( ctx == NULL ) {
        return 1;
    }

#ifdef TALLOC_UTILS_BUFFER
    if ( !test_buffer ( ctx ) ) {
        talloc_free ( ctx );
        return 2;
    }
#endif

#ifdef TALLOC_UTILS_DYNARR
    if ( !test_dynarr ( ctx ) ) {
        talloc_free ( ctx );
        return 3;
    }
#endif

#ifdef TALLOC_UTILS_LIST
    if ( !test_list ( ctx ) ) {
        talloc_free ( ctx );
        return 4;
    }
#endif

    if ( talloc_free ( ctx ) != 0 ) {
        return 5;
    }
    return 0;
}

