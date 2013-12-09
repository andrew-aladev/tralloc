// This file is part of talloc2. Copyright (C) 2013 Andrew Aladjev aladjev.andrew@gmail.com
// talloc2 is free software: you can redistribute it and/or modify it under the terms of the GNU General Public License as published by the Free Software Foundation, either version 3 of the License, or (at your option) any later version.
// talloc2 is distributed in the hope that it will be useful, but WITHOUT ANY WARRANTY; without even the implied warranty of MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU General Public License for more details.
// You should have received a copy of the GNU General Public License along with talloc2. If not, see <http://www.gnu.org/licenses/>.

#include <talloc2/tree.h>

#if defined(TALLOC_DEBUG)
#include <talloc2/events.h>
#endif

#if defined(TALLOC_DESTRUCTOR)
#include "destructor.h"
#endif

int main ()
{
    void * root = talloc_new ( NULL );
    if ( root == NULL ) {
        return 1;
    }

#if defined(TALLOC_DESTRUCTOR)
    if ( !test_destructor (root) ) {
        talloc_free ( root );
        return 2;
    }
#endif

    if ( talloc_free ( root ) != 0 ) {
        return 3;
    }

#if defined(TALLOC_DEBUG)
    if (
        talloc_get_chunks_count()           != 0 ||
        talloc_get_chunks_overhead_length() != 0 ||
        talloc_get_chunks_length()          != 0
    ) {
        return 4;
    }
#endif

    return 0;
}