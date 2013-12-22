// This file is part of tralloc. Copyright (C) 2013 Andrew Aladjev aladjev.andrew@gmail.com
// tralloc is free software: you can redistribute it and/or modify it under the terms of the GNU General Public License as published by the Free Software Foundation, either version 3 of the License, or (at your option) any later version.
// tralloc is distributed in the hope that it will be useful, but WITHOUT ANY WARRANTY; without even the implied warranty of MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU General Public License for more details.
// You should have received a copy of the GNU General Public License along with tralloc. If not, see <http://www.gnu.org/licenses/>.

#include "common.h"
#include <tralloc/tree.h>
#include <tralloc/extensions/chunk.h>
#include <tralloc/reference/main.h>

#include <string.h>

bool test_common ( const tralloc_context * root )
{
    int *    a = tralloc ( root, sizeof ( int ) * 2 );
    char *   b = tralloc ( a, sizeof ( char ) * 3 );
    size_t * c = tralloc ( b, sizeof ( size_t ) * 4 );
    if ( a == NULL || b == NULL || c == NULL ) {
        return false;
    }

    char * common = tralloc ( NULL, sizeof ( char ) * 7 );
    strcpy ( common, "common" );

    void * b_common = tralloc_add_reference ( common, c );
    void * c_common = tralloc_add_reference ( common, b );
    if ( b_common == NULL || c_common == NULL ) {
        tralloc_free ( a );
        return false;
    }

    common = tralloc_realloc ( common, sizeof ( char ) * 256 );
    if ( common == NULL || strncmp ( common, "common", 6 ) != 0 ) {
        tralloc_free ( a );
        return false;
    }

    if (
        tralloc_move ( b_common, NULL ) != 0 ||
        tralloc_move ( b_common, b )    != 0 ||
        tralloc_move ( c_common, c )    != 0
    ) {
        tralloc_free ( a );
        return false;
    }

    c_common = tralloc_realloc ( c_common, sizeof ( uint8_t ) * 50 );
    if ( c_common == NULL ) {
        tralloc_free ( a );
        return false;
    }

    tralloc_chunk * common_chunk = _tralloc_chunk_from_context ( common );
    if ( common_chunk->mode != TRALLOC_MODE_EXTENSIONS ) {
        tralloc_free ( a );
        return false;
    }

    tralloc_extensions * common_extensions = _tralloc_extensions_from_chunk ( common_chunk );
    tralloc_reference * b_reference        = _tralloc_reference_from_chunk ( _tralloc_chunk_from_context ( b_common ) );
    tralloc_reference * c_reference        = _tralloc_reference_from_chunk ( _tralloc_chunk_from_context ( c_common ) );
    tralloc_reference * reference          = common_extensions->first_reference;

    if (
        reference != c_reference ||
        reference->prev != NULL || reference->parent_extensions != common_extensions ||
        ( reference = reference->next ) != b_reference ||
        reference->prev != c_reference || reference->parent_extensions != common_extensions ||
        ( reference = reference->next ) != NULL
    ) {
        tralloc_free ( a );
        return false;
    }

    if ( tralloc_free ( a ) != 0 ) {
        return false;
    }

    return true;
}