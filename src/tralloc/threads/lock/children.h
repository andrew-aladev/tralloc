// This file is part of tralloc. Copyright (C) 2013 Andrew Aladjev aladjev.andrew@gmail.com
// tralloc is free software: you can redistribute it and/or modify it under the terms of the GNU General Lesser Public License as published by the Free Software Foundation, either version 3 of the License, or (at your option) any later version.
// tralloc is distributed in the hope that it will be useful, but WITHOUT ANY WARRANTY; without even the implied warranty of MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU General Lesser Public License for more details.
// You should have received a copy of the GNU General Lesser Public License along with tralloc. If not, see <http://www.gnu.org/licenses/>.

#if !defined ( TRALLOC_THREADS_LOCK_CHILDREN_H )
#define TRALLOC_THREADS_LOCK_CHILDREN_H

#include "../../types.h"
#include "../../macro.h"

#undef _TRALLOC_INLINE
#if defined ( _TRALLOC_INCLUDED_FROM_THREADS_LOCK_CHILDREN_C )
#    define _TRALLOC_INLINE _TRALLOC_INLINE_IN_OBJECT
#else
#    define _TRALLOC_INLINE _TRALLOC_INLINE_IN_HEADER
#endif


_TRALLOC_INLINE
tralloc_error _tralloc_new_children_lock ( _tralloc_children_lock * lock )
{

#   if TRALLOC_CHILDREN_LOCK_TYPE == TRALLOC_THREADS_RWLOCK
    if ( pthread_rwlock_init ( lock, NULL ) != 0 ) {
        return TRALLOC_ERROR_RWLOCK_FAILED;
    }
#   elif TRALLOC_CHILDREN_LOCK_TYPE == TRALLOC_THREADS_MUTEX
    if ( pthread_mutex_init ( lock, NULL ) != 0 ) {
        return TRALLOC_ERROR_MUTEX_FAILED;
    }
#   elif TRALLOC_CHILDREN_LOCK_TYPE == TRALLOC_THREADS_SPINLOCK
    if ( pthread_spin_init ( lock, 0 ) != 0 ) {
        return TRALLOC_ERROR_SPINLOCK_FAILED;
    }
#   endif

    return 0;
}

_TRALLOC_INLINE
tralloc_error _tralloc_rdlock_children ( _tralloc_children_lock * lock )
{

#   if TRALLOC_CHILDREN_LOCK_TYPE == TRALLOC_THREADS_RWLOCK
    if ( pthread_rwlock_rdlock ( lock ) != 0 ) {
        return TRALLOC_ERROR_RWLOCK_FAILED;
    }
#   elif TRALLOC_CHILDREN_LOCK_TYPE == TRALLOC_THREADS_MUTEX
    if ( pthread_mutex_lock ( lock ) != 0 ) {
        return TRALLOC_ERROR_MUTEX_FAILED;
    }
#   elif TRALLOC_CHILDREN_LOCK_TYPE == TRALLOC_THREADS_SPINLOCK
    if ( pthread_spin_lock ( lock ) != 0 ) {
        return TRALLOC_ERROR_SPINLOCK_FAILED;
    }
#   endif

    return 0;
}

_TRALLOC_INLINE
tralloc_error _tralloc_wrlock_children ( _tralloc_children_lock * lock )
{

#   if TRALLOC_CHILDREN_LOCK_TYPE == TRALLOC_THREADS_RWLOCK
    if ( pthread_rwlock_wrlock ( lock ) != 0 ) {
        return TRALLOC_ERROR_RWLOCK_FAILED;
    }
#   elif TRALLOC_CHILDREN_LOCK_TYPE == TRALLOC_THREADS_MUTEX
    if ( pthread_mutex_lock ( lock ) != 0 ) {
        return TRALLOC_ERROR_MUTEX_FAILED;
    }
#   elif TRALLOC_CHILDREN_LOCK_TYPE == TRALLOC_THREADS_SPINLOCK
    if ( pthread_spin_lock ( lock ) != 0 ) {
        return TRALLOC_ERROR_SPINLOCK_FAILED;
    }
#   endif

    return 0;
}

_TRALLOC_INLINE
tralloc_error _tralloc_unlock_children ( _tralloc_children_lock * lock )
{

#   if TRALLOC_CHILDREN_LOCK_TYPE == TRALLOC_THREADS_RWLOCK
    if ( pthread_rwlock_unlock ( lock ) != 0 ) {
        return TRALLOC_ERROR_RWLOCK_FAILED;
    }
#   elif TRALLOC_CHILDREN_LOCK_TYPE == TRALLOC_THREADS_MUTEX
    if ( pthread_mutex_unlock ( lock ) != 0 ) {
        return TRALLOC_ERROR_MUTEX_FAILED;
    }
#   elif TRALLOC_CHILDREN_LOCK_TYPE == TRALLOC_THREADS_SPINLOCK
    if ( pthread_spin_unlock ( lock ) != 0 ) {
        return TRALLOC_ERROR_SPINLOCK_FAILED;
    }
#   endif

    return 0;
}

_TRALLOC_INLINE
tralloc_error _tralloc_free_children_lock ( _tralloc_children_lock * lock )
{

#   if TRALLOC_CHILDREN_LOCK_TYPE == TRALLOC_THREADS_RWLOCK
    if ( pthread_rwlock_destroy ( lock ) != 0 ) {
        return TRALLOC_ERROR_RWLOCK_FAILED;
    }
#   elif TRALLOC_CHILDREN_LOCK_TYPE == TRALLOC_THREADS_MUTEX
    if ( pthread_mutex_destroy ( lock ) != 0 ) {
        return TRALLOC_ERROR_MUTEX_FAILED;
    }
#   elif TRALLOC_CHILDREN_LOCK_TYPE == TRALLOC_THREADS_SPINLOCK
    if ( pthread_spin_destroy ( lock ) != 0 ) {
        return TRALLOC_ERROR_SPINLOCK_FAILED;
    }
#   endif

    return 0;
}


#endif
