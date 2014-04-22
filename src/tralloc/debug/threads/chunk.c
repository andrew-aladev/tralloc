// This file is part of tralloc. Copyright (C) 2013 Andrew Aladjev aladjev.andrew@gmail.com
// tralloc is free software: you can redistribute it and/or modify it under the terms of the GNU General Lesser Public License as published by the Free Software Foundation, either version 3 of the License, or (at your option) any later version.
// tralloc is distributed in the hope that it will be useful, but WITHOUT ANY WARRANTY; without even the implied warranty of MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU General Lesser Public License for more details.
// You should have received a copy of the GNU General Lesser Public License along with tralloc. If not, see <http://www.gnu.org/licenses/>.

#include "chunk.h"

#if defined(TRALLOC_DEBUG_LOG)
#   include <stdio.h>
#   include <stdlib.h>
#endif


tralloc_error _tralloc_debug_threads_before_add_chunk ( _tralloc_chunk * parent_chunk, tralloc_extensions extensions )
{
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

//     chunk->initialized_by_thread    = pthread_self();
//     chunk->used_by_multiple_threads = false;
//
// #   if TRALLOC_DEBUG_THREADS_USED_BY_MULTIPLE_THREADS == TRALLOC_SPINLOCK
//     if ( pthread_spin_init ( &chunk->used_by_multiple_threads_lock, 0 ) != 0 ) {
//         return TRALLOC_ERROR_SPINLOCK_FAILED;
//     }
// #   elif TRALLOC_DEBUG_THREADS_USED_BY_MULTIPLE_THREADS == TRALLOC_MUTEX
//     if ( pthread_mutex_init ( &chunk->used_by_multiple_threads_lock, NULL ) != 0 ) {
//         return TRALLOC_ERROR_MUTEX_FAILED;
//     }
// #   endif

    return 0;
}

tralloc_error _tralloc_debug_threads_before_resize_chunk ( _tralloc_chunk * chunk )
{
    return 0;
}

tralloc_error _tralloc_debug_threads_after_resize_chunk ( _tralloc_chunk * chunk )
{
    return 0;
}

tralloc_error _tralloc_debug_threads_before_move_chunk ( _tralloc_chunk * chunk )
{
    return 0;
}

tralloc_error _tralloc_debug_threads_after_move_chunk ( _tralloc_chunk * chunk )
{
    return 0;
}

tralloc_error _tralloc_debug_threads_before_free_chunk ( _tralloc_chunk * chunk )
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

// #   if TRALLOC_DEBUG_THREADS_USED_BY_MULTIPLE_THREADS == TRALLOC_SPINLOCK
//     if ( pthread_spin_destroy ( &chunk->used_by_multiple_threads_lock ) != 0 ) {
//         return TRALLOC_ERROR_SPINLOCK_FAILED;
//     }
// #   elif TRALLOC_DEBUG_THREADS_USED_BY_MULTIPLE_THREADS == TRALLOC_MUTEX
//     if ( pthread_mutex_destroy ( &chunk->used_by_multiple_threads_lock ) != 0 ) {
//         return TRALLOC_ERROR_MUTEX_FAILED;
//     }
// #   endif

    return 0;
}
