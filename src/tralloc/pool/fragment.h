// This file is part of tralloc. Copyright (C) 2013 Andrew Aladjev aladjev.andrew@gmail.com
// tralloc is free software: you can redistribute it and/or modify it under the terms of the GNU General Lesser Public License as published by the Free Software Foundation, either version 3 of the License, or (at your option) any later version.
// tralloc is distributed in the hope that it will be useful, but WITHOUT ANY WARRANTY; without even the implied warranty of MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU General Lesser Public License for more details.
// You should have received a copy of the GNU General Lesser Public License along with tralloc. If not, see <http://www.gnu.org/licenses/>.

#if !defined ( TRALLOC_POOL_FRAGMENT_H )
#define TRALLOC_POOL_FRAGMENT_H

#include "../macro.h"
#include "../types.h"

#undef _TRALLOC_INLINE
#if defined ( _TRALLOC_INCLUDED_FROM_POOL_FRAGMENT_C )
#    define _TRALLOC_INLINE _TRALLOC_INLINE_IN_OBJECT
#else
#    define _TRALLOC_INLINE _TRALLOC_INLINE_IN_HEADER
#endif


_TRALLOC_INLINE
_tralloc_pool_fragment * _tralloc_new_pool_fragment ( void * memory, size_t length )
{
    if ( length < sizeof ( _tralloc_pool_fragment ) ) {
        return NULL;
    }

    _tralloc_pool_fragment * fragment = memory;
    fragment->prev       = NULL;
    fragment->next       = NULL;
    fragment->prev_child = NULL;
    fragment->next_child = NULL;
    fragment->length     = length;
    return fragment;
}

void _tralloc_attach_pool_fragment ( _tralloc_pool * pool, _tralloc_pool_fragment * current, _tralloc_pool_fragment * prev, _tralloc_pool_fragment * next );
void _tralloc_detach_pool_fragment ( _tralloc_pool * pool, _tralloc_pool_fragment * current );

_tralloc_pool_child * _tralloc_alloc_from_pool_fragment               ( _tralloc_pool * pool, _tralloc_pool_fragment * fragment, size_t length );
void                  _tralloc_resize_pool_fragment_by_prev_child     ( _tralloc_pool_child * pool_child, size_t target_length, size_t next_length );
void                  _tralloc_merge_pool_fragments_around_pool_child ( _tralloc_pool_child * pool_child, size_t prev_length,   size_t next_length );


#endif
