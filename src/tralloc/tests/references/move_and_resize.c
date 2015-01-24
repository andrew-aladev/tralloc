// This file is part of tralloc. Copyright (C) 2013 Andrew Aladjev aladjev.andrew@gmail.com
// tralloc is free software: you can redistribute it and/or modify it under the terms of the GNU General Public License as published by the Free Software Foundation, either version 3 of the License, or (at your option) any later version.
// tralloc is distributed in the hope that it will be useful, but WITHOUT ANY WARRANTY; without even the implied warranty of MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU General Public License for more details.
// You should have received a copy of the GNU General Public License along with tralloc. If not, see <http://www.gnu.org/licenses/>.

#include <tralloc/tests/references/common.h>
#include <tralloc/tests/common/main.h>
#include <tralloc/tree/alloc.h>
#include <tralloc/tree/realloc.h>
#include <tralloc/tree/move.h>
#include <tralloc/tree/free.h>
#include <tralloc/references/move.h>
#include <tralloc/references/clear.h>
#include <tralloc/references/chunk.h>


static
tralloc_bool test_references_check_empty_references ( test_references_tree * tr )
{
    _tralloc_references * common_references  = _tralloc_chunk_get_references ( _tralloc_context_get_chunk ( tr->common ) );
    _tralloc_references * shared_references  = _tralloc_chunk_get_references ( _tralloc_context_get_chunk ( tr->shared ) );
    _tralloc_reference  * common_1_reference = _tralloc_chunk_get_reference  ( _tralloc_context_get_chunk ( tr->common_1 ) );
    _tralloc_reference  * common_2_reference = _tralloc_chunk_get_reference  ( _tralloc_context_get_chunk ( tr->common_2 ) );
    _tralloc_reference  * common_3_reference = _tralloc_chunk_get_reference  ( _tralloc_context_get_chunk ( tr->common_3 ) );
    _tralloc_reference  * shared_1_reference = _tralloc_chunk_get_reference  ( _tralloc_context_get_chunk ( tr->shared_1 ) );
    _tralloc_reference  * shared_2_reference = _tralloc_chunk_get_reference  ( _tralloc_context_get_chunk ( tr->shared_2 ) );

    if (
        common_references->first_reference != NULL ||

        common_3_reference->prev       != NULL ||
        common_3_reference->next       != NULL ||
        common_3_reference->references != NULL ||

        common_2_reference->prev       != NULL ||
        common_2_reference->next       != NULL ||
        common_2_reference->references != NULL ||

        common_1_reference->prev       != NULL ||
        common_1_reference->next       != NULL ||
        common_1_reference->references != NULL ||

        shared_references->first_reference != NULL ||

        shared_2_reference->prev       != NULL ||
        shared_2_reference->next       != NULL ||
        shared_2_reference->references != NULL ||

        shared_1_reference->prev       != NULL ||
        shared_1_reference->next       != NULL ||
        shared_1_reference->references != NULL
    ) {
        return TRALLOC_FALSE;
    }
    return TRALLOC_TRUE;
}

/*
                     tree    common
                     /|\
            1         2      shared
           /|         |        |
common_2    3      common_1  common_3
   |        |
shared_1 shared_2

*/

