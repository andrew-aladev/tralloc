// This file is part of tralloc. Copyright (C) 2013 Andrew Aladjev aladjev.andrew@gmail.com
// tralloc is free software: you can redistribute it and/or modify it under the terms of the GNU General Lesser Public License as published by the Free Software Foundation, either version 3 of the License, or (at your option) any later version.
// tralloc is distributed in the hope that it will be useful, but WITHOUT ANY WARRANTY; without even the implied warranty of MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU General Lesser Public License for more details.
// You should have received a copy of the GNU General Lesser Public License along with tralloc. If not, see <http://www.gnu.org/licenses/>.

#include "events.h"
#include "tree/chunk.h"


#if defined(TRALLOC_THREADS)
static pthread_mutex_t _user_data_mutex = PTHREAD_MUTEX_INITIALIZER;
#endif

static void * _user_data;

#if defined(TRALLOC_THREADS)
static pthread_mutex_t _debug_on_add_mutex    = PTHREAD_MUTEX_INITIALIZER;
static pthread_mutex_t _debug_on_resize_mutex = PTHREAD_MUTEX_INITIALIZER;
static pthread_mutex_t _debug_on_move_mutex   = PTHREAD_MUTEX_INITIALIZER;
static pthread_mutex_t _debug_on_free_mutex   = PTHREAD_MUTEX_INITIALIZER;
#endif

static _tralloc_callback_on_add    _debug_on_add;
static _tralloc_callback_on_resize _debug_on_resize;
static _tralloc_callback_on_move   _debug_on_move;
static _tralloc_callback_on_free   _debug_on_free;

tralloc_error _tralloc_set_user_data ( void * user_data )
{

#   if defined(TRALLOC_THREADS)
    if ( pthread_mutex_lock ( &_user_data_mutex ) != 0 ) {
        return TRALLOC_ERROR_MUTEX_FAILED;
    }
#   endif

    _user_data = user_data;

#   if defined(TRALLOC_THREADS)
    if ( pthread_mutex_unlock ( &_user_data_mutex ) != 0 ) {
        return TRALLOC_ERROR_MUTEX_FAILED;
    }
#   endif

    return 0;
}

tralloc_error _tralloc_set_callback ( _tralloc_callback_on_add on_add, _tralloc_callback_on_resize on_resize, _tralloc_callback_on_move on_move, _tralloc_callback_on_free on_free )
{

#   if defined(TRALLOC_THREADS)
    if ( pthread_mutex_lock ( &_debug_on_add_mutex ) != 0 ) {
        return TRALLOC_ERROR_MUTEX_FAILED;
    }
#   endif

    _debug_on_add = on_add;

#   if defined(TRALLOC_THREADS)
    if (
        pthread_mutex_unlock ( &_debug_on_add_mutex )    != 0 ||
        pthread_mutex_lock   ( &_debug_on_resize_mutex ) != 0
    ) {
        return TRALLOC_ERROR_MUTEX_FAILED;
    }
#   endif

    _debug_on_resize = on_resize;

#   if defined(TRALLOC_THREADS)
    if (
        pthread_mutex_unlock ( &_debug_on_resize_mutex ) != 0 ||
        pthread_mutex_lock   ( &_debug_on_move_mutex )   != 0
    ) {
        return TRALLOC_ERROR_MUTEX_FAILED;
    }
#   endif

    _debug_on_move = on_move;

#   if defined(TRALLOC_THREADS)
    if (
        pthread_mutex_unlock ( &_debug_on_move_mutex ) != 0 ||
        pthread_mutex_lock   ( &_debug_on_free_mutex ) != 0
    ) {
        return TRALLOC_ERROR_MUTEX_FAILED;
    }
#   endif

    _debug_on_free = on_free;

#   if defined(TRALLOC_THREADS)
    if ( pthread_mutex_unlock ( &_debug_on_free_mutex ) != 0 ) {
        return TRALLOC_ERROR_MUTEX_FAILED;
    }
#   endif

    return 0;
}


#if defined(TRALLOC_THREADS)
static pthread_mutex_t _chunks_count_mutex = PTHREAD_MUTEX_INITIALIZER;
#endif

