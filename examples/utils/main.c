// This file is part of tralloc. Copyright (C) 2013 Andrew Aladjev aladjev.andrew@gmail.com
// tralloc is free software: you can redistribute it and/or modify it under the terms of the GNU General Lesser Public License as published by the Free Software Foundation, either version 3 of the License, or (at your option) any later version.
// tralloc is distributed in the hope that it will be useful, but WITHOUT ANY WARRANTY; without even the implied warranty of MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU General Lesser Public License for more details.
// You should have received a copy of the GNU General Lesser Public License along with tralloc. If not, see <http://www.gnu.org/licenses/>.

#include <tralloc/tree.h>

#if defined(TRALLOC_UTILS_BUFFER)
#   include <tralloc/utils/buffer.h>
#endif

#if defined(TRALLOC_DEBUG)
#   include <tralloc/debug.h>
#endif

#include <string.h>


int main ()
{

#   if defined(TRALLOC_UTILS_BUFFER)
    tralloc_buffer * buffer;
    if ( tralloc_buffer_new ( NULL, &buffer ) != 0 ) {
        return 1;
    }
    if ( tralloc_buffer_prepare ( buffer, 7 ) != 0 ) {
        tralloc_free ( buffer );
        return 2;
    }
    uint8_t * buf = tralloc_buffer_get_write_point ( buffer );
    buf[0] = '0';
    buf[1] = '1';
    buf[2] = '2';
    buf[3] = '3';
    tralloc_buffer_written ( buffer, 4 );
    tralloc_buffer_readed ( buffer, 2 );
    if (
        tralloc_buffer_get_length ( buffer ) != 2 ||
        tralloc_buffer_trim ( buffer )       != 0 ||
        strncmp ( ( char * ) tralloc_buffer_get_read_point ( buffer ), "23", 2 ) != 0
    ) {
        tralloc_free ( buffer );
        return 3;
    }
    if ( tralloc_free ( buffer ) != 0 ) {
        return 4;
    }
#   endif

#   if defined(TRALLOC_DEBUG)
    size_t length;
    if ( tralloc_debug_get_chunks_count ( &length ) != 0 || length != 0 ) {
        return 255;
    }
#   endif

    return 0;
}
