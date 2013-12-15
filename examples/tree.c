// This file is part of talloc2. Copyright (C) 2013 Andrew Aladjev aladjev.andrew@gmail.com
// talloc2 is free software: you can redistribute it and/or modify it under the terms of the GNU General Lesser Public License as published by the Free Software Foundation, either version 3 of the License, or (at your option) any later version.
// talloc2 is distributed in the hope that it will be useful, but WITHOUT ANY WARRANTY; without even the implied warranty of MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU General Lesser Public License for more details.
// You should have received a copy of the GNU General Lesser Public License along with talloc2. If not, see <http://www.gnu.org/licenses/>.

#include <talloc2/tree.h>

#if defined(TALLOC_DEBUG)
#include <talloc2/events.h>
#endif

int main ()
{
    talloc_context * root = talloc_new ( NULL );
    if ( root == NULL ) {
        return 1;
    }
    int16_t * numbers = talloc_zero ( root, sizeof ( int16_t ) * 5 );
    char * string     = talloc      ( root, sizeof ( char ) * 10 );
    if ( numbers == NULL || string == NULL ) {
        talloc_free ( root );
        return 2;
    }
    if ( talloc_move ( string, numbers ) != 0 ) {
        talloc_free ( root );
        return 3;
    }
    numbers = talloc_realloc ( numbers, sizeof ( int16_t ) * 3 );
    string  = talloc_realloc ( string,  sizeof ( char ) * 20 );
    if ( numbers == NULL || string == NULL ) {
        talloc_free ( root );
        return 4;
    }
    if ( talloc_free ( root ) != 0 ) {
        return 5;
    }

#if defined(TALLOC_DEBUG)
    if ( talloc_get_chunks_count() != 0 ) {
        return 6;
    }
#endif

    return 0;
}