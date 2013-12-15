// This file is part of talloc2. Copyright (C) 2013 Andrew Aladjev aladjev.andrew@gmail.com
// talloc2 is free software: you can redistribute it and/or modify it under the terms of the GNU General Lesser Public License as published by the Free Software Foundation, either version 3 of the License, or (at your option) any later version.
// talloc2 is distributed in the hope that it will be useful, but WITHOUT ANY WARRANTY; without even the implied warranty of MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU General Lesser Public License for more details.
// You should have received a copy of the GNU General Lesser Public License along with talloc2. If not, see <http://www.gnu.org/licenses/>.

#include <talloc2/helpers.h>

#if defined(TALLOC_DEBUG)
#include <talloc2/events.h>
#endif

int main ()
{
    talloc_context * root = talloc_new ( NULL );
    if ( root == NULL ) {
        return 1;
    }
    char * string = talloc_strdup ( root, "Some test text." );
    if ( string == NULL ) {
        talloc_free ( root );
        return 2;
    }
    char * text = talloc_strndup ( string, string + 10, 4 );
    if ( text == NULL || strcmp ( text, "text" ) != 0 ) {
        talloc_free ( root );
        return 3;
    }
    if ( talloc_free ( root ) != 0 ) {
        return 4;
    }

#if defined(TALLOC_DEBUG)
    if ( talloc_get_chunks_count() != 0 ) {
        return 5;
    }
#endif

    return 0;
}