static
tralloc_bool test_references_check_moved_chunks ( test_references_tree * tr )
{
    _tralloc_chunk * root_chunk     = _tralloc_context_get_chunk ( tr );
    _tralloc_chunk * common_chunk   = _tralloc_context_get_chunk ( tr->common );
    _tralloc_chunk * shared_chunk   = _tralloc_context_get_chunk ( tr->shared );
    _tralloc_chunk * data_1_chunk   = _tralloc_context_get_chunk ( tr->data_1 );
    _tralloc_chunk * data_2_chunk   = _tralloc_context_get_chunk ( tr->data_2 );
    _tralloc_chunk * data_3_chunk   = _tralloc_context_get_chunk ( tr->data_3 );
    _tralloc_chunk * common_1_chunk = _tralloc_context_get_chunk ( tr->common_1 );
    _tralloc_chunk * common_2_chunk = _tralloc_context_get_chunk ( tr->common_2 );
    _tralloc_chunk * common_3_chunk = _tralloc_context_get_chunk ( tr->common_3 );
    _tralloc_chunk * shared_1_chunk = _tralloc_context_get_chunk ( tr->shared_1 );
    _tralloc_chunk * shared_2_chunk = _tralloc_context_get_chunk ( tr->shared_2 );

    if (
        common_chunk->parent      != NULL ||
        common_chunk->prev        != NULL ||
        common_chunk->next        != NULL ||
        common_chunk->first_child != NULL ||

        root_chunk->prev        != NULL         ||
        root_chunk->next        != NULL         ||
        root_chunk->first_child != data_1_chunk ||

        data_1_chunk->parent      != root_chunk     ||
        data_1_chunk->prev        != NULL           ||
        data_1_chunk->next        != data_2_chunk   ||
        data_1_chunk->first_child != common_2_chunk ||

        data_2_chunk->parent      != root_chunk     ||
        data_2_chunk->prev        != data_1_chunk   ||
        data_2_chunk->next        != shared_chunk   ||
        data_2_chunk->first_child != common_1_chunk ||

        data_3_chunk->parent      != data_1_chunk   ||
        data_3_chunk->prev        != common_2_chunk ||
        data_3_chunk->next        != NULL           ||
        data_3_chunk->first_child != shared_2_chunk ||

        shared_chunk->parent      != root_chunk     ||
        shared_chunk->prev        != data_2_chunk   ||
        shared_chunk->next        != NULL           ||
        shared_chunk->first_child != common_3_chunk ||

        shared_1_chunk->parent      != common_2_chunk ||
        shared_1_chunk->prev        != NULL           ||
        shared_1_chunk->next        != NULL           ||
        shared_1_chunk->first_child != NULL           ||

        shared_2_chunk->parent      != data_3_chunk ||
        shared_2_chunk->prev        != NULL         ||
        shared_2_chunk->next        != NULL         ||
        shared_2_chunk->first_child != NULL         ||

        common_1_chunk->parent      != data_2_chunk ||
        common_1_chunk->prev        != NULL         ||
        common_1_chunk->next        != NULL         ||
        common_1_chunk->first_child != NULL         ||

        common_2_chunk->parent      != data_1_chunk   ||
        common_2_chunk->prev        != NULL           ||
        common_2_chunk->next        != data_3_chunk   ||
        common_2_chunk->first_child != shared_1_chunk ||

        common_3_chunk->parent      != shared_chunk ||
        common_3_chunk->prev        != NULL         ||
        common_3_chunk->next        != NULL         ||
        common_3_chunk->first_child != NULL
    ) {
        return TRALLOC_FALSE;
    }

    _tralloc_references * common_references  = _tralloc_chunk_get_references ( _tralloc_context_get_chunk ( tr->common ) );
    _tralloc_references * shared_references  = _tralloc_chunk_get_references ( _tralloc_context_get_chunk ( tr->shared ) );
    _tralloc_reference  * common_1_reference = _tralloc_chunk_get_reference  ( _tralloc_context_get_chunk ( tr->common_1 ) );
    _tralloc_reference  * common_2_reference = _tralloc_chunk_get_reference  ( _tralloc_context_get_chunk ( tr->common_2 ) );
    _tralloc_reference  * common_3_reference = _tralloc_chunk_get_reference  ( _tralloc_context_get_chunk ( tr->common_3 ) );
    _tralloc_reference  * shared_1_reference = _tralloc_chunk_get_reference  ( _tralloc_context_get_chunk ( tr->shared_1 ) );
    _tralloc_reference  * shared_2_reference = _tralloc_chunk_get_reference  ( _tralloc_context_get_chunk ( tr->shared_2 ) );

    if (
        common_references->first_reference != common_3_reference ||

        common_3_reference->prev       != NULL               ||
        common_3_reference->next       != common_2_reference ||
        common_3_reference->references != common_references  ||

        common_2_reference->prev       != common_3_reference ||
        common_2_reference->next       != common_1_reference ||
        common_2_reference->references != common_references  ||

        common_1_reference->prev       != common_2_reference ||
        common_1_reference->next       != NULL               ||
        common_1_reference->references != common_references  ||

        shared_references->first_reference != shared_2_reference ||

        shared_2_reference->prev       != NULL               ||
        shared_2_reference->next       != shared_1_reference ||
        shared_2_reference->references != shared_references  ||

        shared_1_reference->prev       != shared_2_reference ||
        shared_1_reference->next       != NULL               ||
        shared_1_reference->references != shared_references
    ) {
        return TRALLOC_FALSE;
    }
    return TRALLOC_TRUE;
}

