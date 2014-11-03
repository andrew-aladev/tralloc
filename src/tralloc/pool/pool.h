// This file is part of tralloc. Copyright (C) 2013 Andrew Aladjev aladjev.andrew@gmail.com
// tralloc is free software: you can redistribute it and/or modify it under the terms of the GNU General Lesser Public License as published by the Free Software Foundation, either version 3 of the License, or (at your option) any later version.
// tralloc is distributed in the hope that it will be useful, but WITHOUT ANY WARRANTY; without even the implied warranty of MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU General Lesser Public License for more details.
// You should have received a copy of the GNU General Lesser Public License along with tralloc. If not, see <http://www.gnu.org/licenses/>.

#if !defined ( TRALLOC_POOL_POOL_H )
#define TRALLOC_POOL_POOL_H

#include "../types.h"
#include "../macro.h"

#undef _TRALLOC_INLINE
#if defined ( _TRALLOC_INCLUDED_FROM_POOL_POOL_C )
#    define _TRALLOC_INLINE _TRALLOC_INLINE_IN_OBJECT
#else
#    define _TRALLOC_INLINE _TRALLOC_INLINE_IN_HEADER
#endif


_TRALLOC_INLINE
tralloc_bool _tralloc_pool_can_alloc ( _tralloc_pool * pool, size_t length )
{
    _tralloc_pool_fragment * fragment = pool->max_fragment;
    if ( fragment == NULL || length > fragment->length ) {
        return TRALLOC_FALSE;
    } else {
        return TRALLOC_TRUE;
    }
}

_TRALLOC_INLINE
tralloc_bool _tralloc_pool_can_freed ( _tralloc_pool * pool )
{
    if ( pool->first_child == NULL ) {
        return TRALLOC_TRUE;
    } else {
        if ( !pool->autofree ) {
            pool->autofree = TRALLOC_TRUE;
        }
        return TRALLOC_FALSE;
    }
}

_TRALLOC_INLINE
tralloc_bool _tralloc_pool_can_free_children ( _tralloc_pool * _TRALLOC_UNUSED ( pool ) )
{
    return TRALLOC_TRUE;
}

void _tralloc_pool_new   ( _tralloc_pool * pool, void * memory, tralloc_extensions extensions, size_t length );
void _tralloc_pool_alloc ( _tralloc_pool * pool, void ** memory, size_t length, tralloc_bool zero, _tralloc_pool_child ** prev_pool_child, _tralloc_pool_child ** next_pool_child );


#endif
