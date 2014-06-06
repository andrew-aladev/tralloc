// This file is part of tralloc. Copyright (C) 2013 Andrew Aladjev aladjev.andrew@gmail.com
// tralloc is free software: you can redistribute it and/or modify it under the terms of the GNU General Lesser Public License as published by the Free Software Foundation, either version 3 of the License, or (at your option) any later version.
// tralloc is distributed in the hope that it will be useful, but WITHOUT ANY WARRANTY; without even the implied warranty of MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU General Lesser Public License for more details.
// You should have received a copy of the GNU General Lesser Public License along with tralloc. If not, see <http://www.gnu.org/licenses/>.

#define _TRALLOC_POOL_HEAD_CHUNK_INCLUDED_FROM_OBJECT
#include <tralloc/pool/head_chunk.h>
#include <tralloc/common.h>

#include <string.h>


void _tralloc_pool_new_chunk ( _tralloc_chunk * chunk, size_t length )
{
    _tralloc_pool * pool = _tralloc_get_pool_from_chunk ( chunk );
    pool->first_child  = NULL;
    pool->extensions   = chunk->extensions;
    pool->memory       = _tralloc_get_context_from_chunk ( chunk );
    pool->max_fragment = _tralloc_pool_fragment_new_memory ( pool->memory, length );
    pool->length       = length;
    pool->autofree     = TRALLOC_FALSE;
}

void _tralloc_pool_alloc ( _tralloc_pool * pool, void ** memory, size_t length, tralloc_bool zero, _tralloc_pool_child ** prev_pool_child, _tralloc_pool_child ** next_pool_child )
{
    _tralloc_pool_fragment * fragment = pool->max_fragment;
    * prev_pool_child = fragment->prev_child;
    * next_pool_child = fragment->next_child;

    * memory = ( void * ) _tralloc_pool_fragment_alloc ( pool, fragment, length );

    if ( zero ) {
        memset ( * memory, 0, length );
    }
}
