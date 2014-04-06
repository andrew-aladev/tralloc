// This file is part of tralloc. Copyright (C) 2013 Andrew Aladjev aladjev.andrew@gmail.com
// tralloc is free software: you can redistribute it and/or modify it under the terms of the GNU General Lesser Public License as published by the Free Software Foundation, either version 3 of the License, or (at your option) any later version.
// tralloc is distributed in the hope that it will be useful, but WITHOUT ANY WARRANTY; without even the implied warranty of MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU General Lesser Public License for more details.
// You should have received a copy of the GNU General Lesser Public License along with tralloc. If not, see <http://www.gnu.org/licenses/>.

#include "chunk.h"


static inline
tralloc_error _get_used_by_multiple_threads ( _tralloc_chunk * chunk, bool * used_by_multiple_threads )
{

#   if TRALLOC_DEBUG_THREADS_LENGTH == TRALLOC_SPINLOCK
    if ( pthread_spin_lock ( &chunk->used_by_multiple_threads_lock ) != 0 ) {
        return TRALLOC_ERROR_SPINLOCK_FAILED;
    }
#   elif TRALLOC_DEBUG_THREADS_LENGTH == TRALLOC_MUTEX
    if ( pthread_mutex_lock ( &chunk->used_by_multiple_threads_lock ) != 0 ) {
        return TRALLOC_ERROR_MUTEX_FAILED;
    }
#   endif

    * used_by_multiple_threads = chunk->used_by_multiple_threads;

#   if TRALLOC_DEBUG_THREADS_LENGTH == TRALLOC_SPINLOCK
    if ( pthread_spin_unlock ( &chunk->used_by_multiple_threads_lock ) != 0 ) {
        return TRALLOC_ERROR_SPINLOCK_FAILED;
    }
#   elif TRALLOC_DEBUG_THREADS_LENGTH == TRALLOC_MUTEX
    if ( pthread_mutex_unlock ( &chunk->used_by_multiple_threads_lock ) != 0 ) {
        return TRALLOC_ERROR_MUTEX_FAILED;
    }
#   endif

    return 0;
}

static inline
tralloc_error _set_used_by_multiple_threads ( _tralloc_chunk * chunk, bool used_by_multiple_threads )
{

#   if TRALLOC_DEBUG_THREADS_LENGTH == TRALLOC_SPINLOCK
    if ( pthread_spin_lock ( &chunk->used_by_multiple_threads_lock ) != 0 ) {
        return TRALLOC_ERROR_SPINLOCK_FAILED;
    }
#   elif TRALLOC_DEBUG_THREADS_LENGTH == TRALLOC_MUTEX
    if ( pthread_mutex_lock ( &chunk->used_by_multiple_threads_lock ) != 0 ) {
        return TRALLOC_ERROR_MUTEX_FAILED;
    }
#   endif

    chunk->used_by_multiple_threads = used_by_multiple_threads;

#   if TRALLOC_DEBUG_THREADS_LENGTH == TRALLOC_SPINLOCK
    if ( pthread_spin_unlock ( &chunk->used_by_multiple_threads_lock ) != 0 ) {
        return TRALLOC_ERROR_SPINLOCK_FAILED;
    }
#   elif TRALLOC_DEBUG_THREADS_LENGTH == TRALLOC_MUTEX
    if ( pthread_mutex_unlock ( &chunk->used_by_multiple_threads_lock ) != 0 ) {
        return TRALLOC_ERROR_MUTEX_FAILED;
    }
#   endif

    return 0;
}


static inline
tralloc_error _check_lock_between_chunks (
    _tralloc_chunk * chunk_1,
    _tralloc_chunk * chunk_2,
    tralloc_extensions chunk_1_extension,
    tralloc_extensions chunk_2_extension,
    tralloc_error chunk_1_error,
    tralloc_error chunk_2_error,
    bool chunk_1_used_by_multiple_threads
)
{
    if ( chunk_2 != NULL ) {
        bool chunk_2_used_by_multiple_threads;
        tralloc_error result = _get_used_by_multiple_threads ( chunk_2, &chunk_2_used_by_multiple_threads );
        if ( result != 0 ) {
            return result;
        }
        if (
            chunk_1_used_by_multiple_threads ||
            chunk_2_used_by_multiple_threads ||
            pthread_equal ( chunk_1->initialized_by_thread, chunk_2->initialized_by_thread ) == 0
        ) {
            if ( ! ( chunk_1->extensions & chunk_1_extension ) ) {
                return chunk_1_error;
            }
            if ( ! ( chunk_2->extensions & chunk_2_extension ) ) {
                return chunk_2_error;
            }
        }
    }

    return 0;
}

