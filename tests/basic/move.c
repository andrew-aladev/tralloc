// This file is part of tralloc. Copyright (C) 2013 Andrew Aladjev aladjev.andrew@gmail.com
// tralloc is free software: you can redistribute it and/or modify it under the terms of the GNU General Public License as published by the Free Software Foundation, either version 3 of the License, or (at your option) any later version.
// tralloc is distributed in the hope that it will be useful, but WITHOUT ANY WARRANTY; without even the implied warranty of MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU General Public License for more details.
// You should have received a copy of the GNU General Public License along with tralloc. If not, see <http://www.gnu.org/licenses/>.

#include "common.h"
#include <tralloc/tree.h>


/*
           tree
            |
            1
           /|\
       3    4    2
      /|\
  7         6         5
  |         |         |\
trivium trivium_3 trivium_1 trivium_2
                     |
                     8
                    / \
                  10   9
*/

static
bool test_chunks ( tree * tr )
{
    _tralloc_chunk * root_chunk    = _tralloc_chunk_from_context ( tr );
    _tralloc_chunk * data_1_chunk  = _tralloc_chunk_from_context ( tr->data_1 );
    _tralloc_chunk * data_2_chunk  = _tralloc_chunk_from_context ( tr->data_2 );
    _tralloc_chunk * data_3_chunk  = _tralloc_chunk_from_context ( tr->data_3 );
    _tralloc_chunk * data_4_chunk  = _tralloc_chunk_from_context ( tr->data_4 );
    _tralloc_chunk * data_5_chunk  = _tralloc_chunk_from_context ( tr->data_5 );
    _tralloc_chunk * data_6_chunk  = _tralloc_chunk_from_context ( tr->data_6 );
    _tralloc_chunk * data_7_chunk  = _tralloc_chunk_from_context ( tr->data_7 );
    _tralloc_chunk * trivium_chunk = _tralloc_chunk_from_context ( tr->trivium );

    if (
        root_chunk->parent      != NULL         ||
        root_chunk->prev        != NULL         ||
        root_chunk->next        != NULL         ||
        root_chunk->first_child != data_1_chunk ||

        data_1_chunk->parent      != root_chunk   ||
        data_1_chunk->prev        != NULL         ||
        data_1_chunk->next        != NULL         ||
        data_1_chunk->first_child != data_3_chunk ||

        data_2_chunk->parent      != data_1_chunk ||
        data_2_chunk->prev        != data_4_chunk ||
        data_2_chunk->next        != NULL         ||
        data_2_chunk->first_child != NULL         ||

        data_3_chunk->parent      != data_1_chunk ||
        data_3_chunk->prev        != NULL         ||
        data_3_chunk->next        != data_4_chunk ||
        data_3_chunk->first_child != data_7_chunk ||

        data_4_chunk->parent      != data_1_chunk ||
        data_4_chunk->prev        != data_3_chunk ||
        data_4_chunk->next        != data_2_chunk ||
        data_4_chunk->first_child != NULL         ||

        data_5_chunk->parent != data_3_chunk ||
        data_5_chunk->prev   != data_6_chunk ||
        data_5_chunk->next   != NULL         ||

        data_6_chunk->parent != data_3_chunk ||
        data_6_chunk->prev   != data_7_chunk ||
        data_6_chunk->next   != data_5_chunk ||

        data_7_chunk->parent      != data_3_chunk  ||
        data_7_chunk->prev        != NULL          ||
        data_7_chunk->next        != data_6_chunk  ||
        data_7_chunk->first_child != trivium_chunk ||

        trivium_chunk->parent      != data_7_chunk ||
        trivium_chunk->prev        != NULL         ||
        trivium_chunk->next        != NULL         ||
        trivium_chunk->first_child != NULL
    ) {
        return false;
    }

#if defined(TRALLOC_REFERENCE)

    _tralloc_chunk * data_8_chunk  = _tralloc_chunk_from_context ( tr->data_8 );
    _tralloc_chunk * data_9_chunk  = _tralloc_chunk_from_context ( tr->data_9 );
    _tralloc_chunk * data_10_chunk = _tralloc_chunk_from_context ( tr->data_10 );

    _tralloc_chunk * trivium_reference_1_chunk = _tralloc_chunk_from_context ( tr->trivium_reference_1 );
    _tralloc_chunk * trivium_reference_2_chunk = _tralloc_chunk_from_context ( tr->trivium_reference_2 );
    _tralloc_chunk * trivium_reference_3_chunk = _tralloc_chunk_from_context ( tr->trivium_reference_3 );

    if (
        data_5_chunk->first_child != trivium_reference_1_chunk ||
        data_6_chunk->first_child != trivium_reference_3_chunk ||

        trivium_reference_1_chunk->parent      != data_5_chunk              ||
        trivium_reference_1_chunk->prev        != NULL                      ||
        trivium_reference_1_chunk->next        != trivium_reference_2_chunk ||
        trivium_reference_1_chunk->first_child != data_8_chunk              ||

        trivium_reference_2_chunk->parent      != data_5_chunk              ||
        trivium_reference_2_chunk->prev        != trivium_reference_1_chunk ||
        trivium_reference_2_chunk->next        != NULL                      ||
        trivium_reference_2_chunk->first_child != NULL                      ||

        trivium_reference_3_chunk->parent      != data_6_chunk ||
        trivium_reference_3_chunk->prev        != NULL         ||
        trivium_reference_3_chunk->next        != NULL         ||
        trivium_reference_3_chunk->first_child != NULL         ||

        data_8_chunk->parent      != trivium_reference_1_chunk ||
        data_8_chunk->prev        != NULL                      ||
        data_8_chunk->next        != NULL                      ||
        data_8_chunk->first_child != data_10_chunk             ||

        data_10_chunk->parent      != data_8_chunk ||
        data_10_chunk->prev        != NULL         ||
        data_10_chunk->next        != data_9_chunk ||
        data_10_chunk->first_child != NULL         ||

        data_9_chunk->parent      != data_8_chunk  ||
        data_9_chunk->prev        != data_10_chunk ||
        data_9_chunk->next        != NULL          ||
        data_9_chunk->first_child != NULL
    ) {
        return false;
    }

#else

    if (
        data_5_chunk->first_child != NULL ||
        data_6_chunk->first_child != NULL
    ) {
        return false;
    }

#endif

    return true;
}


