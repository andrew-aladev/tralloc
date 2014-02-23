// This file is part of tralloc. Copyright (C) 2013 Andrew Aladjev aladjev.andrew@gmail.com
// tralloc is free software: you can redistribute it and/or modify it under the terms of the GNU General Lesser Public License as published by the Free Software Foundation, either version 3 of the License, or (at your option) any later version.
// tralloc is distributed in the hope that it will be useful, but WITHOUT ANY WARRANTY; without even the implied warranty of MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU General Lesser Public License for more details.
// You should have received a copy of the GNU General Lesser Public License along with tralloc. If not, see <http://www.gnu.org/licenses/>.

#ifndef TRALLOC_POOL_HEAD_CHUNK_H
#define TRALLOC_POOL_HEAD_CHUNK_H

#include "common.h"
#include "../macro.h"


void _tralloc_pool_new_chunk ( _tralloc_chunk * chunk, size_t length );
bool _tralloc_pool_can_alloc ( _tralloc_pool * pool, size_t length );
void _tralloc_pool_alloc     ( _tralloc_pool * pool, void ** memory, size_t length, bool zero, _tralloc_pool_child ** prev_pool_child, _tralloc_pool_child ** next_pool_child );

inline
bool _tralloc_pool_can_free_chunk ( _tralloc_chunk * chunk )
{
    _tralloc_pool * pool = _tralloc_get_pool_from_chunk ( chunk );
    if ( pool->first_child == NULL ) {
        return true;
    } else {
        pool->autofree = true;
        return false;
    }
}

inline
bool _tralloc_pool_can_free_chunk_children ( _tralloc_chunk * UNUSED ( chunk ) )
{
    return true;
}


#endif
