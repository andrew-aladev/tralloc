// This file is part of tralloc. Copyright (C) 2013 Andrew Aladjev aladjev.andrew@gmail.com
// tralloc is free software: you can redistribute it and/or modify it under the terms of the GNU General Lesser Public License as published by the Free Software Foundation, either version 3 of the License, or (at your option) any later version.
// tralloc is distributed in the hope that it will be useful, but WITHOUT ANY WARRANTY; without even the implied warranty of MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU General Lesser Public License for more details.
// You should have received a copy of the GNU General Lesser Public License along with tralloc. If not, see <http://www.gnu.org/licenses/>.

#include "../../macro.h"
#include "../../common.h"
#include "chunk.h"

#if defined(TRALLOC_DEBUG_LOG)
#   include <stdio.h>
#endif


static inline
tralloc_error _check_chunk ( _tralloc_chunk * chunk )
{

#   if TRALLOC_DEBUG_THREADS_CHILDREN == TRALLOC_SPINLOCK
    if ( pthread_spin_lock ( &chunk->thread_usage_lock ) != 0 ) {
        return TRALLOC_ERROR_SPINLOCK_FAILED;
    }
#   elif TRALLOC_DEBUG_THREADS_CHILDREN == TRALLOC_MUTEX
    if ( pthread_mutex_lock ( &chunk->thread_usage_lock ) != 0 ) {
        return TRALLOC_ERROR_MUTEX_FAILED;
    }
#   endif

    tralloc_error error = 0;
    pthread_t thread_id = pthread_self();

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
#       if defined(TRALLOC_DEBUG_LOG)
        fprintf (
            stderr,
            "%s:%zu error: %s\n",
            chunk->initialized_in_file,
            chunk->initialized_at_line,
            tralloc_get_string_for_error ( TRALLOC_ERROR_NO_SUBTREE_LOCK )
        );
#       endif
        error = TRALLOC_ERROR_NO_SUBTREE_LOCK;
    }

#   if TRALLOC_DEBUG_THREADS_CHILDREN == TRALLOC_SPINLOCK
    if ( pthread_spin_unlock ( &chunk->thread_usage_lock ) != 0 ) {
        return TRALLOC_ERROR_SPINLOCK_FAILED;
    }
#   elif TRALLOC_DEBUG_THREADS_CHILDREN == TRALLOC_MUTEX
    if ( pthread_mutex_unlock ( &chunk->thread_usage_lock ) != 0 ) {
        return TRALLOC_ERROR_MUTEX_FAILED;
    }
#   endif

    return error;
}

static inline
tralloc_error _check_parent_chunk ( _tralloc_chunk * parent_chunk )
{

#   if TRALLOC_DEBUG_THREADS_CHILDREN == TRALLOC_SPINLOCK
    if ( pthread_spin_lock ( &parent_chunk->thread_usage_lock ) != 0 ) {
        return TRALLOC_ERROR_SPINLOCK_FAILED;
    }
#   elif TRALLOC_DEBUG_THREADS_CHILDREN == TRALLOC_MUTEX
    if ( pthread_mutex_lock ( &parent_chunk->thread_usage_lock ) != 0 ) {
        return TRALLOC_ERROR_MUTEX_FAILED;
    }
#   endif

    tralloc_error error = 0;
    pthread_t thread_id = pthread_self();

    if ( parent_chunk->children_usage_status == _TRALLOC_NOT_USED_BY_THREADS ) {
        parent_chunk->children_usage_status   = _TRALLOC_USED_BY_SINGLE_THREAD;
        parent_chunk->children_used_by_thread = thread_id;
    } else if (
        parent_chunk->children_usage_status == _TRALLOC_USED_BY_SINGLE_THREAD &&
        !pthread_equal ( parent_chunk->children_used_by_thread, thread_id )
    ) {
        parent_chunk->children_usage_status = _TRALLOC_USED_BY_MULTIPLE_THREADS;
    }

    if (
        parent_chunk->children_usage_status == _TRALLOC_USED_BY_MULTIPLE_THREADS &&
        ! ( parent_chunk->extensions & TRALLOC_EXTENSION_LOCK_CHILDREN )
    ) {
#       if defined(TRALLOC_DEBUG_LOG)
        fprintf (
            stderr,
            "%s:%zu error: %s\n",
            parent_chunk->initialized_in_file,
            parent_chunk->initialized_at_line,
            tralloc_get_string_for_error ( TRALLOC_ERROR_NO_CHILDREN_LOCK )
        );
#       endif
        error = TRALLOC_ERROR_NO_CHILDREN_LOCK;
    }

#   if TRALLOC_DEBUG_THREADS_CHILDREN == TRALLOC_SPINLOCK
    if ( pthread_spin_unlock ( &parent_chunk->thread_usage_lock ) != 0 ) {
        return TRALLOC_ERROR_SPINLOCK_FAILED;
    }
#   elif TRALLOC_DEBUG_THREADS_CHILDREN == TRALLOC_MUTEX
    if ( pthread_mutex_unlock ( &parent_chunk->thread_usage_lock ) != 0 ) {
        return TRALLOC_ERROR_MUTEX_FAILED;
    }
#   endif

    return error;
}

