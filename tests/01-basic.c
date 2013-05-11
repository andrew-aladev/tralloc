// This file is part of talloc. Copyright (C) 2013 Andrew Aladjev aladjev.andrew@gmail.com
// talloc is free software: you can redistribute it and/or modify it under the terms of the GNU General Public License as published by the Free Software Foundation, either version 3 of the License, or (at your option) any later version.
// talloc is distributed in the hope that it will be useful, but WITHOUT ANY WARRANTY; without even the implied warranty of MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU General Public License for more details.
// You should have received a copy of the GNU General Public License along with talloc. If not, see <http://www.gnu.org/licenses/>.

#include <stdlib.h>
#include <stdio.h>
#include <stdint.h>
#include <string.h>
#include <stdbool.h>
#include <limits.h>

#include <talloc/tree.h>
#include "utils/dynarr.h"

#ifdef DEBUG
static talloc_dynarr * arr;

typedef struct talloc_event_t {
    talloc_chunk * chunk;
    bool is_add;
} talloc_event;

void on_add ( talloc_chunk * chunk ) {
    talloc_event * event = malloc ( sizeof ( talloc_event ) );
    event->is_add = true;
    event->chunk  = chunk;
    talloc_dynarr_append ( arr, event );
}
void on_del ( talloc_chunk * chunk ) {
    talloc_event * event = malloc ( sizeof ( talloc_event ) );
    event->is_add = false;
    event->chunk  = chunk;
    talloc_dynarr_append ( arr, event );
}
#endif

/*
           root
            |
            0
           /|\
       02  01   00
           /|\
      012  011  010
       |
     empty
*/

