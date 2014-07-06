// This file is part of tralloc. Copyright (C) 2013 Andrew Aladjev aladjev.andrew@gmail.com
// tralloc is free software: you can redistribute it and/or modify it under the terms of the GNU General Public License as published by the Free Software Foundation, either version 3 of the License, or (at your option) any later version.
// tralloc is distributed in the hope that it will be useful, but WITHOUT ANY WARRANTY; without even the implied warranty of MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU General Public License for more details.
// You should have received a copy of the GNU General Public License along with tralloc. If not, see <http://www.gnu.org/licenses/>.

#include <tralloc/tests/tree/move/common.h>
#include <tralloc/tree.h>


#if defined(TRALLOC_DEBUG_THREADS)

typedef struct test_tree_move_errors_data_type {
    tralloc_context * parent_1;
    tralloc_context * parent_2;
    tralloc_context * child;
} test_tree_move_errors_data;

// Thread moves child to 2 parents or NULL.
static
void * thread ( void * argument )
{
    test_tree_move_errors_data * data = argument;
    tralloc_error first_error = 0, result;
    tralloc_context * parent;

    for ( size_t index = 0; index < 20; index ++ ) {
        switch ( index % 3 ) {
        case 0:
            parent = data->parent_1;
            break;
        case 1:
            parent = data->parent_2;
            break;
        default:
            parent = NULL;
            break;
        }
        result = tralloc_move ( data->child, parent );
        if ( result != 0 && first_error == 0 ) {
            first_error = result;
        }
    }
    return ( void * ) ( ( uintptr_t ) first_error );
}

static inline
tralloc_bool _test_tree_move_errors_subtree ( tralloc_context * ctx )
{
    tralloc_context * parent_1, * parent_2, * child;
    if (
        tralloc_new_empty_with_extensions ( ctx, &parent_1, TRALLOC_EXTENSION_LOCK_CHILDREN ) != 0 ||
        tralloc_new_empty_with_extensions ( ctx, &parent_2, TRALLOC_EXTENSION_LOCK_CHILDREN ) != 0 ||
        tralloc_new_empty ( NULL, &child ) != 0
    ) {
        return TRALLOC_FALSE;
    }

    test_tree_move_errors_data data;
    data.child    = child;
    data.parent_1 = parent_1;
    data.parent_2 = parent_2;

    void * result_1, * result_2;
    pthread_t thread_1, thread_2;

    // 2 threads try to move "child" on "parent_1", "parent_2" or NULL.
    // Several moves from one thread will be done normally when another thread is not active. Next moves will fail.
    // Any of "result_1" or "result_2" will be an error, which means:
    // Chunk should have subtree lock, if it's subtree pointers ("parent", "next" and "prev") are operating from multiple threads.
    if (
        pthread_create ( &thread_1, NULL, &thread, &data ) != 0 ||
        pthread_create ( &thread_2, NULL, &thread, &data ) != 0 ||
        pthread_join   ( thread_1, &result_1 ) != 0 ||
        pthread_join   ( thread_2, &result_2 ) != 0 ||
        (
            ( tralloc_error ) ( ( uintptr_t ) result_1 ) != TRALLOC_ERROR_NO_SUBTREE_LOCK &&
            ( tralloc_error ) ( ( uintptr_t ) result_2 ) != TRALLOC_ERROR_NO_SUBTREE_LOCK
        )
    ) {
        tralloc_free ( child );
        tralloc_free ( parent_1 );
        tralloc_free ( parent_2 );
        return TRALLOC_FALSE;
    }

    if (
        tralloc_free ( child )    != 0 ||
        tralloc_free ( parent_1 ) != 0 ||
        tralloc_free ( parent_2 ) != 0
    ) {
        return TRALLOC_FALSE;
    }
    return TRALLOC_TRUE;
}

static inline
tralloc_bool _test_tree_move_errors_children ( tralloc_context * ctx )
{
    tralloc_context * parent_1, * parent_2, * child;
    if (
        tralloc_new_empty ( ctx, &parent_1 ) != 0 ||
        tralloc_new_empty ( ctx, &parent_2 ) != 0 ||
        tralloc_new_empty_with_extensions ( NULL, &child, TRALLOC_EXTENSION_LOCK_SUBTREE ) != 0
    ) {
        return TRALLOC_FALSE;
    }

    test_tree_move_errors_data data;
    data.child    = child;
    data.parent_1 = parent_1;
    data.parent_2 = parent_2;

    void * result_1, * result_2;
    pthread_t thread_1, thread_2;

    // 2 threads try to move "child" on "parent_1", "parent_2" or NULL.
    // Several moves to each parent will be done normally when moving from only one thread. Other move will fail.
    // Any of "result_1" or "result_2" will be an error, which means:
    // Parent should have lock of it's children list, if any child can be moved to/from different threads.
    if (
        pthread_create ( &thread_1, NULL, &thread, &data ) != 0 ||
        pthread_create ( &thread_2, NULL, &thread, &data ) != 0 ||
        pthread_join   ( thread_1, &result_1 ) != 0 ||
        pthread_join   ( thread_2, &result_2 ) != 0 ||
        (
            ( tralloc_error ) ( ( uintptr_t ) result_1 ) != TRALLOC_ERROR_NO_CHILDREN_LOCK &&
            ( tralloc_error ) ( ( uintptr_t ) result_2 ) != TRALLOC_ERROR_NO_CHILDREN_LOCK
        )
    ) {
        tralloc_free ( child );
        tralloc_free ( parent_1 );
        tralloc_free ( parent_2 );
        return TRALLOC_FALSE;
    }

    // If "child"'s parent is NULL - it will return 0.
    // If "child"'s parent is not NULL - it will return the same error as described above.
    // In this case "child" will be freed normally after free of it's parent.
    tralloc_error result = tralloc_free ( child );

    if (
        (
            result != 0 &&
            result != TRALLOC_ERROR_NO_CHILDREN_LOCK
        ) ||
        tralloc_free ( parent_1 ) != 0 ||
        tralloc_free ( parent_2 ) != 0
    ) {
        return TRALLOC_FALSE;
    }
    return TRALLOC_TRUE;
}

#endif

tralloc_bool test_tree_move_errors ( tralloc_context * ctx )
{
    if (
        tralloc_move ( NULL, NULL ) != TRALLOC_ERROR_REQUIRED_ARGUMENT_IS_NULL ||
        tralloc_move ( ctx, ctx )   != TRALLOC_ERROR_CHILD_EQUALS_PARENT
    ) {
        return TRALLOC_FALSE;
    }

    tralloc_context * test;
    if ( tralloc_new_empty ( ctx, &test ) != 0 ) {
        return TRALLOC_FALSE;
    }
    if (
        tralloc_move ( test, ctx )  != TRALLOC_ERROR_CHILD_HAS_SAME_PARENT ||
        tralloc_move ( test, NULL ) != 0 ||
        tralloc_move ( test, NULL ) != TRALLOC_ERROR_CHILD_HAS_SAME_PARENT ||
        tralloc_move ( test, ctx )  != 0
    ) {
        tralloc_free ( test );
        return TRALLOC_FALSE;
    }
    if ( tralloc_free ( test ) != 0 ) {
        return TRALLOC_FALSE;
    }

#   if defined(TRALLOC_DEBUG_THREADS)
    if (
        !_test_tree_move_errors_subtree  ( ctx ) ||
        !_test_tree_move_errors_children ( ctx )
    ) {
        return TRALLOC_FALSE;
    }
#   endif

    return TRALLOC_TRUE;
}
