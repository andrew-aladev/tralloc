// This file is part of tralloc. Copyright (C) 2013 Andrew Aladjev aladjev.andrew@gmail.com
// tralloc is free software: you can redistribute it and/or modify it under the terms of the GNU General Public License as published by the Free Software Foundation, either version 3 of the License, or (at your option) any later version.
// tralloc is distributed in the hope that it will be useful, but WITHOUT ANY WARRANTY; without even the implied warranty of MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU General Public License for more details.
// You should have received a copy of the GNU General Public License along with tralloc. If not, see <http://www.gnu.org/licenses/>.

#include <tralloc/tests/pool/common.h>
#include <tralloc/tree.h>
#include <tralloc/pool/chunk.h>


static inline
tralloc_bool test_pool_add_overflow ( tralloc_context * ctx )
{
    tralloc_context * empty_pool_data;
    if ( tralloc_new_empty_with_extensions ( ctx, &empty_pool_data, TRALLOC_EXTENSION_POOL ) != 0 ) {
        return TRALLOC_FALSE;
    }
    _tralloc_chunk * empty_pool_chunk = _tralloc_context_get_chunk ( empty_pool_data );
    if ( ! ( empty_pool_chunk->extensions & TRALLOC_EXTENSION_POOL ) ) {
        return TRALLOC_FALSE;
    }
    _tralloc_pool * empty_pool = _tralloc_get_pool_from_chunk ( empty_pool_chunk );
    if (
        empty_pool->first_child  != NULL            ||
        empty_pool->max_fragment != NULL            ||
        empty_pool->memory       != empty_pool_data ||
        empty_pool->length       != 0               ||
        empty_pool->autofree     != TRALLOC_FALSE
    ) {
        return TRALLOC_FALSE;
    }

    tralloc_context * empty_data;
    if ( tralloc_new_empty ( empty_pool_data, &empty_data ) != 0 ) {
        return TRALLOC_FALSE;
    }
    _tralloc_chunk * empty_data_chunk = _tralloc_context_get_chunk ( empty_data );
    if ( empty_data_chunk->extensions & TRALLOC_EXTENSION_POOL_CHILD ) {
        return TRALLOC_FALSE;
    }

    if ( tralloc_free ( empty_pool_data ) != 0 ) {
        return TRALLOC_FALSE;
    }
    return TRALLOC_TRUE;
}

static inline
tralloc_bool test_pool_add_strict ( tralloc_context * ctx )
{
    size_t pool_data_length = tralloc_predict_chunk_length ( TRALLOC_EXTENSION_POOL_CHILD ) + sizeof ( uint8_t ) * 5;

    tralloc_context * pool_data;
    if ( tralloc_new_with_extensions ( ctx, &pool_data, TRALLOC_EXTENSION_POOL, pool_data_length ) != 0 ) {
        return TRALLOC_FALSE;
    }
    _tralloc_chunk * pool_chunk = _tralloc_context_get_chunk ( pool_data );
    if ( ! ( pool_chunk->extensions & TRALLOC_EXTENSION_POOL ) ) {
        return TRALLOC_FALSE;
    }
    _tralloc_pool * pool = _tralloc_get_pool_from_chunk ( pool_chunk );
    _tralloc_pool_fragment * fragment = pool_data;
    if (
        pool->first_child  != NULL             ||
        pool->max_fragment != fragment         ||
        pool->memory       != pool_data        ||
        pool->length       != pool_data_length ||
        pool->autofree     != TRALLOC_FALSE    ||

        fragment->prev       != NULL ||
        fragment->next       != NULL ||
        fragment->prev_child != NULL ||
        fragment->next_child != NULL ||
        fragment->length     != pool_data_length
    ) {
        return TRALLOC_FALSE;
    }

    uint8_t * data_1;
    if ( tralloc_new ( pool_data, ( tralloc_context ** ) &data_1, sizeof ( uint8_t ) * 5 ) != 0 ) {
        return TRALLOC_FALSE;
    }
    _tralloc_chunk * data_1_chunk = _tralloc_context_get_chunk ( data_1 );
    if ( ! ( data_1_chunk->extensions & TRALLOC_EXTENSION_POOL_CHILD ) ) {
        return TRALLOC_FALSE;
    }
    _tralloc_pool_child * data_1_child = _tralloc_get_pool_child_from_chunk ( data_1_chunk );
    if (
        pool->first_child  != data_1_child     ||
        pool->max_fragment != NULL             ||
        pool->memory       != pool_data        ||
        pool->length       != pool_data_length ||
        pool->autofree     != TRALLOC_FALSE    ||

        data_1_child->pool   != pool ||
        data_1_child->prev   != NULL ||
        data_1_child->next   != NULL ||
        data_1_child->length != pool_data_length
    ) {
        return TRALLOC_FALSE;
    }

    if ( tralloc_free ( pool_data ) != 0 ) {
        return TRALLOC_FALSE;
    }
    return TRALLOC_TRUE;
}

