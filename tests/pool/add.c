// This file is part of tralloc. Copyright (C) 2013 Andrew Aladjev aladjev.andrew@gmail.com
// tralloc is free software: you can redistribute it and/or modify it under the terms of the GNU General Public License as published by the Free Software Foundation, either version 3 of the License, or (at your option) any later version.
// tralloc is distributed in the hope that it will be useful, but WITHOUT ANY WARRANTY; without even the implied warranty of MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU General Public License for more details.
// You should have received a copy of the GNU General Public License along with tralloc. If not, see <http://www.gnu.org/licenses/>.

#include "common.h"
#include <tralloc/tree.h>


static
bool test_add_alloc_overflow ( tralloc_context * ctx )
{
    tralloc_context * empty_pool;
    if ( tralloc_with_extensions_new ( ctx, &empty_pool, TRALLOC_EXTENSION_POOL ) != 0 ) {
        return false;
    }
    _tralloc_chunk * pool_chunk = _tralloc_get_chunk_from_context ( empty_pool );
    if ( ! ( pool_chunk->extensions & TRALLOC_EXTENSION_POOL ) ) {
        return false;
    }
    _tralloc_pool * pool = _tralloc_get_pool_from_chunk ( pool_chunk );
    if (
        pool->first_child  != NULL       ||
        pool->max_fragment != NULL       ||
        pool->memory       != empty_pool ||
        pool->length       != 0          ||
        pool->autofree     != false
    ) {
        return false;
    }

    tralloc_context * empty_pool_child;
    if ( tralloc_new ( empty_pool, &empty_pool_child ) != 0 ) {
        return false;
    }
    _tralloc_chunk * pool_child_chunk = _tralloc_get_chunk_from_context ( empty_pool_child );
    if ( pool_child_chunk->extensions & TRALLOC_EXTENSION_POOL_CHILD ) {
        return false;
    }

    if ( tralloc_free ( empty_pool ) != 0 ) {
        return false;
    }

    return true;
}

static
bool test_normal ( tralloc_context * ctx )
{
    tralloc_context * normal_pool;
    if ( tralloc_with_extensions ( ctx, &normal_pool, TRALLOC_EXTENSION_POOL, sizeof ( uint8_t ) * 500 ) != 0 ) {
        return false;
    }
    _tralloc_chunk * pool_chunk = _tralloc_get_chunk_from_context ( normal_pool );
    if ( ! ( pool_chunk->extensions & TRALLOC_EXTENSION_POOL ) ) {
        return false;
    }
    _tralloc_pool * pool = _tralloc_get_pool_from_chunk ( pool_chunk );
    if (
        pool->first_child  != NULL        ||
        pool->max_fragment != normal_pool ||
        pool->memory       != normal_pool ||
        pool->length       != 500         ||
        pool->autofree     != false
    ) {
        return false;
    }

    uint8_t * normal_pool_child;
    if ( tralloc ( normal_pool, ( tralloc_context ** ) &normal_pool_child, sizeof ( uint8_t ) * 5 ) != 0 ) {
        return false;
    }
    _tralloc_chunk * pool_child_chunk = _tralloc_get_chunk_from_context ( normal_pool_child );
    if ( ! ( pool_child_chunk->extensions & TRALLOC_EXTENSION_POOL_CHILD ) ) {
        return false;
    }
    _tralloc_pool_child * pool_child = _tralloc_get_pool_child_from_chunk ( pool_child_chunk );
    if (
        pool_child->length != sizeof ( _tralloc_pool_child ) + sizeof ( _tralloc_chunk ) + sizeof ( uint8_t ) * 5 ||
        
        pool->first_child  != pool_child  ||
        pool->max_fragment != normal_pool ||
        pool->memory       != normal_pool ||
        pool->length       != 500         ||
        pool->autofree     != false
    ) {
        return false;
    }

    if ( tralloc_free ( normal_pool ) != 0 ) {
        return false;
    }

    return true;
}

bool test_add ( tralloc_context * ctx )
{
    if (
        ! test_add_alloc_overflow ( ctx ) ||
        ! test_normal ( ctx )
    ) {
        return false;
    }

    return true;
}

