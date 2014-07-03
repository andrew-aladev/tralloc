// This file is part of tralloc. Copyright (C) 2013 Andrew Aladjev aladjev.andrew@gmail.com
// tralloc is free software: you can redistribute it and/or modify it under the terms of the GNU General Public License as published by the Free Software Foundation, either version 3 of the License, or (at your option) any later version.
// tralloc is distributed in the hope that it will be useful, but WITHOUT ANY WARRANTY; without even the implied warranty of MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU General Public License for more details.
// You should have received a copy of the GNU General Public License along with tralloc. If not, see <http://www.gnu.org/licenses/>.

#include <tralloc/tests/tree/move/common.h>
#include <tralloc/tree.h>

#include <stdlib.h>
#include <time.h>


typedef struct test_tree_move_data_type {
    tralloc_context * parent_1;
    tralloc_context * parent_2;
    tralloc_context * parent_3;
    tralloc_context * child_1;
    tralloc_context * child_2;
    tralloc_context * child_3;
} test_tree_move_data;

// Thread selects random child and moves it on random parent or NULL.
static
void * thread ( void * argument )
{
    test_tree_move_data * data = argument;
    tralloc_context * parent;
    tralloc_context * child;

    for ( size_t index = 0; index < 20; index ++ ) {
        switch ( rand() % 4 ) {
        case 0:
            parent = data->parent_1;
            break;
        case 1:
            parent = data->parent_2;
            break;
        case 2:
            parent = data->parent_3;
            break;
        default:
            parent = NULL;
            break;
        }

        switch ( rand() % 3 ) {
        case 0:
            child = data->child_1;
            break;
        case 1:
            child = data->child_2;
            break;
        default:
            child = data->child_3;
            break;
        }

        tralloc_error result = tralloc_move ( child, parent );
        if ( result != 0 && result != TRALLOC_ERROR_CHILD_HAS_SAME_PARENT ) {
            return ( void * ) ( ( uintptr_t ) result );
        }
    }
    return NULL;
}

tralloc_bool test_tree_move_threads ( tralloc_context * ctx )
{
    test_tree_move_data data;
    tralloc_context * parent_1, * parent_2, * parent_3;
    tralloc_context * child_1, * child_2, * child_3;
    if (
        tralloc_new_empty_with_extensions ( ctx, &parent_1, TRALLOC_EXTENSION_LOCK_CHILDREN ) != 0 ||
        tralloc_new_empty_with_extensions ( ctx, &parent_2, TRALLOC_EXTENSION_LOCK_CHILDREN ) != 0 ||
        tralloc_new_empty_with_extensions ( ctx, &parent_3, TRALLOC_EXTENSION_LOCK_CHILDREN ) != 0 ||
        tralloc_new_empty_with_extensions ( NULL, &child_1, TRALLOC_EXTENSION_LOCK_SUBTREE )  != 0 ||
        tralloc_new_empty_with_extensions ( NULL, &child_2, TRALLOC_EXTENSION_LOCK_SUBTREE )  != 0 ||
        tralloc_new_empty_with_extensions ( NULL, &child_3, TRALLOC_EXTENSION_LOCK_SUBTREE )  != 0
    ) {
        return TRALLOC_FALSE;
    }

    data.parent_1 = parent_1;
    data.parent_2 = parent_2;
    data.parent_3 = parent_3;
    data.child_1  = child_1;
    data.child_2  = child_2;
    data.child_3  = child_3;

    void * result;
    pthread_t thread_1, thread_2, thread_3;

    srand ( time ( NULL ) );

    // This is a competition between 3 childs to be moved on 3 parents or NULL.
    if (
        pthread_create ( &thread_1, NULL, &thread, &data )  != 0 ||
        pthread_create ( &thread_2, NULL, &thread, &data )  != 0 ||
        pthread_create ( &thread_3, NULL, &thread, &data )  != 0 ||
        pthread_join   ( thread_1, &result ) != 0 || result != 0 ||
        pthread_join   ( thread_2, &result ) != 0 || result != 0 ||
        pthread_join   ( thread_3, &result ) != 0 || result != 0
    ) {
        return TRALLOC_FALSE;
    }

    if (
        tralloc_free ( child_1 )  != 0 ||
        tralloc_free ( child_2 )  != 0 ||
        tralloc_free ( child_3 )  != 0 ||
        tralloc_free ( parent_1 ) != 0 ||
        tralloc_free ( parent_2 ) != 0 ||
        tralloc_free ( parent_3 ) != 0
    ) {
        return TRALLOC_FALSE;
    }
    return TRALLOC_TRUE;
}
