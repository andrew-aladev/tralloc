// This file is part of tralloc. Copyright (C) 2013 Andrew Aladjev aladjev.andrew@gmail.com
// tralloc is free software: you can redistribute it and/or modify it under the terms of the GNU General Public License as published by the Free Software Foundation, either version 3 of the License, or (at your option) any later version.
// tralloc is distributed in the hope that it will be useful, but WITHOUT ANY WARRANTY; without even the implied warranty of MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU General Public License for more details.
// You should have received a copy of the GNU General Public License along with tralloc. If not, see <http://www.gnu.org/licenses/>.

#include "common.h"
#include <tralloc/tree.h>
#include <tralloc/reference/head_chunk.h>
#include <tralloc/reference/main.h>

#include <string.h>


bool test_shared ( tralloc_context * ctx )
{
    int * a;
    char * b;
    size_t * c;
    char * shared;
    if (
        tralloc ( ctx, ( tralloc_context ** ) &a, sizeof ( int ) * 2 )    != 0 ||
        tralloc ( ctx, ( tralloc_context ** ) &b, sizeof ( char ) * 3 )   != 0 ||
        tralloc ( ctx, ( tralloc_context ** ) &c, sizeof ( size_t ) * 4 ) != 0 ||
        tralloc_with_extensions ( ctx, ( tralloc_context ** ) &shared, TRALLOC_HAVE_REFERENCES, sizeof ( char ) * 7 ) != 0
    ) {
        return false;
    }

    strcpy ( shared, "shared" );

    tralloc_context * empty;
    if (
        tralloc_reference_new ( NULL, shared, &empty )   == 0 ||
        tralloc_reference_new ( NULL, NULL, &empty )     == 0 ||
        tralloc_reference_new ( shared, shared, &empty ) == 0 ||
        tralloc_reference_new ( shared, ctx, &empty )    == 0
    ) {
        return false;
    }

    tralloc_context * b_shared;
    tralloc_context * c_shared;
    if (
        tralloc_reference_new ( shared, c, &b_shared ) != 0 ||
        tralloc_reference_new ( shared, b, &c_shared ) != 0
    ) {
        return false;
    }

    if (
        tralloc_move ( b_shared, NULL ) != 0 ||
        tralloc_move ( b_shared, b )    != 0 ||
        tralloc_move ( c_shared, c )    != 0
    ) {
        return false;
    }

    uint8_t * a_shared;
    if (
        tralloc_reference_zero ( shared, a, ( tralloc_context ** ) &a_shared, sizeof ( uint8_t ) * 3 ) != 0 ||
        a_shared[0] != 0 ||
        a_shared[1] != 0 ||
        a_shared[2] != 0
    ) {
        return false;
    }

    if ( tralloc_realloc ( ( tralloc_context ** ) &a_shared, sizeof ( uint8_t ) * 40 ) != 0 ) {
        return false;
    }

    if (
        tralloc_realloc ( ( tralloc_context ** ) &shared, sizeof ( char ) * 256 ) != 0 ||
        strncmp ( shared, "shared", 6 ) != 0
    ) {
        return false;
    }

    _tralloc_chunk * shared_chunk = _tralloc_chunk_from_context ( shared );
    if ( shared_chunk->extensions != TRALLOC_HAVE_REFERENCES ) {
        return false;
    }

    _tralloc_references * shared_references = _tralloc_references_from_chunk ( shared_chunk );
    _tralloc_chunk * a_chunk                = _tralloc_chunk_from_context ( a_shared );
    _tralloc_chunk * b_chunk                = _tralloc_chunk_from_context ( b_shared );
    _tralloc_chunk * c_chunk                = _tralloc_chunk_from_context ( c_shared );
    _tralloc_chunk * chunk                  = shared_references->first_reference;
    _tralloc_reference * reference          = _tralloc_reference_from_chunk ( chunk );
    if (
        chunk != a_chunk ||
        reference->prev != NULL || reference->references != shared_chunk ||
        ( chunk = reference->next ) != c_chunk || ( reference = _tralloc_reference_from_chunk ( chunk ) ) == NULL ||
        reference->prev != a_chunk || reference->references != shared_chunk ||
        ( chunk = reference->next ) != b_chunk || ( reference = _tralloc_reference_from_chunk ( chunk ) ) == NULL ||
        reference->prev != c_chunk || reference->references != shared_chunk ||
        reference->next != NULL
    ) {
        return false;
    }

    if (
        tralloc_clear_references ( shared ) != 0 ||
        tralloc_reference_new ( shared, a, ( tralloc_context ** ) &a_shared ) != 0
    ) {
        return false;
    }

    a_chunk = _tralloc_chunk_from_context ( a_shared );
    chunk   = shared_references->first_reference;
    if (
        chunk != a_chunk || ( reference = _tralloc_reference_from_chunk ( chunk ) ) == NULL ||
        reference->prev != NULL || reference->references != shared_chunk ||
        reference->next != NULL
    ) {
        return false;
    }

    if (
        tralloc_reference_new ( shared, b, &b_shared ) != 0 ||
        tralloc_reference_new ( shared, c, &c_shared ) != 0 ||
        tralloc_free ( a_shared ) != 0 ||
        tralloc_free ( c_shared ) != 0
    ) {
        return false;
    }

    b_chunk = _tralloc_chunk_from_context ( b_shared );
    chunk   = shared_references->first_reference;
    if (
        chunk != b_chunk || ( reference = _tralloc_reference_from_chunk ( chunk ) ) == NULL ||
        reference->prev != NULL || reference->references != shared_chunk ||
        reference->next != NULL
    ) {
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
