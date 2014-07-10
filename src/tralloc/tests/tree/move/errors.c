// This file is part of tralloc. Copyright (C) 2013 Andrew Aladjev aladjev.andrew@gmail.com
// tralloc is free software: you can redistribute it and/or modify it under the terms of the GNU General Public License as published by the Free Software Foundation, either version 3 of the License, or (at your option) any later version.
// tralloc is distributed in the hope that it will be useful, but WITHOUT ANY WARRANTY; without even the implied warranty of MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU General Public License for more details.
// You should have received a copy of the GNU General Public License along with tralloc. If not, see <http://www.gnu.org/licenses/>.

#include <tralloc/tests/tree/move/common.h>
#include <tralloc/tree.h>


#if defined ( TRALLOC_DEBUG_THREADS )

typedef struct test_tree_move_errors_data_type {
    tralloc_context * parent_1;
    tralloc_context * parent_2;
    tralloc_context * child_1;
    tralloc_context * child_2;
    tralloc_context * child_3;
    tralloc_context * child_4;
    tralloc_context * child_5;
} test_tree_move_errors_data;

// Thread selects random child and moves it on random parent or NULL.
static
void * _test_tree_move_errors_thread ( void * argument )
{
    test_tree_move_errors_data * data = argument;
    tralloc_error first_error = 0, result;
    tralloc_context * child, * parent;

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

        switch ( index % 5 ) {
        case 0:
            child = data->child_1;
            break;
        case 1:
            child = data->child_2;
            break;
        case 2:
            child = data->child_3;
            break;
        case 3:
            child = data->child_4;
            break;
        default:
            child = data->child_5;
            break;
        }

        result = tralloc_move ( child, parent );
        if ( result != 0 && result != TRALLOC_ERROR_CHILD_HAS_SAME_PARENT && first_error == 0 ) {
            first_error = result;
        }
    }
    return ( void * ) ( ( uintptr_t ) first_error );
}

