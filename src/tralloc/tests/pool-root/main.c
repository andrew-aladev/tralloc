// This file is part of tralloc. Copyright (C) 2013 Andrew Aladjev aladjev.andrew@gmail.com
// tralloc is free software: you can redistribute it and/or modify it under the terms of the GNU General Public License as published by the Free Software Foundation, either version 3 of the License, or (at your option) any later version.
// tralloc is distributed in the hope that it will be useful, but WITHOUT ANY WARRANTY; without even the implied warranty of MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU General Public License for more details.
// You should have received a copy of the GNU General Public License along with tralloc. If not, see <http://www.gnu.org/licenses/>.

#include <tralloc/tests/tree/common.h>
#include <tralloc/tests/helpers/common.h>
#include <tralloc/tests/pool/common.h>

#if defined ( TRALLOC_DESTRUCTORS )
#   include <tralloc/tests/destructors/common.h>
#endif

#if defined ( TRALLOC_DEBUG_CALLBACKS )
#   include <tralloc/tests/debug/common.h>
#endif

#if defined ( TRALLOC_LENGTH )
#   include <tralloc/tests/length/common.h>
#endif

#if defined ( TRALLOC_REFERENCES )
#   include <tralloc/tests/references/common.h>
#endif

#if defined ( TRALLOC_UTILS )
#   include <tralloc/tests/utils/common.h>
#endif

#include <tralloc/tree.h>
#include <tralloc/debug.h>


int main ()
{
    tralloc_context * pool_data;
    if ( tralloc_new_with_extensions ( NULL, &pool_data, sizeof ( uint8_t ) * 5000, TRALLOC_EXTENSION_POOL ) != 0 ) {
        return 253;
    }

    int result = test_tree ( pool_data );
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

#   if defined ( TRALLOC_DESTRUCTORS )
    result = test_destructors ( pool_data );
    if ( result != 0 ) {
        return result;
    }
#   endif

#   if defined ( TRALLOC_DEBUG_CALLBACKS )
    result = test_debug ( pool_data );
    if ( result != 0 ) {
        return result;
    }
#   endif

#   if defined ( TRALLOC_LENGTH )
    result = test_length ( pool_data );
    if ( result != 0 ) {
        return result;
    }
#   endif

#   if defined ( TRALLOC_REFERENCES )
    result = test_references ( pool_data );
    if ( result != 0 ) {
        return result;
    }
#   endif

#   if defined ( TRALLOC_UTILS )
    result = test_utils ( pool_data );
    if ( result != 0 ) {
        return result;
    }
#   endif

    if ( tralloc_free ( pool_data ) != 0 ) {
        return 254;
    }

#   if defined ( TRALLOC_DEBUG_STATS )
    size_t chunks_count, overhead_length, length;
    if (
        tralloc_debug_stats_get_chunks_count    ( &chunks_count )    != 0 || chunks_count    != 0 ||
        tralloc_debug_stats_get_overhead_length ( &overhead_length ) != 0 || overhead_length != 0 ||
        tralloc_debug_stats_get_length          ( &length )          != 0 || length          != 0
    ) {
        return 255;
    }
#   endif

    return 0;
}
