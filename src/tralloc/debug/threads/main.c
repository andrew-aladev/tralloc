// This file is part of tralloc. Copyright (C) 2013 Andrew Aladjev aladjev.andrew@gmail.com
// tralloc is free software: you can redistribute it and/or modify it under the terms of the GNU General Lesser Public License as published by the Free Software Foundation, either version 3 of the License, or (at your option) any later version.
// tralloc is distributed in the hope that it will be useful, but WITHOUT ANY WARRANTY; without even the implied warranty of MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU General Lesser Public License for more details.
// You should have received a copy of the GNU General Lesser Public License along with tralloc. If not, see <http://www.gnu.org/licenses/>.

#include <tralloc/debug/threads/main.h>
#include <tralloc/debug/threads/lock.h>
#include <tralloc/debug/common.h>
#include <tralloc/common.h>

#include <tralloc/threads/lock/subtree.h>
#include <tralloc/threads/lock/children.h>

#if defined ( TRALLOC_DEBUG_LOG )
#   include <stdio.h>
#endif


static inline
tralloc_error _tralloc_check_subtree_lock ( _tralloc_chunk * chunk, pthread_t thread_id )
{
    tralloc_error result = _tralloc_wrlock_debug_threads ( &chunk->thread_usage_lock );
    if ( result != 0 ) {
        return result;
    }

    if ( chunk->subtree_usage_status == _TRALLOC_NOT_USED_BY_THREADS ) {
        chunk->subtree_usage_status   = _TRALLOC_USED_BY_SINGLE_THREAD;
        chunk->subtree_used_by_thread = thread_id;
    } else if (
        chunk->subtree_usage_status == _TRALLOC_USED_BY_SINGLE_THREAD &&
        !pthread_equal ( chunk->subtree_used_by_thread, thread_id )
    ) {
        chunk->subtree_usage_status = _TRALLOC_USED_BY_MULTIPLE_THREADS;
    }

    if (
        chunk->subtree_usage_status == _TRALLOC_USED_BY_MULTIPLE_THREADS &&
        ! ( chunk->extensions & TRALLOC_EXTENSION_LOCK_SUBTREE )
    ) {
#       if defined ( TRALLOC_DEBUG_LOG )
        fprintf (
            stderr,
            "%s:%zu error: %s\n",
            chunk->initialized_in_file,
            chunk->initialized_at_line,
            tralloc_debug_get_string_for_error ( TRALLOC_ERROR_NO_SUBTREE_LOCK )
        );
#       endif

        _tralloc_unlock_debug_threads ( &chunk->thread_usage_lock );
        return TRALLOC_ERROR_NO_SUBTREE_LOCK;
    }

    result = _tralloc_unlock_debug_threads ( &chunk->thread_usage_lock );
    if ( result != 0 ) {
        return result;
    }
    return 0;
}

static inline
tralloc_error _tralloc_check_children_lock ( _tralloc_chunk * chunk, pthread_t thread_id )
{
    tralloc_error result = _tralloc_wrlock_debug_threads ( &chunk->thread_usage_lock );
    if ( result != 0 ) {
        return result;
    }

    if ( chunk->children_usage_status == _TRALLOC_NOT_USED_BY_THREADS ) {
        chunk->children_usage_status   = _TRALLOC_USED_BY_SINGLE_THREAD;
        chunk->children_used_by_thread = thread_id;
    } else if (
        chunk->children_usage_status == _TRALLOC_USED_BY_SINGLE_THREAD &&
        !pthread_equal ( chunk->children_used_by_thread, thread_id )
    ) {
        chunk->children_usage_status = _TRALLOC_USED_BY_MULTIPLE_THREADS;
    }

    if (
        chunk->children_usage_status == _TRALLOC_USED_BY_MULTIPLE_THREADS &&
        ! ( chunk->extensions & TRALLOC_EXTENSION_LOCK_CHILDREN )
    ) {
#       if defined ( TRALLOC_DEBUG_LOG )
        fprintf (
            stderr,
            "%s:%zu error: %s\n",
            chunk->initialized_in_file,
            chunk->initialized_at_line,
            tralloc_debug_get_string_for_error ( TRALLOC_ERROR_NO_CHILDREN_LOCK )
        );
#       endif

        _tralloc_unlock_debug_threads ( &chunk->thread_usage_lock );
        return TRALLOC_ERROR_NO_CHILDREN_LOCK;
    }

    result = _tralloc_unlock_debug_threads ( &chunk->thread_usage_lock );
    if ( result != 0 ) {
        return result;
    }
    return 0;
}

