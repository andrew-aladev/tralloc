// This file is part of tralloc. Copyright (C) 2013 Andrew Aladjev aladjev.andrew@gmail.com
// tralloc is free software: you can redistribute it and/or modify it under the terms of the GNU General Public License as published by the Free Software Foundation, either version 3 of the License, or (at your option) any later version.
// tralloc is distributed in the hope that it will be useful, but WITHOUT ANY WARRANTY; without even the implied warranty of MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU General Public License for more details.
// You should have received a copy of the GNU General Public License along with tralloc. If not, see <http://www.gnu.org/licenses/>.

#include <tralloc/tests/debug/callbacks/common.h>
#include <tralloc/tests/common/dynarr.h>
#include <tralloc/tree.h>
#include <tralloc/debug.h>

#include <stdlib.h>


typedef struct test_debug_callbacks_move_info_t {
    _tralloc_chunk * chunk;
    _tralloc_chunk * old_parent;
} test_debug_callbacks_move_info;

static
tralloc_error test_debug_callbacks_after_move ( void * user_data, _tralloc_chunk * chunk, _tralloc_chunk * old_parent )
{
    dynarr * history = ( dynarr * ) user_data;
    test_debug_callbacks_move_info * info = malloc ( sizeof ( test_debug_callbacks_move_info ) );
    if ( info == NULL ) {
        return TRALLOC_ERROR_MALLOC_FAILED;
    }
    info->chunk      = chunk;
    info->old_parent = old_parent;
    if ( dynarr_append ( history, info ) != 0 ) {
        free ( info );
        return TRALLOC_ERROR_MALLOC_FAILED;
    }

    dynarr_set_free_item ( history, free );
    return 0;
}

static
dynarr * test_debug_callbacks_move_new_history()
{
    dynarr * history = dynarr_new ( 8 );
    if ( history == NULL ) {
        return NULL;
    }
    if ( tralloc_debug_callback_set_move_data ( NULL, history ) != 0 ) {
        dynarr_free ( history );
        return NULL;
    }
    if ( tralloc_debug_callback_set_move_functions ( NULL, test_debug_callbacks_after_move ) != 0 ) {
        tralloc_debug_callback_set_move_data ( NULL, NULL );
        dynarr_free ( history );
        return NULL;
    }
    return history;
}

static
tralloc_error test_debug_callbacks_move_free_history ( dynarr * history )
{
    tralloc_error error  = 0;
    tralloc_error result = tralloc_debug_callback_set_move_data ( NULL, NULL );
    if ( result != 0 ) {
        error = result;
    }
    result = tralloc_debug_callback_set_move_functions ( NULL, NULL );
    if ( result != 0 ) {
        error = result;
    }

    dynarr_free ( history );
    return error;
}

tralloc_bool test_debug_callbacks_move ( tralloc_context * ctx )
{
    dynarr * history = test_debug_callbacks_move_new_history();
    if ( history == NULL ) {
        return TRALLOC_FALSE;
    }

    int * a;
    char * b;
    float * c;
    if (
        tralloc_new ( ctx, ( tralloc_context ** ) &a, sizeof ( int ) * 2 )   != 0 ||
        tralloc_new ( ctx, ( tralloc_context ** ) &b, sizeof ( char ) * 3 )  != 0 ||
        tralloc_new ( a,   ( tralloc_context ** ) &c, sizeof ( float ) * 4 ) != 0
    ) {
        test_debug_callbacks_move_free_history ( history );
        return TRALLOC_FALSE;
    }

    if (
        tralloc_move ( c, b )   != 0 ||
        tralloc_move ( c, ctx ) != 0 ||
        tralloc_move ( b, c )   != 0 ||
        tralloc_move ( c, a )   != 0
    ) {
        tralloc_free ( a );
        tralloc_free ( b );
        test_debug_callbacks_move_free_history ( history );
        return TRALLOC_FALSE;
    }

    _tralloc_chunk * root_chunk = _tralloc_context_get_chunk ( ctx );
    _tralloc_chunk * a_chunk    = _tralloc_context_get_chunk ( a );
    _tralloc_chunk * b_chunk    = _tralloc_context_get_chunk ( b );
    _tralloc_chunk * c_chunk    = _tralloc_context_get_chunk ( c );

    test_debug_callbacks_move_info * info;

    if (
        dynarr_get_length ( history ) != 4 ||
        ( info = dynarr_get ( history, 0 ) ) == NULL ||
        info->chunk != c_chunk || info->old_parent != a_chunk || info->chunk->parent != a_chunk ||
        ( info = dynarr_get ( history, 1 ) ) == NULL ||
        info->chunk != c_chunk || info->old_parent != b_chunk || info->chunk->parent != a_chunk ||
        ( info = dynarr_get ( history, 2 ) ) == NULL ||
        info->chunk != b_chunk || info->old_parent != root_chunk || info->chunk->parent != c_chunk ||
        ( info = dynarr_get ( history, 3 ) ) == NULL ||
        info->chunk != c_chunk || info->old_parent != root_chunk || info->chunk->parent != a_chunk ||

        tralloc_free ( a ) != 0
    ) {
        test_debug_callbacks_move_free_history ( history );
        return TRALLOC_FALSE;
    }

    return test_debug_callbacks_move_free_history ( history ) == 0;
}
