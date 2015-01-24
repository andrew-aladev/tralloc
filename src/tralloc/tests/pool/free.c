// This file is part of tralloc. Copyright (C) 2013 Andrew Aladjev aladjev.andrew@gmail.com
// tralloc is free software: you can redistribute it and/or modify it under the terms of the GNU General Public License as published by the Free Software Foundation, either version 3 of the License, or (at your option) any later version.
// tralloc is distributed in the hope that it will be useful, but WITHOUT ANY WARRANTY; without even the implied warranty of MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU General Public License for more details.
// You should have received a copy of the GNU General Public License along with tralloc. If not, see <http://www.gnu.org/licenses/>.

#include <tralloc/tests/pool/common.h>
#include <tralloc/tree/alloc.h>
#include <tralloc/tree/free.h>
#include <tralloc/pool/pool_data.h>


tralloc_bool test_pool_free ( tralloc_context * ctx )
{
    size_t data_1_user_length = sizeof ( uint8_t ) * 5;
    size_t data_2_user_length = sizeof ( uint8_t ) * 4;
    size_t data_3_user_length = sizeof ( uint8_t ) * 3;

    size_t data_1_length    = tralloc_predict_chunk_length ( TRALLOC_EXTENSION_POOL_CHILD ) + data_1_user_length;
    size_t data_2_length    = tralloc_predict_chunk_length ( TRALLOC_EXTENSION_POOL_CHILD ) + data_2_user_length;
    size_t data_3_length    = tralloc_predict_chunk_length ( TRALLOC_EXTENSION_POOL_CHILD ) + data_3_user_length;
    size_t pool_data_length = data_1_length + data_2_length + data_3_length;

    tralloc_context * pool_data;
    uint8_t * data_1, * data_2, * data_3;
    if (
        tralloc_new_with_extensions ( ctx, &pool_data, TRALLOC_EXTENSION_POOL, pool_data_length ) != 0 ||
        tralloc_new ( pool_data, ( tralloc_context ** ) &data_1, data_1_user_length ) != 0 ||
        tralloc_new ( pool_data, ( tralloc_context ** ) &data_2, data_2_user_length ) != 0 ||
        tralloc_new ( pool_data, ( tralloc_context ** ) &data_3, data_3_user_length ) != 0
    ) {
        return TRALLOC_FALSE;
    }

    _tralloc_chunk * pool_chunk = _tralloc_context_get_chunk ( pool_data );
    if ( !_tralloc_extensions_have_extension ( pool_chunk->extensions, TRALLOC_EXTENSION_POOL ) ) {
        return TRALLOC_FALSE;
    }
    _tralloc_pool * pool = _tralloc_chunk_get_pool ( pool_chunk );

    _tralloc_chunk * data_1_chunk = _tralloc_context_get_chunk ( data_1 );
    _tralloc_chunk * data_2_chunk = _tralloc_context_get_chunk ( data_2 );
    _tralloc_chunk * data_3_chunk = _tralloc_context_get_chunk ( data_3 );
    if (
        !_tralloc_extensions_have_extension ( data_1_chunk->extensions, TRALLOC_EXTENSION_POOL_CHILD ) ||
        !_tralloc_extensions_have_extension ( data_2_chunk->extensions, TRALLOC_EXTENSION_POOL_CHILD ) ||
        !_tralloc_extensions_have_extension ( data_3_chunk->extensions, TRALLOC_EXTENSION_POOL_CHILD )
    ) {
        return TRALLOC_FALSE;
    }
    _tralloc_pool_child * data_1_child = _tralloc_chunk_get_pool_child ( data_1_chunk );
    _tralloc_pool_child * data_2_child = _tralloc_chunk_get_pool_child ( data_2_chunk );
    _tralloc_pool_child * data_3_child = _tralloc_chunk_get_pool_child ( data_3_chunk );

    // [ data_3 ] [ data_2 ] [ data_1 ]
    if (
        pool->first_child  != data_3_child     ||
        pool->max_fragment != NULL             ||
        pool->memory       != pool_data        ||
        pool->length       != pool_data_length ||
        pool->autofree     != TRALLOC_FALSE    ||

        data_1_child->pool   != pool          ||
        data_1_child->prev   != data_2_child  ||
        data_1_child->next   != NULL          ||
        data_1_child->length != data_1_length ||

        data_2_child->pool   != pool          ||
        data_2_child->prev   != data_3_child  ||
        data_2_child->next   != data_1_child  ||
        data_2_child->length != data_2_length ||

        data_3_child->pool   != pool         ||
        data_3_child->prev   != NULL         ||
        data_3_child->next   != data_2_child ||
        data_3_child->length != data_3_length
    ) {
        return TRALLOC_FALSE;
    }

    if ( tralloc_free ( data_1 ) != 0 ) {
        return TRALLOC_FALSE;
    }

    _tralloc_pool_fragment * fragment_1 = ( _tralloc_pool_fragment * ) ( ( uintptr_t ) data_2_child + data_2_child->length );

    // [ data_3 ] [ data_2 ] [ fragment_1 ]
    if (
        pool->first_child  != data_3_child     ||
        pool->max_fragment != fragment_1       ||
        pool->memory       != pool_data        ||
        pool->length       != pool_data_length ||
        pool->autofree     != TRALLOC_FALSE    ||

        data_2_child->pool   != pool          ||
        data_2_child->prev   != data_3_child  ||
        data_2_child->next   != NULL          ||
        data_2_child->length != data_2_length ||

        data_3_child->pool   != pool          ||
        data_3_child->prev   != NULL          ||
        data_3_child->next   != data_2_child  ||
        data_3_child->length != data_3_length ||

        fragment_1->prev       != NULL         ||
        fragment_1->next       != NULL         ||
        fragment_1->prev_child != data_2_child ||
        fragment_1->next_child != NULL         ||
        fragment_1->length     != data_1_length
    ) {
        return TRALLOC_FALSE;
    }

    if ( tralloc_free ( data_3 ) != 0 ) {
        return TRALLOC_FALSE;
    }

    _tralloc_pool_fragment * fragment_3 = ( _tralloc_pool_fragment * ) pool_data;

    // [ fragment_3 ] [ data_2 ] [ fragment_1 ]
    if (
        pool->first_child  != data_2_child     ||
        pool->max_fragment != fragment_1       ||
        pool->memory       != pool_data        ||
        pool->length       != pool_data_length ||
        pool->autofree     != TRALLOC_FALSE    ||

        data_2_child->pool   != pool          ||
        data_2_child->prev   != NULL          ||
        data_2_child->next   != NULL          ||
        data_2_child->length != data_2_length ||

        fragment_1->prev       != NULL          ||
        fragment_1->next       != fragment_3    ||
        fragment_1->prev_child != data_2_child  ||
        fragment_1->next_child != NULL          ||
        fragment_1->length     != data_1_length ||

        fragment_3->prev       != fragment_1   ||
        fragment_3->next       != NULL         ||
        fragment_3->prev_child != NULL         ||
        fragment_3->next_child != data_2_child ||
        fragment_3->length     != data_3_length
    ) {
        return TRALLOC_FALSE;
    }

    if ( tralloc_free ( data_2 ) != 0 ) {
        return TRALLOC_FALSE;
    }

    // [ fragment_3 ]
    if (
        pool->first_child  != NULL             ||
        pool->max_fragment != fragment_3       ||
        pool->memory       != pool_data        ||
        pool->length       != pool_data_length ||
        pool->autofree     != TRALLOC_FALSE    ||

        fragment_3->prev       != NULL ||
        fragment_3->next       != NULL ||
        fragment_3->prev_child != NULL ||
        fragment_3->next_child != NULL ||
        fragment_3->length     != data_3_length + data_2_length + data_1_length
    ) {
        return TRALLOC_FALSE;
    }

    if ( tralloc_free ( pool_data ) != 0 ) {
        return TRALLOC_FALSE;
    }
    return TRALLOC_TRUE;
}
