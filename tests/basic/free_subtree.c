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
          /|
        6  2

*/

static
bool test_chunks ( tree * tr )
{
    _tralloc_chunk * root_chunk    = _tralloc_get_chunk_from_context ( tr );
    _tralloc_chunk * data_1_chunk  = _tralloc_get_chunk_from_context ( tr->data_1 );
    _tralloc_chunk * data_2_chunk  = _tralloc_get_chunk_from_context ( tr->data_2 );
    _tralloc_chunk * data_6_chunk  = _tralloc_get_chunk_from_context ( tr->data_6 );

    if (
        root_chunk->parent      != NULL         ||
        root_chunk->prev        != NULL         ||
        root_chunk->next        != NULL         ||
        root_chunk->first_child != data_1_chunk ||

        data_1_chunk->parent      != root_chunk   ||
        data_1_chunk->prev        != NULL         ||
        data_1_chunk->next        != NULL         ||
        data_1_chunk->first_child != data_6_chunk ||

        data_2_chunk->parent      != data_1_chunk ||
        data_2_chunk->prev        != data_6_chunk ||
        data_2_chunk->next        != NULL         ||
        data_2_chunk->first_child != NULL         ||

        data_6_chunk->parent      != data_1_chunk ||
        data_6_chunk->prev        != NULL         ||
        data_6_chunk->next        != data_2_chunk ||
        data_6_chunk->first_child != NULL
    ) {
        return false;
    }

    return true;
}

bool test_free_subtree ( tree * tr )
{
    if (
        tralloc_free ( tr->data_4 ) != 0 ||
        tralloc_free ( tr->data_7 ) != 0
    ) {
        return false;
    }

    return test_chunks ( tr );
}
