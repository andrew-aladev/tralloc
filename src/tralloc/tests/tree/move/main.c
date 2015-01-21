// This file is part of tralloc. Copyright (C) 2013 Andrew Aladjev aladjev.andrew@gmail.com
// tralloc is free software: you can redistribute it and/or modify it under the terms of the GNU General Public License as published by the Free Software Foundation, either version 3 of the License, or (at your option) any later version.
// tralloc is distributed in the hope that it will be useful, but WITHOUT ANY WARRANTY; without even the implied warranty of MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU General Public License for more details.
// You should have received a copy of the GNU General Public License along with tralloc. If not, see <http://www.gnu.org/licenses/>.

#include <tralloc/tests/tree/alloc/common.h>
#include <tralloc/tree/alloc.h>
#include <tralloc/tree/move.h>
#include <tralloc/tree/free.h>


tralloc_bool test_tree_move_main ( test_tree_data * tr )
{
    if (
        tralloc_move ( tr->data_7, tr->data_2 ) != 0 ||
        tralloc_move ( tr->data_3, tr->data_4 ) != 0 ||
        tralloc_move ( tr->data_6, tr->data_1 ) != 0 ||
        tralloc_move ( tr->data_5, tr->data_4 ) != 0
    ) {
        return TRALLOC_FALSE;
    }

    /*
          tree
           |
           1
          /|\
       6   4    2
          /|    |
        5  3    7
                |
             trivium

    */

    _tralloc_chunk * root_chunk    = _tralloc_context_get_chunk ( tr );
    _tralloc_chunk * data_1_chunk  = _tralloc_context_get_chunk ( tr->data_1 );
    _tralloc_chunk * data_2_chunk  = _tralloc_context_get_chunk ( tr->data_2 );
    _tralloc_chunk * data_3_chunk  = _tralloc_context_get_chunk ( tr->data_3 );
    _tralloc_chunk * data_4_chunk  = _tralloc_context_get_chunk ( tr->data_4 );
    _tralloc_chunk * data_5_chunk  = _tralloc_context_get_chunk ( tr->data_5 );
    _tralloc_chunk * data_6_chunk  = _tralloc_context_get_chunk ( tr->data_6 );
    _tralloc_chunk * data_7_chunk  = _tralloc_context_get_chunk ( tr->data_7 );
    _tralloc_chunk * trivium_chunk = _tralloc_context_get_chunk ( tr->trivium );

    if (
        root_chunk->prev        != NULL         ||
        root_chunk->next        != NULL         ||
        root_chunk->first_child != data_1_chunk ||

        data_1_chunk->parent      != root_chunk   ||
        data_1_chunk->prev        != NULL         ||
        data_1_chunk->next        != NULL         ||
        data_1_chunk->first_child != data_6_chunk ||

        data_2_chunk->parent      != data_1_chunk ||
        data_2_chunk->prev        != data_4_chunk ||
        data_2_chunk->next        != NULL         ||
        data_2_chunk->first_child != data_7_chunk ||

        data_3_chunk->parent      != data_4_chunk ||
        data_3_chunk->prev        != data_5_chunk ||
        data_3_chunk->next        != NULL         ||
        data_3_chunk->first_child != NULL         ||

        data_4_chunk->parent      != data_1_chunk ||
        data_4_chunk->prev        != data_6_chunk ||
        data_4_chunk->next        != data_2_chunk ||
        data_4_chunk->first_child != data_5_chunk ||

        data_5_chunk->parent      != data_4_chunk ||
        data_5_chunk->prev        != NULL         ||
        data_5_chunk->next        != data_3_chunk ||
        data_5_chunk->first_child != NULL         ||

        data_6_chunk->parent      != data_1_chunk ||
        data_6_chunk->prev        != NULL         ||
        data_6_chunk->next        != data_4_chunk ||
        data_6_chunk->first_child != NULL         ||

        data_7_chunk->parent      != data_2_chunk  ||
        data_7_chunk->prev        != NULL          ||
        data_7_chunk->next        != NULL          ||
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
