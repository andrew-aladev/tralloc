// This file is part of tralloc. Copyright (C) 2013 Andrew Aladjev aladjev.andrew@gmail.com
// tralloc is free software: you can redistribute it and/or modify it under the terms of the GNU General Lesser Public License as published by the Free Software Foundation, either version 3 of the License, or (at your option) any later version.
// tralloc is distributed in the hope that it will be useful, but WITHOUT ANY WARRANTY; without even the implied warranty of MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU General Lesser Public License for more details.
// You should have received a copy of the GNU General Lesser Public License along with tralloc. If not, see <http://www.gnu.org/licenses/>.

#include <tralloc/tree.h>
#include <tralloc/length/main.h>

#if defined(TRALLOC_DEBUG)
#   include <tralloc/events.h>
#endif

int main ()
{
    tralloc_context * ctx;
    if ( tralloc_new ( NULL, &ctx ) != 0 ) {
        return 1;
    }
    uint16_t * data;
    size_t length;
    if (
        tralloc_with_extensions ( ctx, ( tralloc_context ** ) &data, TRALLOC_EXTENSION_LENGTH, sizeof ( uint16_t ) * 10 ) != 0 ||
        tralloc_get_length ( data, &length ) != 0 ||
        length != sizeof ( uint16_t ) * 10
    ) {
        tralloc_free ( ctx );
        return 3;
    }
    if ( tralloc_free ( ctx ) != 0 ) {
        return 2;
    }
    
#   if defined(TRALLOC_DEBUG)
    if ( tralloc_get_chunks_count() != 0 ) {
        return 3;
    }
#   endif

    return 0;
}
