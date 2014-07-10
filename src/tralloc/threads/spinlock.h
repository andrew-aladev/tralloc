// This file is part of tralloc. Copyright (C) 2013 Andrew Aladjev aladjev.andrew@gmail.com
// tralloc is free software: you can redistribute it and/or modify it under the terms of the GNU General Lesser Public License as published by the Free Software Foundation, either version 3 of the License, or (at your option) any later version.
// tralloc is distributed in the hope that it will be useful, but WITHOUT ANY WARRANTY; without even the implied warranty of MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU General Lesser Public License for more details.
// You should have received a copy of the GNU General Lesser Public License along with tralloc. If not, see <http://www.gnu.org/licenses/>.

#if !defined ( TRALLOC_THREADS_SPINLOCK_H )
#define TRALLOC_THREADS_SPINLOCK_H

#include "../types.h"
#include "../macro.h"

#undef _TRALLOC_INLINE
#if defined ( _TRALLOC_INCLUDED_FROM_THREADS_SPINLOCK_C )
#    define _TRALLOC_INLINE _TRALLOC_INLINE_IN_OBJECT
#else
#    define _TRALLOC_INLINE _TRALLOC_INLINE_IN_HEADER
#endif


_TRALLOC_INLINE
tralloc_error _tralloc_spinlock_new ( _tralloc_spinlock * spinlock )
{
    if ( pthread_spin_init ( spinlock, 0 ) != 0 ) {
        return TRALLOC_ERROR_SPINLOCK_FAILED;
    }
    return 0;
}

_TRALLOC_INLINE
tralloc_error _tralloc_spinlock_lock ( _tralloc_spinlock * spinlock )
{
    if ( pthread_spin_lock ( spinlock ) != 0 ) {
        return TRALLOC_ERROR_SPINLOCK_FAILED;
    }
    return 0;
}

_TRALLOC_INLINE
tralloc_error _tralloc_spinlock_unlock ( _tralloc_spinlock * spinlock )
{
    if ( pthread_spin_unlock ( spinlock ) != 0 ) {
        return TRALLOC_ERROR_SPINLOCK_FAILED;
    }
    return 0;
}

_TRALLOC_INLINE
tralloc_error _tralloc_spinlock_free ( _tralloc_spinlock * spinlock )
{
    if ( pthread_spin_destroy ( spinlock ) != 0 ) {
        return TRALLOC_ERROR_SPINLOCK_FAILED;
    }
    return 0;
}


#endif
