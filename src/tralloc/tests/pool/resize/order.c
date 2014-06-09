// This file is part of tralloc. Copyright (C) 2013 Andrew Aladjev aladjev.andrew@gmail.com
// tralloc is free software: you can redistribute it and/or modify it under the terms of the GNU General Public License as published by the Free Software Foundation, either version 3 of the License, or (at your option) any later version.
// tralloc is distributed in the hope that it will be useful, but WITHOUT ANY WARRANTY; without even the implied warranty of MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU General Public License for more details.
// You should have received a copy of the GNU General Public License along with tralloc. If not, see <http://www.gnu.org/licenses/>.

#include <tralloc/tests/pool/resize/common.h>
#include <tralloc/tree.h>
#include <tralloc/pool/common.h>


tralloc_bool test_pool_resize_order ( tralloc_context * ctx )
{
    size_t data_1_user_length = sizeof ( uint8_t ) * 5;
    size_t data_2_user_length = sizeof ( uint8_t ) * 4;
    size_t data_3_user_length = sizeof ( uint8_t ) * 3;
    size_t data_4_user_length = sizeof ( uint8_t ) * 2;
    size_t data_1_length      = tralloc_predict_chunk_length ( TRALLOC_EXTENSION_POOL_CHILD ) + data_1_user_length;
    size_t spacer_1_length    = tralloc_predict_chunk_length ( TRALLOC_EXTENSION_POOL_CHILD );
    size_t data_2_length      = tralloc_predict_chunk_length ( TRALLOC_EXTENSION_POOL_CHILD ) + data_2_user_length;
    size_t spacer_2_length    = tralloc_predict_chunk_length ( TRALLOC_EXTENSION_POOL_CHILD );
    size_t data_3_length      = tralloc_predict_chunk_length ( TRALLOC_EXTENSION_POOL_CHILD ) + data_3_user_length;
    size_t spacer_3_length    = tralloc_predict_chunk_length ( TRALLOC_EXTENSION_POOL_CHILD );
    size_t data_4_length      = tralloc_predict_chunk_length ( TRALLOC_EXTENSION_POOL_CHILD ) + data_4_user_length;
    size_t spacer_4_length    = tralloc_predict_chunk_length ( TRALLOC_EXTENSION_POOL_CHILD );
    size_t pool_data_length   = data_1_length + spacer_1_length + data_2_length + spacer_2_length + data_3_length + spacer_3_length + data_4_length + spacer_4_length;

    tralloc_context * pool_data;
    uint8_t * data_1, * data_2, * data_3, * data_4;
    tralloc_context * spacer_1, * spacer_2, * spacer_3, * spacer_4;
    if (
        tralloc_new_with_extensions ( ctx, &pool_data, TRALLOC_EXTENSION_POOL, pool_data_length ) != 0 ||
        tralloc_new ( pool_data, ( tralloc_context ** ) &data_1, data_1_user_length ) != 0 ||
        tralloc_new_empty ( pool_data, &spacer_1 ) != 0 ||
        tralloc_new ( pool_data, ( tralloc_context ** ) &data_2, data_2_user_length ) != 0 ||
        tralloc_new_empty ( pool_data, &spacer_2 ) != 0 ||
        tralloc_new ( pool_data, ( tralloc_context ** ) &data_3, data_3_user_length ) != 0 ||
        tralloc_new_empty ( pool_data, &spacer_3 ) != 0 ||
        tralloc_new ( pool_data, ( tralloc_context ** ) &data_4, data_4_user_length ) != 0 ||
        tralloc_new_empty ( pool_data, &spacer_4 ) != 0 ||
        tralloc_free ( data_1 ) != 0 ||
        tralloc_free ( data_2 ) != 0 ||
        tralloc_free ( data_4 ) != 0
    ) {
        return TRALLOC_FALSE;
    }

    _tralloc_chunk * pool_chunk = _tralloc_get_chunk_from_context ( pool_data );
    if ( ! ( pool_chunk->extensions & TRALLOC_EXTENSION_POOL ) ) {
        return TRALLOC_FALSE;
    }
    _tralloc_pool * pool = _tralloc_get_pool_from_chunk ( pool_chunk );

    _tralloc_chunk * spacer_1_chunk = _tralloc_get_chunk_from_context ( spacer_1 );
    _tralloc_chunk * spacer_2_chunk = _tralloc_get_chunk_from_context ( spacer_2 );
    _tralloc_chunk * data_3_chunk   = _tralloc_get_chunk_from_context ( data_3 );
    _tralloc_chunk * spacer_3_chunk = _tralloc_get_chunk_from_context ( spacer_3 );
    _tralloc_chunk * spacer_4_chunk = _tralloc_get_chunk_from_context ( spacer_4 );
    if (
        ! ( spacer_1_chunk->extensions & TRALLOC_EXTENSION_POOL_CHILD ) ||
        ! ( spacer_2_chunk->extensions & TRALLOC_EXTENSION_POOL_CHILD ) ||
        ! ( data_3_chunk->extensions   & TRALLOC_EXTENSION_POOL_CHILD ) ||
        ! ( spacer_3_chunk->extensions & TRALLOC_EXTENSION_POOL_CHILD ) ||
        ! ( spacer_4_chunk->extensions & TRALLOC_EXTENSION_POOL_CHILD )
    ) {
        return TRALLOC_FALSE;
    }
    _tralloc_pool_child * spacer_1_child = _tralloc_get_pool_child_from_chunk ( spacer_1_chunk );
    _tralloc_pool_child * spacer_2_child = _tralloc_get_pool_child_from_chunk ( spacer_2_chunk );
    _tralloc_pool_child * data_3_child   = _tralloc_get_pool_child_from_chunk ( data_3_chunk );
    _tralloc_pool_child * spacer_3_child = _tralloc_get_pool_child_from_chunk ( spacer_3_chunk );
    _tralloc_pool_child * spacer_4_child = _tralloc_get_pool_child_from_chunk ( spacer_4_chunk );

    _tralloc_pool_fragment * fragment_1 = ( _tralloc_pool_fragment * ) ( ( uintptr_t ) spacer_1_child + spacer_1_child->length );
    _tralloc_pool_fragment * fragment_2 = ( _tralloc_pool_fragment * ) ( ( uintptr_t ) spacer_2_child + spacer_2_child->length );
    _tralloc_pool_fragment * fragment_4 = ( _tralloc_pool_fragment * ) ( ( uintptr_t ) spacer_4_child + spacer_4_child->length );

    // [ spacer_4 ] [ fragment of data_4 ] [ spacer_3 ] [ data_3 ] [ spacer_2 ] [ fragment of data_2 ] [ spacer_1 ] [ fragment of data_1 ]
    if (
        pool->first_child  != spacer_4_child   ||
        pool->max_fragment != fragment_1       ||
        pool->memory       != pool_data        ||
        pool->length       != pool_data_length ||
        pool->autofree     != TRALLOC_FALSE    ||

        spacer_4_child->pool   != pool            ||
        spacer_4_child->prev   != NULL            ||
        spacer_4_child->next   != spacer_3_child  ||
        spacer_4_child->length != spacer_4_length ||

        spacer_3_child->pool   != pool            ||
        spacer_3_child->prev   != spacer_4_child  ||
        spacer_3_child->next   != data_3_child    ||
        spacer_3_child->length != spacer_3_length ||

        data_3_child->pool   != pool           ||
        data_3_child->prev   != spacer_3_child ||
        data_3_child->next   != spacer_2_child ||
        data_3_child->length != data_3_length  ||

        spacer_2_child->pool   != pool            ||
        spacer_2_child->prev   != data_3_child    ||
        spacer_2_child->next   != spacer_1_child  ||
        spacer_2_child->length != spacer_2_length ||

        spacer_1_child->pool   != pool            ||
        spacer_1_child->prev   != spacer_2_child  ||
        spacer_1_child->next   != NULL            ||
        spacer_1_child->length != spacer_1_length ||

        fragment_1->prev       != NULL           ||
        fragment_1->next       != fragment_2     ||
        fragment_1->prev_child != spacer_1_child ||
        fragment_1->next_child != NULL           ||
        fragment_1->length     != data_1_length  ||

        fragment_2->prev       != fragment_1     ||
        fragment_2->next       != fragment_4     ||
        fragment_2->prev_child != spacer_2_child ||
        fragment_2->next_child != spacer_1_child ||
        fragment_2->length     != data_2_length  ||

        fragment_4->prev       != fragment_2     ||
        fragment_4->next       != NULL           ||
        fragment_4->prev_child != spacer_4_child ||
        fragment_4->next_child != spacer_3_child ||
        fragment_4->length     != data_4_length
    ) {
        return TRALLOC_FALSE;
    }

    if ( tralloc_free ( spacer_2 ) != 0 ) {
        return TRALLOC_FALSE;
    }
    fragment_2 = ( _tralloc_pool_fragment * ) ( ( uintptr_t ) data_3_child + data_3_child->length );

    // [ spacer_4 ] [ fragment of data_4 ] [ spacer_3 ] [ data_3 ] [ fragment of spacer_2 + data_2 ] [ spacer_1 ] [ fragment of data_1 ]
    if (
        pool->first_child  != spacer_4_child   ||
        pool->max_fragment != fragment_2       ||
        pool->memory       != pool_data        ||
        pool->length       != pool_data_length ||
        pool->autofree     != TRALLOC_FALSE    ||

        spacer_4_child->pool   != pool            ||
        spacer_4_child->prev   != NULL            ||
        spacer_4_child->next   != spacer_3_child  ||
        spacer_4_child->length != spacer_4_length ||

        spacer_3_child->pool   != pool            ||
        spacer_3_child->prev   != spacer_4_child  ||
        spacer_3_child->next   != data_3_child    ||
        spacer_3_child->length != spacer_3_length ||

        data_3_child->pool   != pool           ||
        data_3_child->prev   != spacer_3_child ||
        data_3_child->next   != spacer_1_child ||
        data_3_child->length != data_3_length  ||

        spacer_1_child->pool   != pool            ||
        spacer_1_child->prev   != data_3_child    ||
        spacer_1_child->next   != NULL            ||
        spacer_1_child->length != spacer_1_length ||

        fragment_2->prev       != NULL           ||
        fragment_2->next       != fragment_1     ||
        fragment_2->prev_child != data_3_child   ||
        fragment_2->next_child != spacer_1_child ||
        fragment_2->length     != spacer_2_length + data_2_length ||

        fragment_1->prev       != fragment_2     ||
        fragment_1->next       != fragment_4     ||
        fragment_1->prev_child != spacer_1_child ||
        fragment_1->next_child != NULL           ||
        fragment_1->length     != data_1_length  ||

        fragment_4->prev       != fragment_1     ||
        fragment_4->next       != NULL           ||
        fragment_4->prev_child != spacer_4_child ||
        fragment_4->next_child != spacer_3_child ||
        fragment_4->length     != data_4_length
    ) {
        return TRALLOC_FALSE;
    }

    if ( tralloc_free ( spacer_3 ) != 0 ) {
        return TRALLOC_FALSE;
    }

    // [ spacer_4 ] [ fragment of data_4 + spacer_3 ] [ data_3 ] [ fragment of spacer_2 + data_2 ] [ spacer_1 ] [ fragment of data_1 ]
    if (
        pool->first_child  != spacer_4_child   ||
        pool->max_fragment != fragment_2       ||
        pool->memory       != pool_data        ||
        pool->length       != pool_data_length ||
        pool->autofree     != TRALLOC_FALSE    ||

        spacer_4_child->pool   != pool            ||
        spacer_4_child->prev   != NULL            ||
        spacer_4_child->next   != data_3_child    ||
        spacer_4_child->length != spacer_4_length ||

        data_3_child->pool   != pool           ||
        data_3_child->prev   != spacer_4_child ||
        data_3_child->next   != spacer_1_child ||
        data_3_child->length != data_3_length  ||

        spacer_1_child->pool   != pool            ||
        spacer_1_child->prev   != data_3_child    ||
        spacer_1_child->next   != NULL            ||
        spacer_1_child->length != spacer_1_length ||

        fragment_2->prev       != NULL           ||
        fragment_2->next       != fragment_4     ||
        fragment_2->prev_child != data_3_child   ||
        fragment_2->next_child != spacer_1_child ||
        fragment_2->length     != spacer_2_length + data_2_length ||

        fragment_4->prev       != fragment_2     ||
        fragment_4->next       != fragment_1     ||
        fragment_4->prev_child != spacer_4_child ||
        fragment_4->next_child != data_3_child   ||
        fragment_4->length     != data_4_length + spacer_3_length ||

        fragment_1->prev       != fragment_4     ||
        fragment_1->next       != NULL           ||
        fragment_1->prev_child != spacer_1_child ||
        fragment_1->next_child != NULL           ||
        fragment_1->length     != data_1_length
    ) {
        return TRALLOC_FALSE;
    }

    if ( tralloc_realloc ( ( tralloc_context ** ) &data_3, data_3_user_length + data_2_length + spacer_2_length - sizeof ( uint8_t ) * 2 ) != 0 ) {
        return TRALLOC_FALSE;
    }

    // [ spacer_4 ] [ fragment of data_4 + spacer_3 ] [ data_3 + data_2 + spacer_2 - empty space (2) ] [ empty space (2) ] [ spacer_1 ] [ fragment of data_1 ]
    if (
        pool->first_child  != spacer_4_child   ||
        pool->max_fragment != fragment_4       ||
        pool->memory       != pool_data        ||
        pool->length       != pool_data_length ||
        pool->autofree     != TRALLOC_FALSE    ||

        spacer_4_child->pool   != pool            ||
        spacer_4_child->prev   != NULL            ||
        spacer_4_child->next   != data_3_child    ||
        spacer_4_child->length != spacer_4_length ||

        data_3_child->pool   != pool           ||
        data_3_child->prev   != spacer_4_child ||
        data_3_child->next   != spacer_1_child ||
        data_3_child->length != data_3_length + data_2_length + spacer_2_length - sizeof ( uint8_t ) * 2 ||

        spacer_1_child->pool   != pool            ||
        spacer_1_child->prev   != data_3_child    ||
        spacer_1_child->next   != NULL            ||
        spacer_1_child->length != spacer_1_length ||

        fragment_4->prev       != NULL           ||
        fragment_4->next       != fragment_1     ||
        fragment_4->prev_child != spacer_4_child ||
        fragment_4->next_child != data_3_child   ||
        fragment_4->length     != data_4_length + spacer_3_length ||

        fragment_1->prev       != fragment_4     ||
        fragment_1->next       != NULL           ||
        fragment_1->prev_child != spacer_1_child ||
        fragment_1->next_child != NULL           ||
        fragment_1->length     != data_1_length
    ) {
        return TRALLOC_FALSE;
    }

    if (
        tralloc_free ( spacer_1 ) != 0 ||
        tralloc_realloc ( ( tralloc_context ** ) &data_3, data_3_user_length + data_2_length + spacer_2_length ) != 0
    ) {
        return TRALLOC_FALSE;
    }
    fragment_1 = ( _tralloc_pool_fragment * ) ( ( uintptr_t ) data_3_child + data_3_child->length );

    // [ spacer_4 ] [ fragment of data_4 + spacer_3 ] [ data_3 + data_2 + spacer_2 ] [ fragment of data_1 + spacer_1 ]
    if (
        pool->first_child  != spacer_4_child   ||
        pool->max_fragment != fragment_1       ||
        pool->memory       != pool_data        ||
        pool->length       != pool_data_length ||
        pool->autofree     != TRALLOC_FALSE    ||

        spacer_4_child->pool   != pool            ||
        spacer_4_child->prev   != NULL            ||
        spacer_4_child->next   != data_3_child    ||
        spacer_4_child->length != spacer_4_length ||

        data_3_child->pool   != pool           ||
        data_3_child->prev   != spacer_4_child ||
        data_3_child->next   != NULL           ||
        data_3_child->length != data_3_length + data_2_length + spacer_2_length ||

        fragment_1->prev       != NULL         ||
        fragment_1->next       != fragment_4   ||
        fragment_1->prev_child != data_3_child ||
        fragment_1->next_child != NULL         ||
        fragment_1->length     != data_1_length + spacer_1_length ||

        fragment_4->prev       != fragment_1     ||
        fragment_4->next       != NULL           ||
        fragment_4->prev_child != spacer_4_child ||
        fragment_4->next_child != data_3_child   ||
        fragment_4->length     != data_4_length + spacer_3_length
    ) {
        return TRALLOC_FALSE;
    }

    if ( tralloc_free ( spacer_4 ) != 0 ) {
        return TRALLOC_FALSE;
    }
    fragment_4 = ( _tralloc_pool_fragment * ) ( pool_data );

    // [ fragment of data_4 + spacer_4 + spacer_3 ] [ data_3 + data_2 + spacer_2 ] [ fragment of data_1 + spacer_1 ]
    if (
        pool->first_child  != data_3_child     ||
        pool->max_fragment != fragment_4       ||
        pool->memory       != pool_data        ||
        pool->length       != pool_data_length ||
        pool->autofree     != TRALLOC_FALSE    ||

        data_3_child->pool   != pool ||
        data_3_child->prev   != NULL ||
        data_3_child->next   != NULL ||
        data_3_child->length != data_3_length + data_2_length + spacer_2_length ||

        fragment_4->prev       != NULL         ||
        fragment_4->next       != fragment_1   ||
        fragment_4->prev_child != NULL         ||
        fragment_4->next_child != data_3_child ||
        fragment_4->length     != data_4_length + spacer_4_length + spacer_3_length ||

        fragment_1->prev       != fragment_4   ||
        fragment_1->next       != NULL         ||
        fragment_1->prev_child != data_3_child ||
        fragment_1->next_child != NULL         ||
        fragment_1->length     != data_1_length + spacer_1_length
    ) {
        return TRALLOC_FALSE;
    }

    if ( tralloc_realloc ( ( tralloc_context ** ) &data_3, data_3_user_length + data_2_length + data_1_length + spacer_1_length + spacer_2_length + spacer_3_length ) != 0 ) {
        return TRALLOC_FALSE;
    }
    data_3_chunk = _tralloc_get_chunk_from_context ( data_3 );
    data_3_child = _tralloc_get_pool_child_from_chunk ( data_3_chunk );
    fragment_4 = ( _tralloc_pool_fragment * ) ( ( uintptr_t ) data_3_child + data_3_child->length );

    // [ data_3 + data_2 + data_1 + spacer_1 + spacer_2 + spacer_3 ] [ fragment of data_4 + spacer_4 ]
    if (
        pool->first_child  != data_3_child     ||
        pool->max_fragment != fragment_4       ||
        pool->memory       != pool_data        ||
        pool->length       != pool_data_length ||
        pool->autofree     != TRALLOC_FALSE    ||

        data_3_child->pool   != pool ||
        data_3_child->prev   != NULL ||
        data_3_child->next   != NULL ||
        data_3_child->length != data_3_length + data_2_length + data_1_length + spacer_1_length + spacer_2_length + spacer_3_length ||

        fragment_4->prev       != NULL         ||
        fragment_4->next       != NULL         ||
        fragment_4->prev_child != data_3_child ||
        fragment_4->next_child != NULL         ||
        fragment_4->length     != data_4_length + spacer_4_length
    ) {
        return TRALLOC_FALSE;
    }

    if ( tralloc_realloc ( ( tralloc_context ** ) &data_3, data_3_user_length + data_4_length + data_2_length + data_1_length + spacer_1_length + spacer_2_length + spacer_3_length + spacer_4_length ) != 0 ) {
        return TRALLOC_FALSE;
    }

    // [ data_3 + data_4 + data_2 + data_1 + spacer_1 + spacer_2 + spacer_3 + spacer_4 ]
    if (
        pool->first_child  != data_3_child     ||
        pool->max_fragment != NULL             ||
        pool->memory       != pool_data        ||
        pool->length       != pool_data_length ||
        pool->autofree     != TRALLOC_FALSE    ||

        data_3_child->pool   != pool ||
        data_3_child->prev   != NULL ||
        data_3_child->next   != NULL ||
        data_3_child->length != data_3_length + data_4_length + data_2_length + data_1_length + spacer_1_length + spacer_2_length + spacer_3_length + spacer_4_length
    ) {
        return TRALLOC_FALSE;
    }

    if ( tralloc_free ( pool_data ) != 0 ) {
        return TRALLOC_FALSE;
    }
    return TRALLOC_TRUE;
}
