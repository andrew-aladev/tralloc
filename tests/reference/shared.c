// This file is part of tralloc. Copyright (C) 2013 Andrew Aladjev aladjev.andrew@gmail.com
// tralloc is free software: you can redistribute it and/or modify it under the terms of the GNU General Public License as published by the Free Software Foundation, either version 3 of the License, or (at your option) any later version.
// tralloc is distributed in the hope that it will be useful, but WITHOUT ANY WARRANTY; without even the implied warranty of MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU General Public License for more details.
// You should have received a copy of the GNU General Public License along with tralloc. If not, see <http://www.gnu.org/licenses/>.

#include "shared.h"
#include <tralloc/tree.h>
#include <tralloc/reference/head_chunk.h>
#include <tralloc/reference/main.h>

#include <string.h>

bool test_shared ( const tralloc_context * root )
{
    int *    a = tralloc ( root, sizeof ( int ) * 2 );
    char *   b = tralloc ( root, sizeof ( char ) * 3 );
    size_t * c = tralloc ( root, sizeof ( size_t ) * 4 );
    if ( a == NULL || b == NULL || c == NULL ) {
        return false;
    }

    char * shared = tralloc ( root, sizeof ( char ) * 7 );
    strcpy ( shared, "shared" );

    if (
        shared == NULL ||
        tralloc_reference ( NULL, shared )   != NULL ||
        tralloc_reference ( NULL, NULL )     != NULL ||
        tralloc_reference ( shared, shared ) != NULL ||
        tralloc_reference ( shared, root )   != NULL
    ) {
        tralloc_free ( a );
        tralloc_free ( b );
        tralloc_free ( c );
        return false;
    }

    void * b_shared = tralloc_reference ( shared, c );
    void * c_shared = tralloc_reference ( shared, b );
    if ( b_shared == NULL || c_shared == NULL ) {
        tralloc_free ( a );
        tralloc_free ( b );
        tralloc_free ( c );
        return false;
    }
    
    if (
        tralloc_move ( b_shared, NULL ) != 0 ||
        tralloc_move ( b_shared, b )    != 0 ||
        tralloc_move ( c_shared, c )    != 0
    ) {
        tralloc_free ( a );
        tralloc_free ( b );
        tralloc_free ( c );
        return false;
    }

    void * a_shared = tralloc_reference ( shared, a );
    if ( a_shared == NULL ) {
        tralloc_free ( a );
        tralloc_free ( b );
        tralloc_free ( c );
        return false;
    }
    
    a_shared = tralloc_realloc ( a_shared, sizeof ( uint8_t ) * 40 );
    if ( a_shared == NULL ) {
        tralloc_free ( a );
        tralloc_free ( b );
        tralloc_free ( c );
        return false;
    }

    shared = tralloc_realloc ( shared, sizeof ( char ) * 256 );
    if ( shared == NULL || strncmp ( shared, "shared", 6 ) != 0 ) {
        tralloc_free ( a );
        tralloc_free ( b );
        tralloc_free ( c );
        return false;
    }

    _tralloc_chunk * shared_chunk = _tralloc_chunk_from_context ( shared );
    if ( shared_chunk->mode != TRALLOC_MODE_REFERENCES ) {
        tralloc_free ( a );
        tralloc_free ( b );
        tralloc_free ( c );
        return false;
    }

    _tralloc_references * shared_references = _tralloc_references_from_chunk ( shared_chunk );
    _tralloc_reference * a_reference        = _tralloc_reference_from_chunk ( _tralloc_chunk_from_context ( a_shared ) );
    _tralloc_reference * b_reference        = _tralloc_reference_from_chunk ( _tralloc_chunk_from_context ( b_shared ) );
    _tralloc_reference * c_reference        = _tralloc_reference_from_chunk ( _tralloc_chunk_from_context ( c_shared ) );
    _tralloc_reference * reference          = shared_references->first_reference;
    if (
        reference != a_reference ||
        reference->prev != NULL || reference->references != shared_references ||
        ( reference = reference->next ) != c_reference ||
        reference->prev != a_reference || reference->references != shared_references ||
        ( reference = reference->next ) != b_reference ||
        reference->prev != c_reference || reference->references != shared_references ||
        ( reference = reference->next ) != NULL
    ) {
        tralloc_free ( a );
        tralloc_free ( b );
        tralloc_free ( c );
        return false;
    }

    if ( tralloc_clear_references ( shared ) != 0 ) {
        tralloc_free ( a );
        tralloc_free ( b );
        tralloc_free ( c );
        return false;
    }

    a_shared = tralloc_reference ( shared, a );

    a_reference = _tralloc_reference_from_chunk ( _tralloc_chunk_from_context ( a_shared ) );
    reference   = shared_references->first_reference;
    if (
        reference != a_reference ||
        reference->prev != NULL || reference->references != shared_references || reference->next != NULL
    ) {
        tralloc_free ( a );
        tralloc_free ( b );
        tralloc_free ( c );
        return false;
    }

    b_shared = tralloc_reference ( shared, b );
    c_shared = tralloc_reference ( shared, c );

    if (
        tralloc_free ( a_shared ) != 0 ||
        tralloc_free ( c_shared ) != 0
    ) {
        tralloc_free ( a );
        tralloc_free ( b );
        tralloc_free ( c );
        return false;
    }

    b_reference = _tralloc_reference_from_chunk ( _tralloc_chunk_from_context ( b_shared ) );
    reference   = shared_references->first_reference;
    if (
        reference != b_reference ||
        reference->prev != NULL || reference->references != shared_references || reference->next != NULL
    ) {
        tralloc_free ( a );
        tralloc_free ( b );
        tralloc_free ( c );
        return false;
    }

    if (
        tralloc_free ( a ) != 0 ||
        tralloc_free ( b ) != 0 ||
        tralloc_free ( c ) != 0
    ) {
        return false;
    }

    return true;
}