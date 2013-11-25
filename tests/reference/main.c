// This file is part of talloc2. Copyright (C) 2013 Andrew Aladjev aladjev.andrew@gmail.com
// talloc2 is free software: you can redistribute it and/or modify it under the terms of the GNU General Public License as published by the Free Software Foundation, either version 3 of the License, or (at your option) any later version.
// talloc2 is distributed in the hope that it will be useful, but WITHOUT ANY WARRANTY; without even the implied warranty of MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU General Public License for more details.
// You should have received a copy of the GNU General Public License along with talloc2. If not, see <http://www.gnu.org/licenses/>.

#include <stdbool.h>
#include <talloc2/tree.h>
#include <talloc2/reference/main.h>

int main ()
{
    void * root = talloc_new ( NULL );
    if ( root == NULL ) {
        return 1;
    }

    int * a = talloc ( root, sizeof ( int ) );
    int * b = talloc ( root, sizeof ( int ) );
    int * c = talloc ( root, sizeof ( int ) );
    if ( a == NULL || b == NULL || c == NULL ) {
        talloc_free ( root );
        return 2;
    }
    
    int * common = talloc ( a, sizeof ( int ) );

    if (
        common == NULL ||
        talloc_add_reference ( NULL, common )   == 0 ||
        talloc_add_reference ( common, NULL )   == 0 ||
        talloc_add_reference ( NULL, NULL )     == 0 ||
        talloc_add_reference ( common, common ) == 0 ||
        talloc_add_reference ( b, common )      != 0 ||
        talloc_add_reference ( c, common )      != 0
    ) {
        talloc_free ( root );
        return 3;
    }

    if ( talloc_free ( root ) != 0 ) {
        return 4;
    }
    return 0;
}