tralloc_error _tralloc_debug_threads_before_add_chunk ( _tralloc_chunk * parent_chunk, tralloc_extensions _TRALLOC_UNUSED ( extensions ) )
{
    if ( parent_chunk != NULL ) {
        // You can have chunk, that wants to attach to "chunk->parent" from thread_1.
        // Other chunk from "chunk->parent"'s list wants to process other operation from thread_2.
        // In this case "chunk->parent" should have children lock.
        return _check_parent_chunk ( parent_chunk );
    }
    return 0;
}

tralloc_error _tralloc_debug_threads_after_add_chunk ( _tralloc_chunk * chunk )
{

#   if TRALLOC_DEBUG_THREADS_LENGTH == TRALLOC_SPINLOCK
    if ( pthread_spin_init ( &chunk->length_lock, 0 ) != 0 ) {
        return TRALLOC_ERROR_SPINLOCK_FAILED;
    }
#   elif TRALLOC_DEBUG_THREADS_LENGTH == TRALLOC_MUTEX
    if ( pthread_mutex_init ( &chunk->length_lock, NULL ) != 0 ) {
        return TRALLOC_ERROR_MUTEX_FAILED;
    }
#   endif

    chunk->subtree_used_by_thread = _TRALLOC_NOT_USED_BY_THREADS;
    chunk->children_usage_status  = _TRALLOC_NOT_USED_BY_THREADS;

#   if TRALLOC_DEBUG_THREADS_CHILDREN == TRALLOC_SPINLOCK
    if ( pthread_spin_init ( &chunk->thread_usage_lock, 0 ) != 0 ) {
        return TRALLOC_ERROR_SPINLOCK_FAILED;
    }
#   elif TRALLOC_DEBUG_THREADS_CHILDREN == TRALLOC_MUTEX
    if ( pthread_mutex_init ( &chunk->thread_usage_lock, NULL ) != 0 ) {
        return TRALLOC_ERROR_MUTEX_FAILED;
    }
#   endif

    return 0;
}

tralloc_error _tralloc_debug_threads_before_resize_chunk ( _tralloc_chunk * chunk )
{
    if ( chunk->parent != NULL ) {
        // You can have "chunk" from "chunk->parent"'s list, that wants to process resize operation from thread_1.
        // Other chunk from "chunk->parent"'s list wants to process other operation from thread_2.
        // In this case "chunk->parent" should have children lock.
        return _check_parent_chunk ( chunk->parent );
    }
    return 0;
}

tralloc_error _tralloc_debug_threads_after_resize_chunk ( _tralloc_chunk * _TRALLOC_UNUSED ( chunk ) )
{
    return 0;
}

tralloc_error _tralloc_debug_threads_before_move_chunk ( _tralloc_chunk * chunk )
{
    if ( chunk->parent != NULL ) {
        // You can have "chunk" from "chunk->parent"'s list, that wants to process move operation from thread_1.
        // Other chunk from "chunk->parent"'s list wants to process other operation from thread_2.
        // In this case "chunk->parent" should have children lock.
        return _check_parent_chunk ( chunk->parent );
    }
    return 0;
}

tralloc_error _tralloc_debug_threads_after_move_chunk ( _tralloc_chunk * chunk )
{
    if ( chunk->parent != NULL ) {
        // You can have "chunk" from new "chunk->parent"'s list, that ends move operation from thread_1.
        // Other chunk from new "chunk->parent"'s list wants to process other operation from thread_2.
        // In this case new "chunk->parent" should have children lock.
        return _check_parent_chunk ( chunk->parent );
    }
    return 0;
}

tralloc_error _tralloc_debug_threads_before_free_chunk ( _tralloc_chunk * _TRALLOC_UNUSED ( chunk ) )
{
    return 0;
}

tralloc_error _tralloc_debug_threads_after_free_chunk ( _tralloc_chunk * chunk )
{

#   if TRALLOC_DEBUG_THREADS_LENGTH == TRALLOC_SPINLOCK
    if ( pthread_spin_destroy ( &chunk->length_lock ) != 0 ) {
        return TRALLOC_ERROR_SPINLOCK_FAILED;
    }
#   elif TRALLOC_DEBUG_THREADS_LENGTH == TRALLOC_MUTEX
    if ( pthread_mutex_destroy ( &chunk->length_lock ) != 0 ) {
        return TRALLOC_ERROR_MUTEX_FAILED;
    }
#   endif

#   if TRALLOC_DEBUG_THREADS_CHILDREN == TRALLOC_SPINLOCK
    if ( pthread_spin_destroy ( &chunk->thread_usage_lock ) != 0 ) {
        return TRALLOC_ERROR_SPINLOCK_FAILED;
    }
#   elif TRALLOC_DEBUG_THREADS_CHILDREN == TRALLOC_MUTEX
    if ( pthread_mutex_destroy ( &chunk->thread_usage_lock ) != 0 ) {
        return TRALLOC_ERROR_MUTEX_FAILED;
    }
#   endif

    return 0;
}
