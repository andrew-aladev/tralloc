// This file is part of tralloc. Copyright (C) 2013 Andrew Aladjev aladjev.andrew@gmail.com
// tralloc is free software: you can redistribute it and/or modify it under the terms of the GNU General Public License as published by the Free Software Foundation, either version 3 of the License, or (at your option) any later version.
// tralloc is distributed in the hope that it will be useful, but WITHOUT ANY WARRANTY; without even the implied warranty of MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU General Public License for more details.
// You should have received a copy of the GNU General Public License along with tralloc. If not, see <http://www.gnu.org/licenses/>.

#include "common.h"
#include <tralloc/tree.h>
#include <tralloc/reference/head_chunk.h>
#include <tralloc/reference/main.h>

#if defined(TRALLOC_DESTRUCTOR)
#include <tralloc/destructor/main.h>
#endif

#include <string.h>


#if defined(TRALLOC_DESTRUCTOR)

tralloc_error empty_destructor ( tralloc_context * UNUSED ( chunk_context ), void * UNUSED ( user_data ) )
{
    return 0;
}

#endif

bool test_common ( tralloc_context * ctx )
{
    int * a;
    char * b;
    size_t * c;
    char * common;
    if (
        tralloc ( ctx, ( tralloc_context ** ) &a, sizeof ( int ) * 2 )    != 0 ||
        tralloc ( a,   ( tralloc_context ** ) &b, sizeof ( char ) * 3 )   != 0 ||
        tralloc ( b,   ( tralloc_context ** ) &c, sizeof ( size_t ) * 4 ) != 0 ||
        tralloc_with_extensions ( NULL, ( tralloc_context ** ) &common, TRALLOC_HAVE_REFERENCES, sizeof ( char ) * 7 ) != 0
    ) {
        return false;
    }

    strcpy ( common, "common" );

    tralloc_context * b_common;
    if ( tralloc_reference_new ( common, c, &b_common ) != 0 ) {
        return false;
    }

#if defined(TRALLOC_DESTRUCTOR)

    int16_t * c_common;
    if (
        tralloc_reference_with_extensions ( common, b, ( tralloc_context ** ) &c_common, TRALLOC_HAVE_DESTRUCTORS, sizeof ( int16_t ) * 2 ) != 0 ||
        tralloc_append_destructor ( c_common, empty_destructor, NULL ) != 0
    ) {
        return false;
    }

#else

    tralloc_context * c_common;
    if ( tralloc_reference_new ( common, b, &c_common ) != 0 ) {
        return false;
    }

#endif

    if (
        tralloc_realloc ( ( tralloc_context ** ) &common, sizeof ( char ) * 256 ) != 0 ||
        strncmp ( common, "common", 6 ) != 0
    ) {
        return false;
    }

    if (
        tralloc_move ( b_common, NULL ) != 0 ||
        tralloc_move ( b_common, b )    != 0 ||
        tralloc_move ( c_common, c )    != 0
    ) {
        return false;
    }

    if ( tralloc_realloc ( ( tralloc_context ** ) &c_common, sizeof ( uint8_t ) * 50 ) != 0 ) {
        return false;
    }

    _tralloc_chunk * common_chunk = _tralloc_chunk_from_context ( common );
    if ( common_chunk->extensions != TRALLOC_HAVE_REFERENCES ) {
        return false;
    }

    _tralloc_references * common_references = _tralloc_references_from_chunk ( common_chunk );
    _tralloc_chunk * b_chunk                = _tralloc_chunk_from_context ( b_common );
    _tralloc_chunk * c_chunk                = _tralloc_chunk_from_context ( c_common );
    _tralloc_chunk * chunk                  = common_references->first_reference;
    _tralloc_reference * reference          = _tralloc_reference_from_chunk ( chunk );

    if (
        chunk != c_chunk ||
        reference->prev != NULL || reference->references != common_chunk ||
        ( chunk = reference->next ) != b_chunk || ( reference = _tralloc_reference_from_chunk ( chunk ) ) == NULL ||
        reference->prev != c_chunk || reference->references != common_chunk ||
        reference->next != NULL
    ) {
        return false;
    }

    if ( tralloc_free ( a ) != 0 ) {
        return false;
    }

    return true;
}