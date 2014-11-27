// This file is part of tralloc. Copyright (C) 2013 Andrew Aladjev aladjev.andrew@gmail.com
// tralloc is free software: you can redistribute it and/or modify it under the terms of the GNU General Lesser Public License as published by the Free Software Foundation, either version 3 of the License, or (at your option) any later version.
// tralloc is distributed in the hope that it will be useful, but WITHOUT ANY WARRANTY; without even the implied warranty of MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU General Lesser Public License for more details.
// You should have received a copy of the GNU General Lesser Public License along with tralloc. If not, see <http://www.gnu.org/licenses/>.

#if !defined ( TRALLOC_TREE_CHUNK_H )
#define TRALLOC_TREE_CHUNK_H

#include "../extensions.h"

#undef _TRALLOC_INLINE
#if defined ( _TRALLOC_INCLUDED_FROM_TREE_CHUNK_C )
#    define _TRALLOC_INLINE _TRALLOC_INLINE_IN_OBJECT
#else
#    define _TRALLOC_INLINE _TRALLOC_INLINE_IN_HEADER
#endif


#if defined ( TRALLOC_THREADS )
_TRALLOC_INLINE
_tralloc_subtree_lock * _tralloc_chunk_get_subtree_lock ( _tralloc_chunk * chunk )
{
    return ( _tralloc_subtree_lock * ) ( ( uintptr_t ) chunk - _tralloc_extensions_get_offset_for_extension ( chunk->extensions, TRALLOC_EXTENSION_LOCK_SUBTREE ) );
}

_TRALLOC_INLINE
_tralloc_children_lock * _tralloc_chunk_get_children_lock ( _tralloc_chunk * chunk )
{
    return ( _tralloc_children_lock * ) ( ( uintptr_t ) chunk - _tralloc_extensions_get_offset_for_extension ( chunk->extensions, TRALLOC_EXTENSION_LOCK_CHILDREN ) );
}
#endif

void _tralloc_chunk_update ( _tralloc_chunk * chunk );
void _tralloc_chunk_attach ( _tralloc_chunk * chunk, _tralloc_chunk * new_parent_chunk );


#endif
