// This file is part of tralloc. Copyright (C) 2013 Andrew Aladjev aladjev.andrew@gmail.com
// tralloc is free software: you can redistribute it and/or modify it under the terms of the GNU General Public License as published by the Free Software Foundation, either version 3 of the License, or (at your option) any later version.
// tralloc is distributed in the hope that it will be useful, but WITHOUT ANY WARRANTY; without even the implied warranty of MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU General Public License for more details.
// You should have received a copy of the GNU General Public License along with tralloc. If not, see <http://www.gnu.org/licenses/>.

#include "free.h"
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
uint8_t on_free ( void * user_data, tralloc_chunk * chunk )
{
    malloc_dynarr * tralloc_history = ( malloc_dynarr * ) user_data;
    if ( malloc_dynarr_append ( tralloc_history, chunk ) != 0 ) {
        return 1;
    }
    return 0;
}

bool test_free ( const tralloc_context * root )
{
    malloc_dynarr * tralloc_history = malloc_history();
    if ( tralloc_history == NULL ) {
        return false;
    }
    _tralloc_set_callback ( NULL, NULL, NULL, on_free );

    int * a   = tralloc ( root, sizeof ( int ) * 2 );
    char * b  = tralloc ( root, sizeof ( char ) * 3 );
    float * c = tralloc ( a,    sizeof ( float ) * 4 );

    if ( a == NULL || b == NULL || c == NULL ) {
        tralloc_free ( a );
        tralloc_free ( b );
        free_history ( tralloc_history );
        return false;
    }

    tralloc_chunk * a_chunk = _tralloc_chunk_from_context ( a );
    tralloc_chunk * b_chunk = _tralloc_chunk_from_context ( b );
    tralloc_chunk * c_chunk = _tralloc_chunk_from_context ( c );

#if defined(TRALLOC_REFERENCE)
    void * c_reference = tralloc_add_reference ( c, b );
    if ( c_reference == NULL ) {
        tralloc_free ( a );
        tralloc_free ( b );
        free_history ( tralloc_history );
        return false;
    }
    tralloc_chunk * c_reference_chunk = _tralloc_chunk_from_context ( c_reference );
#endif

    if (
        tralloc_free ( a ) != 0 ||
        tralloc_free ( b ) != 0
    ) {
        free_history ( tralloc_history );
        return false;
    }

#if defined(TRALLOC_REFERENCE)
    tralloc_chunk * chunk;
    if (
        malloc_dynarr_get_length ( tralloc_history ) != 4 ||
        ( chunk = malloc_dynarr_get ( tralloc_history, 0 ) ) == NULL ||
        chunk != a_chunk ||
        ( chunk = malloc_dynarr_get ( tralloc_history, 1 ) ) == NULL ||
        chunk != b_chunk ||
        ( chunk = malloc_dynarr_get ( tralloc_history, 2 ) ) == NULL ||
        chunk != c_reference_chunk ||
        ( chunk = malloc_dynarr_get ( tralloc_history, 3 ) ) == NULL ||
        chunk != c_chunk
    ) {
        free_history ( tralloc_history );
        return false;
    }
#else
    tralloc_chunk * chunk;
    if (
        malloc_dynarr_get_length ( tralloc_history ) != 3 ||
        ( chunk = malloc_dynarr_get ( tralloc_history, 0 ) ) == NULL ||
        chunk != a_chunk ||
        ( chunk = malloc_dynarr_get ( tralloc_history, 1 ) ) == NULL ||
        chunk != c_chunk ||
        ( chunk = malloc_dynarr_get ( tralloc_history, 2 ) ) == NULL ||
        chunk != b_chunk
    ) {
        free_history ( tralloc_history );
        return false;
    }
#endif

    free_history ( tralloc_history );
    return true;
}