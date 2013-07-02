// This file is part of talloc2. Copyright (C) 2013 Andrew Aladjev aladjev.andrew@gmail.com
// talloc2 is free software: you can redistribute it and/or modify it under the terms of the GNU General Public License as published by the Free Software Foundation, either version 3 of the License, or (at your option) any later version.
// talloc2 is distributed in the hope that it will be useful, but WITHOUT ANY WARRANTY; without even the implied warranty of MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU General Public License for more details.
// You should have received a copy of the GNU General Public License along with talloc2. If not, see <http://www.gnu.org/licenses/>.

#include <stdbool.h>

#include <talloc2/helpers.h>

#ifdef TALLOC_EXT_DESTRUCTOR
#include "destructor.h"
#endif

#ifdef TALLOC_EXT_LENGTH
#include "length.h"
#endif

#include "../lib/malloc_dynarr.h"

void * root;
malloc_dynarr * history;

bool init_data ()
{
    root = talloc_new ( NULL );
    if ( root == NULL ) {
        return false;
    }
    history = malloc_dynarr_new ( 16 );
    if ( history == NULL ) {
        talloc_free ( root );
        return false;
    }
    return true;
}

void free_data ()
{
    talloc_free ( root );
    if ( history != NULL ) {
        malloc_dynarr_free ( history );
    }
}

int main ()
{
    if ( !init_data () ) {
        return 1;
    }

#ifdef TALLOC_EXT_DESTRUCTOR
    if ( !test_destructor () ) {
        free_data ();
        return 2;
    }
#endif

#ifdef TALLOC_EXT_LENGTH
    if ( !test_length () ) {
        free_data ();
        return 3;
    }
#endif

    free_data ();
    return 0;
}