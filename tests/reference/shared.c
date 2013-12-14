// This file is part of talloc2. Copyright (C) 2013 Andrew Aladjev aladjev.andrew@gmail.com
// talloc2 is free software: you can redistribute it and/or modify it under the terms of the GNU General Public License as published by the Free Software Foundation, either version 3 of the License, or (at your option) any later version.
// talloc2 is distributed in the hope that it will be useful, but WITHOUT ANY WARRANTY; without even the implied warranty of MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU General Public License for more details.
// You should have received a copy of the GNU General Public License along with talloc2. If not, see <http://www.gnu.org/licenses/>.

#include "shared.h"
#include <talloc2/tree.h>
#include <talloc2/extensions/chunk.h>
#include <talloc2/reference/main.h>

#include <string.h>

bool test_shared ( const talloc_context * root )
{
    int *    a = talloc ( root, sizeof ( int ) * 2 );
    char *   b = talloc ( root, sizeof ( char ) * 3 );
    size_t * c = talloc ( root, sizeof ( size_t ) * 4 );
    if ( a == NULL || b == NULL || c == NULL ) {
        return false;
    }

    char * shared = talloc ( root, sizeof ( char ) * 7 );
    strcpy ( shared, "shared" );

    if (
        shared == NULL ||
        talloc_add_reference ( NULL, shared )   != NULL ||
        talloc_add_reference ( shared, NULL )   != NULL ||
        talloc_add_reference ( NULL, NULL )     != NULL ||
        talloc_add_reference ( shared, shared ) != NULL ||
        talloc_add_reference ( shared, root )   != NULL
    ) {
        talloc_free ( a );
        talloc_free ( b );
        talloc_free ( c );
        return false;
    }

    void * b_shared = talloc_add_reference ( shared, c );
    void * c_shared = talloc_add_reference ( shared, b );
    if ( b_shared == NULL || c_shared == NULL ) {
        talloc_free ( a );
        talloc_free ( b );
        talloc_free ( c );
        return false;
    }
    
    if (
        talloc_move ( b_shared, NULL ) != 0 ||
        talloc_move ( b_shared, b )    != 0 ||
        talloc_move ( c_shared, c )    != 0
    ) {
        talloc_free ( a );
        talloc_free ( b );
        talloc_free ( c );
        return false;
    }

    void * a_shared = talloc_add_reference ( shared, a );
    if ( a_shared == NULL ) {
        talloc_free ( a );
        talloc_free ( b );
        talloc_free ( c );
        return false;
    }
    
    a_shared = talloc_realloc ( a_shared, sizeof ( uint8_t ) * 40 );
    if ( a_shared == NULL ) {
        talloc_free ( a );
        talloc_free ( b );
        talloc_free ( c );
        return false;
    }

    shared = talloc_realloc ( shared, sizeof ( char ) * 256 );
    if ( shared == NULL || strncmp ( shared, "shared", 6 ) != 0 ) {
        talloc_free ( a );
        talloc_free ( b );
        talloc_free ( c );
        return false;
    }

    talloc_chunk * shared_chunk = talloc_chunk_from_context ( shared );
    if ( shared_chunk->mode != TALLOC_MODE_EXTENSIONS ) {
        talloc_free ( a );
        talloc_free ( b );
        talloc_free ( c );
        return false;
    }

    talloc_extensions * shared_extensions = talloc_extensions_from_chunk ( shared_chunk );
    talloc_reference * a_reference        = talloc_reference_from_chunk ( talloc_chunk_from_context ( a_shared ) );
    talloc_reference * b_reference        = talloc_reference_from_chunk ( talloc_chunk_from_context ( b_shared ) );
    talloc_reference * c_reference        = talloc_reference_from_chunk ( talloc_chunk_from_context ( c_shared ) );
    talloc_reference * reference          = shared_extensions->first_reference;
    if (
        reference != a_reference ||
        reference->prev != NULL || reference->parent_extensions != shared_extensions ||
        ( reference = reference->next ) != c_reference ||
        reference->prev != a_reference || reference->parent_extensions != shared_extensions ||
        ( reference = reference->next ) != b_reference ||
        reference->prev != c_reference || reference->parent_extensions != shared_extensions ||
        ( reference = reference->next ) != NULL
    ) {
        talloc_free ( a );
        talloc_free ( b );
        talloc_free ( c );
        return false;
    }

    if ( talloc_clear_references ( shared ) != 0 ) {
        talloc_free ( a );
        talloc_free ( b );
        talloc_free ( c );
        return false;
    }

    a_shared = talloc_add_reference ( shared, a );

    a_reference = talloc_reference_from_chunk ( talloc_chunk_from_context ( a_shared ) );
    reference   = shared_extensions->first_reference;
    if (
        reference != a_reference ||
        reference->prev != NULL || reference->parent_extensions != shared_extensions || reference->next != NULL
    ) {
        talloc_free ( a );
        talloc_free ( b );
        talloc_free ( c );
        return false;
    }

    b_shared = talloc_add_reference ( shared, b );
    c_shared = talloc_add_reference ( shared, c );

    if (
        talloc_free ( a_shared ) != 0 ||
        talloc_free ( c_shared ) != 0
    ) {
        talloc_free ( a );
        talloc_free ( b );
        talloc_free ( c );
        return false;
    }

    b_reference = talloc_reference_from_chunk ( talloc_chunk_from_context ( b_shared ) );
    reference   = shared_extensions->first_reference;
    if (
        reference != b_reference ||
        reference->prev != NULL || reference->parent_extensions != shared_extensions || reference->next != NULL
    ) {
        talloc_free ( a );
        talloc_free ( b );
        talloc_free ( c );
        return false;
    }

    if (
        talloc_free ( a ) != 0 ||
        talloc_free ( b ) != 0 ||
        talloc_free ( c ) != 0
    ) {
        return false;
    }

    return true;
}