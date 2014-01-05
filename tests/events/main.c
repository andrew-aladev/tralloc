// This file is part of tralloc. Copyright (C) 2013 Andrew Aladjev aladjev.andrew@gmail.com
// tralloc is free software: you can redistribute it and/or modify it under the terms of the GNU General Public License as published by the Free Software Foundation, either version 3 of the License, or (at your option) any later version.
// tralloc is distributed in the hope that it will be useful, but WITHOUT ANY WARRANTY; without even the implied warranty of MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU General Public License for more details.
// You should have received a copy of the GNU General Public License along with tralloc. If not, see <http://www.gnu.org/licenses/>.

#include "common.h"

#include <tralloc/tree.h>
#include <tralloc/events.h>


int main ()
{
    tralloc_context * ctx;
    if ( tralloc_new ( NULL, &ctx ) != 0 ) {
        return 1;
    }

    if ( !test_add ( ctx ) ) {
        tralloc_free ( ctx );
        return 2;
    }

    if ( !test_resize ( ctx ) ) {
        tralloc_free ( ctx );
        return 3;
    }

    if ( !test_move ( ctx ) ) {
        tralloc_free ( ctx );
        return 4;
    }

    if ( !test_free ( ctx ) ) {
        tralloc_free ( ctx );
        return 5;
    }

    if ( tralloc_free ( ctx ) != 0 ) {
        return 6;
    }

    if (
        tralloc_get_chunks_count()           != 0 ||
        tralloc_get_chunks_overhead_length() != 0 ||
        tralloc_get_chunks_length()          != 0
    ) {
        return 7;
    }

    return 0;
}