static inline
tralloc_error _check_locks_around_chunk ( _tralloc_chunk * chunk, bool used_by_multiple_threads )
{
    tralloc_error result;

    if (
        ( result = _check_lock_between_chunks (
                       chunk, chunk->parent,
                       TRALLOC_EXTENSION_LOCK_PARENT, TRALLOC_EXTENSION_LOCK_FIRST_CHILD,
                       TRALLOC_ERROR_NO_PARENT_LOCK, TRALLOC_ERROR_NO_FIRST_CHILD_LOCK,
                       used_by_multiple_threads
                   ) ) != 0 ||
        ( result = _check_lock_between_chunks (
                       chunk, chunk->prev,
                       TRALLOC_EXTENSION_LOCK_PREV, TRALLOC_EXTENSION_LOCK_NEXT,
                       TRALLOC_ERROR_NO_PREV_LOCK, TRALLOC_ERROR_NO_NEXT_LOCK,
                       used_by_multiple_threads
                   ) ) != 0 ||
        ( result = _check_lock_between_chunks (
                       chunk, chunk->next,
                       TRALLOC_EXTENSION_LOCK_NEXT, TRALLOC_EXTENSION_LOCK_PREV,
                       TRALLOC_ERROR_NO_NEXT_LOCK, TRALLOC_ERROR_NO_PREV_LOCK,
                       used_by_multiple_threads
                   ) ) != 0 ||
        ( result = _check_lock_between_chunks (
                       chunk, chunk->first_child,
                       TRALLOC_EXTENSION_LOCK_FIRST_CHILD, TRALLOC_EXTENSION_LOCK_PARENT,
                       TRALLOC_ERROR_NO_FIRST_CHILD_LOCK, TRALLOC_ERROR_NO_PARENT_LOCK,
                       used_by_multiple_threads
                   ) ) != 0
    ) {
        return result;
    }
    return 0;
}

static inline
tralloc_error _refresh_check_locks_around_chunk ( _tralloc_chunk * chunk )
{
    bool used_by_multiple_threads;
    tralloc_error result = _get_used_by_multiple_threads ( chunk, &used_by_multiple_threads );
    if ( result != 0 ) {
        return result;
    }

    pthread_t thread = pthread_self();
    if ( !used_by_multiple_threads && pthread_equal ( chunk->initialized_by_thread, thread ) == 0 ) {
        used_by_multiple_threads = true;
        result = _set_used_by_multiple_threads ( chunk, true );
        if ( result != 0 ) {
            return result;
        }
    }

    return _check_locks_around_chunk ( chunk, used_by_multiple_threads );
}


tralloc_error _tralloc_debug_threads_new_chunk ( _tralloc_chunk * chunk )
{
    chunk->initialized_by_thread    = pthread_self();
    chunk->used_by_multiple_threads = false;

#   if TRALLOC_DEBUG_THREADS_LENGTH == TRALLOC_SPINLOCK
    if ( pthread_spin_init ( &chunk->length_lock, 0 ) != 0 ) {
        return TRALLOC_ERROR_SPINLOCK_FAILED;
    }
#   elif TRALLOC_DEBUG_THREADS_LENGTH == TRALLOC_MUTEX
    if ( pthread_mutex_init ( &chunk->length_lock, NULL ) != 0 ) {
        return TRALLOC_ERROR_MUTEX_FAILED;
    }
#   endif

#   if TRALLOC_DEBUG_THREADS_USED_BY_MULTIPLE_THREADS == TRALLOC_SPINLOCK
    if ( pthread_spin_init ( &chunk->used_by_multiple_threads_lock, 0 ) != 0 ) {
        return TRALLOC_ERROR_SPINLOCK_FAILED;
    }
#   elif TRALLOC_DEBUG_THREADS_USED_BY_MULTIPLE_THREADS == TRALLOC_MUTEX
    if ( pthread_mutex_init ( &chunk->used_by_multiple_threads_lock, NULL ) != 0 ) {
        return TRALLOC_ERROR_MUTEX_FAILED;
    }
#   endif

    return _check_locks_around_chunk ( chunk, false );
}

tralloc_error _tralloc_debug_threads_resize_chunk ( _tralloc_chunk * chunk )
{
    return _refresh_check_locks_around_chunk ( chunk );
}

tralloc_error _tralloc_debug_threads_move_chunk ( _tralloc_chunk * chunk )
{
    return _refresh_check_locks_around_chunk ( chunk );
}

tralloc_error _tralloc_debug_threads_free_chunk ( _tralloc_chunk * chunk )
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

#   if TRALLOC_DEBUG_THREADS_USED_BY_MULTIPLE_THREADS == TRALLOC_SPINLOCK
    if ( pthread_spin_destroy ( &chunk->used_by_multiple_threads_lock ) != 0 ) {
        return TRALLOC_ERROR_SPINLOCK_FAILED;
    }
#   elif TRALLOC_DEBUG_THREADS_USED_BY_MULTIPLE_THREADS == TRALLOC_MUTEX
    if ( pthread_mutex_destroy ( &chunk->used_by_multiple_threads_lock ) != 0 ) {
        return TRALLOC_ERROR_MUTEX_FAILED;
    }
#   endif

    return 0;
}
