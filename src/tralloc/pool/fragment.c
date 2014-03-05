// This file is part of tralloc. Copyright (C) 2013 Andrew Aladjev aladjev.andrew@gmail.com
// tralloc is free software: you can redistribute it and/or modify it under the terms of the GNU General Lesser Public License as published by the Free Software Foundation, either version 3 of the License, or (at your option) any later version.
// tralloc is distributed in the hope that it will be useful, but WITHOUT ANY WARRANTY; without even the implied warranty of MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU General Lesser Public License for more details.
// You should have received a copy of the GNU General Lesser Public License along with tralloc. If not, see <http://www.gnu.org/licenses/>.

#define _TRALLOC_POOL_FRAGMENT_INCLUDED_FROM_OBJECT
#include "fragment.h"
#include <string.h>


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

_tralloc_pool_child * _tralloc_pool_fragment_alloc ( _tralloc_pool * pool, _tralloc_pool_fragment * fragment, size_t length )
{
    _tralloc_pool_fragment_detach ( pool, fragment );

    size_t new_fragment_length       = fragment->length - length;
    _tralloc_pool_child * next_child = ( _tralloc_pool_child * ) ( ( uintptr_t ) fragment + new_fragment_length );

    if ( new_fragment_length >= sizeof ( _tralloc_pool_fragment ) ) {
        fragment->next_child = next_child;
        fragment->length     = new_fragment_length;

        _tralloc_pool_fragment_decreased ( pool, fragment );
    }

    return next_child;
}


// before : [ 200 bytes fragment ]
// after  : [ 100 bytes not used ] [ 100 bytes fragment ]
// or
// before : [ 100 bytes not used ] [ 100 bytes fragment ]
// after  : [ 200 bytes fragment ]

void _tralloc_pool_fragment_resize_next_child ( _tralloc_pool_child * pool_child, size_t target_length, size_t next_length )
{
    _tralloc_pool          * pool       = pool_child->pool;
    _tralloc_pool_child    * next_child = pool_child->next;
    _tralloc_pool_fragment * next       = ( _tralloc_pool_fragment * ) ( ( uintptr_t ) pool_child + pool_child->length );
    _tralloc_pool_fragment * new_next   = ( _tralloc_pool_fragment * ) ( ( uintptr_t ) pool_child + target_length );

    size_t new_next_length = pool_child->length + next_length - target_length;

    if ( next_length < sizeof ( _tralloc_pool_fragment ) ) {
        if ( new_next_length < sizeof ( _tralloc_pool_fragment ) ) {
            // no registered next fragment.
            // no enough length to create new registered next fragment.
            return;
        } else {
            // no registered next fragment.
            // creating new registered next fragment.

            new_next->length     = new_next_length;
            new_next->prev_child = pool_child;
            new_next->next_child = next_child;
            new_next->prev       = NULL;
            new_next->next       = pool->max_fragment;
            _tralloc_pool_fragment_decreased ( pool, new_next );
        }
    } else {
        // next fragment is registered.

        _tralloc_pool_fragment_detach ( pool, next );

        if ( new_next_length < sizeof ( _tralloc_pool_fragment ) ) {
            // no enough length to store new registered next fragment.

            return;
        } else {
            // enough length to resize registered next fragment.

            memmove ( new_next, next, sizeof ( _tralloc_pool_fragment ) );
            new_next->length = new_next_length;

            if ( new_next_length > next_length ) {
                _tralloc_pool_fragment_increased ( pool, new_next );
            } else {
                _tralloc_pool_fragment_decreased ( pool, new_next );
            }
        }
    }
}

void _tralloc_pool_fragment_free_child ( _tralloc_pool_child * pool_child, size_t prev_length, size_t next_length )
{
    size_t length = prev_length + pool_child->length + next_length;
    if ( length < sizeof ( _tralloc_pool_fragment ) ) {
        return;
    }

    _tralloc_pool_fragment * prev = ( _tralloc_pool_fragment * ) ( ( uintptr_t ) pool_child - prev_length );
    _tralloc_pool_fragment * next = ( _tralloc_pool_fragment * ) ( ( uintptr_t ) pool_child + pool_child->length );

    _tralloc_pool       * pool       = pool_child->pool;
    _tralloc_pool_child * prev_child = pool_child->prev;
    _tralloc_pool_child * next_child = pool_child->next;

    if ( prev_length < sizeof ( _tralloc_pool_fragment ) ) {
        if ( next_length < sizeof ( _tralloc_pool_fragment ) ) {
            // no registered prev and next fragments.
            // creating new registered fragment.

            prev->length     = length;
            prev->prev_child = prev_child;
            prev->next_child = next_child;
            prev->prev       = NULL;
            prev->next       = pool->max_fragment;
            _tralloc_pool_fragment_decreased ( pool, prev );
        } else {
            // no registered prev fragment.
            // next fragment is registered.
            // increasing length of next fragment.

            _tralloc_pool_fragment_detach ( pool, next );

            next->length     = length;
            next->prev_child = prev_child;

            memmove ( prev, next, sizeof ( _tralloc_pool_fragment ) );
            _tralloc_pool_fragment_increased ( pool, prev );
        }
    } else {
        if ( next_length < sizeof ( _tralloc_pool_fragment ) ) {
            // no registered next fragment.
            // prev fragment is registered.
            // increasing length of prev fragment.

            _tralloc_pool_fragment_detach ( pool, prev );

            prev->length     = length;
            prev->next_child = next_child;

            _tralloc_pool_fragment_increased ( pool, prev );
        } else {
            // both registered prev and next fragments.
            // increasing length of prev fragment.
            // detaching next fragment.

            _tralloc_pool_fragment_detach ( pool, prev );
            _tralloc_pool_fragment_detach ( pool, next );

            prev->next_child = next_child;
            prev->length     = length;

            _tralloc_pool_fragment_increased ( pool, prev );
        }
    }
}


