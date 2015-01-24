// This file is part of tralloc. Copyright (C) 2013 Andrew Aladjev aladjev.andrew@gmail.com
// tralloc is free software: you can redistribute it and/or modify it under the terms of the GNU General Lesser Public License as published by the Free Software Foundation, either version 3 of the License, or (at your option) any later version.
// tralloc is distributed in the hope that it will be useful, but WITHOUT ANY WARRANTY; without even the implied warranty of MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU General Lesser Public License for more details.
// You should have received a copy of the GNU General Lesser Public License along with tralloc. If not, see <http://www.gnu.org/licenses/>.

#include <tralloc/debug/threads/main.h>
#include <tralloc/debug/threads/lock.h>
#include <tralloc/context.h>

#include <tralloc/tree/chunk.h>
#include <tralloc/tree/locks/subtree.h>
#include <tralloc/tree/locks/children.h>

#if defined ( TRALLOC_POOL )
#   include <tralloc/pool/pool_data.h>
#endif

#if defined ( TRALLOC_DEBUG_LOG )
#   include <tralloc/debug/log/main.h>
#   include <stdio.h>
#endif


static
tralloc_error _tralloc_debug_threads_check_usage_of_extension (
    _tralloc_chunk * chunk,
    pthread_t thread_id,
    _tralloc_extension extension,
    tralloc_error error,
    _tralloc_thread_usage_status * usage_status,
    pthread_t * used_by_thread
)
{
    tralloc_error result = _tralloc_debug_threads_lock_wrlock ( &chunk->thread_usage_lock );
    if ( result != 0 ) {
        return result;
    }

    if ( * usage_status == _TRALLOC_DEBUG_NOT_USED_BY_THREADS ) {
        * usage_status   = _TRALLOC_DEBUG_USED_BY_SINGLE_THREAD;
        * used_by_thread = thread_id;
    } else if ( * usage_status == _TRALLOC_DEBUG_USED_BY_SINGLE_THREAD && !pthread_equal ( * used_by_thread, thread_id ) ) {
        * usage_status = _TRALLOC_DEBUG_USED_BY_MULTIPLE_THREADS;
    }

    tralloc_extensions original_extensions = _tralloc_extensions_get_original_by_forced ( chunk->extensions, chunk->forced_extensions );
    if ( * usage_status == _TRALLOC_DEBUG_USED_BY_MULTIPLE_THREADS && !_tralloc_extensions_have_extension ( original_extensions, extension ) ) {
#       if defined ( TRALLOC_DEBUG_LOG )
        fprintf (
            stderr,
            "%s:%zu error: %s\n",
            chunk->initialized_in_file,
            chunk->initialized_at_line,
            tralloc_debug_log_get_string_for_error ( error )
        );
#       endif

        _tralloc_debug_threads_lock_unlock ( &chunk->thread_usage_lock );
        return error;
    }

    result = _tralloc_debug_threads_lock_unlock ( &chunk->thread_usage_lock );
    if ( result != 0 ) {
        return result;
    }
    return 0;
}

static
tralloc_error _tralloc_debug_threads_check_usage_of_subtree ( _tralloc_chunk * chunk, pthread_t thread_id )
{
    return _tralloc_debug_threads_check_usage_of_extension (
               chunk,
               thread_id,
               TRALLOC_EXTENSION_LOCK_SUBTREE,
               TRALLOC_ERROR_NO_SUBTREE_LOCK,
               &chunk->subtree_usage_status,
               &chunk->subtree_used_by_thread
           );
}

static
tralloc_error _tralloc_debug_threads_check_usage_of_children ( _tralloc_chunk * chunk, pthread_t thread_id )
{
    return _tralloc_debug_threads_check_usage_of_extension (
               chunk,
               thread_id,
               TRALLOC_EXTENSION_LOCK_CHILDREN,
               TRALLOC_ERROR_NO_CHILDREN_LOCK,
               &chunk->children_usage_status,
               &chunk->children_used_by_thread
           );
}

#if defined ( TRALLOC_POOL )
static
tralloc_error _tralloc_debug_threads_check_usage_of_pool ( _tralloc_chunk * chunk, pthread_t thread_id )
{
    return _tralloc_debug_threads_check_usage_of_extension (
               chunk,
               thread_id,
               TRALLOC_EXTENSION_LOCK_POOL,
               TRALLOC_ERROR_NO_POOL_LOCK,
               &chunk->pool_usage_status,
               &chunk->pool_used_by_thread
           );
}
#endif

tralloc_error _tralloc_debug_threads_before_add_chunk ( _tralloc_chunk * parent_chunk )
{
    // Add operation can not take part in threads competition, because it runs in single thread and returns pointer to data only after operation.
    // So chunk should not be checked.

    if ( parent_chunk != NULL ) {
        pthread_t thread_id = pthread_self();

#       if defined ( TRALLOC_POOL )
        // Some chunk wants to be attached to pool from thread_1.
        // Other chunk from pool's children list wants to process other operation from thread_2.
        // In this case pool should have pool lock.

        _tralloc_pool * pool = _tralloc_chunk_get_closest_pool ( parent_chunk );
        if ( pool != NULL ) {
            _tralloc_chunk * pool_chunk = _tralloc_pool_get_chunk ( pool );
            tralloc_error result = _tralloc_debug_threads_check_usage_of_pool ( pool_chunk, thread_id );
            if ( result != 0 ) {
                return result;
            }
        }
#       endif

        // Some chunk wants to be attached to "chunk->parent" from thread_1.
        // Other chunk from "chunk->parent"'s children list wants to process other operation from thread_2.
        // In this case "chunk->parent" should have children lock.
        return _tralloc_debug_threads_check_usage_of_children ( parent_chunk, thread_id );
    }
    return 0;
}