static size_t _chunks_count = 0;

static inline
tralloc_error _tralloc_add_chunks_count ( size_t length )
{

#   if defined(TRALLOC_THREADS)
    if ( pthread_mutex_lock ( &_chunks_count_mutex ) != 0 ) {
        return TRALLOC_ERROR_MUTEX_FAILED;
    }
#   endif

    _chunks_count += length;

#   if defined(TRALLOC_THREADS)
    if ( pthread_mutex_unlock ( &_chunks_count_mutex ) != 0 ) {
        return TRALLOC_ERROR_MUTEX_FAILED;
    }
#   endif

    return 0;
}

static inline
tralloc_error _tralloc_subtract_chunks_count ( size_t length )
{

#   if defined(TRALLOC_THREADS)
    if ( pthread_mutex_lock ( &_chunks_count_mutex ) != 0 ) {
        return TRALLOC_ERROR_MUTEX_FAILED;
    }
#   endif

    _chunks_count -= length;

#   if defined(TRALLOC_THREADS)
    if ( pthread_mutex_unlock ( &_chunks_count_mutex ) != 0 ) {
        return TRALLOC_ERROR_MUTEX_FAILED;
    }
#   endif

    return 0;
}

tralloc_error tralloc_get_chunks_count ( size_t * length )
{

#   if defined(TRALLOC_THREADS)
    if ( pthread_mutex_lock ( &_chunks_count_mutex ) != 0 ) {
        return TRALLOC_ERROR_MUTEX_FAILED;
    }
#   endif

    * length = _chunks_count;

#   if defined(TRALLOC_THREADS)
    if ( pthread_mutex_unlock ( &_chunks_count_mutex ) != 0 ) {
        return TRALLOC_ERROR_MUTEX_FAILED;
    }
#   endif

    return 0;
}


#if defined(TRALLOC_THREADS)
static pthread_mutex_t _chunks_overhead_length_mutex = PTHREAD_MUTEX_INITIALIZER;
#endif

static size_t _chunks_overhead_length = 0;

tralloc_error _tralloc_add_chunks_overhead_length ( size_t length )
{

#   if defined(TRALLOC_THREADS)
    if ( pthread_mutex_lock ( &_chunks_overhead_length_mutex ) != 0 ) {
        return TRALLOC_ERROR_MUTEX_FAILED;
    }
#   endif

    _chunks_overhead_length += length;

#   if defined(TRALLOC_THREADS)
    if ( pthread_mutex_unlock ( &_chunks_overhead_length_mutex ) != 0 ) {
        return TRALLOC_ERROR_MUTEX_FAILED;
    }
#   endif

    return 0;
}

tralloc_error _tralloc_subtract_chunks_overhead_length ( size_t length )
{

#   if defined(TRALLOC_THREADS)
    if ( pthread_mutex_lock ( &_chunks_overhead_length_mutex ) != 0 ) {
        return TRALLOC_ERROR_MUTEX_FAILED;
    }
#   endif

    _chunks_overhead_length -= length;

#   if defined(TRALLOC_THREADS)
    if ( pthread_mutex_unlock ( &_chunks_overhead_length_mutex ) != 0 ) {
        return TRALLOC_ERROR_MUTEX_FAILED;
    }
#   endif

    return 0;
}

tralloc_error tralloc_get_chunks_overhead_length ( size_t * length )
{

#   if defined(TRALLOC_THREADS)
    if ( pthread_mutex_lock ( &_chunks_overhead_length_mutex ) != 0 ) {
        return TRALLOC_ERROR_MUTEX_FAILED;
    }
#   endif

    * length = _chunks_overhead_length;

#   if defined(TRALLOC_THREADS)
    if ( pthread_mutex_unlock ( &_chunks_overhead_length_mutex ) != 0 ) {
        return TRALLOC_ERROR_MUTEX_FAILED;
    }
#   endif

    return 0;
}


#if defined(TRALLOC_THREADS)
static pthread_mutex_t _chunks_length_mutex = PTHREAD_MUTEX_INITIALIZER;
#endif

static size_t _chunks_length = 0;

