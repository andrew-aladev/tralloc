// This file is part of talloc2. Copyright (C) 2013 Andrew Aladjev aladjev.andrew@gmail.com
// talloc2 is free software: you can redistribute it and/or modify it under the terms of the GNU General Public License as published by the Free Software Foundation, either version 3 of the License, or (at your option) any later version.
// talloc2 is distributed in the hope that it will be useful, but WITHOUT ANY WARRANTY; without even the implied warranty of MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU General Public License for more details.
// You should have received a copy of the GNU General Public License along with talloc2. If not, see <http://www.gnu.org/licenses/>.

#include "common.h"
#include <talloc2/tree.h>
#include <talloc2/extensions/chunk.h>
#include <talloc2/reference/main.h>

#include <string.h>

bool test_common ( void * root )
{
    int *    a = talloc ( root, sizeof ( int ) * 2 );
    char *   b = talloc ( a, sizeof ( char ) * 3 );
    size_t * c = talloc ( b, sizeof ( size_t ) * 4 );
    if ( a == NULL || b == NULL || c == NULL ) {
        return false;
    }

    char * common = talloc ( NULL, sizeof ( char ) * 7 );
    strcpy ( common, "common" );

    char ** b_common = ( char ** ) talloc_add_reference ( common, b );
    char ** c_common = ( char ** ) talloc_add_reference ( common, c );

    if (
        b_common == NULL || * b_common != common ||
        c_common == NULL || * c_common != common
    ) {
        talloc_free ( a );
        return false;
    }

    common = talloc_realloc ( common, sizeof ( char ) * 256 );
    if ( common == NULL || strncmp ( common, "common", 6 ) != 0 ) {
        talloc_free ( a );
        return false;
    }

    talloc_chunk * common_chunk = talloc_chunk_from_data ( common );
    if ( common_chunk->mode != TALLOC_MODE_EXTENSIONS ) {
        talloc_free ( a );
        return false;
    }

    talloc_extensions * common_extensions = talloc_extensions_from_chunk ( common_chunk );
    talloc_reference * b_reference        = talloc_reference_from_chunk ( talloc_chunk_from_data ( b_common ) );
    talloc_reference * c_reference        = talloc_reference_from_chunk ( talloc_chunk_from_data ( c_common ) );
    talloc_reference * reference          = common_extensions->first_reference;

    if (
        reference != c_reference ||
        reference->prev != NULL || reference->parent_extensions != common_extensions ||
        ( reference = reference->next ) != b_reference ||
        reference->prev != c_reference || reference->parent_extensions != common_extensions ||
        ( reference = reference->next ) != NULL
    ) {
        talloc_free ( a );
        return false;
    }

    if ( talloc_free ( a ) != 0 ) {
        return false;
    }

    return true;
}