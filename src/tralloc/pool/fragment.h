// This file is part of tralloc. Copyright (C) 2013 Andrew Aladjev aladjev.andrew@gmail.com
// tralloc is free software: you can redistribute it and/or modify it under the terms of the GNU General Lesser Public License as published by the Free Software Foundation, either version 3 of the License, or (at your option) any later version.
// tralloc is distributed in the hope that it will be useful, but WITHOUT ANY WARRANTY; without even the implied warranty of MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU General Lesser Public License for more details.
// You should have received a copy of the GNU General Lesser Public License along with tralloc. If not, see <http://www.gnu.org/licenses/>.

#ifndef TRALLOC_POOL_FRAGMENT_H
#define TRALLOC_POOL_FRAGMENT_H

#include "../types.h"


inline
_tralloc_pool_fragment * _tralloc_pool_fragment_new_memory ( void * memory, size_t length )
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

void _tralloc_pool_fragment_attach ( _tralloc_pool * pool, _tralloc_pool_fragment * fragment, _tralloc_pool_fragment * prev, _tralloc_pool_fragment * next );
void _tralloc_pool_fragment_detach ( _tralloc_pool * pool, _tralloc_pool_fragment * fragment );

void _tralloc_pool_fragment_increased ( _tralloc_pool * pool, _tralloc_pool_fragment * fragment );
void _tralloc_pool_fragment_decreased ( _tralloc_pool * pool, _tralloc_pool_fragment * fragment );

inline
bool _tralloc_pool_fragment_can_alloc ( _tralloc_pool_fragment * fragment, size_t length )
{
    if ( fragment == NULL || length > fragment->length ) {
        return false;
    }
    return true;
}

_tralloc_pool_child * _tralloc_pool_fragment_alloc ( _tralloc_pool * pool, _tralloc_pool_fragment * fragment, size_t length );


void _tralloc_pool_fragment_resize_next_child ( _tralloc_pool_child * pool_child, size_t target_length, size_t next_length );
void _tralloc_pool_fragment_free_child        ( _tralloc_pool_child * pool_child, size_t prev_length,   size_t next_length );


#endif
