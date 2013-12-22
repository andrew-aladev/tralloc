// This file is part of tralloc. Copyright (C) 2013 Andrew Aladjev aladjev.andrew@gmail.com
// tralloc is free software: you can redistribute it and/or modify it under the terms of the GNU General Public License as published by the Free Software Foundation, either version 3 of the License, or (at your option) any later version.
// tralloc is distributed in the hope that it will be useful, but WITHOUT ANY WARRANTY; without even the implied warranty of MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU General Public License for more details.
// You should have received a copy of the GNU General Public License along with tralloc. If not, see <http://www.gnu.org/licenses/>.

#include "str.h"

#if defined(TRALLOC_DEBUG)
#include <tralloc/events.h>
#endif

#if defined(TRALLOC_DESTRUCTOR)
#include "file.h"
#endif

int main ()
{
    tralloc_context * ctx = tralloc_new ( NULL );
    if ( ctx == NULL ) {
        return 1;
    }

    if ( !test_str ( ctx ) ) {
        tralloc_free ( ctx );
        return 2;
    }

#if defined(TRALLOC_DESTRUCTOR)
    if ( !test_file ( ctx ) ) {
        tralloc_free ( ctx );
        return 3;
    }
#endif

    if ( tralloc_free ( ctx ) != 0 ) {
        return 4;
    }

#if defined(TRALLOC_DEBUG)
    if (
        tralloc_get_chunks_count()           != 0 ||
        tralloc_get_chunks_overhead_length() != 0 ||
        tralloc_get_chunks_length()          != 0
    ) {
        return 5;
    }
#endif

    return 0;
}
