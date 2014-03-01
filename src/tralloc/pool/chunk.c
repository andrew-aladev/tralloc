// This file is part of tralloc. Copyright (C) 2013 Andrew Aladjev aladjev.andrew@gmail.com
// tralloc is free software: you can redistribute it and/or modify it under the terms of the GNU General Lesser Public License as published by the Free Software Foundation, either version 3 of the License, or (at your option) any later version.
// tralloc is distributed in the hope that it will be useful, but WITHOUT ANY WARRANTY; without even the implied warranty of MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU General Lesser Public License for more details.
// You should have received a copy of the GNU General Lesser Public License along with tralloc. If not, see <http://www.gnu.org/licenses/>.

#define _TRALLOC_POOL_CHUNK_INCLUDED_FROM_OBJECT
#include "chunk.h"
#include "fragment.h"
#include "../tree/common.h"
#include <string.h>


static inline
void _tralloc_pool_child_attach ( _tralloc_pool_child * pool_child, _tralloc_pool_child * prev, _tralloc_pool_child * next )
{
    pool_child->prev = prev;
    pool_child->next = next;

    if ( prev != NULL ) {
        prev->next = pool_child;
    } else {
        pool_child->pool->first_child = pool_child;
    }
    if ( next != NULL ) {
        next->prev = pool_child;
    }
}

static inline
void _tralloc_pool_child_detach ( _tralloc_pool_child * pool_child )
{
    _tralloc_pool_child * prev = pool_child->prev;
    _tralloc_pool_child * next = pool_child->next;

    if ( prev != NULL ) {
        prev->next = next;
    } else {
        pool_child->pool->first_child = next;
    }
    if ( next != NULL ) {
        next->prev = prev;
    }
}

static inline
void _tralloc_pool_child_update ( _tralloc_pool_child * pool_child )
{
    _tralloc_pool_child * prev = pool_child->prev;
    _tralloc_pool_child * next = pool_child->next;

    if ( prev != NULL ) {
        prev->next = pool_child;
    } else {
        pool_child->pool->first_child = pool_child;
    }
    if ( next != NULL ) {
        next->prev = pool_child;
    }
}


void _tralloc_pool_child_new_chunk ( _tralloc_chunk * chunk, _tralloc_pool * pool, size_t length, _tralloc_pool_child * prev, _tralloc_pool_child * next )
{
    _tralloc_pool_child * pool_child = _tralloc_get_pool_child_from_chunk ( chunk );
    pool_child->pool   = pool;
    pool_child->length = length;
    _tralloc_pool_child_attach ( pool_child, prev, next );
}


_tralloc_pool_child * _tralloc_pool_child_resize ( _tralloc_pool_child * pool_child, size_t target_length )
{
    size_t next_fragment_length = _tralloc_pool_child_get_next_fragment_length ( pool_child );

    size_t length = pool_child->length + next_fragment_length;
    if ( length >= target_length ) {
        // src pointer will not be changed.
        // resize pool child by next fragment.

        _tralloc_pool_fragment_resize_next_child ( pool_child, target_length, next_fragment_length );

        pool_child->length = target_length;
        return pool_child;
    }

    size_t prev_fragment_length = _tralloc_pool_child_get_prev_fragment_length ( pool_child );

    length += prev_fragment_length;
    if ( length >= target_length ) {
        // src pointer will be changed.
        // prev fragment will be lost.
        // resize pool child by next fragment.

        _tralloc_pool_fragment * prev_fragment = ( _tralloc_pool_fragment * ) ( ( uintptr_t ) pool_child - prev_fragment_length );
        if ( prev_fragment_length >= sizeof ( _tralloc_pool_fragment ) ) {
            _tralloc_pool_fragment_detach ( pool_child->pool, prev_fragment );
        }

        _tralloc_pool_child * new_pool_child = ( _tralloc_pool_child * ) prev_fragment;
        memmove ( new_pool_child, pool_child, pool_child->length );
        new_pool_child->length += prev_fragment_length;
        _tralloc_pool_child_update ( new_pool_child );

        _tralloc_pool_fragment_resize_next_child ( pool_child, target_length, next_fragment_length );

        new_pool_child->length = target_length;
        return new_pool_child;
    }

    return NULL;
}


tralloc_error _tralloc_pool_child_free_chunk ( _tralloc_chunk * chunk )
{
    _tralloc_pool_child * pool_child      = _tralloc_get_pool_child_from_chunk ( chunk );
    _tralloc_pool       * pool            = pool_child->pool;
    _tralloc_pool_child * prev_pool_child = pool_child->prev;
    _tralloc_pool_child * next_pool_child = pool_child->next;

    _tralloc_pool_child_detach ( pool_child );

    _tralloc_pool_fragment_free_child ( pool_child, _tralloc_pool_child_get_prev_fragment_length ( pool_child ), _tralloc_pool_child_get_next_fragment_length ( pool_child ) );

    if ( prev_pool_child == NULL && next_pool_child == NULL ) {
        if ( pool->autofree ) {
            return _tralloc_free_single ( _tralloc_get_chunk_from_pool ( pool ) );
        }
    }

    return 0;
}
