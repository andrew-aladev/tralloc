// This file is part of tralloc. Copyright (C) 2013 Andrew Aladjev aladjev.andrew@gmail.com
// tralloc is free software: you can redistribute it and/or modify it under the terms of the GNU General Lesser Public License as published by the Free Software Foundation, either version 3 of the License, or (at your option) any later version.
// tralloc is distributed in the hope that it will be useful, but WITHOUT ANY WARRANTY; without even the implied warranty of MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU General Lesser Public License for more details.
// You should have received a copy of the GNU General Lesser Public License along with tralloc. If not, see <http://www.gnu.org/licenses/>.

#include <tralloc/tree.h>

#if defined(TRALLOC_UTILS_BUFFER)
#   include <tralloc/utils/buffer.h>
#endif

#if defined(TRALLOC_DEBUG_STATS)
#   include <tralloc/debug/stats.h>
#endif

#include <string.h>


int main ()
{

#   if defined(TRALLOC_UTILS_BUFFER)

    tralloc_buffer * buffer;
    if ( tralloc_buffer_new ( NULL, &buffer, 3 ) != 0 ) {
        return 1;
    }
    if ( tralloc_buffer_prepare_write_length ( buffer, 7 ) != 0 ) {
        tralloc_free ( buffer );
        return 2;
    }
    uint8_t * data = tralloc_buffer_get_write_pointer ( buffer );
    data[0] = '0';
    data[1] = '1';
    data[2] = '2';
    data[3] = '3';
    if (
        tralloc_buffer_add_write_length ( buffer, 4 ) != 0 ||
        tralloc_buffer_add_read_length ( buffer, 2 )  != 0
    ) {
        tralloc_free ( buffer );
        return 2;
    }

    data = tralloc_buffer_get_read_pointer ( buffer );
    if (
        tralloc_buffer_get_read_length ( buffer ) != 2 ||
        tralloc_buffer_trim ( buffer )            != 0 ||
        strncmp ( ( char * ) data, "23", 2 )      != 0
    ) {
        tralloc_free ( buffer );
        return 3;
    }
    if ( tralloc_free ( buffer ) != 0 ) {
        return 4;
    }

#   endif

#   if defined(TRALLOC_DEBUG_STATS)
    size_t length;
    if ( tralloc_debug_stats_get_chunks_count ( &length ) != 0 || length != 0 ) {
        return 255;
    }
#   endif

    return 0;
}
