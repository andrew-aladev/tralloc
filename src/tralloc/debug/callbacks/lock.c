// This file is part of tralloc. Copyright (C) 2013 Andrew Aladjev aladjev.andrew@gmail.com
// tralloc is free software: you can redistribute it and/or modify it under the terms of the GNU General Lesser Public License as published by the Free Software Foundation, either version 3 of the License, or (at your option) any later version.
// tralloc is distributed in the hope that it will be useful, but WITHOUT ANY WARRANTY; without even the implied warranty of MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU General Lesser Public License for more details.
// You should have received a copy of the GNU General Lesser Public License along with tralloc. If not, see <http://www.gnu.org/licenses/>.

#include <tralloc/debug/callbacks/lock.h>

#if defined ( TRALLOC_DEBUG_LOG )
#   include <stdio.h>
#   include <stdlib.h>
#endif


#if TRALLOC_DEBUG_CALLBACKS_LOCK_TYPE == TRALLOC_THREADS_RWLOCK
typedef pthread_rwlock_t _tralloc_debug_callbacks_lock;
static _tralloc_debug_callbacks_lock _tralloc_debug_callback_add_lock    = PTHREAD_RWLOCK_INITIALIZER;
static _tralloc_debug_callbacks_lock _tralloc_debug_callback_move_lock   = PTHREAD_RWLOCK_INITIALIZER;
static _tralloc_debug_callbacks_lock _tralloc_debug_callback_resize_lock = PTHREAD_RWLOCK_INITIALIZER;
static _tralloc_debug_callbacks_lock _tralloc_debug_callback_free_lock   = PTHREAD_RWLOCK_INITIALIZER;
#elif TRALLOC_DEBUG_CALLBACKS_LOCK_TYPE == TRALLOC_THREADS_MUTEX
typedef pthread_mutex_t _tralloc_debug_callbacks_lock;
static _tralloc_debug_callbacks_lock _tralloc_debug_callback_add_lock    = PTHREAD_MUTEX_INITIALIZER;
static _tralloc_debug_callbacks_lock _tralloc_debug_callback_move_lock   = PTHREAD_MUTEX_INITIALIZER;
static _tralloc_debug_callbacks_lock _tralloc_debug_callback_resize_lock = PTHREAD_MUTEX_INITIALIZER;
static _tralloc_debug_callbacks_lock _tralloc_debug_callback_free_lock   = PTHREAD_MUTEX_INITIALIZER;
#elif TRALLOC_DEBUG_CALLBACKS_LOCK_TYPE == TRALLOC_THREADS_SPINLOCK
typedef pthread_spinlock_t _tralloc_debug_callbacks_lock;
static _tralloc_debug_callbacks_lock _tralloc_debug_callback_add_lock;
static _tralloc_debug_callbacks_lock _tralloc_debug_callback_move_lock;
static _tralloc_debug_callbacks_lock _tralloc_debug_callback_resize_lock;
static _tralloc_debug_callbacks_lock _tralloc_debug_callback_free_lock;

__attribute__ ( ( constructor ) )
void _tralloc_debug_callbacks_lock_constructor ()
{
    if (
        pthread_spin_init ( &_tralloc_debug_callback_add_lock, 0 )    != 0 ||
        pthread_spin_init ( &_tralloc_debug_callback_move_lock, 0 )   != 0 ||
        pthread_spin_init ( &_tralloc_debug_callback_resize_lock, 0 ) != 0 ||
        pthread_spin_init ( &_tralloc_debug_callback_free_lock, 0 )   != 0
    ) {
#       if defined ( TRALLOC_DEBUG_LOG )
        fprintf (
            stderr,
            "%s:%d error: %s\n",
            __FILE__,
            __LINE__,
            "static spinlock was failed to be initialized"
        );
#       endif
        exit ( TRALLOC_ERROR_SPINLOCK_FAILED );
    }
}

__attribute__ ( ( destructor ) )
void _tralloc_debug_callbacks_lock_destructor ()
{
    if (
        pthread_spin_destroy ( &_tralloc_debug_callback_add_lock )    != 0 ||
        pthread_spin_destroy ( &_tralloc_debug_callback_move_lock )   != 0 ||
        pthread_spin_destroy ( &_tralloc_debug_callback_resize_lock ) != 0 ||
        pthread_spin_destroy ( &_tralloc_debug_callback_free_lock )   != 0
    ) {
#       if defined ( TRALLOC_DEBUG_LOG )
        fprintf (
            stderr,
            "%s:%d error: %s\n",
            __FILE__,
            __LINE__,
            "static spinlock was failed to be destroyed"
        );
#       endif
        exit ( TRALLOC_ERROR_SPINLOCK_FAILED );
    }
}
#endif

