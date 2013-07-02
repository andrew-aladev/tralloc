// This file is part of talloc2. Copyright (C) 2013 Andrew Aladjev aladjev.andrew@gmail.com
// talloc2 is free software: you can redistribute it and/or modify it under the terms of the GNU General Public License as published by the Free Software Foundation, either version 3 of the License, or (at your option) any later version.
// talloc2 is distributed in the hope that it will be useful, but WITHOUT ANY WARRANTY; without even the implied warranty of MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU General Public License for more details.
// You should have received a copy of the GNU General Public License along with talloc2. If not, see <http://www.gnu.org/licenses/>.

#include <talloc2/ext/length.h>

extern void * root;

bool test_length ()
{
    size_t length;
    char * str = talloc_zero_mode ( root, sizeof ( char ) * 200, TALLOC_MODE_LENGTH );
    if ( str == NULL ) {
        return false;
    }
    if ( talloc_get_length ( str, &length ) != 0 || length != 200 ) {
        return false;
    }
    str = talloc_realloc ( str, sizeof ( char ) * 300 );
    if ( str == NULL ) {
        return false;
    }
    if ( talloc_get_length ( str, &length ) != 0 || length != 300 ) {
        return false;
    }
    str = talloc_realloc ( str, sizeof ( char ) * 3 );
    if ( str == NULL ) {
        return false;
    }
    if ( talloc_get_length ( str, &length ) != 0 || length != 3 ) {
        return false;
    }

    str[0] = 'a';
    str[1] = 'b';
    str[2] = '\0';

    if ( talloc_free ( str ) != 0 ) {
        return false;
    }

    return true;
}