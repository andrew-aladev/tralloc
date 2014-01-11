// This file is part of tralloc. Copyright (C) 2013 Andrew Aladjev aladjev.andrew@gmail.com
// tralloc is free software: you can redistribute it and/or modify it under the terms of the GNU General Public License as published by the Free Software Foundation, either version 3 of the License, or (at your option) any later version.
// tralloc is distributed in the hope that it will be useful, but WITHOUT ANY WARRANTY; without even the implied warranty of MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU General Public License for more details.
// You should have received a copy of the GNU General Public License along with tralloc. If not, see <http://www.gnu.org/licenses/>.

#include "common.h"
#include <tralloc/tree.h>
#include <tralloc/utils/buffer.h>
#include <string.h>


bool test_errors ()
{

#if defined(TRALLOC_EXTENSIONS)
    if (
        tralloc_buffer_new                 ( NULL, NULL )    != TRALLOC_ERROR_CONTEXT_IS_NULL ||
        tralloc_buffer_with_extensions_new ( NULL, NULL, 0 ) != TRALLOC_ERROR_CONTEXT_IS_NULL
    ) {
        return false;
    }
#else
    if ( tralloc_buffer_new ( NULL, NULL ) != TRALLOC_ERROR_CONTEXT_IS_NULL ) {
        return false;
    }
#endif

    return true;
}

bool test_buffer ( tralloc_context * ctx )
{
    if ( !test_errors () ) {
        return false;
    }

    tralloc_buffer * buffer;

#if defined(TRALLOC_EXTENSIONS)
    if ( tralloc_buffer_with_extensions_new ( ctx, &buffer, 0 ) != 0 ) {
        return false;
    }
#else
    if ( tralloc_buffer_new ( ctx, &buffer ) != 0 ) {
        return false;
    }
#endif

    if ( tralloc_buffer_prepare ( buffer, 9 ) != 0 ) {
        return false;
    }
    uint8_t * buf = tralloc_buffer_get_write_point ( buffer );
    buf[0] = '0';
    buf[1] = '1';
    buf[2] = '2';
    tralloc_buffer_written ( buffer, 3 );

    if ( tralloc_buffer_prepare ( buffer, 3 ) != 0 ) {
        return false;
    }
    buf    = tralloc_buffer_get_write_point ( buffer );
    buf[0] = '3';
    buf[1] = '4';
    tralloc_buffer_written ( buffer, 2 );

    if ( tralloc_buffer_prepare ( buffer, 5 ) != 0 ) {
        return false;
    }
    buf    = tralloc_buffer_get_write_point ( buffer );
    buf[0] = '5';
    tralloc_buffer_written ( buffer, 1 );

    if (
        tralloc_buffer_get_length ( buffer ) != 6 ||
        buffer->length != 10
    ) {
        return false;
    }

    if ( tralloc_buffer_prepare ( buffer, 4 ) != 0 ) {
        return false;
    }
    buf    = tralloc_buffer_get_write_point ( buffer );
    buf[0] = '6';
    buf[1] = '7';
    buf[2] = '8';
    buf[3] = '9';
    tralloc_buffer_written ( buffer, 4 );

    if ( tralloc_buffer_trim ( buffer ) != 0 ) {
        return false;
    }

    if (
        buffer->data_offset != 0  ||
        buffer->length      != 10 ||
        tralloc_buffer_get_length ( buffer ) != 10 ||
        strncmp ( ( char * ) tralloc_buffer_get_read_point ( buffer ), "0123456789", 10 ) != 0
    ) {
        return false;
    }

    if ( tralloc_buffer_readed ( buffer, 20 ) != TRALLOC_ERROR_UTILS_BUFFER_OVERFLOW ) {
        return false;
    }

    if ( tralloc_buffer_readed ( buffer, 4 ) != 0 ) {
        return false;
    }

    if (
        buffer->data_offset != 4  ||
        buffer->length      != 10 ||
        tralloc_buffer_get_length ( buffer ) != 6 ||
        strncmp ( ( char * ) tralloc_buffer_get_read_point ( buffer ), "456789", 6 ) != 0
    ) {
        return false;
    }

    if (
        tralloc_buffer_readed ( buffer, 1 ) != 0 ||
        tralloc_buffer_trim ( buffer )      != 0
    ) {
        return false;
    }

    if (
        buffer->data_offset != 0 ||
        buffer->length      != 5 ||
        tralloc_buffer_get_length ( buffer ) != 5 ||
        strncmp ( ( char * ) tralloc_buffer_get_read_point ( buffer ), "56789", 5 ) != 0
    ) {
        return false;
    }

    if (
        tralloc_buffer_readed ( buffer, 5 )  != 0 ||
        tralloc_buffer_trim ( buffer )       != 0 ||
        tralloc_buffer_get_length ( buffer ) != 0 ||
        buffer->data_offset != 0 ||
        buffer->length      != 0 ||
        buffer->buf         != NULL
    ) {
        return false;
    }

    if ( tralloc_free ( buffer ) != 0 ) {
        return false;
    }

    return true;
}
