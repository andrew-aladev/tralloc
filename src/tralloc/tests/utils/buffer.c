// This file is part of tralloc. Copyright (C) 2013 Andrew Aladjev aladjev.andrew@gmail.com
// tralloc is free software: you can redistribute it and/or modify it under the terms of the GNU General Public License as published by the Free Software Foundation, either version 3 of the License, or (at your option) any later version.
// tralloc is distributed in the hope that it will be useful, but WITHOUT ANY WARRANTY; without even the implied warranty of MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU General Public License for more details.
// You should have received a copy of the GNU General Public License along with tralloc. If not, see <http://www.gnu.org/licenses/>.

#include <tralloc/tests/utils/common.h>
#include <tralloc/tree.h>
#include <tralloc/utils/buffer.h>
#include <string.h>


tralloc_bool _tralloc_test_utils_buffer ( tralloc_context * ctx )
{
    if (
        tralloc_buffer_new                 ( NULL, NULL, 0 )    != TRALLOC_ERROR_REQUIRED_ARGUMENT_IS_NULL ||
        tralloc_buffer_new_with_extensions ( NULL, NULL, 0, 0 ) != TRALLOC_ERROR_REQUIRED_ARGUMENT_IS_NULL
    ) {
        return TRALLOC_FALSE;
    }

    tralloc_buffer * buffer;

    if (
        tralloc_buffer_new ( ctx, &buffer, 0 )        != 0 ||
        tralloc_buffer_add_write_length ( buffer, 1 ) != TRALLOC_ERROR_UTILS_BUFFER_OVERFLOW
    ) {
        return TRALLOC_FALSE;
    }

    if ( tralloc_buffer_prepare_write_length ( buffer, sizeof ( char ) * 9 ) != 0 ) {
        return TRALLOC_FALSE;
    }
    char * data = ( char * ) tralloc_buffer_get_write_pointer ( buffer );
    data[0] = '0';
    data[1] = '1';
    data[2] = '2';
    if ( tralloc_buffer_add_write_length ( buffer, sizeof ( char ) * 3 ) != 0 ) {
        return TRALLOC_FALSE;
    }

    if ( tralloc_buffer_prepare_write_length ( buffer, sizeof ( char ) * 3 ) != 0 ) {
        return TRALLOC_FALSE;
    }
    data    = ( char * ) tralloc_buffer_get_write_pointer ( buffer );
    data[0] = '3';
    data[1] = '4';
    if ( tralloc_buffer_add_write_length ( buffer, sizeof ( char ) * 2 ) != 0 ) {
        return TRALLOC_FALSE;
    }

    if ( tralloc_buffer_prepare_write_length ( buffer, sizeof ( char ) * 5 ) != 0 ) {
        return TRALLOC_FALSE;
    }
    data    = ( char * ) tralloc_buffer_get_write_pointer ( buffer );
    data[0] = '5';
    if ( tralloc_buffer_add_write_length ( buffer, sizeof ( char ) ) != 0 ) {
        return TRALLOC_FALSE;
    }

    if (
        tralloc_buffer_get_read_length ( buffer ) != sizeof ( char ) * 6 ||
        tralloc_buffer_resize ( buffer, sizeof ( char ) * 5 ) != TRALLOC_ERROR_UTILS_BUFFER_OVERFLOW ||
        buffer->capacity != sizeof ( char ) * 10
    ) {
        return TRALLOC_FALSE;
    }

    if ( tralloc_buffer_prepare_write_length ( buffer, sizeof ( char ) * 8 ) != 0 ) {
        return TRALLOC_FALSE;
    }
    data    = ( char * ) tralloc_buffer_get_write_pointer ( buffer );
    data[0] = '6';
    data[1] = '7';
    data[2] = '8';
    data[3] = '9';
    if ( tralloc_buffer_add_write_length ( buffer, sizeof ( char ) * 4 ) != 0 ) {
        return TRALLOC_FALSE;
    }

    if ( tralloc_buffer_right_trim ( buffer ) != 0 ) {
        return TRALLOC_FALSE;
    }

    data = ( char * ) tralloc_buffer_get_read_pointer ( buffer );
    if (
        buffer->offset   != 0  ||
        buffer->capacity != sizeof ( char ) * 10 ||
        tralloc_buffer_get_read_length ( buffer ) != sizeof ( char ) * 10 ||
        strncmp ( ( char * ) data, "0123456789", sizeof ( char ) * 10 ) != 0
    ) {
        return TRALLOC_FALSE;
    }

    if (
        tralloc_buffer_add_read_length ( buffer, sizeof ( char ) * 20 ) != TRALLOC_ERROR_UTILS_BUFFER_OVERFLOW ||
        tralloc_buffer_add_read_length ( buffer, sizeof ( char ) * 4 )  != 0
    ) {
        return TRALLOC_FALSE;
    }

    data = ( char * ) tralloc_buffer_get_read_pointer ( buffer );
    if (
        buffer->offset   != sizeof ( char ) * 4  ||
        buffer->capacity != sizeof ( char ) * 10 ||
        tralloc_buffer_get_read_length ( buffer ) != sizeof ( char ) * 6 ||
        strncmp ( ( char * ) data, "456789", sizeof ( char ) * 6 ) != 0
    ) {
        return TRALLOC_FALSE;
    }

    if (
        tralloc_buffer_add_read_length ( buffer, sizeof ( char ) ) != 0 ||
        tralloc_buffer_left_trim ( buffer ) != 0
    ) {
        return TRALLOC_FALSE;
    }

    data = ( char * ) tralloc_buffer_get_read_pointer ( buffer );
    if (
        buffer->offset   != 0 ||
        buffer->capacity != sizeof ( char ) * 5 ||
        tralloc_buffer_get_read_length ( buffer ) != sizeof ( char ) * 5 ||
        strncmp ( ( char * ) data, "56789", sizeof ( char ) * 5 ) != 0
    ) {
        return TRALLOC_FALSE;
    }

    if (
        tralloc_buffer_add_read_length ( buffer, sizeof ( char ) * 5 ) != 0 ||
        tralloc_buffer_trim ( buffer ) != 0 ||

        buffer->offset != 0 ||
        buffer->length != 0 ||
        tralloc_buffer_get_read_length ( buffer ) != 0
    ) {
        return TRALLOC_FALSE;
    }

    if ( tralloc_free ( buffer ) != 0 ) {
        return TRALLOC_FALSE;
    }
    return TRALLOC_TRUE;
}
