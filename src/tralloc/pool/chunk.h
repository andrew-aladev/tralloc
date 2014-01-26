// This file is part of tralloc. Copyright (C) 2013 Andrew Aladjev aladjev.andrew@gmail.com
// tralloc is free software: you can redistribute it and/or modify it under the terms of the GNU General Lesser Public License as published by the Free Software Foundation, either version 3 of the License, or (at your option) any later version.
// tralloc is distributed in the hope that it will be useful, but WITHOUT ANY WARRANTY; without even the implied warranty of MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU General Lesser Public License for more details.
// You should have received a copy of the GNU General Lesser Public License along with tralloc. If not, see <http://www.gnu.org/licenses/>.

#ifndef TRALLOC_POOL_CHUNK_H
#define TRALLOC_POOL_CHUNK_H

#include "../common.h"
#include <stdlib.h>


inline
_tralloc_pool * _tralloc_pool_child_get_pool ( _tralloc_chunk * parent_chunk )
{
    if ( parent_chunk->extensions & TRALLOC_EXTENSION_POOL ) {
        return _tralloc_get_pool_from_chunk ( parent_chunk );
    } else {
        _tralloc_pool_child * pool_child = _tralloc_get_pool_child_from_chunk ( parent_chunk );
        return pool_child->pool;
    }
}

inline
void _tralloc_pool_child_new_chunk ( _tralloc_chunk * chunk, _tralloc_pool * pool, size_t length, _tralloc_pool_child * prev, _tralloc_pool_child * next )
{
    _tralloc_pool_child * pool_child = _tralloc_get_pool_child_from_chunk ( chunk );
    pool_child->pool   = pool;
    pool_child->length = length;
    pool_child->prev   = prev;
    pool_child->next   = next;
}

inline
void _tralloc_pool_child_free_chunk ( _tralloc_chunk * chunk )
{
    _tralloc_pool_child * pool_child = _tralloc_get_pool_child_from_chunk ( chunk );
}


#endif
