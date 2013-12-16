// This file is part of tralloc. Copyright (C) 2013 Andrew Aladjev aladjev.andrew@gmail.com
// tralloc is free software: you can redistribute it and/or modify it under the terms of the GNU General Lesser Public License as published by the Free Software Foundation, either version 3 of the License, or (at your option) any later version.
// tralloc is distributed in the hope that it will be useful, but WITHOUT ANY WARRANTY; without even the implied warranty of MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU General Lesser Public License for more details.
// You should have received a copy of the GNU General Lesser Public License along with tralloc. If not, see <http://www.gnu.org/licenses/>.

#include <tralloc/tree.h>

#if defined(TRALLOC_DEBUG)
#include <tralloc/events.h>
#endif

int main ()
{
    tralloc_context * root = tralloc_new ( NULL );
    if ( root == NULL ) {
        return 1;
    }
    int16_t * numbers = tralloc_zero ( root, sizeof ( int16_t ) * 5 );
    char * string     = tralloc      ( root, sizeof ( char ) * 10 );
    if ( numbers == NULL || string == NULL ) {
        tralloc_free ( root );
        return 2;
    }
    if ( tralloc_move ( string, numbers ) != 0 ) {
        tralloc_free ( root );
        return 3;
    }
    numbers = tralloc_realloc ( numbers, sizeof ( int16_t ) * 3 );
    string  = tralloc_realloc ( string,  sizeof ( char ) * 20 );
    if ( numbers == NULL || string == NULL ) {
        tralloc_free ( root );
        return 4;
    }
    if ( tralloc_free ( root ) != 0 ) {
        return 5;
    }

#if defined(TRALLOC_DEBUG)
    if ( tralloc_get_chunks_count() != 0 ) {
        return 6;
    }
#endif

    return 0;
}