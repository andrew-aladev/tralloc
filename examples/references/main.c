// This file is part of tralloc. Copyright (C) 2013 Andrew Aladjev aladjev.andrew@gmail.com
// tralloc is free software: you can redistribute it and/or modify it under the terms of the GNU General Lesser Public License as published by the Free Software Foundation, either version 3 of the License, or (at your option) any later version.
// tralloc is distributed in the hope that it will be useful, but WITHOUT ANY WARRANTY; without even the implied warranty of MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU General Lesser Public License for more details.
// You should have received a copy of the GNU General Lesser Public License along with tralloc. If not, see <http://www.gnu.org/licenses/>.

#include <tralloc/tree.h>
#include <tralloc/helpers/string.h>
#include <tralloc/references.h>
#include <tralloc/debug.h>


int main ()
{
    char * common;
    if ( tralloc_strdup_with_extensions ( NULL, &common, TRALLOC_EXTENSION_REFERENCES, "big data" ) != 0 ) {
        return 1;
    }
    tralloc_context * common_reference_1;
    if ( tralloc_new_empty_with_extensions ( NULL, &common_reference_1, TRALLOC_EXTENSION_REFERENCE ) != 0 ) {
        tralloc_free ( common );
        return 2;
    }
    tralloc_context * common_reference_2;
    if ( tralloc_new_empty_with_extensions ( NULL, &common_reference_2, TRALLOC_EXTENSION_REFERENCE ) != 0 ) {
        tralloc_free ( common_reference_1 );
        tralloc_free ( common );
        return 3;
    }
    if (
        tralloc_move_reference ( common_reference_1, common ) != 0 ||
        tralloc_move_reference ( common_reference_2, common ) != 0
    ) {
        tralloc_free ( common_reference_1 );
        tralloc_free ( common_reference_2 );
        tralloc_free ( common );
        return 3;
    }
    if ( tralloc_free ( common ) != 0 ) {
        tralloc_free ( common_reference_1 );
        tralloc_free ( common_reference_2 );
        return 4;
    }
    if (
        strcmp ( common, "big data" ) != 0       ||
        tralloc_free ( common_reference_1 ) != 0 ||
        strcmp ( common, "big data" ) != 0       ||
        tralloc_free ( common_reference_2 ) != 0 // common will be freed here
    ) {
        return 5;
    }

#   if defined(TRALLOC_DEBUG_STATS)
    size_t length;
    if ( tralloc_debug_stats_get_chunks_count ( &length ) != 0 || length != 0 ) {
        return 255;
    }
#   endif

    return 0;
}