tralloc_error _tralloc_debug_threads_before_move_chunk ( _tralloc_chunk * chunk )
{
    tralloc_error result;

    tralloc_bool have_subtree_lock       = _tralloc_extensions_have_extension ( chunk->extensions, TRALLOC_EXTENSION_LOCK_SUBTREE );
    _tralloc_subtree_lock * subtree_lock = _tralloc_chunk_get_subtree_lock ( chunk );
    if ( have_subtree_lock ) {
        result = _tralloc_subtree_lock_rdlock ( subtree_lock );
        if ( result != 0 ) {
            return result;
        }
    } else {
        return TRALLOC_ERROR_NO_SUBTREE_LOCK;
    }

    _tralloc_chunk * parent_chunk = chunk->parent;

    if ( have_subtree_lock ) {
        result = _tralloc_subtree_lock_unlock ( subtree_lock );
        if ( result != 0 ) {
            return result;
        }
    }

    pthread_t thread_id = pthread_self();

    if ( parent_chunk != NULL ) {
        // "chunk" from "parent_chunk"'s children list wants to process move operation from thread_1.
        // Other chunk from "parent_chunk"'s children list wants to process other operation from thread_2.
        // In this case "parent_chunk" should have children lock.
        result = _tralloc_debug_threads_check_usage_of_children ( parent_chunk, thread_id );
        if ( result != 0 ) {
            return result;
        }
    }

    // "chunk" wants to process move operation to different "parent_chunk"s from different threads.
    // In this case "chunk" should have subtree lock.
    return _tralloc_debug_threads_check_usage_of_subtree ( chunk, thread_id );
}

tralloc_error _tralloc_debug_threads_after_move_chunk ( _tralloc_chunk * chunk )
{
    tralloc_error result;

    tralloc_bool have_subtree_lock       = _tralloc_extensions_have_extension ( chunk->extensions, TRALLOC_EXTENSION_LOCK_SUBTREE );
    _tralloc_subtree_lock * subtree_lock = _tralloc_chunk_get_subtree_lock ( chunk );
    if ( have_subtree_lock ) {
        result = _tralloc_subtree_lock_rdlock ( subtree_lock );
        if ( result != 0 ) {
            return result;
        }
    } else {
        return TRALLOC_ERROR_NO_SUBTREE_LOCK;
    }

    _tralloc_chunk * parent_chunk = chunk->parent;

    if ( have_subtree_lock ) {
        result = _tralloc_subtree_lock_unlock ( subtree_lock );
        if ( result != 0 ) {
            return result;
        }
    }

    if ( parent_chunk != NULL ) {
        // "chunk" from new "parent_chunk"'s children list wants to end move operation from thread_1.
        // Other chunk from new "parent_chunk"'s children list wants to process other operation from thread_2.
        // In this case new "parent_chunk" should have children lock.
        return _tralloc_debug_threads_check_usage_of_children ( parent_chunk, pthread_self() );
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

    tralloc_error result;

    tralloc_bool have_subtree_lock       = _tralloc_extensions_have_extension ( chunk->extensions, TRALLOC_EXTENSION_LOCK_SUBTREE );
    _tralloc_subtree_lock * subtree_lock = _tralloc_chunk_get_subtree_lock ( chunk );
    if ( have_subtree_lock ) {
        result = _tralloc_subtree_lock_rdlock ( subtree_lock );
        if ( result != 0 ) {
            return result;
        }
    } else {
        return TRALLOC_ERROR_NO_SUBTREE_LOCK;
    }

    _tralloc_chunk * parent_chunk = chunk->parent;

    if ( have_subtree_lock ) {
        result = _tralloc_subtree_lock_unlock ( subtree_lock );
        if ( result != 0 ) {
            return result;
        }
    }

    if ( parent_chunk != NULL ) {
        // "chunk" from "parent_chunk"'s children will be freed from thread_1.
        // Other chunk from "parent_chunk"'s children list wants to process other operation from thread_2.
        // In this case "parent_chunk" should have children lock.
        return _tralloc_debug_threads_check_usage_of_children ( parent_chunk, pthread_self() );
    }
    return 0;
}

tralloc_error _tralloc_debug_threads_before_free_chunk ( _tralloc_chunk * _TRALLOC_UNUSED ( chunk ) )
{
    return 0;
}

tralloc_error _tralloc_debug_threads_before_refuse_to_free_subtree ( _tralloc_chunk * chunk )
{
    // If subtree with root "chunk" will refuse to be freed - it will be detached.
    // So this operation equals movement to NULL.

    // "chunk" wants to process move operation to NULL from different threads.
    // In this case "chunk" should have subtree lock.
    return _tralloc_debug_threads_check_usage_of_subtree ( chunk, pthread_self() );
}

tralloc_error _tralloc_debug_threads_before_refuse_to_free_chunk ( _tralloc_chunk * chunk )
{
    // If "chunk" will refuse to be freed - it will be detached.
    // So this operation equals movement to NULL.

    // "chunk" wants to process move operation to NULL from different threads.
    // In this case "chunk" should have subtree lock.
    return _tralloc_debug_threads_check_usage_of_subtree ( chunk, pthread_self() );
}
