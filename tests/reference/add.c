// This file is part of tralloc. Copyright (C) 2013 Andrew Aladjev aladjev.andrew@gmail.com
// tralloc is free software: you can redistribute it and/or modify it under the terms of the GNU General Public License as published by the Free Software Foundation, either version 3 of the License, or (at your option) any later version.
// tralloc is distributed in the hope that it will be useful, but WITHOUT ANY WARRANTY; without even the implied warranty of MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU General Public License for more details.
// You should have received a copy of the GNU General Public License along with tralloc. If not, see <http://www.gnu.org/licenses/>.

#include "common.h"
#include <tralloc/tree.h>
#include <tralloc/reference/main.h>


/*
                    tree    common
                    /|\
           1         2      shared
          /|         |        |
common_1   3      common_2  common_3
           |         |
        shared_1  shared_2

*/

static
bool test_chunks ( tree * tr )
{
    _tralloc_chunk * root_chunk     = _tralloc_get_chunk_from_context ( tr );
    _tralloc_chunk * common_chunk   = _tralloc_get_chunk_from_context ( tr->common );
    _tralloc_chunk * shared_chunk   = _tralloc_get_chunk_from_context ( tr->shared );
    _tralloc_chunk * data_1_chunk   = _tralloc_get_chunk_from_context ( tr->data_1 );
    _tralloc_chunk * data_2_chunk   = _tralloc_get_chunk_from_context ( tr->data_2 );
    _tralloc_chunk * data_3_chunk   = _tralloc_get_chunk_from_context ( tr->data_3 );
    _tralloc_chunk * common_1_chunk = _tralloc_get_chunk_from_context ( tr->common_1 );
    _tralloc_chunk * common_2_chunk = _tralloc_get_chunk_from_context ( tr->common_2 );
    _tralloc_chunk * common_3_chunk = _tralloc_get_chunk_from_context ( tr->common_3 );
    _tralloc_chunk * shared_1_chunk = _tralloc_get_chunk_from_context ( tr->shared_1 );
    _tralloc_chunk * shared_2_chunk = _tralloc_get_chunk_from_context ( tr->shared_2 );

    if (
        common_chunk->parent      != NULL ||
        common_chunk->prev        != NULL ||
        common_chunk->next        != NULL ||
        common_chunk->first_child != NULL ||

        root_chunk->parent      != NULL         ||
        root_chunk->prev        != NULL         ||
        root_chunk->next        != NULL         ||
        root_chunk->first_child != data_1_chunk ||

        data_1_chunk->parent      != root_chunk     ||
        data_1_chunk->prev        != NULL           ||
        data_1_chunk->next        != data_2_chunk   ||
        data_1_chunk->first_child != common_1_chunk ||

        data_2_chunk->parent      != root_chunk     ||
        data_2_chunk->prev        != data_1_chunk   ||
        data_2_chunk->next        != shared_chunk   ||
        data_2_chunk->first_child != common_2_chunk ||

        data_3_chunk->parent      != data_1_chunk   ||
        data_3_chunk->prev        != common_1_chunk ||
        data_3_chunk->next        != NULL           ||
        data_3_chunk->first_child != shared_1_chunk ||

        shared_chunk->parent      != root_chunk     ||
        shared_chunk->prev        != data_2_chunk   ||
        shared_chunk->next        != NULL           ||
        shared_chunk->first_child != common_3_chunk ||

        shared_1_chunk->parent      != data_3_chunk ||
        shared_1_chunk->prev        != NULL         ||
        shared_1_chunk->next        != NULL         ||
        shared_1_chunk->first_child != NULL         ||

        shared_2_chunk->parent      != common_2_chunk ||
        shared_2_chunk->prev        != NULL           ||
        shared_2_chunk->next        != NULL           ||
        shared_2_chunk->first_child != NULL           ||

        common_1_chunk->parent      != data_1_chunk ||
        common_1_chunk->prev        != NULL         ||
        common_1_chunk->next        != data_3_chunk ||
        common_1_chunk->first_child != NULL         ||

        common_2_chunk->parent      != data_2_chunk   ||
        common_2_chunk->prev        != NULL           ||
        common_2_chunk->next        != NULL           ||
        common_2_chunk->first_child != shared_2_chunk ||

        common_3_chunk->parent      != shared_chunk ||
        common_3_chunk->prev        != NULL         ||
        common_3_chunk->next        != NULL         ||
        common_3_chunk->first_child != NULL
    ) {
        return false;
    }

    return true;
}

bool test_add ( tree * tr )
{
    if ( tralloc_with_extensions ( NULL, ( tralloc_context ** ) &tr->common, TRALLOC_EXTENSION_REFERENCES, sizeof ( uint32_t ) * 2 ) != 0 ) {
        return false;
    }
    tr->common[0] = 123456;
    tr->common[1] = 654321;

    if ( tralloc_with_extensions ( tr, ( tralloc_context ** ) &tr->shared, TRALLOC_EXTENSION_REFERENCES, sizeof ( float ) ) != 0 ) {
        return false;
    }
    * tr->shared = 0.123456;

    if ( tralloc ( tr, ( tralloc_context ** ) &tr->data_2, sizeof ( uint16_t ) ) != 0 ) {
        return false;
    }
    * tr->data_2 = 12345;

    if ( tralloc ( tr, ( tralloc_context ** ) &tr->data_1, sizeof ( uint8_t ) * 3 ) != 0 ) {
        return false;
    }
    tr->data_1[0] = 1;
    tr->data_1[1] = 2;
    tr->data_1[2] = 3;

    if (
        tralloc_zero ( tr->data_1, ( tralloc_context ** ) &tr->data_3, sizeof ( char ) * 4 ) != 0 ||
        tr->data_3[0] != 0 || tr->data_3[1] != 0 || tr->data_3[2] != 0 || tr->data_3[3] != 0
    ) {
        return false;
    }
    tr->data_3[0] = 'q';
    tr->data_3[1] = 'w';
    tr->data_3[2] = 'e';
    tr->data_3[3] = 'r';

    if ( tralloc_with_extensions_new ( tr->data_3, ( tralloc_context ** ) &tr->shared_1, TRALLOC_EXTENSION_REFERENCE ) != 0 ) {
        return false;
    }

    if (
        tralloc_zero_with_extensions ( tr->data_1, ( tralloc_context ** ) &tr->common_1, TRALLOC_EXTENSION_REFERENCE, sizeof ( int ) * 2 ) != 0 ||
        tr->common_1[0] != 0 || tr->common_1[1] != 0
    ) {
        return false;
    }
    tr->common_1[0] = 456789;
    tr->common_1[1] = 987654;

    if ( tralloc_with_extensions_new ( tr->data_2, ( tralloc_context ** ) &tr->common_2, TRALLOC_EXTENSION_REFERENCE ) != 0 ) {
        return false;
    }

    if ( tralloc_with_extensions ( tr->common_2, ( tralloc_context ** ) &tr->shared_2, TRALLOC_EXTENSION_REFERENCE, sizeof ( double ) ) != 0 ) {
        return false;
    }
    * tr->shared_2 = 0.123456789;

    if ( tralloc_with_extensions ( tr->shared, ( tralloc_context ** ) &tr->common_3, TRALLOC_EXTENSION_REFERENCE, sizeof ( uint8_t ) * 2 ) != 0 ) {
        return false;
    }
    tr->common_3[0] = 2;
    tr->common_3[1] = 1;

    return test_chunks ( tr );
}