int main () {
    // all history will be available here
#ifdef DEBUG
    arr = talloc_dynarr_new ( 128 );
    talloc_set_callback ( on_add, on_del );
#endif

    // allocation will return NULL on error
    void *     root     = talloc_new ( NULL );
    int8_t *   data_0   = talloc ( root,    sizeof ( int8_t ) );
    uint16_t * data_00  = talloc_zero ( data_0,  sizeof ( uint16_t ) );
    char *     data_01  = talloc ( data_0,  sizeof ( char ) * 3 );
    uint32_t * data_02  = talloc_zero ( data_0,  sizeof ( uint32_t ) );
    size_t *   data_010 = talloc ( data_01, sizeof ( size_t ) );
    size_t *   data_011 = talloc_zero ( data_01, sizeof ( size_t ) );
    size_t *   data_012 = talloc ( data_01, sizeof ( size_t ) );
    void *     empty    = talloc_new ( data_012 );

    if (
        ! (
            root     != NULL &&
            data_0   != NULL &&
            data_00  != NULL &&
            data_01  != NULL &&
            data_02  != NULL &&
            data_010 != NULL &&
            data_011 != NULL &&
            data_012 != NULL &&
            empty    != NULL
        )
    ) {
        talloc_free ( root );
#ifdef DEBUG
        talloc_dynarr_free ( arr, true );
#endif
        return 1;
    }

    // check data from talloc_zero is zero
    if (
        ! (
            *data_00  == 0 &&
            *data_02  == 0 &&
            *data_011 == 0
        )
    ) {
        talloc_free ( root );
#ifdef DEBUG
        talloc_dynarr_free ( arr, true );
#endif
        return 2;
    }

    // set all bits to 1
    *data_0    = INT8_MAX;
    *data_00   = UINT16_MAX;
    data_01[0] = CHAR_MAX;
    data_01[1] = CHAR_MAX;
    data_01[2] = CHAR_MAX;
    *data_02   = UINT32_MAX;
    *data_010  = SIZE_MAX;
    *data_011  = SIZE_MAX;
    *data_012  = SIZE_MAX;

    // obtain chunks from data. chunks should be not NULL
    talloc_chunk * chunk_root  = talloc_chunk_from_data ( root );
    talloc_chunk * chunk_0     = talloc_chunk_from_data ( data_0 );
    talloc_chunk * chunk_00    = talloc_chunk_from_data ( data_00 );
    talloc_chunk * chunk_01    = talloc_chunk_from_data ( data_01 );
    talloc_chunk * chunk_02    = talloc_chunk_from_data ( data_02 );
    talloc_chunk * chunk_010   = talloc_chunk_from_data ( data_010 );
    talloc_chunk * chunk_011   = talloc_chunk_from_data ( data_011 );
    talloc_chunk * chunk_012   = talloc_chunk_from_data ( data_012 );
    talloc_chunk * chunk_empty = talloc_chunk_from_data ( empty );

    if (
        ! (
            chunk_root  != NULL &&
            chunk_0     != NULL &&
            chunk_00    != NULL &&
            chunk_01    != NULL &&
            chunk_02    != NULL &&
            chunk_010   != NULL &&
            chunk_011   != NULL &&
            chunk_012   != NULL &&
            chunk_empty != NULL
        )
    ) {
        talloc_free ( root );
#ifdef DEBUG
        talloc_dynarr_free ( arr, true );
#endif
        return 3;
    }

    // checking tree structure. see scheme above
    if (
        ! (
            chunk_root->parent      == NULL    &&
            chunk_root->prev        == NULL    &&
            chunk_root->next        == NULL    &&
            chunk_root->first_child == chunk_0 &&

            chunk_0->parent      == chunk_root &&
            chunk_0->prev        == NULL       &&
            chunk_0->next        == NULL       &&
            chunk_0->first_child == chunk_02   &&

            chunk_00->parent      == chunk_0  &&
            chunk_00->prev        == chunk_01 &&
            chunk_00->next        == NULL     &&
            chunk_00->first_child == NULL     &&

            chunk_01->parent      == chunk_0   &&
            chunk_01->prev        == chunk_02  &&
            chunk_01->next        == chunk_00  &&
            chunk_01->first_child == chunk_012 &&

            chunk_02->parent      == chunk_0  &&
            chunk_02->prev        == NULL     &&
            chunk_02->next        == chunk_01 &&
            chunk_02->first_child == NULL     &&

            chunk_010->parent      == chunk_01  &&
            chunk_010->prev        == chunk_011 &&
            chunk_010->next        == NULL      &&
            chunk_010->first_child == NULL      &&

            chunk_011->parent      == chunk_01  &&
            chunk_011->prev        == chunk_012 &&
            chunk_011->next        == chunk_010 &&
            chunk_011->first_child == NULL      &&

            chunk_012->parent      == chunk_01    &&
            chunk_012->prev        == NULL        &&
            chunk_012->next        == chunk_011   &&
            chunk_012->first_child == chunk_empty &&

            chunk_empty->parent      == chunk_012 &&
            chunk_empty->prev        == NULL      &&
            chunk_empty->next        == NULL      &&
            chunk_empty->first_child == NULL
        )
    ) {
        talloc_free ( root );
#ifdef DEBUG
        talloc_dynarr_free ( arr, true );
#endif
        return 4;
    }

    talloc_free ( data_01 );

    // checking tree structure after delete 01. see scheme above
    if (
        ! (
            chunk_root->parent      == NULL    &&
            chunk_root->prev        == NULL    &&
            chunk_root->next        == NULL    &&
            chunk_root->first_child == chunk_0 &&

            chunk_0->parent      == chunk_root &&
            chunk_0->prev        == NULL       &&
            chunk_0->next        == NULL       &&
            chunk_0->first_child == chunk_02   &&

            chunk_00->parent      == chunk_0  &&
            chunk_00->prev        == chunk_02 &&
            chunk_00->next        == NULL     &&
            chunk_00->first_child == NULL     &&

            chunk_02->parent      == chunk_0  &&
            chunk_02->prev        == NULL     &&
            chunk_02->next        == chunk_00 &&
            chunk_02->first_child == NULL
        )
    ) {
        talloc_free ( root );
#ifdef DEBUG
        talloc_dynarr_free ( arr, true );
#endif
        return 5;
    }

    // check data
    if (
        ! (
            *data_0  == INT8_MAX &&
            *data_00 == UINT16_MAX &&
            *data_02 == UINT32_MAX
        )
    ) {
        talloc_free ( root );
#ifdef DEBUG
        talloc_dynarr_free ( arr, true );
#endif
        return 6;
    }

    talloc_free ( root );

#ifdef DEBUG
    // checking all history
    talloc_event * event;
    if (
        ! (
            arr->length == 18 &&
            ( event = talloc_dynarr_get ( arr, 0 ) ) != NULL && event->is_add && event->chunk == chunk_root  &&
            ( event = talloc_dynarr_get ( arr, 1 ) ) != NULL && event->is_add && event->chunk == chunk_0     &&
            ( event = talloc_dynarr_get ( arr, 2 ) ) != NULL && event->is_add && event->chunk == chunk_00    &&
            ( event = talloc_dynarr_get ( arr, 3 ) ) != NULL && event->is_add && event->chunk == chunk_01    &&
            ( event = talloc_dynarr_get ( arr, 4 ) ) != NULL && event->is_add && event->chunk == chunk_02    &&
            ( event = talloc_dynarr_get ( arr, 5 ) ) != NULL && event->is_add && event->chunk == chunk_010   &&
            ( event = talloc_dynarr_get ( arr, 6 ) ) != NULL && event->is_add && event->chunk == chunk_011   &&
            ( event = talloc_dynarr_get ( arr, 7 ) ) != NULL && event->is_add && event->chunk == chunk_012   &&
            ( event = talloc_dynarr_get ( arr, 8 ) ) != NULL && event->is_add && event->chunk == chunk_empty &&

            ( event = talloc_dynarr_get ( arr, 9 ) )  != NULL && !event->is_add && event->chunk == chunk_01    &&
            ( event = talloc_dynarr_get ( arr, 10 ) ) != NULL && !event->is_add && event->chunk == chunk_012   &&
            ( event = talloc_dynarr_get ( arr, 11 ) ) != NULL && !event->is_add && event->chunk == chunk_empty &&
            ( event = talloc_dynarr_get ( arr, 12 ) ) != NULL && !event->is_add && event->chunk == chunk_011   &&
            ( event = talloc_dynarr_get ( arr, 13 ) ) != NULL && !event->is_add && event->chunk == chunk_010   &&

            ( event = talloc_dynarr_get ( arr, 14 ) ) != NULL && !event->is_add && event->chunk == chunk_root &&
            ( event = talloc_dynarr_get ( arr, 15 ) ) != NULL && !event->is_add && event->chunk == chunk_0    &&
            ( event = talloc_dynarr_get ( arr, 16 ) ) != NULL && !event->is_add && event->chunk == chunk_02   &&
            ( event = talloc_dynarr_get ( arr, 17 ) ) != NULL && !event->is_add && event->chunk == chunk_00
        )
    ) {
        talloc_dynarr_free ( arr, true );
        return 7;
    }

    talloc_dynarr_free ( arr, true );
#endif

    return 0;
}