static inline
tralloc_error _tralloc_add_chunks_length ( size_t length )
{

#   if defined(TRALLOC_THREADS)
    if ( pthread_mutex_lock ( &_chunks_length_mutex ) != 0 ) {
        return TRALLOC_ERROR_MUTEX_FAILED;
    }
#   endif

    _chunks_length += length;

#   if defined(TRALLOC_THREADS)
    if ( pthread_mutex_unlock ( &_chunks_length_mutex ) != 0 ) {
        return TRALLOC_ERROR_MUTEX_FAILED;
    }
#   endif

    return 0;
}

static inline
tralloc_error _tralloc_subtract_chunks_length ( size_t length )
{

#   if defined(TRALLOC_THREADS)
    if ( pthread_mutex_lock ( &_chunks_length_mutex ) != 0 ) {
        return TRALLOC_ERROR_MUTEX_FAILED;
    }
#   endif

    _chunks_length -= length;

#   if defined(TRALLOC_THREADS)
    if ( pthread_mutex_unlock ( &_chunks_length_mutex ) != 0 ) {
        return TRALLOC_ERROR_MUTEX_FAILED;
    }
#   endif

    return 0;
}

tralloc_error tralloc_get_chunks_length ( size_t * length )
{

#   if defined(TRALLOC_THREADS)
    if ( pthread_mutex_lock ( &_chunks_length_mutex ) != 0 ) {
        return TRALLOC_ERROR_MUTEX_FAILED;
    }
#   endif

    * length = _chunks_length;

#   if defined(TRALLOC_THREADS)
    if ( pthread_mutex_unlock ( &_chunks_length_mutex ) != 0 ) {
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
        tralloc_error result = _tralloc_get_chunk_used_by_multiple_threads ( chunk_2, &chunk_2_used_by_multiple_threads );
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

tralloc_error _tralloc_on_add ( _tralloc_chunk * chunk )
{
    tralloc_error result;

#   if defined(TRALLOC_THREADS)
    chunk->initialized_by_thread    = pthread_self();
    chunk->used_by_multiple_threads = false;

#   if TRALLOC_THREADS_LENGTH == TRALLOC_SPINLOCK
    if ( pthread_spin_init ( &chunk->length_lock, 0 ) != 0 ) {
        return TRALLOC_ERROR_SPINLOCK_FAILED;
    }
#   elif TRALLOC_THREADS_LENGTH == TRALLOC_MUTEX
    if ( pthread_mutex_init ( &chunk->length_lock, NULL ) != 0 ) {
        return TRALLOC_ERROR_MUTEX_FAILED;
    }
#   endif

#   if TRALLOC_THREADS_USED_BY_MULTIPLE_THREADS == TRALLOC_SPINLOCK
    if ( pthread_spin_init ( &chunk->used_by_multiple_threads_lock, 0 ) != 0 ) {
        return TRALLOC_ERROR_SPINLOCK_FAILED;
    }
#   elif TRALLOC_THREADS_USED_BY_MULTIPLE_THREADS == TRALLOC_MUTEX
    if ( pthread_mutex_init ( &chunk->used_by_multiple_threads_lock, NULL ) != 0 ) {
        return TRALLOC_ERROR_MUTEX_FAILED;
    }
#   endif

    result = _check_locks_around_chunk ( chunk, false );
    if ( result != 0 ) {
        return result;
    }

#   endif

    size_t length;
    if (
        ( result = _tralloc_add_chunks_count           ( 1 ) )                   != 0 ||
        ( result = _tralloc_add_chunks_overhead_length ( chunk->chunk_length ) ) != 0 ||
        ( result = _tralloc_get_chunk_length           ( chunk, &length ) )      != 0 ||
        ( result = _tralloc_add_chunks_length          ( length ) )              != 0
    ) {
        return result;
    }

#   if defined(TRALLOC_THREADS)
    if ( pthread_mutex_lock ( &_debug_on_add_mutex ) != 0 ) {
        return TRALLOC_ERROR_MUTEX_FAILED;
    }
#   endif

    if ( _debug_on_add != NULL ) {

#       if defined(TRALLOC_THREADS)
        if ( pthread_mutex_lock ( &_user_data_mutex ) != 0 ) {
            return TRALLOC_ERROR_MUTEX_FAILED;
        }
#       endif

        tralloc_error result = _debug_on_add ( _user_data, chunk );

#       if defined(TRALLOC_THREADS)
        if (
            pthread_mutex_unlock ( &_user_data_mutex )    != 0 ||
            pthread_mutex_unlock ( &_debug_on_add_mutex ) != 0
        ) {
            return TRALLOC_ERROR_MUTEX_FAILED;
        }
#       endif

        return result;
    } else {

#       if defined(TRALLOC_THREADS)
        if ( pthread_mutex_unlock ( &_debug_on_add_mutex ) != 0 ) {
            return TRALLOC_ERROR_MUTEX_FAILED;
        }
#       endif

        return 0;
    }
}

tralloc_error _tralloc_on_resize ( _tralloc_chunk * chunk, size_t old_length )
{
    tralloc_error result;

#   if defined(TRALLOC_THREADS)

    bool used_by_multiple_threads;
    result = _tralloc_get_chunk_used_by_multiple_threads ( chunk, &used_by_multiple_threads );
    if ( result != 0 ) {
        return result;
    }

    pthread_t thread = pthread_self();
    if ( !used_by_multiple_threads && pthread_equal ( chunk->initialized_by_thread, thread ) == 0 ) {
        used_by_multiple_threads = true;
        result = _tralloc_set_chunk_used_by_multiple_threads ( chunk, true );
        if ( result != 0 ) {
            return result;
        }
    }

    result = _check_locks_around_chunk ( chunk, used_by_multiple_threads );
    if ( result != 0 ) {
        return result;
    }

#   endif

    size_t length;
    result = _tralloc_get_chunk_length ( chunk, &length );
    if ( result != 0 ) {
        return result;
    }

    if ( length > old_length ) {
        result = _tralloc_add_chunks_length ( length - old_length );
        if ( result != 0 ) {
            return result;
        }
    } else if ( length < old_length ) {
        result = _tralloc_subtract_chunks_length ( old_length - length );
        if ( result != 0 ) {
            return result;
        }
    }

#   if defined(TRALLOC_THREADS)
    if ( pthread_mutex_lock ( &_debug_on_resize_mutex ) != 0 ) {
        return TRALLOC_ERROR_MUTEX_FAILED;
    }
#   endif

    if ( _debug_on_resize != NULL ) {

#       if defined(TRALLOC_THREADS)
        if ( pthread_mutex_lock ( &_user_data_mutex ) != 0 ) {
            return TRALLOC_ERROR_MUTEX_FAILED;
        }
#       endif

        tralloc_error result = _debug_on_resize ( _user_data, chunk, old_length );

#       if defined(TRALLOC_THREADS)
        if (
            pthread_mutex_unlock ( &_user_data_mutex )       != 0 ||
            pthread_mutex_unlock ( &_debug_on_resize_mutex ) != 0
        ) {
            return TRALLOC_ERROR_MUTEX_FAILED;
        }
#       endif

        return result;
    } else {

#       if defined(TRALLOC_THREADS)
        if ( pthread_mutex_unlock ( &_debug_on_resize_mutex ) != 0 ) {
            return TRALLOC_ERROR_MUTEX_FAILED;
        }
#       endif

        return 0;
    }
}

tralloc_error _tralloc_on_move ( _tralloc_chunk * chunk, _tralloc_chunk * old_parent_chunk )
{
    tralloc_error result;

#   if defined(TRALLOC_THREADS)

    bool used_by_multiple_threads;
    result = _tralloc_get_chunk_used_by_multiple_threads ( chunk, &used_by_multiple_threads );
    if ( result != 0 ) {
        return result;
    }

    pthread_t thread = pthread_self();
    if ( !used_by_multiple_threads && pthread_equal ( chunk->initialized_by_thread, thread ) == 0 ) {
        used_by_multiple_threads = true;
        result = _tralloc_set_chunk_used_by_multiple_threads ( chunk, true );
        if ( result != 0 ) {
            return result;
        }
    }

    result = _check_locks_around_chunk ( chunk, used_by_multiple_threads );
    if ( result != 0 ) {
        return result;
    }

#   endif

#   if defined(TRALLOC_THREADS)
    if ( pthread_mutex_lock ( &_debug_on_move_mutex ) != 0 ) {
        return TRALLOC_ERROR_MUTEX_FAILED;
    }
#   endif

    if ( _debug_on_move != NULL ) {

#       if defined(TRALLOC_THREADS)
        if ( pthread_mutex_lock ( &_user_data_mutex ) != 0 ) {
            return TRALLOC_ERROR_MUTEX_FAILED;
        }
#       endif

        tralloc_error result = _debug_on_move ( _user_data, chunk, old_parent_chunk );

#       if defined(TRALLOC_THREADS)
        if (
            pthread_mutex_unlock ( &_user_data_mutex )     != 0 ||
            pthread_mutex_unlock ( &_debug_on_move_mutex ) != 0
        ) {
            return TRALLOC_ERROR_MUTEX_FAILED;
        }
#       endif

        return result;
    } else {

#       if defined(TRALLOC_THREADS)
        if ( pthread_mutex_unlock ( &_debug_on_move_mutex ) != 0 ) {
            return TRALLOC_ERROR_MUTEX_FAILED;
        }
#       endif

        return 0;
    }
}

tralloc_error _tralloc_on_free ( _tralloc_chunk * chunk )
{

#   if defined(TRALLOC_THREADS)

#   if TRALLOC_THREADS_LENGTH == TRALLOC_SPINLOCK
    if ( pthread_spin_destroy ( &chunk->length_lock ) != 0 ) {
        return TRALLOC_ERROR_SPINLOCK_FAILED;
    }
#   elif TRALLOC_THREADS_LENGTH == TRALLOC_MUTEX
    if ( pthread_mutex_destroy ( &chunk->length_lock ) != 0 ) {
        return TRALLOC_ERROR_MUTEX_FAILED;
    }
#   endif

#   if TRALLOC_THREADS_USED_BY_MULTIPLE_THREADS == TRALLOC_SPINLOCK
    if ( pthread_spin_destroy ( &chunk->used_by_multiple_threads_lock ) != 0 ) {
        return TRALLOC_ERROR_SPINLOCK_FAILED;
    }
#   elif TRALLOC_THREADS_USED_BY_MULTIPLE_THREADS == TRALLOC_MUTEX
    if ( pthread_mutex_destroy ( &chunk->used_by_multiple_threads_lock ) != 0 ) {
        return TRALLOC_ERROR_MUTEX_FAILED;
    }
#   endif

#   endif

    tralloc_error result;
    size_t length;
    if (
        ( result = _tralloc_subtract_chunks_count           ( 1 ) )                   != 0 ||
        ( result = _tralloc_subtract_chunks_overhead_length ( chunk->chunk_length ) ) != 0 ||
        ( result = _tralloc_get_chunk_length                ( chunk, &length ) )      != 0 ||
        ( result = _tralloc_subtract_chunks_length          ( length ) )              != 0
    ) {
        return result;
    }

#   if defined(TRALLOC_THREADS)
    if ( pthread_mutex_lock ( &_debug_on_free_mutex ) != 0 ) {
        return TRALLOC_ERROR_MUTEX_FAILED;
    }
#   endif

    if ( _debug_on_free != NULL ) {

#       if defined(TRALLOC_THREADS)
        if ( pthread_mutex_lock ( &_user_data_mutex ) != 0 ) {
            return TRALLOC_ERROR_MUTEX_FAILED;
        }
#       endif

        tralloc_error result = _debug_on_free ( _user_data, chunk );

#       if defined(TRALLOC_THREADS)
        if (
            pthread_mutex_unlock ( &_user_data_mutex )     != 0 ||
            pthread_mutex_unlock ( &_debug_on_free_mutex ) != 0
        ) {
            return TRALLOC_ERROR_MUTEX_FAILED;
        }
#       endif

        return result;
    } else {

#       if defined(TRALLOC_THREADS)
        if ( pthread_mutex_unlock ( &_debug_on_free_mutex ) != 0 ) {
            return TRALLOC_ERROR_MUTEX_FAILED;
        }
#       endif

        return 0;
    }
}
