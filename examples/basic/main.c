// This file is part of tralloc. Copyright (C) 2013 Andrew Aladjev aladjev.andrew@gmail.com
// tralloc is free software: you can redistribute it and/or modify it under the terms of the GNU General Lesser Public License as published by the Free Software Foundation, either version 3 of the License, or (at your option) any later version.
// tralloc is distributed in the hope that it will be useful, but WITHOUT ANY WARRANTY; without even the implied warranty of MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU General Lesser Public License for more details.
// You should have received a copy of the GNU General Lesser Public License along with tralloc. If not, see <http://www.gnu.org/licenses/>.

#include <tralloc/tree.h>

#if defined(TRALLOC_DEBUG)
#   include <tralloc/debug.h>
#endif


int main ()
{
    tralloc_context * ctx;
    if ( tralloc_empty ( NULL, &ctx ) != 0 ) {
        return 1;
    }
    int16_t * numbers;
    char * string;
    if (
        tralloc_zero ( ctx, ( tralloc_context ** ) &numbers, sizeof ( int16_t ) * 5 ) != 0 ||
        tralloc ( ctx, ( tralloc_context ** ) &string, sizeof ( char ) * 10 ) != 0
    ) {
        tralloc_free ( ctx );
        return 2;
    }
    if ( tralloc_move ( string, numbers ) != 0 ) {
        tralloc_free ( ctx );
        return 3;
    }
    if (
        tralloc_realloc ( ( tralloc_context ** ) &numbers, sizeof ( int16_t ) * 3 ) != 0 ||
        tralloc_realloc ( ( tralloc_context ** ) &string,  sizeof ( char ) * 20 )   != 0
    ) {
        tralloc_free ( ctx );
        return 4;
    }
    if ( tralloc_free ( ctx ) != 0 ) {
        return 5;
    }

#   if defined(TRALLOC_DEBUG)
    size_t length;
    if ( tralloc_debug_get_chunks_count ( &length ) != 0 ) {
        return 255;
    }
#   endif

    return 0;
}
