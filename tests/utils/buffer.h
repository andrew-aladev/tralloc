// This file is part of talloc2. Copyright (C) 2013 Andrew Aladjev aladjev.andrew@gmail.com
// talloc2 is free software: you can redistribute it and/or modify it under the terms of the GNU General Public License as published by the Free Software Foundation, either version 3 of the License, or (at your option) any later version.
// talloc2 is distributed in the hope that it will be useful, but WITHOUT ANY WARRANTY; without even the implied warranty of MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU General Public License for more details.
// You should have received a copy of the GNU General Public License along with talloc2. If not, see <http://www.gnu.org/licenses/>.

#include <talloc2/utils/buffer.h>

bool test_buffer ( void * ctx )
{
    talloc_buffer * buffer = talloc_buffer_new ( ctx );
    if ( buffer == NULL ) {
        return false;
    }
    char * buf;

    if ( ( buf = talloc_buffer_get ( buffer, 9 ) ) == NULL ) {
        talloc_free ( buffer );
        return false;
    }
    buf[0] = '0';
    buf[1] = '1';
    buf[2] = '2';
    talloc_buffer_add ( buffer, 3 );

    if ( ( buf = talloc_buffer_get ( buffer, 3 ) ) == NULL ) {
        talloc_free ( buffer );
        return false;
    }
    buf[0] = '3';
    buf[1] = '4';
    talloc_buffer_add ( buffer, 2 );

    if ( ( buf = talloc_buffer_get ( buffer, 10 ) ) == NULL ) {
        talloc_free ( buffer );
        return false;
    }
    buf[0] = '5';
    talloc_buffer_add ( buffer, 1 );

    if (
        buffer->data_length != 6 ||
        buffer->length != 15
    ) {
        talloc_free ( buffer );
        return false;
    }

    if ( ( buf = talloc_buffer_get ( buffer, 4 ) ) == NULL ) {
        talloc_free ( buffer );
        return false;
    }
    buf[0] = '6';
    buf[1] = '7';
    buf[2] = '8';
    buf[3] = '9';
    talloc_buffer_add ( buffer, 4 );

    if ( talloc_buffer_trim ( buffer ) != 0 ) {
        talloc_free ( buffer );
        return false;
    }

    if (
        buffer->data_length != 10 ||
        buffer->length != 10 ||
        strncmp ( talloc_buffer_get_result ( buffer ), "0123456789", 10 ) != 0
    ) {
        talloc_free ( buffer );
        return false;
    }
    return true;
}