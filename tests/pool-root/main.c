// This file is part of tralloc. Copyright (C) 2013 Andrew Aladjev aladjev.andrew@gmail.com
// tralloc is free software: you can redistribute it and/or modify it under the terms of the GNU General Public License as published by the Free Software Foundation, either version 3 of the License, or (at your option) any later version.
// tralloc is distributed in the hope that it will be useful, but WITHOUT ANY WARRANTY; without even the implied warranty of MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU General Public License for more details.
// You should have received a copy of the GNU General Public License along with tralloc. If not, see <http://www.gnu.org/licenses/>.

#include "../basic/common.h"
#include "../helpers/common.h"
#include "../pool/common.h"

#if defined(TRALLOC_DESTRUCTOR)
#include "../destructor/common.h"
#endif

#if defined(TRALLOC_DEBUG)
#include "../events/common.h"
#include <tralloc/events.h>
#endif

#if defined(TRALLOC_LENGTH)
#include "../length/common.h"
#endif

#if defined(TRALLOC_REFERENCE)
#include "../reference/common.h"
#endif

#if defined(TRALLOC_UTILS)
#include "../utils/common.h"
#endif

#include <tralloc/tree.h>


int main ()
{
    tralloc_context * pool_data;
    if ( tralloc_with_extensions ( NULL, &pool_data, TRALLOC_EXTENSION_POOL, sizeof ( uint8_t ) * 5000 ) != 0 ) {
        return 255;
    }

    int result = test_basic ( pool_data );
    if ( result != 0 ) {
        return result;
    }
    result = test_helpers ( pool_data );
    if ( result != 0 ) {
        return result;
    }
    result = test_pool ( pool_data );
    if ( result != 0 ) {
        return result;
    }

#if defined(TRALLOC_DESTRUCTOR)
    result = test_destructor ( pool_data );
    if ( result != 0 ) {
        return result;
    }
#endif

#if defined(TRALLOC_DEBUG)
    result = test_events ( pool_data );
    if ( result != 0 ) {
        return result;
    }
#endif

#if defined(TRALLOC_LENGTH)
    result = test_length ( pool_data );
    if ( result != 0 ) {
        return result;
    }
#endif

#if defined(TRALLOC_REFERENCE)
    result = test_reference ( pool_data );
    if ( result != 0 ) {
        return result;
    }
#endif

#if defined(TRALLOC_UTILS)
    result = test_utils ( pool_data );
    if ( result != 0 ) {
        return result;
    }
#endif

#if defined(TRALLOC_DEBUG)
    if (
        tralloc_get_chunks_count()           != 0 ||
        tralloc_get_chunks_overhead_length() != 0 ||
        tralloc_get_chunks_length()          != 0
    ) {
        return 256;
    }
#endif

    return 0;
}