static inline
tralloc_error _tralloc_rdlock_debug_callback ( _tralloc_debug_callbacks_lock * lock )
{

#   if TRALLOC_DEBUG_CALLBACKS_LOCK_TYPE == TRALLOC_THREADS_RWLOCK
    if ( pthread_rwlock_rdlock ( lock ) != 0 ) {
        return TRALLOC_ERROR_RWLOCK_FAILED;
    }
#   elif TRALLOC_DEBUG_CALLBACKS_LOCK_TYPE == TRALLOC_THREADS_MUTEX
    if ( pthread_mutex_lock ( lock ) != 0 ) {
        return TRALLOC_ERROR_MUTEX_FAILED;
    }
#   elif TRALLOC_DEBUG_CALLBACKS_LOCK_TYPE == TRALLOC_THREADS_SPINLOCK
    if ( pthread_spin_lock ( lock ) != 0 ) {
        return TRALLOC_ERROR_SPINLOCK_FAILED;
    }
#   endif

    return 0;
}

static inline
tralloc_error _tralloc_wrlock_debug_callback ( _tralloc_debug_callbacks_lock * lock )
{

#   if TRALLOC_DEBUG_CALLBACKS_LOCK_TYPE == TRALLOC_THREADS_RWLOCK
    if ( pthread_rwlock_wrlock ( lock ) != 0 ) {
        return TRALLOC_ERROR_RWLOCK_FAILED;
    }
#   elif TRALLOC_DEBUG_CALLBACKS_LOCK_TYPE == TRALLOC_THREADS_MUTEX
    if ( pthread_mutex_lock ( lock ) != 0 ) {
        return TRALLOC_ERROR_MUTEX_FAILED;
    }
#   elif TRALLOC_DEBUG_CALLBACKS_LOCK_TYPE == TRALLOC_THREADS_SPINLOCK
    if ( pthread_spin_lock ( lock ) != 0 ) {
        return TRALLOC_ERROR_SPINLOCK_FAILED;
    }
#   endif

    return 0;
}

static inline
tralloc_error _tralloc_unlock_debug_callback ( _tralloc_debug_callbacks_lock * lock )
{

#   if TRALLOC_DEBUG_CALLBACKS_LOCK_TYPE == TRALLOC_THREADS_RWLOCK
    if ( pthread_rwlock_unlock ( lock ) != 0 ) {
        return TRALLOC_ERROR_RWLOCK_FAILED;
    }
#   elif TRALLOC_DEBUG_CALLBACKS_LOCK_TYPE == TRALLOC_THREADS_MUTEX
    if ( pthread_mutex_unlock ( lock ) != 0 ) {
        return TRALLOC_ERROR_MUTEX_FAILED;
    }
#   elif TRALLOC_DEBUG_CALLBACKS_LOCK_TYPE == TRALLOC_THREADS_SPINLOCK
    if ( pthread_spin_unlock ( lock ) != 0 ) {
        return TRALLOC_ERROR_SPINLOCK_FAILED;
    }
#   endif

    return 0;
}

tralloc_error _tralloc_debug_callback_rdlock_add ()
{
    return _tralloc_rdlock_debug_callback ( &_tralloc_debug_callback_add_lock );
}

tralloc_error _tralloc_debug_callback_wrlock_add ()
{
    return _tralloc_wrlock_debug_callback ( &_tralloc_debug_callback_add_lock );
}

tralloc_error _tralloc_debug_callback_unlock_add ()
{
    return _tralloc_unlock_debug_callback ( &_tralloc_debug_callback_add_lock );
}


tralloc_error _tralloc_debug_callback_rdlock_move ()
{
    return _tralloc_rdlock_debug_callback ( &_tralloc_debug_callback_move_lock );
}

tralloc_error _tralloc_debug_callback_wrlock_move ()
{
    return _tralloc_wrlock_debug_callback ( &_tralloc_debug_callback_move_lock );
}

tralloc_error _tralloc_debug_callback_unlock_move ()
{
    return _tralloc_unlock_debug_callback ( &_tralloc_debug_callback_move_lock );
}


tralloc_error _tralloc_debug_callback_rdlock_resize ()
{
    return _tralloc_rdlock_debug_callback ( &_tralloc_debug_callback_resize_lock );
}

tralloc_error _tralloc_debug_callback_wrlock_resize ()
{
    return _tralloc_wrlock_debug_callback ( &_tralloc_debug_callback_resize_lock );
}

tralloc_error _tralloc_debug_callback_unlock_resize ()
{
    return _tralloc_unlock_debug_callback ( &_tralloc_debug_callback_resize_lock );
}


tralloc_error _tralloc_debug_callback_rdlock_free ()
{
    return _tralloc_rdlock_debug_callback ( &_tralloc_debug_callback_free_lock );
}

tralloc_error _tralloc_debug_callback_wrlock_free ()
{
    return _tralloc_wrlock_debug_callback ( &_tralloc_debug_callback_free_lock );
}

tralloc_error _tralloc_debug_callback_unlock_free ()
{
    return _tralloc_unlock_debug_callback ( &_tralloc_debug_callback_free_lock );
}