tralloc_bool test_references_move_and_resize ( test_references_tree * tr )
{
    if ( !test_references_check_empty_references ( tr ) ) {
        return TRALLOC_FALSE;
    }

    if (
        tralloc_reference_move ( tr->common_1, tr->common ) != 0 ||
        tralloc_reference_move ( tr->common_2, tr->common ) != 0 ||
        tralloc_reference_move ( tr->common_3, tr->common ) != 0 ||
        tralloc_reference_move ( tr->shared_1, tr->shared ) != 0 ||
        tralloc_reference_move ( tr->shared_2, tr->shared ) != 0
    ) {
        return TRALLOC_FALSE;
    }

    if (
        tralloc_move ( tr->shared_2, tr->data_3 )   != 0 ||
        tralloc_move ( tr->shared_1, tr->common_2 ) != 0 ||
        tralloc_move ( tr->common_2, tr->data_1 )   != 0 ||
        tralloc_move ( tr->common_1, tr->data_2 )   != 0
    ) {
        return TRALLOC_FALSE;
    }

    if ( !test_references_check_moved_chunks ( tr ) ) {
        return TRALLOC_FALSE;
    }

    if (
        tralloc_references_clear ( tr->common ) != 0 ||
        tralloc_references_clear ( tr->shared ) != 0
    ) {
        return TRALLOC_FALSE;
    }

    if ( !test_references_check_empty_references ( tr ) ) {
        return TRALLOC_FALSE;
    }

    if (
        tralloc_reference_move ( tr->common_1, tr->common ) != 0 ||
        tralloc_reference_move ( tr->common_2, tr->common ) != 0 ||
        tralloc_reference_move ( tr->common_3, tr->common ) != 0 ||
        tralloc_free           ( tr->common )               != 0 ||
        tralloc_reference_move ( tr->shared_1, tr->shared ) != 0 ||
        tralloc_reference_move ( tr->shared_2, tr->shared ) != 0
    ) {
        return TRALLOC_FALSE;
    }

    if ( !test_references_check_moved_chunks ( tr ) ) {
        return TRALLOC_FALSE;
    }

    if (
        tralloc_realloc ( ( tralloc_context ** ) &tr->common, sizeof ( int8_t ) * 20 ) != 0 ||
        tr->common[0] != -100 || tr->common[1] != 50 ||

        tralloc_realloc ( ( tralloc_context ** ) &tr->shared, sizeof ( float ) * 20 ) != 0 ||
        !compare_float ( tr->shared[0], 0.123456 ) ||

        tralloc_realloc ( ( tralloc_context ** ) &tr->data_2, sizeof ( uint8_t ) * 30 ) != 0 ||
        tr->data_2[0] != 123 ||

        tralloc_realloc ( ( tralloc_context ** ) &tr->data_3, sizeof ( char ) * 40 ) != 0 ||
        tr->data_3[0] != 'q' || tr->data_3[1] != 'w' || tr->data_3[2] != 'e' || tr->data_3[3] != 'r' ||

        tralloc_realloc ( ( tralloc_context ** ) &tr->shared_1, sizeof ( uint8_t ) * 40 ) != 0 ||

        tralloc_realloc ( ( tralloc_context ** ) &tr->common_1, sizeof ( int ) * 20 ) != 0 ||
        tr->common_1[0] != -45 || tr->common_1[1] != 56 ||

        tralloc_realloc ( ( tralloc_context ** ) &tr->common_2, sizeof ( uint8_t ) * 40 ) != 0 ||

        tralloc_realloc ( ( tralloc_context ** ) &tr->shared_2, sizeof ( float ) * 15 ) != 0 ||
        !compare_float ( tr->shared_2[0], 0.123456 ) ||

        tralloc_realloc ( ( tralloc_context ** ) &tr->common_3, sizeof ( size_t ) * 40 ) != 0 ||
        tr->common_3[0] != 2 || tr->common_3[1] != 1
    ) {
        return TRALLOC_FALSE;
    }

    return test_references_check_moved_chunks ( tr );
}
