// This file is part of tralloc. Copyright (C) 2013 Andrew Aladjev aladjev.andrew@gmail.com
// tralloc is free software: you can redistribute it and/or modify it under the terms of the GNU General Public License as published by the Free Software Foundation, either version 3 of the License, or (at your option) any later version.
// tralloc is distributed in the hope that it will be useful, but WITHOUT ANY WARRANTY; without even the implied warranty of MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU General Public License for more details.
// You should have received a copy of the GNU General Public License along with tralloc. If not, see <http://www.gnu.org/licenses/>.

#include "common.h"
#include <tralloc/tree.h>


static
bool test_memory_overflow ( tralloc_context * ctx )
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
bool test_memory_strict ( tralloc_context * ctx )
{
    size_t pool_child_length = tralloc_predict_chunk_length ( TRALLOC_EXTENSION_POOL_CHILD ) + sizeof ( uint8_t ) * 5;
    size_t pool_length       = pool_child_length;

    tralloc_context * pool_data;
    if ( tralloc_with_extensions ( ctx, &pool_data, TRALLOC_EXTENSION_POOL, pool_length ) != 0 ) {
        return false;
    }
    _tralloc_chunk * pool_chunk = _tralloc_get_chunk_from_context ( pool_data );
    if ( ! ( pool_chunk->extensions & TRALLOC_EXTENSION_POOL ) ) {
        return false;
    }
    _tralloc_pool * pool = _tralloc_get_pool_from_chunk ( pool_chunk );
    _tralloc_pool_fragment * fragment = pool_data;
    if (
        pool->first_child  != NULL        ||
        pool->max_fragment != fragment    ||
        pool->memory       != pool_data   ||
        pool->length       != pool_length ||
        pool->autofree     != false       ||

        fragment->prev       != NULL ||
        fragment->next       != NULL ||
        fragment->prev_child != NULL ||
        fragment->next_child != NULL ||
        fragment->length     != pool_length
    ) {
        return false;
    }

    uint8_t * pool_child_data;
    if ( tralloc ( pool_data, ( tralloc_context ** ) &pool_child_data, sizeof ( uint8_t ) * 5 ) != 0 ) {
        return false;
    }
    _tralloc_chunk * pool_child_chunk = _tralloc_get_chunk_from_context ( pool_child_data );
    if ( ! ( pool_child_chunk->extensions & TRALLOC_EXTENSION_POOL_CHILD ) ) {
        return false;
    }
    _tralloc_pool_child * pool_child = _tralloc_get_pool_child_from_chunk ( pool_child_chunk );
    if (
        pool->first_child  != pool_child  ||
        pool->max_fragment != NULL        ||
        pool->memory       != pool_data   ||
        pool->length       != pool_length ||
        pool->autofree     != false       ||

        pool_child->pool   != pool ||
        pool_child->prev   != NULL ||
        pool_child->next   != NULL ||
        pool_child->length != pool_child_length
    ) {
        return false;
    }

    if ( tralloc_free ( pool_data ) != 0 ) {
        return false;
    }

    return true;
}

static
bool test_memory_much ( tralloc_context * ctx )
{
    size_t pool_child_length = tralloc_predict_chunk_length ( TRALLOC_EXTENSION_POOL_CHILD ) + sizeof ( uint8_t ) * 5;
    size_t pool_length       = pool_child_length + sizeof ( _tralloc_pool_fragment );

    tralloc_context * pool_data;
    if ( tralloc_with_extensions ( ctx, &pool_data, TRALLOC_EXTENSION_POOL, pool_length ) != 0 ) {
        return false;
    }
    _tralloc_chunk * pool_chunk = _tralloc_get_chunk_from_context ( pool_data );
    if ( ! ( pool_chunk->extensions & TRALLOC_EXTENSION_POOL ) ) {
        return false;
    }
    _tralloc_pool * pool = _tralloc_get_pool_from_chunk ( pool_chunk );
    _tralloc_pool_fragment * fragment = pool_data;
    if (
        pool->first_child  != NULL        ||
        pool->max_fragment != fragment    ||
        pool->memory       != pool_data   ||
        pool->length       != pool_length ||
        pool->autofree     != false       ||

        fragment->prev       != NULL ||
        fragment->next       != NULL ||
        fragment->prev_child != NULL ||
        fragment->next_child != NULL ||
        fragment->length     != pool_length
    ) {
        return false;
    }

    uint8_t * pool_child_data;
    if ( tralloc ( pool_data, ( tralloc_context ** ) &pool_child_data, sizeof ( uint8_t ) * 5 ) != 0 ) {
        return false;
    }
    _tralloc_chunk * pool_child_chunk = _tralloc_get_chunk_from_context ( pool_child_data );
    if ( ! ( pool_child_chunk->extensions & TRALLOC_EXTENSION_POOL_CHILD ) ) {
        return false;
    }
    _tralloc_pool_child * pool_child = _tralloc_get_pool_child_from_chunk ( pool_child_chunk );
    if (
        pool->first_child  != pool_child  ||
        pool->max_fragment != fragment    ||
        pool->memory       != pool_data   ||
        pool->length       != pool_length ||
        pool->autofree     != false       ||

        pool_child->pool   != pool ||
        pool_child->prev   != NULL ||
        pool_child->next   != NULL ||
        pool_child->length != pool_child_length ||

        fragment->prev       != NULL ||
        fragment->next       != NULL ||
        fragment->prev_child != NULL ||
        fragment->next_child != pool_child ||
        fragment->length     != pool_length - pool_child_length
    ) {
        return false;
    }

    if ( tralloc_free ( pool_data ) != 0 ) {
        return false;
    }

    return true;
}

bool test_add ( tralloc_context * ctx )
{
    if (
        ! test_memory_overflow ( ctx ) ||
        ! test_memory_strict   ( ctx ) ||
        ! test_memory_much     ( ctx )
    ) {
        return false;
    }

    return true;
}
