// This file is part of tralloc. Copyright (C) 2013 Andrew Aladjev aladjev.andrew@gmail.com
// tralloc is free software: you can redistribute it and/or modify it under the terms of the GNU General Lesser Public License as published by the Free Software Foundation, either version 3 of the License, or (at your option) any later version.
// tralloc is distributed in the hope that it will be useful, but WITHOUT ANY WARRANTY; without even the implied warranty of MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU General Lesser Public License for more details.
// You should have received a copy of the GNU General Lesser Public License along with tralloc. If not, see <http://www.gnu.org/licenses/>.

#include <tralloc/debug/stats/main.h>

#if defined ( TRALLOC_THREADS )
#   include <tralloc/threads/lock/debug/stats.h>
#   include <stdlib.h>
#endif

#if defined ( TRALLOC_DEBUG_LOG )
#   include <stdio.h>
#endif


#if defined ( TRALLOC_THREADS )

#if TRALLOC_DEBUG_STATS_LOCK_TYPE == TRALLOC_THREADS_RWLOCK
static pthread_rwlock_t _tralloc_chunks_count_lock           = PTHREAD_RWLOCK_INITIALIZER;
static pthread_rwlock_t _tralloc_chunks_overhead_length_lock = PTHREAD_RWLOCK_INITIALIZER;
static pthread_rwlock_t _tralloc_chunks_length_lock          = PTHREAD_RWLOCK_INITIALIZER;
#elif TRALLOC_DEBUG_STATS_LOCK_TYPE == TRALLOC_THREADS_MUTEX
static pthread_mutex_t _tralloc_chunks_count_lock           = PTHREAD_MUTEX_INITIALIZER;
static pthread_mutex_t _tralloc_chunks_overhead_length_lock = PTHREAD_MUTEX_INITIALIZER;
static pthread_mutex_t _tralloc_chunks_length_lock          = PTHREAD_MUTEX_INITIALIZER;
#elif TRALLOC_DEBUG_STATS_LOCK_TYPE == TRALLOC_THREADS_SPINLOCK
static pthread_spinlock_t _tralloc_chunks_count_lock;
static pthread_spinlock_t _tralloc_chunks_overhead_length_lock;
static pthread_spinlock_t _tralloc_chunks_length_lock;

__attribute__ ( ( constructor ) )
void lock_constructor ()
{
    if (
        pthread_spin_init ( &_tralloc_chunks_count_lock, 0 )           != 0 ||
        pthread_spin_init ( &_tralloc_chunks_overhead_length_lock, 0 ) != 0 ||
        pthread_spin_init ( &_tralloc_chunks_length_lock, 0 )          != 0
    ) {
#       if defined ( TRALLOC_DEBUG_LOG )
        fprintf (
            stderr,
            "%s:%zu error: %s\n",
            __FILE__,
            __LINE__,
            "static spinlock was failed to be initialized"
        );
#       endif
        exit ( TRALLOC_ERROR_SPINLOCK_FAILED );
    }
}
__attribute__ ( ( destructor ) )
void lock_destructor ()
{
    if (
        pthread_spin_destroy ( &_tralloc_chunks_count_lock )           != 0 ||
        pthread_spin_destroy ( &_tralloc_chunks_overhead_length_lock ) != 0 ||
        pthread_spin_destroy ( &_tralloc_chunks_length_lock )          != 0
    ) {
#       if defined ( TRALLOC_DEBUG_LOG )
        fprintf (
            stderr,
            "%s:%zu error: %s\n",
            __FILE__,
            __LINE__,
            "static spinlock was failed to be destroyed"
        );
#       endif
        exit ( TRALLOC_ERROR_SPINLOCK_FAILED );
    }
}
#endif

#endif


static size_t _tralloc_chunks_count = 0;

static inline
tralloc_error _tralloc_add_chunks_count ( size_t length )
{

#   if defined ( TRALLOC_THREADS )
    tralloc_error result = _tralloc_wrlock_debug_stats ( &_tralloc_chunks_count_lock );
    if ( result != 0 ) {
        return result;
    }
#   endif

    _tralloc_chunks_count += length;

#   if defined ( TRALLOC_THREADS )
    result = _tralloc_unlock_debug_stats ( &_tralloc_chunks_count_lock );
    if ( result != 0 ) {
        return result;
    }
#   endif

    return 0;
}

static inline
tralloc_error _tralloc_subtract_chunks_count ( size_t length )
{

#   if defined ( TRALLOC_THREADS )
    tralloc_error result = _tralloc_wrlock_debug_stats ( &_tralloc_chunks_count_lock );
    if ( result != 0 ) {
        return result;
    }
#   endif

    _tralloc_chunks_count -= length;

#   if defined ( TRALLOC_THREADS )
    result = _tralloc_unlock_debug_stats ( &_tralloc_chunks_count_lock );
    if ( result != 0 ) {
        return result;
    }
#   endif

    return 0;
}

tralloc_error tralloc_debug_stats_get_chunks_count ( size_t * length )
{

#   if defined ( TRALLOC_THREADS )
    tralloc_error result = _tralloc_rdlock_debug_stats ( &_tralloc_chunks_count_lock );
    if ( result != 0 ) {
        return result;
    }
#   endif

    * length = _tralloc_chunks_count;

#   if defined ( TRALLOC_THREADS )
    result = _tralloc_unlock_debug_stats ( &_tralloc_chunks_count_lock );
    if ( result != 0 ) {
        return result;
    }
#   endif

    return 0;
}