tralloc_error _tralloc_debug_threads_before_add_chunk ( _tralloc_chunk * parent_chunk, tralloc_extensions _TRALLOC_UNUSED ( extensions ) )
{
    // Add operation can not take part in threads competition, because it runs in single thread and returns pointer to data only after operation.
    // So chunk should not be checked.

    if ( parent_chunk != NULL ) {
        // Some chunk wants to attach to "chunk->parent" from thread_1.
        // Other chunk from "chunk->parent"'s children list wants to process other operation from thread_2.
        // In this case "chunk->parent" should have children lock.
        return _tralloc_check_children_lock ( parent_chunk, pthread_self() );
    }
    return 0;
}

tralloc_error _tralloc_debug_threads_after_add_chunk ( _tralloc_chunk * chunk )
{
    chunk->subtree_usage_status  = _TRALLOC_NOT_USED_BY_THREADS;
    chunk->children_usage_status = _TRALLOC_NOT_USED_BY_THREADS;

    tralloc_error result;
    if (
        ( result = _tralloc_new_debug_threads_lock ( &chunk->thread_usage_lock ) ) != 0 ||
        ( result = _tralloc_new_subtree_lock       ( &chunk->subtree_lock )      ) != 0 ||
        ( result = _tralloc_new_children_lock      ( &chunk->children_lock )     ) != 0
    ) {
        return result;
    }
    return 0;
}

tralloc_error _tralloc_debug_threads_before_move_chunk ( _tralloc_chunk * chunk )
{
    tralloc_error result = _tralloc_rdlock_subtree ( &chunk->subtree_lock );
    if ( result != 0 ) {
        return result;
    }

    _tralloc_chunk * parent_chunk = chunk->parent;
    result = _tralloc_unlock_subtree ( &chunk->subtree_lock );
    if ( result != 0 ) {
        return result;
    }

    pthread_t thread_id = pthread_self();

    if ( parent_chunk != NULL ) {
        // "chunk" from "parent_chunk"'s children list wants to process move operation from thread_1.
        // Other chunk from "parent_chunk"'s children list wants to process other operation from thread_2.
        // In this case "parent_chunk" should have children lock.
        result = _tralloc_check_children_lock ( parent_chunk, thread_id );
        if ( result != 0 ) {
            return result;
        }
    }

    // "chunk" wants to process move operation to different "parent_chunk"s from different threads.
    // In this case "chunk" should have subtree lock.
    return _tralloc_check_subtree_lock ( chunk, thread_id );
}

tralloc_error _tralloc_debug_threads_after_move_chunk ( _tralloc_chunk * chunk )
{
    tralloc_error result = _tralloc_rdlock_subtree ( &chunk->subtree_lock );
    if ( result != 0 ) {
        return result;
    }

    _tralloc_chunk * parent_chunk = chunk->parent;
    result = _tralloc_unlock_subtree ( &chunk->subtree_lock );
    if ( result != 0 ) {
        return result;
    }

    if ( parent_chunk != NULL ) {
        // "chunk" from new "parent_chunk"'s children list wants to end move operation from thread_1.
        // Other chunk from new "parent_chunk"'s children list wants to process other operation from thread_2.
        // In this case new "parent_chunk" should have children lock.
        return _tralloc_check_children_lock ( parent_chunk, pthread_self() );
    }
    return 0;
}