bool test_move ( tree * tr )
{
    tralloc_context * data_1 = tr->data_1;
    tralloc_context * data_3 = tr->data_3;
    tralloc_context * data_4 = tr->data_4;

    _tralloc_chunk * data_1_chunk = _tralloc_chunk_from_context ( tr->data_1 );
    _tralloc_chunk * data_3_chunk = _tralloc_chunk_from_context ( tr->data_3 );
    _tralloc_chunk * data_4_chunk = _tralloc_chunk_from_context ( tr->data_4 );

    if (
        tralloc_move ( NULL, NULL )     != TRALLOC_ERROR_CONTEXT_IS_NULL     ||
        tralloc_move ( data_3, data_3 ) != TRALLOC_ERROR_CHILD_EQUALS_PARENT ||

        tralloc_move ( data_3, data_4 ) != 0            ||
        data_3_chunk->parent            != data_4_chunk ||

        tralloc_move ( data_3, data_4 ) != TRALLOC_ERROR_CHILD_HAS_SAME_PARENT ||

        tralloc_move ( data_3, NULL ) != 0    ||
        data_3_chunk->parent          != NULL ||

        tralloc_move ( data_3, data_1 ) != 0 ||
        data_3_chunk->parent            != data_1_chunk
    ) {
        return false;
    }

#if defined(TRALLOC_REFERENCE)

    tralloc_context * data_5              = tr->data_5;
    tralloc_context * data_6              = tr->data_6;
    tralloc_context * trivium_reference_1 = tr->trivium_reference_1;
    tralloc_context * trivium_reference_3 = tr->trivium_reference_3;

    _tralloc_chunk * data_5_chunk              = _tralloc_chunk_from_context ( data_5 );
    _tralloc_chunk * data_6_chunk              = _tralloc_chunk_from_context ( data_6 );
    _tralloc_chunk * trivium_reference_1_chunk = _tralloc_chunk_from_context ( trivium_reference_1 );
    _tralloc_chunk * trivium_reference_3_chunk = _tralloc_chunk_from_context ( trivium_reference_3 );

    if (
        tralloc_move ( tr->trivium_reference_1, NULL )       != 0            ||
        tralloc_move ( tr->trivium_reference_1, tr->data_5 ) != 0            ||
        trivium_reference_1_chunk->parent                    != data_5_chunk ||

        tralloc_move ( tr->trivium_reference_3, tr->data_6 ) != 0 ||
        trivium_reference_3_chunk->parent                    != data_6_chunk
    ) {
        return false;
    }

#endif

    return test_chunks ( tr );
}
