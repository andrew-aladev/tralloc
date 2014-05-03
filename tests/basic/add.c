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
       4  3  2
         /|\
       7  6  5
       |
    trivium

*/

static
tralloc_bool test_chunks ( test_basic_tree * tr )
{
    _tralloc_chunk * root_chunk    = _tralloc_get_chunk_from_context ( tr );
    _tralloc_chunk * data_1_chunk  = _tralloc_get_chunk_from_context ( tr->data_1 );
    _tralloc_chunk * data_2_chunk  = _tralloc_get_chunk_from_context ( tr->data_2 );
    _tralloc_chunk * data_3_chunk  = _tralloc_get_chunk_from_context ( tr->data_3 );
    _tralloc_chunk * data_4_chunk  = _tralloc_get_chunk_from_context ( tr->data_4 );
    _tralloc_chunk * data_5_chunk  = _tralloc_get_chunk_from_context ( tr->data_5 );
    _tralloc_chunk * data_6_chunk  = _tralloc_get_chunk_from_context ( tr->data_6 );
    _tralloc_chunk * data_7_chunk  = _tralloc_get_chunk_from_context ( tr->data_7 );
    _tralloc_chunk * trivium_chunk = _tralloc_get_chunk_from_context ( tr->trivium );

    if (
        root_chunk->prev        != NULL         ||
        root_chunk->next        != NULL         ||
        root_chunk->first_child != data_1_chunk ||

        data_1_chunk->parent      != root_chunk   ||
        data_1_chunk->prev        != NULL         ||
        data_1_chunk->next        != NULL         ||
        data_1_chunk->first_child != data_4_chunk ||

        data_2_chunk->parent      != data_1_chunk ||
        data_2_chunk->prev        != data_3_chunk ||
        data_2_chunk->next        != NULL         ||
        data_2_chunk->first_child != NULL         ||

        data_3_chunk->parent      != data_1_chunk ||
        data_3_chunk->prev        != data_4_chunk ||
        data_3_chunk->next        != data_2_chunk ||
        data_3_chunk->first_child != data_7_chunk ||

        data_4_chunk->parent      != data_1_chunk ||
        data_4_chunk->prev        != NULL         ||
        data_4_chunk->next        != data_3_chunk ||
        data_4_chunk->first_child != NULL         ||

        data_5_chunk->parent      != data_3_chunk ||
        data_5_chunk->prev        != data_6_chunk ||
        data_5_chunk->next        != NULL         ||
        data_5_chunk->first_child != NULL         ||

        data_6_chunk->parent      != data_3_chunk ||
        data_6_chunk->prev        != data_7_chunk ||
        data_6_chunk->next        != data_5_chunk ||
        data_6_chunk->first_child != NULL         ||

        data_7_chunk->parent      != data_3_chunk  ||
        data_7_chunk->prev        != NULL          ||
        data_7_chunk->next        != data_6_chunk  ||
        data_7_chunk->first_child != trivium_chunk ||

        trivium_chunk->parent      != data_7_chunk ||
        trivium_chunk->prev        != NULL         ||
        trivium_chunk->next        != NULL         ||
        trivium_chunk->first_child != NULL
    ) {
        return TRALLOC_FALSE;
    }
    return TRALLOC_TRUE;
}

tralloc_bool test_basic_add ( test_basic_tree * tr )
{
    if ( tralloc ( tr, ( tralloc_context ** ) &tr->data_1, sizeof ( uint8_t ) ) != 0 ) {
        return TRALLOC_FALSE;
    }
    * tr->data_1 = 123;

    if (
        tralloc_zero ( tr->data_1, ( tralloc_context ** ) &tr->data_2, sizeof ( uint8_t ) * 4 ) != 0 ||
        tr->data_2[0] != 0 || tr->data_2[1] != 0 || tr->data_2[2] != 0 || tr->data_2[3] != 0
    ) {
        return TRALLOC_FALSE;
    }
    tr->data_2[0] = 012;
    tr->data_2[1] = 34;
    tr->data_2[2] = 56;
    tr->data_2[3] = 78;

    if ( tralloc ( tr->data_1, ( tralloc_context ** ) &tr->data_3, sizeof ( char ) * 3 ) != 0 ) {
        return TRALLOC_FALSE;
    }
    tr->data_3[0] = 'q';
    tr->data_3[1] = 'w';
    tr->data_3[2] = 'e';

    if (
        tralloc_zero ( tr->data_1, ( tralloc_context ** ) &tr->data_4, sizeof ( int ) * 2 ) != 0 ||
        tr->data_4[0] != 0 || tr->data_4[1] != 0
    ) {
        return TRALLOC_FALSE;
    }
    tr->data_4[0] = -102;
    tr->data_4[1] = 103;

    if ( tralloc ( tr->data_3, ( tralloc_context ** ) &tr->data_5, sizeof ( size_t ) * 3 ) != 0 ) {
        return TRALLOC_FALSE;
    }
    tr->data_5[0] = 123;
    tr->data_5[1] = 213;
    tr->data_5[2] = 123;

    if (
        tralloc_zero ( tr->data_3, ( tralloc_context ** ) &tr->data_6, sizeof ( float ) ) != 0 ||
        * tr->data_6 != 0
    ) {
        return TRALLOC_FALSE;
    }
    * tr->data_6 = 0.012345;

    if ( tralloc ( tr->data_3, ( tralloc_context ** ) &tr->data_7, sizeof ( float ) * 2 ) != 0 ) {
        return TRALLOC_FALSE;
    }
    tr->data_7[0] = 0.01234;
    tr->data_7[1] = 0.56789;

    if ( tralloc_empty ( tr->data_7, ( tralloc_context ** ) &tr->trivium ) != 0 ) {
        return TRALLOC_FALSE;
    }

    return test_chunks ( tr );
}
