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
size_t _tralloc_pool_child_get_prev_fragment_length ( _tralloc_pool_child * pool_child )
{
    _tralloc_pool_child * prev = pool_child->prev;
    if ( prev == NULL ) {
        return ( uintptr_t ) pool_child - ( uintptr_t ) pool_child->pool->memory;
    } else {
        return ( uintptr_t ) pool_child - ( uintptr_t ) prev - prev->length;
    }
}

inline
size_t _tralloc_pool_child_get_next_fragment_length ( _tralloc_pool_child * pool_child )
{
    _tralloc_pool_child * next = pool_child->next;
    if ( next == NULL ) {
        _tralloc_pool * pool = pool_child->pool;
        return ( uintptr_t ) pool->memory + pool->length - ( uintptr_t ) pool_child - pool_child->length;
    } else {
        return ( uintptr_t ) next - ( uintptr_t ) pool_child - pool_child->length;
    }
}

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
void _tralloc_pool_child_new_chunk ( _tralloc_chunk * chunk, _tralloc_pool * pool, size_t length, _tralloc_pool_child * prev, _tralloc_pool_child * next )
{
    _tralloc_pool_child * pool_child = _tralloc_get_pool_child_from_chunk ( chunk );
    pool_child->pool   = pool;
    pool_child->length = length;
    _tralloc_pool_child_attach ( pool_child, prev, next );
}

inline
_tralloc_pool_child * _tralloc_pool_child_resize ( _tralloc_pool_child * pool_child, size_t target_length )
{
    _tralloc_pool * pool     = pool_child->pool;
    size_t pool_child_length = pool_child->length;

    size_t next_fragment_length = _tralloc_pool_child_get_next_fragment_length ( pool_child );
    _tralloc_pool_fragment * next_fragment;

    size_t length = pool_child_length + next_fragment_length;
    if ( length >= target_length ) {
        // src pointer will not be changed.
        // resize pool child by next fragment.

        size_t new_next_fragment_length = length - target_length;

        if ( next_fragment_length < sizeof ( _tralloc_pool_fragment ) ) {
            if ( new_next_fragment_length >= sizeof ( _tralloc_pool_fragment ) ) {
                next_fragment = ( _tralloc_pool_fragment * ) ( ( uintptr_t ) pool_child + target_length );

                next_fragment->length     = new_next_fragment_length;
                next_fragment->prev_child = pool_child;
                next_fragment->next_child = pool_child->next;
                next_fragment->prev       = NULL;
                next_fragment->next       = pool->max_fragment;
                _tralloc_pool_fragment_decreased ( pool, next_fragment );
            }
        } else {
            next_fragment = ( _tralloc_pool_fragment * ) ( ( uintptr_t ) pool_child + pool_child_length );
            _tralloc_pool_fragment_resize_with_right_align ( pool, next_fragment, new_next_fragment_length );
        }

        pool_child->length = target_length;
        return pool_child;
    }

    size_t prev_fragment_length = _tralloc_pool_child_get_prev_fragment_length ( pool_child );
    _tralloc_pool_fragment * prev_fragment;

    length += prev_fragment_length;
    if ( length >= target_length ) {
        // src pointer will be changed.
        // prev fragment will be lost.
        // resize pool child by next fragment.

        prev_fragment = ( _tralloc_pool_fragment * ) ( ( uintptr_t ) pool_child - prev_fragment_length );
        if ( prev_fragment_length >= sizeof ( _tralloc_pool_fragment ) ) {
            _tralloc_pool_fragment_detach ( pool, prev_fragment );
        }

        _tralloc_pool_child * new_pool_child = ( _tralloc_pool_child * ) prev_fragment;
        memmove ( new_pool_child, pool_child, pool_child_length );
        new_pool_child->length = target_length;

        size_t new_next_fragment_length = length - target_length;

        if ( next_fragment_length < sizeof ( _tralloc_pool_fragment ) ) {
            if ( new_next_fragment_length >= sizeof ( _tralloc_pool_fragment ) ) {
                next_fragment = ( _tralloc_pool_fragment * ) ( ( uintptr_t ) new_pool_child + target_length );

                next_fragment->length     = new_next_fragment_length;
                next_fragment->prev_child = new_pool_child;
                next_fragment->next_child = new_pool_child->next;
                next_fragment->prev       = NULL;
                next_fragment->next       = pool->max_fragment;
                _tralloc_pool_fragment_decreased ( pool, next_fragment );
            }
        } else {
            next_fragment = ( _tralloc_pool_fragment * ) ( ( uintptr_t ) pool_child + pool_child_length );
            _tralloc_pool_fragment_resize_with_right_align ( pool, next_fragment, new_next_fragment_length );
        }

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
    _tralloc_pool_child_detach ( pool_child );

    size_t prev_fragment_length = _tralloc_pool_child_get_prev_fragment_length ( pool_child );
    size_t next_fragment_length = _tralloc_pool_child_get_next_fragment_length ( pool_child );

    size_t length = prev_fragment_length + pool_child_length + next_fragment_length;
    if ( length < sizeof ( _tralloc_pool_fragment ) ) {
        return;
    }

    _tralloc_pool_fragment * prev_fragment = ( _tralloc_pool_fragment * ) ( ( uintptr_t ) pool_child - prev_fragment_length );
    _tralloc_pool_fragment * next_fragment = ( _tralloc_pool_fragment * ) ( ( uintptr_t ) pool_child + pool_child_length );

    if ( prev_fragment_length < sizeof ( _tralloc_pool_fragment ) ) {
        if ( next_fragment_length < sizeof ( _tralloc_pool_fragment ) ) {
            // no registered prev and next fragments.
            // creating new registered fragment.

            prev_fragment->length     = length;
            prev_fragment->prev_child = pool_child->prev;
            prev_fragment->next_child = pool_child->next;
            prev_fragment->prev       = NULL;
            prev_fragment->next       = pool->max_fragment;
            _tralloc_pool_fragment_decreased ( pool, prev_fragment );
        } else {
            // no registered prev fragment.
            // next fragment is registered.
            // increasing length of next fragment.

            _tralloc_pool_fragment_detach ( pool, next_fragment );

            next_fragment->length     = length;
            next_fragment->prev_child = pool_child->prev;

            memmove ( prev_fragment, next_fragment, sizeof ( _tralloc_pool_fragment ) );
            _tralloc_pool_fragment_increased ( pool, prev_fragment );
        }
    } else {
        if ( next_fragment_length < sizeof ( _tralloc_pool_fragment ) ) {
            // no registered next fragment.
            // prev fragment is registered.
            // increasing length of prev fragment.

            _tralloc_pool_fragment_detach ( pool, prev_fragment );

            prev_fragment->length     = length;
            prev_fragment->next_child = pool_child->next;

            _tralloc_pool_fragment_increased ( pool, prev_fragment );
        } else {
            // both registered prev and next fragments.
            // increasing length of maximum between prev or next.
            // detaching minimum between prev and next.

            _tralloc_pool_fragment_detach ( pool, prev_fragment );
            _tralloc_pool_fragment_detach ( pool, next_fragment );

            if ( prev_fragment_length < next_fragment_length ) {
                next_fragment->prev_child = pool_child->prev;
                memmove ( prev_fragment, next_fragment, sizeof ( _tralloc_pool_fragment ) );
            } else {
                prev_fragment->next_child = pool_child->next;
            }
            prev_fragment->length = length;

            _tralloc_pool_fragment_increased ( pool, prev_fragment );
        }
    }
}


#endif


