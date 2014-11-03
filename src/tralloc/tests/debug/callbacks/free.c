// This file is part of tralloc. Copyright (C) 2013 Andrew Aladjev aladjev.andrew@gmail.com
// tralloc is free software: you can redistribute it and/or modify it under the terms of the GNU General Public License as published by the Free Software Foundation, either version 3 of the License, or (at your option) any later version.
// tralloc is distributed in the hope that it will be useful, but WITHOUT ANY WARRANTY; without even the implied warranty of MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU General Public License for more details.
// You should have received a copy of the GNU General Public License along with tralloc. If not, see <http://www.gnu.org/licenses/>.

#include <tralloc/tests/debug/callbacks/common.h>
#include <tralloc/tests/common/dynarr.h>
#include <tralloc/tree.h>
#include <tralloc/debug.h>


static
tralloc_error test_debug_callbacks_before_free ( void * user_data, _tralloc_chunk * chunk )
{
    dynarr * tralloc_history = ( dynarr * ) user_data;
    if ( dynarr_append ( tralloc_history, chunk ) != 0 ) {
        return TRALLOC_ERROR_MALLOC_FAILED;
    }
    return 0;
}

static inline
dynarr * test_debug_callbacks_free_new_history()
{
    dynarr * history = dynarr_new ( 8 );
    if ( history == NULL ) {
        return NULL;
    }
    if ( tralloc_debug_callback_set_free_data ( NULL, history, NULL, NULL, NULL, NULL ) != 0 ) {
        dynarr_free ( history );
        return NULL;
    }
    if ( tralloc_debug_callback_set_free_functions ( NULL, test_debug_callbacks_before_free, NULL, NULL, NULL, NULL ) != 0 ) {
        tralloc_debug_callback_set_free_data ( NULL, NULL, NULL, NULL, NULL, NULL );
        dynarr_free ( history );
        return NULL;
    }
    return history;
}

static inline
tralloc_error test_debug_callbacks_free_free_history ( dynarr * history )
{
    tralloc_error error  = 0;
    tralloc_error result = tralloc_debug_callback_set_free_data ( NULL, NULL, NULL, NULL, NULL, NULL );
    if ( result != 0 ) {
        error = result;
    }
    result = tralloc_debug_callback_set_free_functions ( NULL, NULL, NULL, NULL, NULL, NULL );
    if ( result != 0 ) {
        error = result;
    }

    dynarr_free ( history );
    return error;
}

tralloc_bool test_debug_callbacks_free ( tralloc_context * ctx )
{
    dynarr * history = test_debug_callbacks_free_new_history();
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
        test_debug_callbacks_free_free_history ( history );
        return TRALLOC_FALSE;
    }

    _tralloc_chunk * chunk;
    _tralloc_chunk * a_chunk = _tralloc_context_get_chunk ( a );
    _tralloc_chunk * b_chunk = _tralloc_context_get_chunk ( b );
    _tralloc_chunk * c_chunk = _tralloc_context_get_chunk ( c );

    if (
        tralloc_free ( a ) != 0 ||
        tralloc_free ( b ) != 0 ||

        dynarr_get_length ( history ) != 3 ||
        ( chunk = dynarr_get ( history, 0 ) ) == NULL ||
        chunk != a_chunk ||
        ( chunk = dynarr_get ( history, 1 ) ) == NULL ||
        chunk != c_chunk ||
        ( chunk = dynarr_get ( history, 2 ) ) == NULL ||
        chunk != b_chunk
    ) {
        test_debug_callbacks_free_free_history ( history );
        return TRALLOC_FALSE;
    }

    return test_debug_callbacks_free_free_history ( history ) == 0;
}
