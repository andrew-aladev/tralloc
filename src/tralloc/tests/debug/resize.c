// This file is part of tralloc. Copyright (C) 2013 Andrew Aladjev aladjev.andrew@gmail.com
// tralloc is free software: you can redistribute it and/or modify it under the terms of the GNU General Public License as published by the Free Software Foundation, either version 3 of the License, or (at your option) any later version.
// tralloc is distributed in the hope that it will be useful, but WITHOUT ANY WARRANTY; without even the implied warranty of MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU General Public License for more details.
// You should have received a copy of the GNU General Public License along with tralloc. If not, see <http://www.gnu.org/licenses/>.

#include <tralloc/tests/debug/common.h>
#include <tralloc/tests/common/dynarr.h>
#include <tralloc/tree.h>
#include <tralloc/debug.h>

#include <stdlib.h>


typedef struct test_debug_resize_info_t {
    _tralloc_chunk * chunk;
    size_t old_length;
} test_debug_resize_info;

static
tralloc_error test_debug_after_resize ( void * user_data, _tralloc_chunk * chunk, size_t old_length )
{
    dynarr * history = ( dynarr * ) user_data;
    test_debug_resize_info * info = malloc ( sizeof ( test_debug_resize_info ) );
    if ( info == NULL ) {
        return TRALLOC_ERROR_MALLOC_FAILED;
    }
    info->chunk      = chunk;
    info->old_length = old_length;
    if ( dynarr_append ( history, info ) != 0 ) {
        free ( info );
        return TRALLOC_ERROR_MALLOC_FAILED;
    }
    dynarr_set_free_item ( history, free );
    return 0;
}

static inline
dynarr * test_debug_resize_new_history()
{
    dynarr * history = dynarr_new ( 8 );
    if ( history == NULL ) {
        return NULL;
    }
    if ( tralloc_debug_callback_set_resize_data ( NULL, history ) != 0 ) {
        dynarr_free ( history );
        return NULL;
    }
    if ( tralloc_debug_callback_set_resize_functions ( NULL, test_debug_after_resize ) != 0 ) {
        tralloc_debug_callback_set_resize_data ( NULL, NULL );
        dynarr_free ( history );
        return NULL;
    }
    return history;
}

static inline
tralloc_error test_debug_resize_free_history ( dynarr * history )
{
    tralloc_error error  = 0;
    tralloc_error result = tralloc_debug_callback_set_resize_data ( NULL, NULL );
    if ( result != 0 ) {
        error = result;
    }
    result = tralloc_debug_callback_set_resize_functions ( NULL, NULL );
    if ( result != 0 ) {
        error = result;
    }

    dynarr_free ( history );
    return error;
}

tralloc_bool test_debug_resize ( tralloc_context * ctx )
{
    dynarr * history = test_debug_resize_new_history();
    if ( history == NULL ) {
        return TRALLOC_FALSE;
    }

    int * a;
    char * b;
    float * c;
    if (
        tralloc_new ( ctx, ( tralloc_context ** ) &a, sizeof ( int ) * 2 )   != 0 ||
        tralloc_new ( ctx, ( tralloc_context ** ) &b, sizeof ( char ) * 3 )  != 0 ||
        tralloc_new ( a,   ( tralloc_context ** ) &c, sizeof ( float ) * 4 ) != 0 ||

        tralloc_realloc ( ( tralloc_context ** ) &b, sizeof ( char ) * 8 )   != 0 ||
        tralloc_realloc ( ( tralloc_context ** ) &a, sizeof ( int ) * 9 )    != 0 ||
        tralloc_realloc ( ( tralloc_context ** ) &c, sizeof ( float ) * 10 ) != 0
    ) {
        test_debug_resize_free_history ( history );
        return TRALLOC_FALSE;
    }

    _tralloc_chunk * a_chunk = _tralloc_get_chunk_from_context ( a );
    _tralloc_chunk * b_chunk = _tralloc_get_chunk_from_context ( b );
    _tralloc_chunk * c_chunk = _tralloc_get_chunk_from_context ( c );
    test_debug_resize_info * info;

    if (
        dynarr_get_length ( history ) != 3 ||
        ( info = dynarr_get ( history, 0 ) ) == NULL ||
        info->chunk != b_chunk || info->old_length != sizeof ( char ) * 3 || info->chunk->length != sizeof ( char ) * 8 ||
        ( info = dynarr_get ( history, 1 ) ) == NULL ||
        info->chunk != a_chunk || info->old_length != sizeof ( int ) * 2 || info->chunk->length != sizeof ( int ) * 9 ||
        ( info = dynarr_get ( history, 2 ) ) == NULL ||
        info->chunk != c_chunk || info->old_length != sizeof ( float ) * 4 || info->chunk->length != sizeof ( float ) * 10 ||

        tralloc_free ( a ) != 0 ||
        tralloc_free ( b ) != 0
    ) {
        test_debug_resize_free_history ( history );
        return TRALLOC_FALSE;
    }

    return test_debug_resize_free_history ( history ) == 0;
}
