// This file is part of tralloc. Copyright (C) 2013 Andrew Aladjev aladjev.andrew@gmail.com
// tralloc is free software: you can redistribute it and/or modify it under the terms of the GNU General Public License as published by the Free Software Foundation, either version 3 of the License, or (at your option) any later version.
// tralloc is distributed in the hope that it will be useful, but WITHOUT ANY WARRANTY; without even the implied warranty of MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU General Public License for more details.
// You should have received a copy of the GNU General Public License along with tralloc. If not, see <http://www.gnu.org/licenses/>.

#include "common.h"
#include "../lib/malloc_dynarr.h"
#include <tralloc/tree.h>
#include <tralloc/events.h>

#if defined(TRALLOC_REFERENCE)
#include <tralloc/reference/main.h>
#endif


static
malloc_dynarr * malloc_history()
{
    malloc_dynarr * tralloc_history = malloc_dynarr_new ( 8 );
    if ( tralloc_history == NULL ) {
        return NULL;
    }
    _tralloc_set_user_data ( tralloc_history );
    return tralloc_history;
}

static
void free_history ( malloc_dynarr * tralloc_history )
{
    malloc_dynarr_free ( tralloc_history );
}

static
tralloc_error on_add ( void * user_data, _tralloc_chunk * chunk )
{
    malloc_dynarr * tralloc_history = ( malloc_dynarr * ) user_data;
    if ( malloc_dynarr_append ( tralloc_history, chunk ) != 0 ) {
        return TRALLOC_ERROR_MALLOC_FAILED;
    }
    return 0;
}

bool test_add ( tralloc_context * ctx )
{
    malloc_dynarr * tralloc_history = malloc_history();
    if ( tralloc_history == NULL ) {
        return false;
    }
    _tralloc_set_callback ( on_add, NULL, NULL, NULL );

    int * a;
    char * b;
    if (
        tralloc ( ctx, ( tralloc_context ** ) &a, sizeof ( int ) * 2 )  != 0 ||
        tralloc ( ctx, ( tralloc_context ** ) &b, sizeof ( char ) * 3 ) != 0
    ) {
        free_history ( tralloc_history );
        return false;
    }

    float * c;

#if defined(TRALLOC_REFERENCE)
    if ( tralloc_with_extensions ( a, ( tralloc_context ** ) &c, TRALLOC_HAVE_REFERENCES, sizeof ( float ) * 4 ) != 0 ) {
        free_history ( tralloc_history );
        return false;
    }
#else
    if ( tralloc ( a, ( tralloc_context ** ) &c, sizeof ( float ) * 4 ) != 0 ) {
        free_history ( tralloc_history );
        return false;
    }
#endif

    _tralloc_chunk * a_chunk = _tralloc_chunk_from_context ( a );
    _tralloc_chunk * b_chunk = _tralloc_chunk_from_context ( b );
    _tralloc_chunk * c_chunk = _tralloc_chunk_from_context ( c );

#if defined(TRALLOC_REFERENCE)
    tralloc_context * c_reference;
    if ( tralloc_reference_new ( c, b, &c_reference ) != 0 ) {
        free_history ( tralloc_history );
        return false;
    }
    _tralloc_chunk * c_reference_chunk = _tralloc_chunk_from_context ( c_reference );

    double * d;
    if ( tralloc ( c_reference, ( tralloc_context ** ) &d, sizeof ( double ) * 2 ) != 0 ) {
        free_history ( tralloc_history );
        return false;
    }
    _tralloc_chunk * d_chunk = _tralloc_chunk_from_context ( d );

    _tralloc_chunk * chunk;
    if (
        malloc_dynarr_get_length ( tralloc_history ) != 5            ||
        ( chunk = malloc_dynarr_get ( tralloc_history, 0 ) ) == NULL ||
        chunk != a_chunk || chunk->length != sizeof ( int ) * 2      ||
        ( chunk = malloc_dynarr_get ( tralloc_history, 1 ) ) == NULL ||
        chunk != b_chunk || chunk->length != sizeof ( char ) * 3     ||
        ( chunk = malloc_dynarr_get ( tralloc_history, 2 ) ) == NULL ||
        chunk != c_chunk || chunk->length != sizeof ( float ) * 4    ||
        ( chunk = malloc_dynarr_get ( tralloc_history, 3 ) ) == NULL ||
        chunk != c_reference_chunk || chunk->length != 0             ||
        ( chunk = malloc_dynarr_get ( tralloc_history, 4 ) ) == NULL ||
        chunk != d_chunk || chunk->length != sizeof ( double ) * 2
    ) {
        free_history ( tralloc_history );
        return false;
    }
#else
    _tralloc_chunk * chunk;
    if (
        malloc_dynarr_get_length ( tralloc_history ) != 3            ||
        ( chunk = malloc_dynarr_get ( tralloc_history, 0 ) ) == NULL ||
        chunk != a_chunk || chunk->length != sizeof ( int ) * 2      ||
        ( chunk = malloc_dynarr_get ( tralloc_history, 1 ) ) == NULL ||
        chunk != b_chunk || chunk->length != sizeof ( char ) * 3     ||
        ( chunk = malloc_dynarr_get ( tralloc_history, 2 ) ) == NULL ||
        chunk != c_chunk || chunk->length != sizeof ( float ) * 4
    ) {
        free_history ( tralloc_history );
        return false;
    }
#endif

    if (
        tralloc_free ( a ) != 0 ||
        tralloc_free ( b ) != 0
    ) {
        return false;
    }

    free_history ( tralloc_history );
    return true;
}
