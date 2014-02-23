// This file is part of tralloc. Copyright (C) 2013 Andrew Aladjev aladjev.andrew@gmail.com
// tralloc is free software: you can redistribute it and/or modify it under the terms of the GNU General Public License as published by the Free Software Foundation, either version 3 of the License, or (at your option) any later version.
// tralloc is distributed in the hope that it will be useful, but WITHOUT ANY WARRANTY; without even the implied warranty of MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU General Public License for more details.
// You should have received a copy of the GNU General Public License along with tralloc. If not, see <http://www.gnu.org/licenses/>.

#include "common.h"
#include <tralloc/tree.h>
#include <tralloc/reference/main.h>
#include <tralloc/reference/common.h>

/*
    tree  common shared
      |            |
      1          common_3
      |
      3
      |
  shared_2

*/

static
bool test_references ( tree * tr )
{
    _tralloc_references * common_references  = _tralloc_get_references_from_chunk ( _tralloc_get_chunk_from_context ( tr->common ) );
    _tralloc_references * shared_references  = _tralloc_get_references_from_chunk ( _tralloc_get_chunk_from_context ( tr->shared ) );
    _tralloc_reference  * common_3_reference = _tralloc_get_reference_from_chunk  ( _tralloc_get_chunk_from_context ( tr->common_3 ) );
    _tralloc_reference  * shared_2_reference = _tralloc_get_reference_from_chunk  ( _tralloc_get_chunk_from_context ( tr->shared_2 ) );

    if (
        common_references->first_reference != common_3_reference ||

        common_3_reference->prev       != NULL              ||
        common_3_reference->next       != NULL              ||
        common_3_reference->references != common_references ||

        shared_references->first_reference != shared_2_reference ||

        shared_2_reference->prev       != NULL ||
        shared_2_reference->next       != NULL ||
        shared_2_reference->references != shared_references
    ) {
        return false;
    }
    return true;
}

static
bool test_chunks ( tree * tr )
{
    _tralloc_chunk * root_chunk     = _tralloc_get_chunk_from_context ( tr );
    _tralloc_chunk * common_chunk   = _tralloc_get_chunk_from_context ( tr->common );
    _tralloc_chunk * shared_chunk   = _tralloc_get_chunk_from_context ( tr->shared );
    _tralloc_chunk * data_1_chunk   = _tralloc_get_chunk_from_context ( tr->data_1 );
    _tralloc_chunk * data_3_chunk   = _tralloc_get_chunk_from_context ( tr->data_3 );
    _tralloc_chunk * common_3_chunk = _tralloc_get_chunk_from_context ( tr->common_3 );
    _tralloc_chunk * shared_2_chunk = _tralloc_get_chunk_from_context ( tr->shared_2 );

    if (
        shared_chunk->parent      != NULL           ||
        shared_chunk->prev        != NULL           ||
        shared_chunk->next        != NULL           ||
        shared_chunk->first_child != common_3_chunk ||

        common_chunk->parent      != NULL ||
        common_chunk->prev        != NULL ||
        common_chunk->next        != NULL ||
        common_chunk->first_child != NULL ||

        root_chunk->parent      != NULL         ||
        root_chunk->prev        != NULL         ||
        root_chunk->next        != NULL         ||
        root_chunk->first_child != data_1_chunk ||

        data_1_chunk->parent      != root_chunk   ||
        data_1_chunk->prev        != NULL         ||
        data_1_chunk->next        != NULL         ||
        data_1_chunk->first_child != data_3_chunk ||

        data_3_chunk->parent      != data_1_chunk   ||
        data_3_chunk->prev        != NULL           ||
        data_3_chunk->next        != NULL           ||
        data_3_chunk->first_child != shared_2_chunk ||

        shared_2_chunk->parent      != data_3_chunk ||
        shared_2_chunk->prev        != NULL         ||
        shared_2_chunk->next        != NULL         ||
        shared_2_chunk->first_child != NULL         ||

        common_3_chunk->parent      != shared_chunk ||
        common_3_chunk->prev        != NULL         ||
        common_3_chunk->next        != NULL         ||
        common_3_chunk->first_child != NULL
    ) {
        return false;
    }

    return test_references ( tr );
}

bool test_free_subtree ( tree * tr )
{
    if (
        tralloc_free ( tr->shared )   != 0 ||
        tralloc_free ( tr->common_2 ) != 0 ||
        tralloc_free ( tr->data_2 )   != 0
    ) {
        return false;
    }

    return test_chunks ( tr );
}
