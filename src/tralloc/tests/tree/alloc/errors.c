// This file is part of tralloc. Copyright (C) 2013 Andrew Aladjev aladjev.andrew@gmail.com
// tralloc is free software: you can redistribute it and/or modify it under the terms of the GNU General Public License as published by the Free Software Foundation, either version 3 of the License, or (at your option) any later version.
// tralloc is distributed in the hope that it will be useful, but WITHOUT ANY WARRANTY; without even the implied warranty of MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU General Public License for more details.
// You should have received a copy of the GNU General Public License along with tralloc. If not, see <http://www.gnu.org/licenses/>.

#include <tralloc/tests/tree/alloc/common.h>
#include <tralloc/tree.h>


#if defined(TRALLOC_DEBUG_THREADS)

// Thread creates several contexts on parent.
static
void * thread ( void * argument )
{
    tralloc_context * ctx = argument;
    tralloc_context * temp;
    tralloc_error result;

    for ( size_t index = 0; index < 20; index ++ ) {
        result = tralloc_new ( ctx, &temp, sizeof ( uint8_t ) * index * 2 );
        if ( result != 0 ) {
            return ( void * ) ( ( uintptr_t ) result );
        }
    }
    return ( void * ) ( ( uintptr_t ) 0 );
}
#endif

tralloc_bool test_tree_alloc_errors ( tralloc_context * _TRALLOC_UNUSED ( ctx ) )
{
    if (
        tralloc_new       ( NULL, NULL, 0 ) != TRALLOC_ERROR_REQUIRED_ARGUMENT_IS_NULL ||
        tralloc_new_zero  ( NULL, NULL, 0 ) != TRALLOC_ERROR_REQUIRED_ARGUMENT_IS_NULL ||
        tralloc_new_empty ( NULL, NULL )    != TRALLOC_ERROR_REQUIRED_ARGUMENT_IS_NULL
    ) {
        return TRALLOC_FALSE;
    }

#   if defined(TRALLOC_EXTENSIONS)

    if (
        tralloc_new_with_extensions       ( NULL, NULL, 0, 0 ) != TRALLOC_ERROR_REQUIRED_ARGUMENT_IS_NULL ||
        tralloc_new_zero_with_extensions  ( NULL, NULL, 0, 0 ) != TRALLOC_ERROR_REQUIRED_ARGUMENT_IS_NULL ||
        tralloc_new_empty_with_extensions ( NULL, NULL, 0 )    != TRALLOC_ERROR_REQUIRED_ARGUMENT_IS_NULL
    ) {
        return TRALLOC_FALSE;
    }

#   if defined(TRALLOC_DEBUG_THREADS)

    tralloc_context * parent;
    if ( tralloc_new_empty ( ctx, &parent ) != 0 ) {
        return TRALLOC_FALSE;
    }

    void * result_1, * result_2, * result_3;
    pthread_t thread_1, thread_2, thread_3;

    // 3 threads try to create several chunks on parent.
    // First chunk will be created normally, second will not be even created.
    // Any of "result_1", "result_2" or "result_3" will be an error, which means:
    // Parent should lock it's children list, if it's children can be allocated from different threads.
    // This code should not fail with unknown error, because each chunk in debug mode have extra locks.
    if (
        pthread_create ( &thread_1, NULL, &thread, parent ) != 0 ||
        pthread_create ( &thread_2, NULL, &thread, parent ) != 0 ||
        pthread_create ( &thread_3, NULL, &thread, parent ) != 0 ||
        pthread_join   ( thread_1, &result_1 ) != 0 ||
        pthread_join   ( thread_2, &result_2 ) != 0 ||
        pthread_join   ( thread_3, &result_3 ) != 0 ||
        (
            ( tralloc_error ) ( ( uintptr_t ) result_1 ) != TRALLOC_ERROR_NO_CHILDREN_LOCK &&
            ( tralloc_error ) ( ( uintptr_t ) result_2 ) != TRALLOC_ERROR_NO_CHILDREN_LOCK &&
            ( tralloc_error ) ( ( uintptr_t ) result_3 ) != TRALLOC_ERROR_NO_CHILDREN_LOCK
        )
    ) {
        return TRALLOC_FALSE;
    }

    if ( tralloc_free ( parent ) != 0 ) {
        return TRALLOC_FALSE;
    }

#   endif

#   endif

    return TRALLOC_TRUE;
}
