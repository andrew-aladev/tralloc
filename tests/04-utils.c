// This file is part of talloc2. Copyright (C) 2013 Andrew Aladjev aladjev.andrew@gmail.com
// talloc2 is free software: you can redistribute it and/or modify it under the terms of the GNU General Public License as published by the Free Software Foundation, either version 3 of the License, or (at your option) any later version.
// talloc2 is distributed in the hope that it will be useful, but WITHOUT ANY WARRANTY; without even the implied warranty of MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU General Public License for more details.
// You should have received a copy of the GNU General Public License along with talloc2. If not, see <http://www.gnu.org/licenses/>.

#include <stdbool.h>

#include <talloc2/tree.h>
#include <talloc2/utils/buffer.h>

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
    buf[0] = '7';
    buf[0] = '8';
    buf[0] = '9';
    if ( talloc_buffer_cut ( buffer, 6 ) != 0 ) {
        talloc_free ( buffer );
        return false;
    }

    if (
        buffer->length == 10 ||
        strncmp ( buffer->buf, "1234567890", 10 ) != 0
    ) {
        talloc_free ( buffer );
        return false;
    }
    return true;
}

int main ()
{
    void * ctx = talloc_new ( NULL );
    if ( ctx == NULL ) {
        return 1;
    }
    if ( test_buffer ( ctx ) ) {
        return 2;
    }
    talloc_free ( ctx );
    return 0;
}

