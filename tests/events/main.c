// This file is part of talloc2. Copyright (C) 2013 Andrew Aladjev aladjev.andrew@gmail.com
// talloc2 is free software: you can redistribute it and/or modify it under the terms of the GNU General Public License as published by the Free Software Foundation, either version 3 of the License, or (at your option) any later version.
// talloc2 is distributed in the hope that it will be useful, but WITHOUT ANY WARRANTY; without even the implied warranty of MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU General Public License for more details.
// You should have received a copy of the GNU General Public License along with talloc2. If not, see <http://www.gnu.org/licenses/>.

#include "add.h"
#include "resize.h"
#include "move.h"
#include "free.h"

#include <talloc2/tree.h>
#include <talloc2/events.h>

int main ()
{
    talloc_context * root = talloc_new ( NULL );
    if ( root == NULL ) {
        return 1;
    }

    if ( !test_add ( root ) ) {
        talloc_free ( root );
        return 2;
    }

    if ( !test_resize ( root ) ) {
        talloc_free ( root );
        return 3;
    }

    if ( !test_move ( root ) ) {
        talloc_free ( root );
        return 4;
    }

    if ( !test_free ( root ) ) {
        talloc_free ( root );
        return 5;
    }

    if ( talloc_free ( root ) != 0 ) {
        return 6;
    }

    if (
        talloc_get_chunks_count()           != 0 ||
        talloc_get_chunks_overhead_length() != 0 ||
        talloc_get_chunks_length()          != 0
    ) {
        return 7;
    }

    return 0;
}
