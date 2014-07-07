// This file is part of tralloc. Copyright (C) 2013 Andrew Aladjev aladjev.andrew@gmail.com
// tralloc is free software: you can redistribute it and/or modify it under the terms of the GNU General Public License as published by the Free Software Foundation, either version 3 of the License, or (at your option) any later version.
// tralloc is distributed in the hope that it will be useful, but WITHOUT ANY WARRANTY; without even the implied warranty of MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU General Public License for more details.
// You should have received a copy of the GNU General Public License along with tralloc. If not, see <http://www.gnu.org/licenses/>.

#include <tralloc/tests/tree/alloc/common.h>
#include <tralloc/tree.h>


// Thread creates several contexts on parent.
static
void * _test_tree_alloc_thread ( void * argument )
{
    tralloc_error result;
    uint8_t * data;
    tralloc_context * parent = argument;

    for ( size_t index = 0; index < 20; index ++ ) {
        result = tralloc_new ( parent, ( tralloc_context ** ) &data, sizeof ( uint8_t ) * index * 2 );
        if ( result != 0 ) {
            return ( void * ) ( ( uintptr_t ) result );
        }
    }
    return ( void * ) ( ( uintptr_t ) 0 );
}

tralloc_bool test_tree_alloc_threads ( tralloc_context * ctx )
{
    tralloc_context * parent;
    if ( tralloc_new_empty_with_extensions ( ctx, &parent, TRALLOC_EXTENSION_LOCK_CHILDREN ) != 0 ) {
        return TRALLOC_FALSE;
    }

    void * result;
    pthread_t thread_1, thread_2, thread_3;

    // This is a competition between 3 threads to create contexts on 1 parent.
    if (
        pthread_create ( &thread_1, NULL, &_test_tree_alloc_thread, parent ) != 0 ||
        pthread_create ( &thread_2, NULL, &_test_tree_alloc_thread, parent ) != 0 ||
        pthread_create ( &thread_3, NULL, &_test_tree_alloc_thread, parent ) != 0 ||
        pthread_join   ( thread_1, &result ) != 0 || ( uintptr_t ) result != 0 ||
        pthread_join   ( thread_2, &result ) != 0 || ( uintptr_t ) result != 0 ||
        pthread_join   ( thread_3, &result ) != 0 || ( uintptr_t ) result != 0
    ) {
        return TRALLOC_FALSE;
    }

    if ( tralloc_free ( parent ) != 0 ) {
        return TRALLOC_FALSE;
    }
    return TRALLOC_TRUE;
}
