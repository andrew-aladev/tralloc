// This file is part of talloc2. Copyright (C) 2013 Andrew Aladjev aladjev.andrew@gmail.com
// talloc2 is free software: you can redistribute it and/or modify it under the terms of the GNU General Public License as published by the Free Software Foundation, either version 3 of the License, or (at your option) any later version.
// talloc2 is distributed in the hope that it will be useful, but WITHOUT ANY WARRANTY; without even the implied warranty of MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU General Public License for more details.
// You should have received a copy of the GNU General Public License along with talloc2. If not, see <http://www.gnu.org/licenses/>.

#include "buffer.h"
#include <talloc2/utils/buffer.h>

#include <string.h>

bool test_buffer ( void * root )
{
    talloc_buffer * buffer = talloc_buffer_new ( root );
    if ( buffer == NULL ) {
        return false;
    }

    if ( talloc_buffer_prepare ( buffer, 9 ) != 0 ) {
        talloc_free ( buffer );
        return false;
    }
    uint8_t * buf = talloc_buffer_get_write_point ( buffer );
    buf[0] = '0';
    buf[1] = '1';
    buf[2] = '2';
    talloc_buffer_written ( buffer, 3 );

    if ( talloc_buffer_prepare ( buffer, 3 ) != 0 ) {
        talloc_free ( buffer );
        return false;
    }
    buf    = talloc_buffer_get_write_point ( buffer );
    buf[0] = '3';
    buf[1] = '4';
    talloc_buffer_written ( buffer, 2 );

    if ( talloc_buffer_prepare ( buffer, 10 ) != 0 ) {
        talloc_free ( buffer );
        return false;
    }
    buf    = talloc_buffer_get_write_point ( buffer );
    buf[0] = '5';
    talloc_buffer_written ( buffer, 1 );

    if (
        talloc_buffer_get_length ( buffer ) != 6 ||
        buffer->length != 15
    ) {
        talloc_free ( buffer );
        return false;
    }

    if ( talloc_buffer_prepare ( buffer, 4 ) != 0 ) {
        talloc_free ( buffer );
        return false;
    }
    buf    = talloc_buffer_get_write_point ( buffer );
    buf[0] = '6';
    buf[1] = '7';
    buf[2] = '8';
    buf[3] = '9';
    talloc_buffer_written ( buffer, 4 );

    if ( talloc_buffer_trim ( buffer ) != 0 ) {
        talloc_free ( buffer );
        return false;
    }

    if (
        buffer->data_offset != 0 ||
        talloc_buffer_get_length ( buffer ) != 10 ||
        buffer->length != 10 ||
        strncmp ( ( char * ) talloc_buffer_get_read_point ( buffer ), "0123456789", 10 ) != 0
    ) {
        talloc_free ( buffer );
        return false;
    }

    if ( talloc_buffer_readed ( buffer, 4 ) != 0 ) {
        talloc_free ( buffer );
        return false;
    }

    if (
        buffer->data_offset != 4 ||
        talloc_buffer_get_length ( buffer ) != 6 ||
        buffer->length != 10 ||
        strncmp ( ( char * ) talloc_buffer_get_read_point ( buffer ), "456789", 6 ) != 0
    ) {
        talloc_free ( buffer );
        return false;
    }

    if (
        talloc_buffer_readed ( buffer, 1 ) != 0 ||
        talloc_buffer_trim ( buffer ) != 0
    ) {
        talloc_free ( buffer );
        return false;
    }

    if (
        buffer->data_offset != 0 ||
        talloc_buffer_get_length ( buffer ) != 5 ||
        buffer->length != 5 ||
        strncmp ( ( char * ) talloc_buffer_get_read_point ( buffer ), "56789", 5 ) != 0
    ) {
        talloc_free ( buffer );
        return false;
    }

    return true;
}