// This file is part of tralloc. Copyright (C) 2013 Andrew Aladjev aladjev.andrew@gmail.com
// tralloc is free software: you can redistribute it and/or modify it under the terms of the GNU General Lesser Public License as published by the Free Software Foundation, either version 3 of the License, or (at your option) any later version.
// tralloc is distributed in the hope that it will be useful, but WITHOUT ANY WARRANTY; without even the implied warranty of MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU General Lesser Public License for more details.
// You should have received a copy of the GNU General Lesser Public License along with tralloc. If not, see <http://www.gnu.org/licenses/>.

#ifndef TRALLOC_POOL_FRAGMENT_H
#define TRALLOC_POOL_FRAGMENT_H

#include "../types.h"
#include <string.h>


inline
void _tralloc_pool_fragment_attach ( _tralloc_pool * pool, _tralloc_pool_fragment * fragment, _tralloc_pool_fragment * prev, _tralloc_pool_fragment * next )
{
    fragment->prev = prev;
    fragment->next = next;

    if ( prev != NULL ) {
        prev->next = fragment;
    } else {
        pool->max_fragment = fragment;
    }
    if ( next != NULL ) {
        next->prev = fragment;
    }
}

inline
void _tralloc_pool_fragment_detach ( _tralloc_pool * pool, _tralloc_pool_fragment * fragment )
{
    _tralloc_pool_fragment * prev = fragment->prev;
    _tralloc_pool_fragment * next = fragment->next;

    if ( prev != NULL ) {
        prev->next = next;
    } else {
        pool->max_fragment = next;
    }
    if ( next != NULL ) {
        next->prev = prev;
    }
}

inline
void _tralloc_pool_fragment_increased ( _tralloc_pool * pool, _tralloc_pool_fragment * fragment )
{
    _tralloc_pool_fragment * prev_fragment = fragment->prev;
    _tralloc_pool_fragment * next_fragment = fragment->next;

    while ( prev_fragment != NULL && fragment->length > prev_fragment->length ) {
        prev_fragment = prev_fragment->prev;
        next_fragment = prev_fragment;
    }

    _tralloc_pool_fragment_attach ( pool, fragment, prev_fragment, next_fragment );
}

inline
void _tralloc_pool_fragment_decreased ( _tralloc_pool * pool, _tralloc_pool_fragment * fragment )
{
    _tralloc_pool_fragment * prev_fragment = fragment->prev;
    _tralloc_pool_fragment * next_fragment = fragment->next;

    while ( next_fragment != NULL && fragment->length < next_fragment->length ) {
        prev_fragment = next_fragment;
        next_fragment = next_fragment->next;
    }

    _tralloc_pool_fragment_attach ( pool, fragment, prev_fragment, next_fragment );
}

// before : [ 200 bytes fragment ]
// after  : [ 100 bytes not used ] [ 100 bytes fragment ]
// or
// before : [ 100 bytes not used ] [ 100 bytes fragment ]
// after  : [ 200 bytes fragment ]

inline
void _tralloc_pool_fragment_resize_with_right_align ( _tralloc_pool * pool, _tralloc_pool_fragment * fragment, size_t new_fragment_length )
{
    _tralloc_pool_fragment_detach ( pool, fragment );

    if ( new_fragment_length >= sizeof ( _tralloc_pool_fragment ) ) {
        _tralloc_pool_fragment * new_fragment = ( _tralloc_pool_fragment * ) ( ( uintptr_t ) fragment + fragment->length - new_fragment_length );
        memmove ( new_fragment, fragment, sizeof ( _tralloc_pool_fragment ) );
        new_fragment->length = new_fragment_length;

        if ( new_fragment_length > fragment->length ) {
            _tralloc_pool_fragment_increased ( pool, new_fragment );
        } else {
            _tralloc_pool_fragment_decreased ( pool, new_fragment );
        }
    }
}


#endif
