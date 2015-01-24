// This file is part of tralloc. Copyright (C) 2013 Andrew Aladjev aladjev.andrew@gmail.com
// tralloc is free software: you can redistribute it and/or modify it under the terms of the GNU General Lesser Public License as published by the Free Software Foundation, either version 3 of the License, or (at your option) any later version.
// tralloc is distributed in the hope that it will be useful, but WITHOUT ANY WARRANTY; without even the implied warranty of MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU General Lesser Public License for more details.
// You should have received a copy of the GNU General Lesser Public License along with tralloc. If not, see <http://www.gnu.org/licenses/>.

#define _TRALLOC_INCLUDED_FROM_POOL_POOL_DATA_C
#include <tralloc/pool/pool_data.h>

#include <string.h>


void _tralloc_pool_new ( _tralloc_pool * pool, void * memory, tralloc_extensions extensions, size_t length )
{
    pool->first_child  = NULL;
    pool->extensions   = extensions;
    pool->memory       = memory;
    pool->max_fragment = _tralloc_pool_fragment_new ( pool->memory, length );
    pool->length       = length;
    pool->autofree     = TRALLOC_FALSE;
}

void _tralloc_pool_alloc ( _tralloc_pool * pool, void ** memory, size_t length, tralloc_bool zero, _tralloc_pool_child ** prev_pool_child, _tralloc_pool_child ** next_pool_child )
{
    _tralloc_pool_fragment * fragment = pool->max_fragment;
    * prev_pool_child = fragment->prev_child;
    * next_pool_child = fragment->next_child;

    * memory = ( void * ) _tralloc_pool_alloc_from_fragment ( pool, fragment, length );

    if ( zero ) {
        memset ( * memory, 0, length );
    }
}

_tralloc_pool * _tralloc_chunk_get_closest_pool ( _tralloc_chunk * chunk )
{
    if ( _tralloc_extensions_have_extension ( chunk->extensions, TRALLOC_EXTENSION_POOL ) ) {
        return _tralloc_chunk_get_pool ( chunk );
    } else if ( _tralloc_extensions_have_extension ( chunk->extensions, TRALLOC_EXTENSION_POOL_CHILD ) ) {
        _tralloc_pool_child * parent_pool_child = _tralloc_chunk_get_pool_child ( chunk );
        return parent_pool_child->pool;
    } else {
        return NULL;
    }
}