static inline
tralloc_bool test_pool_add_much ( tralloc_context * ctx )
{
    size_t data_1_length    = tralloc_predict_chunk_length ( TRALLOC_EXTENSION_POOL_CHILD ) + sizeof ( uint8_t ) * 5;
    size_t pool_data_length = data_1_length + sizeof ( _tralloc_pool_fragment );

    tralloc_context * pool_data;
    if ( tralloc_new_with_extensions ( ctx, &pool_data, TRALLOC_EXTENSION_POOL, pool_data_length ) != 0 ) {
        return TRALLOC_FALSE;
    }
    _tralloc_chunk * pool_chunk = _tralloc_context_get_chunk ( pool_data );
    if ( ! ( pool_chunk->extensions & TRALLOC_EXTENSION_POOL ) ) {
        return TRALLOC_FALSE;
    }
    _tralloc_pool * pool = _tralloc_get_pool_from_chunk ( pool_chunk );
    _tralloc_pool_fragment * fragment = pool_data;
    if (
        pool->first_child  != NULL             ||
        pool->max_fragment != fragment         ||
        pool->memory       != pool_data        ||
        pool->length       != pool_data_length ||
        pool->autofree     != TRALLOC_FALSE    ||

        fragment->prev       != NULL ||
        fragment->next       != NULL ||
        fragment->prev_child != NULL ||
        fragment->next_child != NULL ||
        fragment->length     != pool_data_length
    ) {
        return TRALLOC_FALSE;
    }

    uint8_t * data_1;
    if ( tralloc_new ( pool_data, ( tralloc_context ** ) &data_1, sizeof ( uint8_t ) * 5 ) != 0 ) {
        return TRALLOC_FALSE;
    }
    _tralloc_chunk * data_1_chunk = _tralloc_context_get_chunk ( data_1 );
    if ( ! ( data_1_chunk->extensions & TRALLOC_EXTENSION_POOL_CHILD ) ) {
        return TRALLOC_FALSE;
    }
    _tralloc_pool_child * data_1_child = _tralloc_get_pool_child_from_chunk ( data_1_chunk );
    if (
        pool->first_child  != data_1_child     ||
        pool->max_fragment != fragment         ||
        pool->memory       != pool_data        ||
        pool->length       != pool_data_length ||
        pool->autofree     != TRALLOC_FALSE    ||

        data_1_child->pool   != pool          ||
        data_1_child->prev   != NULL          ||
        data_1_child->next   != NULL          ||
        data_1_child->length != data_1_length ||

        fragment->prev       != NULL         ||
        fragment->next       != NULL         ||
        fragment->prev_child != NULL         ||
        fragment->next_child != data_1_child ||
        fragment->length     != pool_data_length - data_1_length
    ) {
        return TRALLOC_FALSE;
    }

    if ( tralloc_free ( pool_data ) != 0 ) {
        return TRALLOC_FALSE;
    }
    return TRALLOC_TRUE;
}

