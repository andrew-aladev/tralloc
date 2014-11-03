// This file is part of tralloc. Copyright (C) 2013 Andrew Aladjev aladjev.andrew@gmail.com
// tralloc is free software: you can redistribute it and/or modify it under the terms of the GNU General Lesser Public License as published by the Free Software Foundation, either version 3 of the License, or (at your option) any later version.
// tralloc is distributed in the hope that it will be useful, but WITHOUT ANY WARRANTY; without even the implied warranty of MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU General Lesser Public License for more details.
// You should have received a copy of the GNU General Lesser Public License along with tralloc. If not, see <http://www.gnu.org/licenses/>.

#if !defined ( TRALLOC_POOL_CHUNK_H )
#define TRALLOC_POOL_CHUNK_H

#include "../common.h"

#undef _TRALLOC_INLINE
#if defined ( _TRALLOC_INCLUDED_FROM_POOL_CHUNK_C )
#    define _TRALLOC_INLINE _TRALLOC_INLINE_IN_OBJECT
#else
#    define _TRALLOC_INLINE _TRALLOC_INLINE_IN_HEADER
#endif


_TRALLOC_INLINE
_tralloc_pool_child * _tralloc_chunk_get_pool_child ( _tralloc_chunk * chunk )
{
    return ( _tralloc_pool_child * ) ( ( uintptr_t ) chunk - _tralloc_extensions_get_offset_for_extension ( chunk->extensions, TRALLOC_EXTENSION_POOL_CHILD ) );
}

_TRALLOC_INLINE
_tralloc_pool * _tralloc_chunk_get_pool ( _tralloc_chunk * chunk )
{
    return ( _tralloc_pool * ) ( ( uintptr_t ) chunk - _tralloc_extensions_get_offset_for_extension ( chunk->extensions, TRALLOC_EXTENSION_POOL ) );
}

_TRALLOC_INLINE
_tralloc_chunk * _tralloc_pool_get_chunk ( _tralloc_pool * pool )
{
    return ( _tralloc_chunk * ) ( ( uintptr_t ) pool + _tralloc_extensions_get_offset_for_extension ( pool->extensions, TRALLOC_EXTENSION_POOL ) );
}

_TRALLOC_INLINE
_tralloc_pool * _tralloc_chunk_get_closest_pool ( _tralloc_chunk * chunk )
{
    if ( chunk->extensions & TRALLOC_EXTENSION_POOL ) {
        return _tralloc_chunk_get_pool ( chunk );
    } else if ( chunk->extensions & TRALLOC_EXTENSION_POOL_CHILD ) {
        _tralloc_pool_child * parent_pool_child = _tralloc_chunk_get_pool_child ( chunk );
        return parent_pool_child->pool;
    } else {
        return NULL;
    }
}

#if defined ( TRALLOC_THREADS )
_TRALLOC_INLINE
_tralloc_pool_lock * _tralloc_chunk_get_pool_lock ( _tralloc_chunk * chunk )
{
    return ( _tralloc_pool_lock * ) ( ( uintptr_t ) chunk - _tralloc_extensions_get_offset_for_extension ( chunk->extensions, TRALLOC_EXTENSION_LOCK_POOL ) );
}
#endif


#endif
