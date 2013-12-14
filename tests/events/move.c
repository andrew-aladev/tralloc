// This file is part of talloc2. Copyright (C) 2013 Andrew Aladjev aladjev.andrew@gmail.com
// talloc2 is free software: you can redistribute it and/or modify it under the terms of the GNU General Public License as published by the Free Software Foundation, either version 3 of the License, or (at your option) any later version.
// talloc2 is distributed in the hope that it will be useful, but WITHOUT ANY WARRANTY; without even the implied warranty of MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU General Public License for more details.
// You should have received a copy of the GNU General Public License along with talloc2. If not, see <http://www.gnu.org/licenses/>.

#include "move.h"
#include "../lib/malloc_dynarr.h"
#include <talloc2/tree.h>
#include <talloc2/events.h>

#if defined(TALLOC_REFERENCE)
#include <talloc2/reference/main.h>
#endif

typedef struct move_info_t {
    talloc_chunk * chunk;
    talloc_chunk * old_parent;
} move_info;

static
malloc_dynarr * malloc_history()
{
    malloc_dynarr * talloc_history = malloc_dynarr_new ( 8 );
    if ( talloc_history == NULL ) {
        return NULL;
    }
    malloc_dynarr_set_free_item ( talloc_history, free );
    talloc_set_user_data ( talloc_history );
    return talloc_history;
}

static
void free_history ( malloc_dynarr * talloc_history )
{
    malloc_dynarr_free ( talloc_history );
}

static
uint8_t on_move ( void * user_data, talloc_chunk * chunk, talloc_chunk * old_parent )
{
    malloc_dynarr * talloc_history = ( malloc_dynarr * ) user_data;
    move_info * info = malloc ( sizeof ( move_info ) );
    if ( info == NULL ) {
        return 1;
    }
    info->chunk      = chunk;
    info->old_parent = old_parent;
    if ( malloc_dynarr_append ( talloc_history, info ) != 0 ) {
        free ( info );
        return 2;
    }
    return 0;
}

bool test_move ( const talloc_context * root )
{
    malloc_dynarr * talloc_history = malloc_history();
    if ( talloc_history == NULL ) {
        return false;
    }
    talloc_set_callback ( NULL, NULL, on_move, NULL );

    int * a   = talloc ( root, sizeof ( int ) * 2 );
    char * b  = talloc ( root, sizeof ( char ) * 3 );
    float * c = talloc ( a,    sizeof ( float ) * 4 );

    if ( a == NULL || b == NULL || c == NULL ) {
        talloc_free ( a );
        talloc_free ( b );
        free_history ( talloc_history );
        return false;
    }

#if defined(TALLOC_REFERENCE)
    void * c_reference = talloc_add_reference ( c, b );
    if ( c_reference == NULL ) {
        talloc_free ( a );
        talloc_free ( b );
        free_history ( talloc_history );
        return false;
    }
#endif

    if (
        talloc_move ( c, b )    != 0 ||
        talloc_move ( c, root ) != 0 ||
        talloc_move ( b, c )    != 0 ||
        talloc_move ( c, a )    != 0
    ) {
        talloc_free ( a );
        talloc_free ( b );
        free_history ( talloc_history );
        return false;
    }

    talloc_chunk * root_chunk = talloc_chunk_from_context ( root );
    talloc_chunk * a_chunk    = talloc_chunk_from_context ( a );
    talloc_chunk * b_chunk    = talloc_chunk_from_context ( b );
    talloc_chunk * c_chunk    = talloc_chunk_from_context ( c );

#if defined(TALLOC_REFERENCE)
    if ( talloc_move ( c_reference, a ) != 0 ) {
        talloc_free ( a );
        free_history ( talloc_history );
        return false;
    }
    talloc_chunk * c_reference_chunk = talloc_chunk_from_context ( c_reference );
#endif

    move_info * info;

#if defined(TALLOC_REFERENCE)
    if (
        malloc_dynarr_get_length ( talloc_history ) != 5 ||
        ( info = malloc_dynarr_get ( talloc_history, 0 ) ) == NULL ||
        info->chunk != c_chunk || info->old_parent != a_chunk || info->chunk->parent != a_chunk ||
        ( info = malloc_dynarr_get ( talloc_history, 1 ) ) == NULL ||
        info->chunk != c_chunk || info->old_parent != b_chunk || info->chunk->parent != a_chunk ||
        ( info = malloc_dynarr_get ( talloc_history, 2 ) ) == NULL ||
        info->chunk != b_chunk || info->old_parent != root_chunk || info->chunk->parent != c_chunk ||
        ( info = malloc_dynarr_get ( talloc_history, 3 ) ) == NULL ||
        info->chunk != c_chunk || info->old_parent != root_chunk || info->chunk->parent != a_chunk ||
        ( info = malloc_dynarr_get ( talloc_history, 4 ) ) == NULL ||
        info->chunk != c_reference_chunk || info->old_parent != b_chunk || info->chunk->parent != a_chunk
    ) {
        talloc_free ( a );
        free_history ( talloc_history );
        return false;
    }
#else
    if (
        malloc_dynarr_get_length ( talloc_history ) != 4 ||
        ( info = malloc_dynarr_get ( talloc_history, 0 ) ) == NULL ||
        info->chunk != c_chunk || info->old_parent != a_chunk || info->chunk->parent != a_chunk ||
        ( info = malloc_dynarr_get ( talloc_history, 1 ) ) == NULL ||
        info->chunk != c_chunk || info->old_parent != b_chunk || info->chunk->parent != a_chunk ||
        ( info = malloc_dynarr_get ( talloc_history, 2 ) ) == NULL ||
        info->chunk != b_chunk || info->old_parent != root_chunk || info->chunk->parent != c_chunk ||
        ( info = malloc_dynarr_get ( talloc_history, 3 ) ) == NULL ||
        info->chunk != c_chunk || info->old_parent != root_chunk || info->chunk->parent != a_chunk
    ) {
        talloc_free ( a );
        free_history ( talloc_history );
        return false;
    }
#endif

    if (
        talloc_free ( a ) != 0
    ) {
        free_history ( talloc_history );
        return false;
    }

    free_history ( talloc_history );
    return true;
}