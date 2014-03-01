// This file is part of tralloc. Copyright (C) 2013 Andrew Aladjev aladjev.andrew@gmail.com
// tralloc is free software: you can redistribute it and/or modify it under the terms of the GNU General Public License as published by the Free Software Foundation, either version 3 of the License, or (at your option) any later version.
// tralloc is distributed in the hope that it will be useful, but WITHOUT ANY WARRANTY; without even the implied warranty of MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU General Public License for more details.
// You should have received a copy of the GNU General Public License along with tralloc. If not, see <http://www.gnu.org/licenses/>.

#include "common.h"
#include <tralloc/tree.h>
#include <tralloc/pool/common.h>


tralloc_bool test_pool_move ( tralloc_context * ctx )
{
    size_t data_1_length    = tralloc_predict_chunk_length ( TRALLOC_EXTENSION_POOL_CHILD ) + sizeof ( uint8_t ) * 5;
    size_t data_2_length    = tralloc_predict_chunk_length ( TRALLOC_EXTENSION_POOL_CHILD ) + sizeof ( uint8_t ) * 4;
    size_t pool_data_length = data_1_length + data_2_length;

    tralloc_context * pool_data;
    uint8_t * data_1, * data_2;
    if (
        tralloc_with_extensions ( ctx, &pool_data, TRALLOC_EXTENSION_POOL, pool_data_length ) != 0 ||
        tralloc ( pool_data, ( tralloc_context ** ) &data_1, sizeof ( uint8_t ) * 5 ) != 0 ||
        tralloc ( pool_data, ( tralloc_context ** ) &data_2, sizeof ( uint8_t ) * 4 ) != 0 ||
        tralloc_move ( data_2, ctx ) != 0
    ) {
        return TRALLOC_FALSE;
    }

    _tralloc_chunk * pool_chunk = _tralloc_get_chunk_from_context ( pool_data );
    if ( ! ( pool_chunk->extensions & TRALLOC_EXTENSION_POOL ) ) {
        return TRALLOC_FALSE;
    }
    _tralloc_pool * pool = _tralloc_get_pool_from_chunk ( pool_chunk );

    _tralloc_chunk * data_1_chunk = _tralloc_get_chunk_from_context ( data_1 );
    _tralloc_chunk * data_2_chunk = _tralloc_get_chunk_from_context ( data_2 );
    if (
        ! ( data_1_chunk->extensions & TRALLOC_EXTENSION_POOL_CHILD ) ||
        ! ( data_2_chunk->extensions & TRALLOC_EXTENSION_POOL_CHILD )
    ) {
        return TRALLOC_FALSE;
    }
    _tralloc_pool_child * data_1_child = _tralloc_get_pool_child_from_chunk ( data_1_chunk );
    _tralloc_pool_child * data_2_child = _tralloc_get_pool_child_from_chunk ( data_2_chunk );

    if (
        pool->first_child  != data_2_child     ||
        pool->max_fragment != NULL             ||
        pool->memory       != pool_data        ||
        pool->length       != pool_data_length ||
        pool->autofree     != TRALLOC_FALSE    ||

        data_1_child->pool   != pool ||
        data_1_child->prev   != data_2_child ||
        data_1_child->next   != NULL ||
        data_1_child->length != data_1_length ||

        data_2_child->pool   != pool ||
        data_2_child->prev   != NULL ||
        data_2_child->next   != data_1_child ||
        data_2_child->length != data_2_length
    ) {
        return TRALLOC_FALSE;
    }

    if (
        tralloc_free ( pool_data ) != 0 ||
        tralloc_free ( data_2 )    != 0
    ) {
        return TRALLOC_FALSE;
    }
    return TRALLOC_TRUE;
}