static inline
tralloc_bool _test_tree_move_errors_subtree ( tralloc_context * ctx )
{
    tralloc_context * parent_1, * parent_2;
    tralloc_context * child_1, * child_2, * child_3, * child_4, * child_5;
    if (
        tralloc_new_empty_with_extensions ( ctx, &parent_1, TRALLOC_EXTENSION_LOCK_CHILDREN ) != 0 ||
        tralloc_new_empty_with_extensions ( ctx, &parent_2, TRALLOC_EXTENSION_LOCK_CHILDREN ) != 0 ||
        tralloc_new_empty ( NULL, &child_1 ) != 0 ||
        tralloc_new_empty ( NULL, &child_2 ) != 0 ||
        tralloc_new_empty ( NULL, &child_3 ) != 0 ||
        tralloc_new_empty ( NULL, &child_4 ) != 0 ||
        tralloc_new_empty ( NULL, &child_5 ) != 0
    ) {
        return TRALLOC_FALSE;
    }

    test_tree_move_errors_data data;
    data.parent_1 = parent_1;
    data.parent_2 = parent_2;
    data.child_1  = child_1;
    data.child_2  = child_2;
    data.child_3  = child_3;
    data.child_4  = child_4;
    data.child_5  = child_5;

    void * result_1, * result_2, * result_3;
    pthread_t thread_1, thread_2, thread_3;

    // 3 threads try to move childs on "parent_1", "parent_2" or NULL.
    // Several moves from one thread will be done normally when another thread is not active. Next moves will fail.
    // Any of "result_1", "result_2" or "result_3" will be an error, which means:
    // Chunk should have subtree lock, if it's subtree pointers ("parent", "next" and "prev") are operating from multiple threads.
    if (
        pthread_create ( &thread_1, NULL, &_test_tree_move_errors_thread, &data ) != 0 ||
        pthread_create ( &thread_2, NULL, &_test_tree_move_errors_thread, &data ) != 0 ||
        pthread_create ( &thread_3, NULL, &_test_tree_move_errors_thread, &data ) != 0 ||
        pthread_join   ( thread_1, &result_1 ) != 0 ||
        pthread_join   ( thread_2, &result_2 ) != 0 ||
        pthread_join   ( thread_3, &result_3 ) != 0 ||
        (
            ( tralloc_error ) ( ( uintptr_t ) result_1 ) != TRALLOC_ERROR_NO_SUBTREE_LOCK &&
            ( tralloc_error ) ( ( uintptr_t ) result_2 ) != TRALLOC_ERROR_NO_SUBTREE_LOCK &&
            ( tralloc_error ) ( ( uintptr_t ) result_3 ) != TRALLOC_ERROR_NO_SUBTREE_LOCK
        )
    ) {
        tralloc_free ( child_1 );
        tralloc_free ( child_2 );
        tralloc_free ( child_3 );
        tralloc_free ( child_4 );
        tralloc_free ( child_5 );
        tralloc_free ( parent_1 );
        tralloc_free ( parent_2 );
        return TRALLOC_FALSE;
    }

    if (
        tralloc_free ( child_1 )  != 0 ||
        tralloc_free ( child_2 )  != 0 ||
        tralloc_free ( child_3 )  != 0 ||
        tralloc_free ( child_4 )  != 0 ||
        tralloc_free ( child_5 )  != 0 ||
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
    tralloc_context * parent_1, * parent_2;
    tralloc_context * child_1, * child_2, * child_3, * child_4, * child_5;
    if (
        tralloc_new_empty ( ctx, &parent_1 ) != 0 ||
        tralloc_new_empty ( ctx, &parent_2 ) != 0 ||
        tralloc_new_empty_with_extensions ( NULL, &child_1, TRALLOC_EXTENSION_LOCK_SUBTREE ) != 0 ||
        tralloc_new_empty_with_extensions ( NULL, &child_2, TRALLOC_EXTENSION_LOCK_SUBTREE ) != 0 ||
        tralloc_new_empty_with_extensions ( NULL, &child_3, TRALLOC_EXTENSION_LOCK_SUBTREE ) != 0 ||
        tralloc_new_empty_with_extensions ( NULL, &child_4, TRALLOC_EXTENSION_LOCK_SUBTREE ) != 0 ||
        tralloc_new_empty_with_extensions ( NULL, &child_5, TRALLOC_EXTENSION_LOCK_SUBTREE ) != 0
    ) {
        return TRALLOC_FALSE;
    }

    test_tree_move_errors_data data;
    data.child_1  = child_1;
    data.child_2  = child_2;
    data.child_3  = child_3;
    data.child_4  = child_4;
    data.child_5  = child_5;
    data.parent_1 = parent_1;
    data.parent_2 = parent_2;

    void * result_1, * result_2, * result_3;
    pthread_t thread_1, thread_2, thread_3;

    // 3 threads try to move childs on "parent_1", "parent_2" or NULL.
    // Several moves to each parent will be done normally when moving from only one thread. Other move will fail.
    // Any of "result_1", "result_2" or "result_3" will be an error, which means:
    // Parent should have lock of it's children list, if any child can be moved to/from different threads.
    if (
        pthread_create ( &thread_1, NULL, &_test_tree_move_errors_thread, &data ) != 0 ||
        pthread_create ( &thread_2, NULL, &_test_tree_move_errors_thread, &data ) != 0 ||
        pthread_create ( &thread_3, NULL, &_test_tree_move_errors_thread, &data ) != 0 ||
        pthread_join   ( thread_1, &result_1 ) != 0 ||
        pthread_join   ( thread_2, &result_2 ) != 0 ||
        pthread_join   ( thread_3, &result_3 ) != 0 ||
        (
            ( tralloc_error ) ( ( uintptr_t ) result_1 ) != TRALLOC_ERROR_NO_CHILDREN_LOCK &&
            ( tralloc_error ) ( ( uintptr_t ) result_2 ) != TRALLOC_ERROR_NO_CHILDREN_LOCK &&
            ( tralloc_error ) ( ( uintptr_t ) result_3 ) != TRALLOC_ERROR_NO_CHILDREN_LOCK
        )
    ) {
        tralloc_free ( child_1 );
        tralloc_free ( child_2 );
        tralloc_free ( child_3 );
        tralloc_free ( child_4 );
        tralloc_free ( child_5 );
        tralloc_free ( parent_1 );
        tralloc_free ( parent_2 );
        return TRALLOC_FALSE;
    }

    tralloc_error result;

    // If "child"'s parent is NULL - it will return 0.
    // If "child"'s parent is not NULL - it will return the same error as described above.
    // In this case "child" will be freed normally after free of it's parent.
    if (
        (
            ( result = tralloc_free ( child_1 ) ) != 0 &&
            result != TRALLOC_ERROR_NO_CHILDREN_LOCK
        ) || (
            ( result = tralloc_free ( child_2 ) ) != 0 &&
            result != TRALLOC_ERROR_NO_CHILDREN_LOCK
        ) || (
            ( result = tralloc_free ( child_3 ) ) != 0 &&
            result != TRALLOC_ERROR_NO_CHILDREN_LOCK
        ) || (
            ( result = tralloc_free ( child_4 ) ) != 0 &&
            result != TRALLOC_ERROR_NO_CHILDREN_LOCK
        ) || (
            ( result = tralloc_free ( child_5 ) ) != 0 &&
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

#   if defined ( TRALLOC_DEBUG_THREADS )
    if (
        !_test_tree_move_errors_subtree  ( ctx ) ||
        !_test_tree_move_errors_children ( ctx )
    ) {
        return TRALLOC_FALSE;
    }
#   endif

    return TRALLOC_TRUE;
}