static inline
tralloc_bool test_pool_add_zero ( tralloc_context * ctx )
{
    size_t pool_data_length = tralloc_predict_chunk_length ( TRALLOC_EXTENSION_POOL_CHILD ) + sizeof ( uint8_t ) * 3;

    tralloc_context * pool_data;
    uint8_t * data_1;
    if (
        tralloc_new_with_extensions ( ctx, &pool_data, TRALLOC_EXTENSION_POOL, pool_data_length ) != 0 ||
        tralloc_new_zero ( pool_data, ( tralloc_context ** ) &data_1, sizeof ( uint8_t ) * 3 ) != 0
    ) {
        return TRALLOC_FALSE;
    }

    if (
        data_1[0] != 0 ||
        data_1[1] != 0 ||
        data_1[2] != 0
    ) {
        return TRALLOC_FALSE;
    }

    if ( tralloc_free ( pool_data ) != 0 ) {
        return TRALLOC_FALSE;
    }
    return TRALLOC_TRUE;
}

static inline
tralloc_bool test_pool_add_normal ( tralloc_context * ctx )
{
    size_t data_1_length    = tralloc_predict_chunk_length ( TRALLOC_EXTENSION_POOL_CHILD ) + sizeof ( uint8_t ) * 5;
    size_t data_2_length    = tralloc_predict_chunk_length ( TRALLOC_EXTENSION_POOL_CHILD ) + sizeof ( uint8_t ) * 3;
    size_t pool_data_length = data_1_length + data_2_length;

    tralloc_context * new_ctx, * pool_data;
    uint8_t * data_1, * data_2;
    if (
        tralloc_new_empty ( ctx, &new_ctx ) != 0 ||
        tralloc_new_with_extensions ( new_ctx, &pool_data, TRALLOC_EXTENSION_POOL, pool_data_length ) != 0 ||
        tralloc_new ( pool_data, ( tralloc_context ** ) &data_1, sizeof ( uint8_t ) * 5 ) != 0 ||
        tralloc_new ( pool_data, ( tralloc_context ** ) &data_2, sizeof ( uint8_t ) * 3 ) != 0
    ) {
        return TRALLOC_FALSE;
    }

    _tralloc_chunk * pool_chunk = _tralloc_context_get_chunk ( pool_data );
    if ( ! ( pool_chunk->extensions & TRALLOC_EXTENSION_POOL ) ) {
        return TRALLOC_FALSE;
    }
    _tralloc_pool * pool = _tralloc_get_pool_from_chunk ( pool_chunk );

    _tralloc_chunk * data_1_chunk = _tralloc_context_get_chunk ( data_1 );
    _tralloc_chunk * data_2_chunk = _tralloc_context_get_chunk ( data_2 );
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

        data_1_child->pool   != pool          ||
        data_1_child->prev   != data_2_child  ||
        data_1_child->next   != NULL          ||
        data_1_child->length != data_1_length ||

        data_2_child->pool   != pool         ||
        data_2_child->prev   != NULL         ||
        data_2_child->next   != data_1_child ||
        data_2_child->length != data_2_length
    ) {
        return TRALLOC_FALSE;
    }

    if ( tralloc_free ( new_ctx ) != 0 ) {
        return TRALLOC_FALSE;
    }
    return TRALLOC_TRUE;
}

tralloc_bool test_pool_add ( tralloc_context * ctx )
{
    if (
        ! test_pool_add_overflow ( ctx ) ||
        ! test_pool_add_strict   ( ctx ) ||
        ! test_pool_add_much     ( ctx ) ||
        ! test_pool_add_zero     ( ctx ) ||
        ! test_pool_add_normal   ( ctx )
    ) {
        return TRALLOC_FALSE;
    }
    return TRALLOC_TRUE;
}
