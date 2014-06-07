// This file is part of tralloc. Copyright (C) 2013 Andrew Aladjev aladjev.andrew@gmail.com
// tralloc is free software: you can redistribute it and/or modify it under the terms of the GNU General Public License as published by the Free Software Foundation, either version 3 of the License, or (at your option) any later version.
// tralloc is distributed in the hope that it will be useful, but WITHOUT ANY WARRANTY; without even the implied warranty of MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU General Public License for more details.
// You should have received a copy of the GNU General Public License along with tralloc. If not, see <http://www.gnu.org/licenses/>.

#include <tralloc/tests/debug/threads/common.h>
#include <tralloc/tree.h>


// typedef struct _arguments_type {
//     tralloc_context * ctx;
//     tralloc_extensions extensions;
// } _arguments;

// static
// void * thread ( void * args )
// {
//     _arguments * arguments = args;
//     tralloc_context * test;
//     tralloc_error result;
//     if (
//         ( result = tralloc_empty_with_extensions ( arguments->ctx, &test, arguments->extensions ) ) != 0 ||
//         ( result = tralloc_free ( test ) ) != 0
//     ) {
//         return ( void * ) ( ( uintptr_t ) result );
//     }
//     return NULL;
// }

static
tralloc_bool test_parent_and_first_child ( tralloc_context * _TRALLOC_UNUSED ( ctx ) )
{
//     tralloc_context * parent_1;
//     if ( tralloc_new ( ctx, &parent_1 ) != 0 ) {
//         return TRALLOC_FALSE;
//     }
//
//     void * result;
//     pthread_t thread_1;
//
//     _arguments arguments;
//     arguments.ctx        = parent_1;
//     arguments.extensions = 0;
//     if (
//         pthread_create ( &thread_1, NULL, &thread, &arguments ) != 0 ||
//         pthread_join   ( thread_1, &result ) != 0
//         ( tralloc_error ) ( ( uintptr_t ) result ) != TRALLOC_ERROR_NO_PARENT_LOCK
//     ) {
//         return TRALLOC_FALSE;
//     }

//     arguments.extensions = TRALLOC_EXTENSION_LOCK_PARENT;
//     if (
//         pthread_create ( &thread_1, NULL, &thread, &arguments ) != 0 ||
//         pthread_join   ( thread_1, &result ) != 0 ||
//         ( tralloc_error ) ( ( uintptr_t ) result ) != TRALLOC_ERROR_NO_FIRST_CHILD_LOCK ||
//         tralloc_free ( parent_1 ) != 0
//     ) {
//         return TRALLOC_FALSE;
//     }
//
//     tralloc_context * parent_2;
//     if ( tralloc_with_extensions_new ( ctx, &parent_2, TRALLOC_EXTENSION_LOCK_FIRST_CHILD ) != 0 ) {
//         return TRALLOC_FALSE;
//     }
//
//     arguments.ctx = parent_2;
//     if (
//         pthread_create ( &thread_1, NULL, &thread, &arguments ) != 0 ||
//         pthread_join   ( thread_1, &result ) != 0 ||
//         ( tralloc_error ) ( ( uintptr_t ) result ) != 0 ||
//         tralloc_free ( parent_2 ) != 0
//     ) {
//         return TRALLOC_FALSE;
//     }

    return TRALLOC_TRUE;
}

static
tralloc_bool test_prev_and_next ( tralloc_context * _TRALLOC_UNUSED ( ctx ) )
{
//     tralloc_context * parent;
//     tralloc_context * next_1;
//     if (
//         tralloc_with_extensions_new ( ctx, &parent, TRALLOC_EXTENSION_LOCK_FIRST_CHILD ) != 0 ||
//         tralloc_new ( parent, &next_1 ) != 0
//     ) {
//         return TRALLOC_FALSE;
//     }
//
//     void * result;
//     pthread_t thread_1;
//
//     _arguments arguments;
//     arguments.ctx        = parent;
//     arguments.extensions = TRALLOC_EXTENSION_LOCK_PARENT;
//     if (
//         pthread_create ( &thread_1, NULL, &thread, &arguments ) != 0 ||
//         pthread_join   ( thread_1, &result ) != 0
//         ( tralloc_error ) ( ( uintptr_t ) result ) != TRALLOC_ERROR_NO_NEXT_LOCK
//     ) {
//         return TRALLOC_FALSE;
//     }
//
//     arguments.extensions = TRALLOC_EXTENSION_LOCK_PARENT | TRALLOC_EXTENSION_LOCK_NEXT;
//     if (
//         pthread_create ( &thread_1, NULL, &thread, &arguments ) != 0 ||
//         pthread_join   ( thread_1, &result ) != 0 ||
//         ( tralloc_error ) ( ( uintptr_t ) result ) != TRALLOC_ERROR_NO_PREV_LOCK ||
//         tralloc_free ( next_1 ) != 0
//     ) {
//         return TRALLOC_FALSE;
//     }
//
//     tralloc_context * next_2;
//     if ( tralloc_with_extensions_new ( parent, &next_2, TRALLOC_EXTENSION_LOCK_PREV ) != 0 ) {
//         return TRALLOC_FALSE;
//     }
//
//     if (
//         pthread_create ( &thread_1, NULL, &thread, &arguments ) != 0 ||
//         pthread_join   ( thread_1, &result ) != 0 ||
//         ( tralloc_error ) ( ( uintptr_t ) result ) != 0 ||
//         tralloc_free ( next_2 ) != 0
//     ) {
//         return TRALLOC_FALSE;
//     }
//
//     if ( tralloc_free ( parent ) != 0 ) {
//         return TRALLOC_FALSE;
//     }

    return TRALLOC_TRUE;
}

tralloc_bool test_debug_threads_add ( tralloc_context * ctx )
{
    if (
        !test_parent_and_first_child ( ctx ) ||
        !test_prev_and_next          ( ctx )
    ) {
        return TRALLOC_FALSE;
    }
    return TRALLOC_TRUE;
}
