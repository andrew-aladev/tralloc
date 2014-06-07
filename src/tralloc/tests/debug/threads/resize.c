// This file is part of tralloc. Copyright (C) 2013 Andrew Aladjev aladjev.andrew@gmail.com
// tralloc is free software: you can redistribute it and/or modify it under the terms of the GNU General Public License as published by the Free Software Foundation, either version 3 of the License, or (at your option) any later version.
// tralloc is distributed in the hope that it will be useful, but WITHOUT ANY WARRANTY; without even the implied warranty of MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU General Public License for more details.
// You should have received a copy of the GNU General Public License along with tralloc. If not, see <http://www.gnu.org/licenses/>.

#include <tralloc/tests/debug/threads/common.h>
#include <tralloc/tree.h>


// static
// void * thread ( tralloc_context * current )
// {
//     return ( void * ) ( ( uintptr_t ) tralloc_realloc ( &current, sizeof ( uintptr_t ) ) );
// }

tralloc_bool test_debug_threads_resize ( tralloc_context * _TRALLOC_UNUSED ( ctx ) )
{
//     tralloc_context * parent;
//     tralloc_context * next;
//     tralloc_context * current;
//     tralloc_context * prev;
//     tralloc_context * first_child;
//     if (
//         tralloc_new ( ctx, &parent )          != 0 ||
//         tralloc_new ( parent, &next )         != 0 ||
//         tralloc_new ( parent, &current )      != 0 ||
//         tralloc_new ( parent, &prev )         != 0 ||
//         tralloc_new ( current, &first_child ) != 0
//     ) {
//         return TRALLOC_FALSE;
//     }
//
//     void * result;
//     pthread_t thread_1;
//
//     if (
//         pthread_create ( &thread_1, NULL, &thread, current ) != 0 ||
//         pthread_join   ( thread_1, &result ) != 0 ||
//         ( tralloc_error ) ( ( uintptr_t ) result ) != TRALLOC_ERROR_NO_PARENT_LOCK ||
//         tralloc_free ( parent ) != 0
//     ) {
//         return TRALLOC_FALSE;
//     }
//
//     if (
//         tralloc_new ( ctx, &parent )  != 0 ||
//         tralloc_new ( parent, &next ) != 0 ||
//         tralloc_with_extensions_new ( parent, &current, TRALLOC_EXTENSION_LOCK_PARENT ) != 0 ||
//         tralloc_new ( parent, &prev )         != 0 ||
//         tralloc_new ( current, &first_child ) != 0
//     ) {
//         return TRALLOC_FALSE;
//     }
//
//     if (
//         pthread_create ( &thread_1, NULL, &thread, current ) != 0 ||
//         pthread_join   ( thread_1, &result ) != 0 ||
//         ( tralloc_error ) ( ( uintptr_t ) result ) != TRALLOC_ERROR_NO_FIRST_CHILD_LOCK ||
//         tralloc_free ( parent ) != 0
//     ) {
//         return TRALLOC_FALSE;
//     }
//
//     if (
//         tralloc_with_extensions_new ( ctx, &parent, TRALLOC_EXTENSION_LOCK_FIRST_CHILD ) != 0 ||
//         tralloc_new ( parent, &next ) != 0 ||
//         tralloc_with_extensions_new ( parent, &current, TRALLOC_EXTENSION_LOCK_PARENT ) != 0 ||
//         tralloc_new ( parent, &prev )         != 0 ||
//         tralloc_new ( current, &first_child ) != 0
//     ) {
//         return TRALLOC_FALSE;
//     }
//
//     if (
//         pthread_create ( &thread_1, NULL, &thread, current ) != 0 ||
//         pthread_join   ( thread_1, &result ) != 0 ||
//         ( tralloc_error ) ( ( uintptr_t ) result ) != TRALLOC_ERROR_NO_PREV_LOCK ||
//         tralloc_free ( parent ) != 0
//     ) {
//         return TRALLOC_FALSE;
//     }
//
//     if (
//         tralloc_with_extensions_new ( ctx, &parent, TRALLOC_EXTENSION_LOCK_FIRST_CHILD ) != 0 ||
//         tralloc_new ( parent, &next ) != 0 ||
//         tralloc_with_extensions_new ( parent, &current, TRALLOC_EXTENSION_LOCK_PARENT | TRALLOC_EXTENSION_LOCK_PREV ) != 0 ||
//         tralloc_new ( parent, &prev )         != 0 ||
//         tralloc_new ( current, &first_child ) != 0
//     ) {
//         return TRALLOC_FALSE;
//     }
//
//     if (
//         pthread_create ( &thread_1, NULL, &thread, current ) != 0 ||
//         pthread_join   ( thread_1, &result ) != 0 ||
//         ( tralloc_error ) ( ( uintptr_t ) result ) != TRALLOC_ERROR_NO_NEXT_LOCK ||
//         tralloc_free ( parent ) != 0
//     ) {
//         return TRALLOC_FALSE;
//     }
//
//     if (
//         tralloc_with_extensions_new ( ctx, &parent, TRALLOC_EXTENSION_LOCK_FIRST_CHILD ) != 0 ||
//         tralloc_new ( parent, &next ) != 0 ||
//         tralloc_with_extensions_new ( parent, &current, TRALLOC_EXTENSION_LOCK_PARENT | TRALLOC_EXTENSION_LOCK_PREV ) != 0 ||
//         tralloc_with_extensions_new ( parent, &prev, TRALLOC_EXTENSION_LOCK_NEXT ) != 0 ||
//         tralloc_new ( current, &first_child ) != 0
//     ) {
//         return TRALLOC_FALSE;
//     }
//
//     if (
//         pthread_create ( &thread_1, NULL, &thread, current ) != 0 ||
//         pthread_join   ( thread_1, &result ) != 0 ||
//         ( tralloc_error ) ( ( uintptr_t ) result ) != TRALLOC_ERROR_NO_NEXT_LOCK ||
//         tralloc_free ( parent ) != 0
//     ) {
//         return TRALLOC_FALSE;
//     }
//
//     if (
//         tralloc_with_extensions_new ( ctx, &parent, TRALLOC_EXTENSION_LOCK_FIRST_CHILD ) != 0 ||
//         tralloc_new ( parent, &next ) != 0 ||
//         tralloc_with_extensions_new ( parent, &current, TRALLOC_EXTENSION_LOCK_PARENT | TRALLOC_EXTENSION_LOCK_PREV | TRALLOC_EXTENSION_LOCK_NEXT ) != 0 ||
//         tralloc_with_extensions_new ( parent, &prev, TRALLOC_EXTENSION_LOCK_NEXT ) != 0 ||
//         tralloc_new ( current, &first_child ) != 0
//     ) {
//         return TRALLOC_FALSE;
//     }
//
//     if (
//         pthread_create ( &thread_1, NULL, &thread, current ) != 0 ||
//         pthread_join   ( thread_1, &result ) != 0 ||
//         ( tralloc_error ) ( ( uintptr_t ) result ) != TRALLOC_ERROR_NO_PREV_LOCK ||
//         tralloc_free ( parent ) != 0
//     ) {
//         return TRALLOC_FALSE;
//     }
//
//     if (
//         tralloc_with_extensions_new ( ctx, &parent, TRALLOC_EXTENSION_LOCK_FIRST_CHILD ) != 0 ||
//         tralloc_with_extensions_new ( parent, &next, TRALLOC_EXTENSION_LOCK_PREV ) != 0 ||
//         tralloc_with_extensions_new ( parent, &current, TRALLOC_EXTENSION_LOCK_PARENT | TRALLOC_EXTENSION_LOCK_PREV | TRALLOC_EXTENSION_LOCK_NEXT ) != 0 ||
//         tralloc_with_extensions_new ( parent, &prev, TRALLOC_EXTENSION_LOCK_NEXT ) != 0 ||
//         tralloc_new ( current, &first_child ) != 0
//     ) {
//         return TRALLOC_FALSE;
//     }
//
//     if (
//         pthread_create ( &thread_1, NULL, &thread, current ) != 0 ||
//         pthread_join   ( thread_1, &result ) != 0 ||
//         ( tralloc_error ) ( ( uintptr_t ) result ) != TRALLOC_ERROR_NO_FIRST_CHILD_LOCK ||
//         tralloc_free ( parent ) != 0
//     ) {
//         return TRALLOC_FALSE;
//     }
//
//     if (
//         tralloc_with_extensions_new ( ctx, &parent, TRALLOC_EXTENSION_LOCK_FIRST_CHILD ) != 0 ||
//         tralloc_with_extensions_new ( parent, &next, TRALLOC_EXTENSION_LOCK_PREV ) != 0 ||
//         tralloc_with_extensions_new ( parent, &current, TRALLOC_EXTENSION_LOCK_PARENT | TRALLOC_EXTENSION_LOCK_PREV | TRALLOC_EXTENSION_LOCK_NEXT | TRALLOC_EXTENSION_LOCK_FIRST_CHILD ) != 0 ||
//         tralloc_with_extensions_new ( parent, &prev, TRALLOC_EXTENSION_LOCK_NEXT ) != 0 ||
//         tralloc_new ( current, &first_child ) != 0
//     ) {
//         return TRALLOC_FALSE;
//     }
//
//     if (
//         pthread_create ( &thread_1, NULL, &thread, current ) != 0 ||
//         pthread_join   ( thread_1, &result ) != 0 ||
//         ( tralloc_error ) ( ( uintptr_t ) result ) != TRALLOC_ERROR_NO_PARENT_LOCK ||
//         tralloc_free ( parent ) != 0
//     ) {
//         return TRALLOC_FALSE;
//     }
//
//     if (
//         tralloc_with_extensions_new ( ctx, &parent, TRALLOC_EXTENSION_LOCK_FIRST_CHILD ) != 0 ||
//         tralloc_with_extensions_new ( parent, &next, TRALLOC_EXTENSION_LOCK_PREV ) != 0 ||
//         tralloc_with_extensions_new ( parent, &current, TRALLOC_EXTENSION_LOCK_PARENT | TRALLOC_EXTENSION_LOCK_PREV | TRALLOC_EXTENSION_LOCK_NEXT | TRALLOC_EXTENSION_LOCK_FIRST_CHILD ) != 0 ||
//         tralloc_with_extensions_new ( parent, &prev, TRALLOC_EXTENSION_LOCK_NEXT ) != 0 ||
//         tralloc_with_extensions_new ( current, &first_child, TRALLOC_EXTENSION_LOCK_PARENT ) != 0
//     ) {
//         return TRALLOC_FALSE;
//     }
//
//     if (
//         pthread_create ( &thread_1, NULL, &thread, current ) != 0 ||
//         pthread_join   ( thread_1, &result ) != 0 ||
//         ( tralloc_error ) ( ( uintptr_t ) result ) != 0 ||
//         tralloc_free ( parent ) != 0
//     ) {
//         return TRALLOC_FALSE;
//     }

    return TRALLOC_TRUE;
}
