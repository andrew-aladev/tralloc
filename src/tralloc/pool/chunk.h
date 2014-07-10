// This file is part of tralloc. Copyright (C) 2013 Andrew Aladjev aladjev.andrew@gmail.com
// tralloc is free software: you can redistribute it and/or modify it under the terms of the GNU General Lesser Public License as published by the Free Software Foundation, either version 3 of the License, or (at your option) any later version.
// tralloc is distributed in the hope that it will be useful, but WITHOUT ANY WARRANTY; without even the implied warranty of MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU General Lesser Public License for more details.
// You should have received a copy of the GNU General Lesser Public License along with tralloc. If not, see <http://www.gnu.org/licenses/>.

#if !defined ( TRALLOC_POOL_CHUNK_H )
#define TRALLOC_POOL_CHUNK_H

#include "common.h"
#include "../common.h"

#undef _TRALLOC_INLINE
#if defined ( _TRALLOC_INCLUDED_FROM_POOL_CHUNK_C )
#    define _TRALLOC_INLINE _TRALLOC_INLINE_IN_OBJECT
#else
#    define _TRALLOC_INLINE _TRALLOC_INLINE_IN_HEADER
#endif


_TRALLOC_INLINE
_tralloc_pool * _tralloc_pool_child_get_pool_from_chunk ( _tralloc_chunk * chunk )
{
    if ( chunk->extensions & TRALLOC_EXTENSION_POOL ) {
        return _tralloc_get_pool_from_chunk ( chunk );
    } else if ( chunk->extensions & TRALLOC_EXTENSION_POOL_CHILD ) {
        _tralloc_pool_child * parent_pool_child = _tralloc_get_pool_child_from_chunk ( chunk );
        return parent_pool_child->pool;
    } else {
        return NULL;
    }
}

// Function returns size of empty space ( >= 0 ).
// There is no warranty, that this size will be enough for "_tralloc_pool_fragment".
_TRALLOC_INLINE
size_t _tralloc_pool_child_get_prev_fragment_length ( _tralloc_pool_child * pool_child )
{
    _tralloc_pool_child * prev = pool_child->prev;
    if ( prev == NULL ) {
        // Returning size of empty space between pool memory's start and first "pool_child".
        return ( uintptr_t ) pool_child - ( uintptr_t ) pool_child->pool->memory;
    } else {
        // Returning size of empty space between prev and current pool_childs.
        return ( uintptr_t ) pool_child - ( uintptr_t ) prev - prev->length;
    }
}

// Function returns size of empty space ( >= 0 ).
// There is no warranty, that this size will be enough for "_tralloc_pool_fragment".
_TRALLOC_INLINE
size_t _tralloc_pool_child_get_next_fragment_length ( _tralloc_pool_child * pool_child )
{
    _tralloc_pool_child * next = pool_child->next;
    if ( next == NULL ) {
        // Returning size of empty space between last "pool_child" and pool memory's end.
        return ( uintptr_t ) pool_child->pool->memory + pool_child->pool->length - ( uintptr_t ) pool_child - pool_child->length;
    } else {
        // Returning size of empty space between current and next "pool_child"s.
        return ( uintptr_t ) next - ( uintptr_t ) pool_child - pool_child->length;
    }
}

void                  _tralloc_pool_child_new_chunk  ( _tralloc_chunk * chunk, _tralloc_pool * pool, size_t length, _tralloc_pool_child * prev, _tralloc_pool_child * next );
_tralloc_pool_child * _tralloc_pool_child_resize     ( _tralloc_pool_child * pool_child, size_t target_length );
tralloc_error         _tralloc_pool_child_free_chunk ( _tralloc_chunk * chunk );


#endif
