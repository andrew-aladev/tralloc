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


typedef struct move_info_t {
    _tralloc_chunk * chunk;
    _tralloc_chunk * old_parent;
} move_info;

static
malloc_dynarr * malloc_history()
{
    malloc_dynarr * tralloc_history = malloc_dynarr_new ( 8 );
    if ( tralloc_history == NULL ) {
        return NULL;
    }
    malloc_dynarr_set_free_item ( tralloc_history, free );
    _tralloc_set_user_data ( tralloc_history );
    return tralloc_history;
}

static
void free_history ( malloc_dynarr * tralloc_history )
{
    malloc_dynarr_free ( tralloc_history );
}

static
uint8_t on_move ( void * user_data, _tralloc_chunk * chunk, _tralloc_chunk * old_parent )
{
    malloc_dynarr * tralloc_history = ( malloc_dynarr * ) user_data;
    move_info * info = malloc ( sizeof ( move_info ) );
    if ( info == NULL ) {
        return 1;
    }
    info->chunk      = chunk;
    info->old_parent = old_parent;
    if ( malloc_dynarr_append ( tralloc_history, info ) != 0 ) {
        free ( info );
        return 2;
    }
    return 0;
}

bool test_move ( tralloc_context * ctx )
{
    malloc_dynarr * tralloc_history = malloc_history();
    if ( tralloc_history == NULL ) {
        return false;
    }
    _tralloc_set_callback ( NULL, NULL, on_move, NULL );

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

#if defined(TRALLOC_REFERENCE)
    tralloc_context * c_reference;
    if ( tralloc_reference_new ( c, b, &c_reference ) != 0 ) {
        free_history ( tralloc_history );
        return false;
    }
#endif

    if (
        tralloc_move ( c, b )   != 0 ||
        tralloc_move ( c, ctx ) != 0 ||
        tralloc_move ( b, c )   != 0 ||
        tralloc_move ( c, a )   != 0
    ) {
        tralloc_free ( a );
        tralloc_free ( b );
        free_history ( tralloc_history );
        return false;
    }

    _tralloc_chunk * root_chunk = _tralloc_chunk_from_context ( ctx );
    _tralloc_chunk * a_chunk    = _tralloc_chunk_from_context ( a );
    _tralloc_chunk * b_chunk    = _tralloc_chunk_from_context ( b );
    _tralloc_chunk * c_chunk    = _tralloc_chunk_from_context ( c );

#if defined(TRALLOC_REFERENCE)
    if ( tralloc_move ( c_reference, a ) != 0 ) {
        free_history ( tralloc_history );
        return false;
    }
    _tralloc_chunk * c_reference_chunk = _tralloc_chunk_from_context ( c_reference );
#endif

    move_info * info;

#if defined(TRALLOC_REFERENCE)
    if (
        malloc_dynarr_get_length ( tralloc_history ) != 5 ||
        ( info = malloc_dynarr_get ( tralloc_history, 0 ) ) == NULL ||
        info->chunk != c_chunk || info->old_parent != a_chunk || info->chunk->parent != a_chunk ||
        ( info = malloc_dynarr_get ( tralloc_history, 1 ) ) == NULL ||
        info->chunk != c_chunk || info->old_parent != b_chunk || info->chunk->parent != a_chunk ||
        ( info = malloc_dynarr_get ( tralloc_history, 2 ) ) == NULL ||
        info->chunk != b_chunk || info->old_parent != root_chunk || info->chunk->parent != c_chunk ||
        ( info = malloc_dynarr_get ( tralloc_history, 3 ) ) == NULL ||
        info->chunk != c_chunk || info->old_parent != root_chunk || info->chunk->parent != a_chunk ||
        ( info = malloc_dynarr_get ( tralloc_history, 4 ) ) == NULL ||
        info->chunk != c_reference_chunk || info->old_parent != b_chunk || info->chunk->parent != a_chunk
    ) {
        free_history ( tralloc_history );
        return false;
    }
#else
    if (
        malloc_dynarr_get_length ( tralloc_history ) != 4 ||
        ( info = malloc_dynarr_get ( tralloc_history, 0 ) ) == NULL ||
        info->chunk != c_chunk || info->old_parent != a_chunk || info->chunk->parent != a_chunk ||
        ( info = malloc_dynarr_get ( tralloc_history, 1 ) ) == NULL ||
        info->chunk != c_chunk || info->old_parent != b_chunk || info->chunk->parent != a_chunk ||
        ( info = malloc_dynarr_get ( tralloc_history, 2 ) ) == NULL ||
        info->chunk != b_chunk || info->old_parent != root_chunk || info->chunk->parent != c_chunk ||
        ( info = malloc_dynarr_get ( tralloc_history, 3 ) ) == NULL ||
        info->chunk != c_chunk || info->old_parent != root_chunk || info->chunk->parent != a_chunk
    ) {
        free_history ( tralloc_history );
        return false;
    }
#endif

    if (
        tralloc_free ( a ) != 0
    ) {
        free_history ( tralloc_history );
        return false;
    }

    free_history ( tralloc_history );
    return true;
}