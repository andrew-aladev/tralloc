// This file is part of tralloc. Copyright (C) 2013 Andrew Aladjev aladjev.andrew@gmail.com
// tralloc is free software: you can redistribute it and/or modify it under the terms of the GNU General Public License as published by the Free Software Foundation, either version 3 of the License, or (at your option) any later version.
// tralloc is distributed in the hope that it will be useful, but WITHOUT ANY WARRANTY; without even the implied warranty of MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU General Public License for more details.
// You should have received a copy of the GNU General Public License along with tralloc. If not, see <http://www.gnu.org/licenses/>.

#include "add.h"
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
uint8_t on_add ( void * user_data, _tralloc_chunk * chunk )
{
    malloc_dynarr * tralloc_history = ( malloc_dynarr * ) user_data;
    if ( malloc_dynarr_append ( tralloc_history, chunk ) != 0 ) {
        return 1;
    }
    return 0;
}

bool test_add ( const tralloc_context * root )
{
    malloc_dynarr * tralloc_history = malloc_history();
    if ( tralloc_history == NULL ) {
        return false;
    }
    _tralloc_set_callback ( on_add, NULL, NULL, NULL );

    int * a   = tralloc ( root, sizeof ( int ) * 2 );
    char * b  = tralloc ( root, sizeof ( char ) * 3 );
    float * c = tralloc ( a,    sizeof ( float ) * 4 );

    if ( a == NULL || b == NULL || c == NULL ) {
        tralloc_free ( a );
        tralloc_free ( b );
        free_history ( tralloc_history );
        return false;
    }

    _tralloc_chunk * a_chunk = _tralloc_chunk_from_context ( a );
    _tralloc_chunk * b_chunk = _tralloc_chunk_from_context ( b );
    _tralloc_chunk * c_chunk = _tralloc_chunk_from_context ( c );

#if defined(TRALLOC_REFERENCE)
    void * c_reference = tralloc_reference ( c, b );
    if ( c_reference == NULL ) {
        tralloc_free ( a );
        tralloc_free ( b );
        free_history ( tralloc_history );
        return false;
    }
    _tralloc_chunk * c_reference_chunk = _tralloc_chunk_from_context ( c_reference );

    double * d = tralloc ( c_reference, sizeof ( double ) * 2 );
    if ( d == NULL ) {
        tralloc_free ( a );
        tralloc_free ( b );
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
        tralloc_free ( a );
        tralloc_free ( b );
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
        tralloc_free ( a );
        tralloc_free ( b );
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