// This file is part of tralloc. Copyright (C) 2013 Andrew Aladjev aladjev.andrew@gmail.com
// tralloc is free software: you can redistribute it and/or modify it under the terms of the GNU General Lesser Public License as published by the Free Software Foundation, either version 3 of the License, or (at your option) any later version.
// tralloc is distributed in the hope that it will be useful, but WITHOUT ANY WARRANTY; without even the implied warranty of MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU General Lesser Public License for more details.
// You should have received a copy of the GNU General Lesser Public License along with tralloc. If not, see <http://www.gnu.org/licenses/>.

#include "main.h"


tralloc_error _tralloc_debug_get_length ( _tralloc_chunk * chunk, size_t * length )
{

#   if defined(TRALLOC_THREADS)

#   if TRALLOC_THREADS_LENGTH == TRALLOC_SPINLOCK
    if ( pthread_spin_lock ( &chunk->length_lock ) != 0 ) {
        return TRALLOC_ERROR_SPINLOCK_FAILED;
    }
#   elif TRALLOC_THREADS_LENGTH == TRALLOC_MUTEX
    if ( pthread_mutex_lock ( &chunk->length_lock ) != 0 ) {
        return TRALLOC_ERROR_MUTEX_FAILED;
    }
#   endif

#   endif

    * length = chunk->length;

#   if defined(TRALLOC_THREADS)

#   if TRALLOC_THREADS_LENGTH == TRALLOC_SPINLOCK
    if ( pthread_spin_unlock ( &chunk->length_lock ) != 0 ) {
        return TRALLOC_ERROR_SPINLOCK_FAILED;
    }
#   elif TRALLOC_THREADS_LENGTH == TRALLOC_MUTEX
    if ( pthread_mutex_unlock ( &chunk->length_lock ) != 0 ) {
        return TRALLOC_ERROR_MUTEX_FAILED;
    }
#   endif

#   endif

    return 0;
}

tralloc_error _tralloc_debug_set_length ( _tralloc_chunk * chunk, size_t length )
{

#   if defined(TRALLOC_THREADS)

#   if TRALLOC_THREADS_LENGTH == TRALLOC_SPINLOCK
    if ( pthread_spin_lock ( &chunk->length_lock ) != 0 ) {
        return TRALLOC_ERROR_SPINLOCK_FAILED;
    }
#   elif TRALLOC_THREADS_LENGTH == TRALLOC_MUTEX
    if ( pthread_mutex_lock ( &chunk->length_lock ) != 0 ) {
        return TRALLOC_ERROR_MUTEX_FAILED;
    }
#   endif

#   endif

    chunk->length = length;

#   if defined(TRALLOC_THREADS)

#   if TRALLOC_THREADS_LENGTH == TRALLOC_SPINLOCK
    if ( pthread_spin_unlock ( &chunk->length_lock ) != 0 ) {
        return TRALLOC_ERROR_SPINLOCK_FAILED;
    }
#   elif TRALLOC_THREADS_LENGTH == TRALLOC_MUTEX
    if ( pthread_mutex_unlock ( &chunk->length_lock ) != 0 ) {
        return TRALLOC_ERROR_MUTEX_FAILED;
    }
#   endif

#   endif

    return 0;
}
