// This file is part of tralloc. Copyright (C) 2013 Andrew Aladjev aladjev.andrew@gmail.com
// tralloc is free software: you can redistribute it and/or modify it under the terms of the GNU General Public License as published by the Free Software Foundation, either version 3 of the License, or (at your option) any later version.
// tralloc is distributed in the hope that it will be useful, but WITHOUT ANY WARRANTY; without even the implied warranty of MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU General Public License for more details.
// You should have received a copy of the GNU General Public License along with tralloc. If not, see <http://www.gnu.org/licenses/>.

#include "common.h"
#include "../lib/malloc_dynarr.h"
#include <tralloc/tree.h>
#include <tralloc/debug.h>


typedef struct resize_info_t {
    _tralloc_chunk * chunk;
    size_t old_length;
} resize_info;

static
malloc_dynarr * malloc_history()
{
    malloc_dynarr * tralloc_history = malloc_dynarr_new ( 8 );
    if (
        tralloc_history == NULL ||
        _tralloc_debug_set_user_data ( tralloc_history ) != 0
    ) {
        return NULL;
    }
    malloc_dynarr_set_free_item ( tralloc_history, free );
    return tralloc_history;
}

static
void free_history ( malloc_dynarr * tralloc_history )
{
    malloc_dynarr_free ( tralloc_history );
}

static
tralloc_error on_resize ( void * user_data, _tralloc_chunk * chunk, size_t old_length )
{
    malloc_dynarr * tralloc_history = ( malloc_dynarr * ) user_data;
    resize_info * info = malloc ( sizeof ( resize_info ) );
    if ( info == NULL ) {
        return TRALLOC_ERROR_MALLOC_FAILED;
    }
    info->chunk      = chunk;
    info->old_length = old_length;
    if ( malloc_dynarr_append ( tralloc_history, info ) != 0 ) {
        free ( info );
        return TRALLOC_ERROR_MALLOC_FAILED;
    }
    return 0;
}

tralloc_bool test_debug_resize ( tralloc_context * ctx )
{
    malloc_dynarr * tralloc_history = malloc_history();
    if (
        tralloc_history == NULL ||
        _tralloc_debug_set_callbacks ( NULL, on_resize, NULL, NULL ) != 0
    ) {
        return TRALLOC_FALSE;
    }

    int * a;
    char * b;
    float * c;
    if (
        tralloc ( ctx, ( tralloc_context ** ) &a, sizeof ( int ) * 2 )   != 0 ||
        tralloc ( ctx, ( tralloc_context ** ) &b, sizeof ( char ) * 3 )  != 0 ||
        tralloc ( a,   ( tralloc_context ** ) &c, sizeof ( float ) * 4 ) != 0
    ) {
        free_history ( tralloc_history );
        _tralloc_debug_set_callbacks ( NULL, NULL, NULL, NULL );
        return TRALLOC_FALSE;
    }

    if (
        tralloc_realloc ( ( tralloc_context ** ) &b, sizeof ( char ) * 8 )   != 0 ||
        tralloc_realloc ( ( tralloc_context ** ) &a, sizeof ( int ) * 9 )    != 0 ||
        tralloc_realloc ( ( tralloc_context ** ) &c, sizeof ( float ) * 10 ) != 0
    ) {
        free_history ( tralloc_history );
        _tralloc_debug_set_callbacks ( NULL, NULL, NULL, NULL );
        return TRALLOC_FALSE;
    }

    _tralloc_chunk * a_chunk = _tralloc_get_chunk_from_context ( a );
    _tralloc_chunk * b_chunk = _tralloc_get_chunk_from_context ( b );
    _tralloc_chunk * c_chunk = _tralloc_get_chunk_from_context ( c );
    resize_info * info;

    if (
        malloc_dynarr_get_length ( tralloc_history ) != 3 ||
        ( info = malloc_dynarr_get ( tralloc_history, 0 ) ) == NULL ||
        info->chunk != b_chunk || info->old_length != sizeof ( char ) * 3 || info->chunk->length != sizeof ( char ) * 8 ||
        ( info = malloc_dynarr_get ( tralloc_history, 1 ) ) == NULL ||
        info->chunk != a_chunk || info->old_length != sizeof ( int ) * 2 || info->chunk->length != sizeof ( int ) * 9 ||
        ( info = malloc_dynarr_get ( tralloc_history, 2 ) ) == NULL ||
        info->chunk != c_chunk || info->old_length != sizeof ( float ) * 4 || info->chunk->length != sizeof ( float ) * 10
    ) {
        free_history ( tralloc_history );
        _tralloc_debug_set_callbacks ( NULL, NULL, NULL, NULL );
        return TRALLOC_FALSE;
    }

    if (
        tralloc_free ( a ) != 0 ||
        tralloc_free ( b ) != 0 ||
        _tralloc_debug_set_callbacks ( NULL, NULL, NULL, NULL ) != 0
    ) {
        free_history ( tralloc_history );
        return TRALLOC_FALSE;
    }

    free_history ( tralloc_history );
    return TRALLOC_TRUE;
}
