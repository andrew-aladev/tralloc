// This file is part of tralloc. Copyright (C) 2013 Andrew Aladjev aladjev.andrew@gmail.com
// tralloc is free software: you can redistribute it and/or modify it under the terms of the GNU General Lesser Public License as published by the Free Software Foundation, either version 3 of the License, or (at your option) any later version.
// tralloc is distributed in the hope that it will be useful, but WITHOUT ANY WARRANTY; without even the implied warranty of MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU General Lesser Public License for more details.
// You should have received a copy of the GNU General Lesser Public License along with tralloc. If not, see <http://www.gnu.org/licenses/>.

#ifndef TRALLOC_POOL_CHUNK_H
#define TRALLOC_POOL_CHUNK_H

#include "fragment.h"
#include "../common.h"
#include <string.h>


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
void _tralloc_pool_child_attach ( _tralloc_pool * pool, _tralloc_pool_child * pool_child, _tralloc_pool_child * prev, _tralloc_pool_child * next )
{
    pool_child->prev = prev;
    pool_child->next = next;

    if ( prev != NULL ) {
        prev->next = pool_child;
    } else {
        pool->first_child = pool_child;
    }
    if ( next != NULL ) {
        next->prev = pool_child;
    }
}

inline
void _tralloc_pool_child_detach ( _tralloc_pool * pool, _tralloc_pool_child * pool_child )
{
    _tralloc_pool_child * prev = pool_child->prev;
    _tralloc_pool_child * next = pool_child->next;

    if ( prev != NULL ) {
        prev->next = next;
    } else {
        pool->first_child = next;
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
    _tralloc_pool_child_attach ( pool, pool_child, prev, next );
}

inline
_tralloc_pool_child * _tralloc_pool_child_resize ( _tralloc_pool_child * pool_child, size_t target_length )
{
    _tralloc_pool * pool     = pool_child->pool;
    size_t pool_child_length = pool_child->length;

    size_t next_fragment_length;
    _tralloc_pool_child * next = pool_child->next;
    if ( next == NULL ) {
        next_fragment_length = 0;
    } else {
        next_fragment_length = pool_child + pool_child_length - next;
    }

    size_t length = pool_child_length + next_fragment_length;
    if ( length >= target_length ) {
        // src pointer will not be changed.
        // resize pool child by next fragment.

        if ( next_fragment_length >= sizeof ( _tralloc_pool_fragment ) ) {
            _tralloc_pool_fragment_resize_to_left ( pool, ( _tralloc_pool_fragment * ) ( pool_child + pool_child_length ), length - target_length );
        }

        pool_child->length = target_length;
        return pool_child;
    }

    size_t prev_fragment_length;
    _tralloc_pool_child * prev = pool_child->prev;
    if ( prev == NULL ) {
        prev_fragment_length = 0;
    } else {
        prev_fragment_length = prev + prev->length - pool_child;
    }

    length += prev_fragment_length;
    if ( length >= target_length ) {
        // src pointer will be changed.
        // prev fragment will be lost.
        // resize pool child by next fragment.

        _tralloc_pool_fragment * prev_fragment = ( _tralloc_pool_fragment * ) ( pool_child - prev_fragment_length );
        if ( prev_fragment_length >= sizeof ( _tralloc_pool_fragment ) ) {
            _tralloc_pool_fragment_detach ( pool, prev_fragment );
        }
        if ( next_fragment_length >= sizeof ( _tralloc_pool_fragment ) ) {
            _tralloc_pool_fragment_resize_to_left ( pool, ( _tralloc_pool_fragment * ) pool_child + pool_child_length, length - target_length );
        }

        _tralloc_pool_child * new_pool_child = ( _tralloc_pool_child * ) prev_fragment;
        memmove ( new_pool_child, pool_child, pool_child_length );
        pool_child->length = target_length;
        return new_pool_child;
    }

    return NULL;
}

inline
void _tralloc_pool_child_free_chunk ( _tralloc_chunk * chunk )
{
    _tralloc_pool_child * pool_child = _tralloc_get_pool_child_from_chunk ( chunk );

    _tralloc_pool * pool     = pool_child->pool;
    size_t pool_child_length = pool_child->length;
    _tralloc_pool_child_detach ( pool, pool_child );

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
        next_fragment_length = pool_child + pool_child_length - next;
    }

    size_t length = prev_fragment_length + pool_child_length + next_fragment_length;
    if ( length < sizeof ( _tralloc_pool_fragment ) ) {
        return;
    }

    _tralloc_pool_fragment * prev_fragment = ( _tralloc_pool_fragment * ) ( pool_child - prev_fragment_length );
    _tralloc_pool_fragment * next_fragment;

    if ( prev_fragment_length < sizeof ( _tralloc_pool_fragment ) ) {
        if ( next_fragment_length < sizeof ( _tralloc_pool_fragment ) ) {
            // no registered prev and next fragments.
            // creating new registered fragment.

            prev_fragment->length     = length;
            prev_fragment->next_child = next;
            prev_fragment->prev_child = prev;
            _tralloc_pool_fragment_insert_after ( pool, prev_fragment, NULL, pool->max_fragment );
        } else {
            // no registered prev fragment.
            // next fragment is registered.
            // increasing length of next fragment.

            next_fragment = ( _tralloc_pool_fragment * ) ( pool_child + pool_child_length );
            memmove ( prev_fragment, next_fragment, sizeof ( _tralloc_pool_fragment ) );
            _tralloc_pool_fragment_detach ( pool, prev_fragment );
            prev_fragment->length     = length;
            prev_fragment->prev_child = prev;
            _tralloc_pool_fragment_insert_before ( pool, prev_fragment, prev_fragment->prev, prev_fragment->next );
        }
    } else {
        if ( next_fragment_length < sizeof ( _tralloc_pool_fragment ) ) {
            // no registered next fragment.
            // prev fragment is registered.
            // increasing length of prev fragment.

            prev_fragment->length     = length;
            prev_fragment->next_child = next;
            _tralloc_pool_fragment_insert_before ( pool, prev_fragment, prev_fragment->prev, prev_fragment->next );
        } else {
            // both registered prev and next fragments.
            // increasing length of maximum between prev or next.
            // detaching minimum between prev and next.

            next_fragment = ( _tralloc_pool_fragment * ) ( pool_child + pool_child_length );
            _tralloc_pool_fragment_detach ( pool, prev_fragment );
            _tralloc_pool_fragment_detach ( pool, next_fragment );

            if ( prev_fragment_length < next_fragment_length ) {
                memmove ( prev_fragment, next_fragment, sizeof ( _tralloc_pool_fragment ) );
                prev_fragment->prev_child = prev;
            } else {
                prev_fragment->next_child = next;
            }
            prev_fragment->length = length;
            _tralloc_pool_fragment_insert_before ( pool, prev_fragment, prev_fragment->prev, prev_fragment->next );
        }
    }
}


#endif
