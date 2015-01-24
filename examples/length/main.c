// This file is part of tralloc. Copyright (C) 2013 Andrew Aladjev aladjev.andrew@gmail.com
// tralloc is free software: you can redistribute it and/or modify it under the terms of the GNU General Lesser Public License as published by the Free Software Foundation, either version 3 of the License, or (at your option) any later version.
// tralloc is distributed in the hope that it will be useful, but WITHOUT ANY WARRANTY; without even the implied warranty of MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU General Lesser Public License for more details.
// You should have received a copy of the GNU General Lesser Public License along with tralloc. If not, see <http://www.gnu.org/licenses/>.

#include <tralloc/tree/alloc.h>
#include <tralloc/tree/free.h>
#include <tralloc/length.h>
#include <tralloc/debug/stats/main.h>


int main ()
{
    tralloc_context * ctx;
    if ( tralloc_new_empty ( NULL, &ctx ) != 0 ) {
        return 1;
    }
    uint8_t * data;
    size_t data_length;
    if (
        tralloc_new_with_extensions ( ctx, ( tralloc_context ** ) &data, TRALLOC_EXTENSION_LENGTH, sizeof ( uint8_t ) * 10 ) != 0 ||
        tralloc_length_get ( data, &data_length ) != 0 || data_length != sizeof ( uint8_t ) * 10
    ) {
        tralloc_free ( ctx );
        return 3;
    }
    if ( tralloc_free ( ctx ) != 0 ) {
        return 2;
    }

#   if defined(TRALLOC_DEBUG_STATS)
    size_t chunks_count;
    if ( tralloc_debug_stats_get_chunks_count ( &chunks_count ) != 0 || chunks_count != 0 ) {
        return 255;
    }
#   endif

    return 0;
}
