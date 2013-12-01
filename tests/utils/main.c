// This file is part of talloc2. Copyright (C) 2013 Andrew Aladjev aladjev.andrew@gmail.com
// talloc2 is free software: you can redistribute it and/or modify it under the terms of the GNU General Public License as published by the Free Software Foundation, either version 3 of the License, or (at your option) any later version.
// talloc2 is distributed in the hope that it will be useful, but WITHOUT ANY WARRANTY; without even the implied warranty of MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU General Public License for more details.
// You should have received a copy of the GNU General Public License along with talloc2. If not, see <http://www.gnu.org/licenses/>.

#include <stdbool.h>

#include <talloc2/helpers.h>

#if defined(TALLOC_DEBUG)
#include <talloc2/events.h>
#endif

#if defined(TALLOC_UTILS_BUFFER)
#include "buffer.h"
#endif

#if defined(TALLOC_UTILS_DYNARR)
#include "dynarr.h"
#endif

#if defined(TALLOC_UTILS_LIST)
#include "list.h"
#endif

int main ()
{
    void * ctx = talloc_new ( NULL );
    if ( ctx == NULL ) {
        return 1;
    }

#if defined(TALLOC_UTILS_BUFFER)
    if ( !test_buffer ( ctx ) ) {
        talloc_free ( ctx );
        return 2;
    }
#endif

#if defined(TALLOC_UTILS_DYNARR)
    if ( !test_dynarr ( ctx ) ) {
        talloc_free ( ctx );
        return 3;
    }
#endif

#if defined(TALLOC_UTILS_LIST)
    if ( !test_list ( ctx ) ) {
        talloc_free ( ctx );
        return 4;
    }
#endif

    if ( talloc_free ( ctx ) != 0 ) {
        return 5;
    }

#if defined(TALLOC_DEBUG)
    // no memory leaks should be here
    if ( talloc_get_objects_count() != 0 ) {
        return 6;
    }
#endif

    return 0;
}

