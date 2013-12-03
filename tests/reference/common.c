// This file is part of talloc2. Copyright (C) 2013 Andrew Aladjev aladjev.andrew@gmail.com
// talloc2 is free software: you can redistribute it and/or modify it under the terms of the GNU General Public License as published by the Free Software Foundation, either version 3 of the License, or (at your option) any later version.
// talloc2 is distributed in the hope that it will be useful, but WITHOUT ANY WARRANTY; without even the implied warranty of MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU General Public License for more details.
// You should have received a copy of the GNU General Public License along with talloc2. If not, see <http://www.gnu.org/licenses/>.

#include "common.h"
#include <talloc2/tree.h>
#include <talloc2/ext/chunk.h>
#include <talloc2/reference/main.h>

#include <string.h>

bool test_common ( void * root )
{
    int *    a = talloc ( root, sizeof ( int ) * 2 );
    char *   b = talloc ( a, sizeof ( char ) * 3 );
    size_t * c = talloc ( b, sizeof ( size_t ) * 4 );
    if ( a == NULL || b == NULL || c == NULL ) {
        talloc_free ( root );
        return false;
    }

    char * common = talloc ( NULL, sizeof ( char ) * 7 );
    strcpy ( common, "common" );

    char ** b_common = ( char ** ) talloc_add_reference ( b, common );
    char ** c_common = ( char ** ) talloc_add_reference ( c, common );

    if (
        b_common == NULL || * b_common != common ||
        c_common == NULL || * c_common != common
    ) {
        talloc_free ( a );
        return false;
    }

    if ( talloc_free ( a ) != 0 ) {
        return false;
    }

    return true;
}