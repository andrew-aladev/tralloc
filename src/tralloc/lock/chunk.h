// This file is part of tralloc. Copyright (C) 2013 Andrew Aladjev aladjev.andrew@gmail.com
// tralloc is free software: you can redistribute it and/or modify it under the terms of the GNU General Lesser Public License as published by the Free Software Foundation, either version 3 of the License, or (at your option) any later version.
// tralloc is distributed in the hope that it will be useful, but WITHOUT ANY WARRANTY; without even the implied warranty of MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU General Lesser Public License for more details.
// You should have received a copy of the GNU General Lesser Public License along with tralloc. If not, see <http://www.gnu.org/licenses/>.

#ifndef TRALLOC_LOCK_CHUNK_H
#define TRALLOC_LOCK_CHUNK_H

#include "common.h"

#undef _TRALLOC_INLINE
#ifdef _TRALLOC_LOCK_CHUNK_INCLUDED_FROM_OBJECT
#    define _TRALLOC_INLINE _TRALLOC_INLINE_IN_OBJECT
#else
#    define _TRALLOC_INLINE _TRALLOC_INLINE_IN_HEADER
#endif


_TRALLOC_INLINE
tralloc_error _tralloc_lock_subtree_new_chunk ( _tralloc_chunk * chunk )
{
    _tralloc_lock * lock = _tralloc_get_lock_subtree_from_chunk ( chunk );
    if ( pthread_mutex_init ( lock, NULL ) != 0 ) {
        return TRALLOC_ERROR_MUTEX_FAILED;
    }
    return 0;
}

_TRALLOC_INLINE
tralloc_error _tralloc_lock_subtree_free_chunk ( _tralloc_chunk * chunk )
{
    _tralloc_lock * lock = _tralloc_get_lock_subtree_from_chunk ( chunk );
    if ( pthread_mutex_destroy ( lock ) != 0 ) {
        return TRALLOC_ERROR_MUTEX_FAILED;
    }
    return 0;
}

_TRALLOC_INLINE
tralloc_error _tralloc_lock_subtree_chunk ( _tralloc_chunk * chunk )
{
    _tralloc_lock * lock = _tralloc_get_lock_children_from_chunk ( chunk );
    if ( pthread_mutex_lock ( lock ) != 0 ) {
        return TRALLOC_ERROR_MUTEX_FAILED;
    }
    return 0;
}

_TRALLOC_INLINE
tralloc_error _tralloc_unlock_subtree_chunk ( _tralloc_chunk * chunk )
{
    _tralloc_lock * lock = _tralloc_get_lock_children_from_chunk ( chunk );
    if ( pthread_mutex_unlock ( lock ) != 0 ) {
        return TRALLOC_ERROR_MUTEX_FAILED;
    }
    return 0;
}

_TRALLOC_INLINE
tralloc_error _tralloc_lock_children_new_chunk ( _tralloc_chunk * chunk )
{
    _tralloc_lock * lock = _tralloc_get_lock_children_from_chunk ( chunk );
    if ( pthread_mutex_init ( lock, NULL ) != 0 ) {
        return TRALLOC_ERROR_MUTEX_FAILED;
    }
    return 0;
}

_TRALLOC_INLINE
tralloc_error _tralloc_lock_children_free_chunk ( _tralloc_chunk * chunk )
{
    _tralloc_lock * lock = _tralloc_get_lock_children_from_chunk ( chunk );
    if ( pthread_mutex_destroy ( lock ) != 0 ) {
        return TRALLOC_ERROR_MUTEX_FAILED;
    }
    return 0;
}

_TRALLOC_INLINE
tralloc_error _tralloc_lock_children_chunk ( _tralloc_chunk * chunk )
{
    _tralloc_lock * lock = _tralloc_get_lock_children_from_chunk ( chunk );
    if ( pthread_mutex_lock ( lock ) != 0 ) {
        return TRALLOC_ERROR_MUTEX_FAILED;
    }
    return 0;
}

_TRALLOC_INLINE
tralloc_error _tralloc_unlock_children_chunk ( _tralloc_chunk * chunk )
{
    _tralloc_lock * lock = _tralloc_get_lock_children_from_chunk ( chunk );
    if ( pthread_mutex_unlock ( lock ) != 0 ) {
        return TRALLOC_ERROR_MUTEX_FAILED;
    }
    return 0;
}


#endif