static size_t _tralloc_chunks_overhead_length = 0;

tralloc_error _tralloc_debug_stats_add_chunks_overhead_length ( size_t length )
{

#   if defined ( TRALLOC_THREADS )
    tralloc_error result = _tralloc_wrlock_debug_stats ( &_tralloc_chunks_overhead_length_lock );
    if ( result != 0 ) {
        return result;
    }
#   endif

    _tralloc_chunks_overhead_length += length;

#   if defined ( TRALLOC_THREADS )
    result = _tralloc_unlock_debug_stats ( &_tralloc_chunks_overhead_length_lock );
    if ( result != 0 ) {
        return result;
    }
#   endif

    return 0;
}

tralloc_error _tralloc_debug_stats_subtract_chunks_overhead_length ( size_t length )
{

#   if defined ( TRALLOC_THREADS )
    tralloc_error result = _tralloc_wrlock_debug_stats ( &_tralloc_chunks_overhead_length_lock );
    if ( result != 0 ) {
        return result;
    }
#   endif

    _tralloc_chunks_overhead_length -= length;

#   if defined ( TRALLOC_THREADS )
    result = _tralloc_unlock_debug_stats ( &_tralloc_chunks_overhead_length_lock );
    if ( result != 0 ) {
        return result;
    }
#   endif

    return 0;
}

tralloc_error tralloc_debug_stats_get_chunks_overhead_length ( size_t * length )
{

#   if defined ( TRALLOC_THREADS )
    tralloc_error result = _tralloc_rdlock_debug_stats ( &_tralloc_chunks_overhead_length_lock );
    if ( result != 0 ) {
        return result;
    }
#   endif

    * length = _tralloc_chunks_overhead_length;

#   if defined ( TRALLOC_THREADS )
    result = _tralloc_unlock_debug_stats ( &_tralloc_chunks_overhead_length_lock );
    if ( result != 0 ) {
        return result;
    }
#   endif

    return 0;
}


static size_t _tralloc_chunks_length = 0;

static inline
tralloc_error _tralloc_add_chunks_length ( size_t length )
{

#   if defined ( TRALLOC_THREADS )
    tralloc_error result = _tralloc_wrlock_debug_stats ( &_tralloc_chunks_length_lock );
    if ( result != 0 ) {
        return result;
    }
#   endif

    _tralloc_chunks_length += length;

#   if defined ( TRALLOC_THREADS )
    result = _tralloc_unlock_debug_stats ( &_tralloc_chunks_length_lock );
    if ( result != 0 ) {
        return result;
    }
#   endif

    return 0;
}

static inline
tralloc_error _tralloc_subtract_chunks_length ( size_t length )
{

#   if defined ( TRALLOC_THREADS )
    tralloc_error result = _tralloc_wrlock_debug_stats ( &_tralloc_chunks_length_lock );
    if ( result != 0 ) {
        return result;
    }
#   endif

    _tralloc_chunks_length -= length;

#   if defined ( TRALLOC_THREADS )
    result = _tralloc_unlock_debug_stats ( &_tralloc_chunks_length_lock );
    if ( result != 0 ) {
        return result;
    }
#   endif

    return 0;
}

tralloc_error tralloc_debug_stats_get_chunks_length ( size_t * length )
{

#   if defined ( TRALLOC_THREADS )
    tralloc_error result = _tralloc_rdlock_debug_stats ( &_tralloc_chunks_length_lock );
    if ( result != 0 ) {
        return result;
    }
#   endif

    * length = _tralloc_chunks_length;

#   if defined ( TRALLOC_THREADS )
    result = _tralloc_unlock_debug_stats ( &_tralloc_chunks_length_lock );
    if ( result != 0 ) {
        return result;
    }
#   endif

    return 0;
}

tralloc_error _tralloc_debug_stats_after_add_chunk ( size_t chunk_length, size_t length )
{
    tralloc_error result;
    if (
        ( result = _tralloc_add_chunks_count ( 1 ) ) != 0 ||
        ( result = _tralloc_debug_stats_add_chunks_overhead_length ( chunk_length ) ) != 0 ||
        ( result = _tralloc_add_chunks_length ( length ) ) != 0
    ) {
        return result;
    }
    return 0;
}

tralloc_error _tralloc_debug_stats_after_resize_chunk ( size_t old_length, size_t length )
{
    if ( length > old_length ) {
        return _tralloc_add_chunks_length ( length - old_length );
    } else if ( length < old_length ) {
        return _tralloc_subtract_chunks_length ( old_length - length );
    }
    return 0;
}

tralloc_error _tralloc_debug_stats_before_free_chunk ( _tralloc_chunk * chunk )
{
    tralloc_error error = 0, result;

    result = _tralloc_subtract_chunks_count ( 1 );
    if ( result != 0 ) {
        error = result;
    }

    result = _tralloc_debug_stats_subtract_chunks_overhead_length ( chunk->chunk_length );
    if ( result != 0 ) {
        error = result;
    }

    result = _tralloc_subtract_chunks_length ( chunk->length );
    if ( result != 0 ) {
        error = result;
    }

    return error;
}
