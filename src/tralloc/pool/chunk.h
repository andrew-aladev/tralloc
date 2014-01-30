// This file is part of tralloc. Copyright (C) 2013 Andrew Aladjev aladjev.andrew@gmail.com
// tralloc is free software: you can redistribute it and/or modify it under the terms of the GNU General Lesser Public License as published by the Free Software Foundation, either version 3 of the License, or (at your option) any later version.
// tralloc is distributed in the hope that it will be useful, but WITHOUT ANY WARRANTY; without even the implied warranty of MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU General Lesser Public License for more details.
// You should have received a copy of the GNU General Lesser Public License along with tralloc. If not, see <http://www.gnu.org/licenses/>.

#ifndef TRALLOC_POOL_CHUNK_H
#define TRALLOC_POOL_CHUNK_H

#include "../common.h"
#include <stdlib.h>


inline
_tralloc_pool * _tralloc_pool_child_get_pool ( tralloc_context * parent_context )
{
    if ( parent_context == NULL ) {
        return NULL;
    } else {
        _tralloc_chunk * parent_chunk = _tralloc_get_chunk_from_context ( parent_context );
        if ( parent_chunk->extensions & TRALLOC_EXTENSION_POOL ) {
            return _tralloc_get_pool_from_chunk ( parent_chunk );
        } else if ( parent_chunk->extensions & TRALLOC_EXTENSION_POOL_CHILD ) {
            _tralloc_pool_child * parent_pool_child = _tralloc_get_pool_child_from_chunk ( parent_chunk );
            return parent_pool_child->pool;
        } else {
            return NULL;
        }
    }
}

inline
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

inline
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

inline
void _tralloc_pool_child_new_chunk ( _tralloc_chunk * chunk, _tralloc_pool * pool, size_t length, _tralloc_pool_child * prev, _tralloc_pool_child * next )
{
    _tralloc_pool_child * pool_child = _tralloc_get_pool_child_from_chunk ( chunk );
    pool_child->pool   = pool;
    pool_child->length = length;
    _tralloc_pool_child_attach ( pool_child, prev, next );
}

inline
void _tralloc_pool_child_free_chunk ( _tralloc_chunk * chunk )
{
    _tralloc_pool_child * pool_child = _tralloc_get_pool_child_from_chunk ( chunk );
    _tralloc_pool_child_detach ( pool_child );

    size_t prev_fragment_length;
    _tralloc_pool_child * prev = pool_child->prev;
    if ( prev == NULL ) {
        prev_fragment_length = 0;
    } else {
        prev_fragment_length = prev + prev->length - pool_child;
    }

    size_t next_fragment_length;
    _tralloc_pool_child * next = pool_child->next;
    if ( next == NULL ) {
        next_fragment_length = 0;
    } else {
        next_fragment_length = pool_child + pool_child->length - next;
    }
}


#endif