tralloc_error _tralloc_debug_threads_before_resize_chunk ( _tralloc_chunk * _TRALLOC_UNUSED ( chunk ) )
{
    /*
    // Resize operation can not take part in threads competition, because it runs in single thread and returns pointer to valid data only after operation.
    // So chunk should not be checked.

    _tralloc_mutex * subtree_mutex = &chunk->subtree_mutex;
    tralloc_error result = _tralloc_mutex_lock ( subtree_mutex );
    if ( result != 0 ) {
        return result;
    }

    _tralloc_chunk * parent_chunk = chunk->parent;
    result = _tralloc_mutex_unlock ( subtree_mutex );
    if ( result != 0 ) {
        return result;
    }

    pthread_t thread_id = pthread_self();

    if ( parent_chunk != NULL ) {
        // "chunk" from "parent_chunk"'s children list wants to process resize operation from thread_1.
        // Other chunk from "parent_chunk"'s children list wants to process other operation from thread_2.
        // In this case "parent_chunk" should have children lock.
        result = _tralloc_check_children_lock ( parent_chunk, thread_id );
        if ( result != 0 ) {
            return result;
        }
    }

    // "chunk" wants to process resize operation from thread_1.
    // Other chunk from "chunk"'s children list wants to process other operation from thread_2.
    // In this case "parent" of this other chunk can be invalid.
    // So "chunk"'s children should have subtree lock.

    _tralloc_chunk * child_chunk = chunk->first_child;
    while ( child_chunk != NULL ) {
        result = _tralloc_check_subtree_lock ( child_chunk, thread_id );
        if ( result != 0 ) {
            return result;
        }
        child_chunk = child_chunk->next;
    }
    */

    return 0;
}

tralloc_error _tralloc_debug_threads_after_resize_chunk ( _tralloc_chunk * _TRALLOC_UNUSED ( chunk ) )
{
    return 0;
}

tralloc_error _tralloc_debug_threads_before_free_subtree ( _tralloc_chunk * chunk )
{
    // Free operation can not take part in threads competition, because pointer will become invalid after this operation.
    // So chunk should not be checked.

    tralloc_error result = _tralloc_rdlock_subtree ( &chunk->subtree_lock );
    if ( result != 0 ) {
        return result;
    }

    _tralloc_chunk * parent_chunk = chunk->parent;
    result = _tralloc_unlock_subtree ( &chunk->subtree_lock );
    if ( result != 0 ) {
        return result;
    }

    if ( parent_chunk != NULL ) {
        // "chunk" from "parent_chunk"'s children will be freed from thread_1.
        // Other chunk from "parent_chunk"'s children list wants to process other operation from thread_2.
        // In this case "parent_chunk" should have children lock.
        return _tralloc_check_children_lock ( parent_chunk, pthread_self() );
    }
    return 0;
}

tralloc_error _tralloc_debug_threads_before_free_chunk ( _tralloc_chunk * chunk )
{
    tralloc_error error = 0;

    tralloc_error result = _tralloc_free_debug_threads_lock ( &chunk->thread_usage_lock );
    if ( result != 0 ) {
        error = result;
    }
    result = _tralloc_free_subtree_lock ( &chunk->subtree_lock );
    if ( result != 0 ) {
        error = result;
    }
    result = _tralloc_free_children_lock ( &chunk->children_lock );
    if ( result != 0 ) {
        error = result;
    }

    return error;
}

tralloc_error _tralloc_debug_threads_before_refuse_to_free_subtree ( _tralloc_chunk * chunk )
{
    // If subtree with root "chunk" will refuse to be freed - it will be detached.
    // So this operation equals movement to NULL.

    // "chunk" wants to process move operation to NULL from different threads.
    // In this case "chunk" should have subtree lock.
    return _tralloc_check_subtree_lock ( chunk, pthread_self() );
}

tralloc_error _tralloc_debug_threads_before_refuse_to_free_chunk ( _tralloc_chunk * chunk )
{
    // If "chunk" will refuse to be freed - it will be detached.
    // So this operation equals movement to NULL.

    // "chunk" wants to process move operation to NULL from different threads.
    // In this case "chunk" should have subtree lock.
    return _tralloc_check_subtree_lock ( chunk, pthread_self() );
}