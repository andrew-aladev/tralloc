// This file is part of talloc2. Copyright (C) 2013 Andrew Aladjev aladjev.andrew@gmail.com
// talloc2 is free software: you can redistribute it and/or modify it under the terms of the GNU General Public License as published by the Free Software Foundation, either version 3 of the License, or (at your option) any later version.
// talloc2 is distributed in the hope that it will be useful, but WITHOUT ANY WARRANTY; without even the implied warranty of MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU General Public License for more details.
// You should have received a copy of the GNU General Public License along with talloc2. If not, see <http://www.gnu.org/licenses/>.

#include <stdlib.h>
#include <stdio.h>
#include <stdint.h>
#include <string.h>
#include <stdbool.h>
#include <limits.h>

#include <talloc2/tree.h>
#include <talloc2/events.h>
#include "utils/dynarr.h"

/*
           root
            |
            0
           /|\
       02  01   00
           /|\
      012  011  010
       |
    trivium
*/

static void     * root     = NULL;
static void     * trivium  = NULL;
static int8_t   * data_0   = NULL;
static uint16_t * data_00  = NULL;
static char     * data_01  = NULL;
static uint32_t * data_02  = NULL;
static size_t   * data_010 = NULL;
static size_t   * data_011 = NULL;
static size_t   * data_012 = NULL;

talloc_chunk * chunk_root    = NULL;
talloc_chunk * chunk_trivium = NULL;
talloc_chunk * chunk_0       = NULL;
talloc_chunk * chunk_00      = NULL;
talloc_chunk * chunk_01      = NULL;
talloc_chunk * chunk_02      = NULL;
talloc_chunk * chunk_010     = NULL;
talloc_chunk * chunk_011     = NULL;
talloc_chunk * chunk_012     = NULL;

#ifdef TALLOC_DEBUG
static talloc_dynarr * history;

enum {
    ADD_MODE = 0,
    UPDATE_MODE,
    MOVE_MODE,
    DELETE_MODE
};

typedef struct talloc_event_t {
    talloc_chunk * chunk;
    uint8_t mode;
} talloc_event;

uint8_t history_append ( talloc_chunk * chunk, uint8_t mode )
{
    talloc_event * event = malloc ( sizeof ( talloc_event ) );
    if ( event == NULL ) {
        return 1;
    }
    event->mode  = mode;
    event->chunk = chunk;
    if ( talloc_dynarr_append ( history, event ) != 0 ) {
        return 2;
    }
    return 0;
}

uint8_t on_add ( talloc_chunk * chunk )
{
    return history_append ( chunk, ADD_MODE );
}
uint8_t on_update ( talloc_chunk * chunk )
{
    return history_append ( chunk, UPDATE_MODE );
}
uint8_t on_move ( talloc_chunk * chunk )
{
    return history_append ( chunk, MOVE_MODE );
}
uint8_t on_del ( talloc_chunk * chunk )
{
    return history_append ( chunk, DELETE_MODE );
}
#endif

bool init ()
{
#ifdef TALLOC_DEBUG
    // all history will be available here
    history = talloc_dynarr_new ( 16 );
    if ( history == NULL ) {
        return false;
    }
    talloc_set_callback ( on_add, on_update, on_move, on_del );
#endif
    return true;
}

bool alloc ()
{
    root = talloc_new ( NULL );
    if ( root == NULL ) {
        return false;
    }
    data_0 = talloc ( root, sizeof ( int8_t ) );
    if ( data_0 == NULL ) {
        return false;
    }
    data_00 = talloc_zero ( data_0, sizeof ( uint16_t ) );
    data_01 = talloc      ( data_0, sizeof ( char ) * 3 );
    data_02 = talloc_zero ( data_0, sizeof ( uint32_t ) );
    if (
        data_00 == NULL ||
        data_01 == NULL ||
        data_02 == NULL
    ) {
        return false;
    }
    data_010 = talloc      ( data_01, sizeof ( size_t ) );
    data_011 = talloc_zero ( data_01, sizeof ( size_t ) );
    data_012 = talloc      ( data_01, sizeof ( size_t ) );
    if (
        data_010 == NULL ||
        data_011 == NULL ||
        data_012 == NULL
    ) {
        return false;
    }
    trivium = talloc_new ( data_012 );
    if ( trivium == NULL ) {
        return false;
    }
    return true;
}

bool free_data ()
{
    bool result = true;
    if ( talloc_free ( root ) != 0 ) {
        result = false;
    }
#ifdef TALLOC_DEBUG
    if ( history != NULL ) {
        size_t length = talloc_dynarr_get_length ( history );
        for ( size_t index = 0; index < length; index ++ ) {
            free ( talloc_dynarr_get ( history, index ) );
        }
        talloc_dynarr_free ( history );
    }
#endif
    return result;
}

bool test_alloc ()
{
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
            trivium  != NULL &&

            *data_00  == 0 &&
            *data_02  == 0 &&
            *data_011 == 0
        )
    ) {
        return false;
    }
    return true;
}

bool test_realloc ()
{
    data_01[0] = CHAR_MAX;
    data_01[1] = CHAR_MAX;
    data_01[2] = CHAR_MAX;

    void * null = talloc_realloc ( NULL, 1 );
    if ( null != NULL ) {
        return false;
    }
    data_01 = talloc_realloc ( data_01, sizeof ( char ) * 4 );
    if ( data_01 == NULL ) {
        return false;
    }
    // realloc can change chunk pointer
    chunk_01 = talloc_chunk_from_data ( data_01 );

    if (
        data_01[0] != CHAR_MAX ||
        data_01[1] != CHAR_MAX ||
        data_01[2] != CHAR_MAX
    ) {
        return false;
    }

    data_01[3] = CHAR_MAX;
    data_01    = talloc_realloc ( data_01, sizeof ( char ) * 2 );
    if ( data_01 == NULL ) {
        return false;
    }
    // realloc can change chunk pointer
    chunk_01 = talloc_chunk_from_data ( data_01 );

    if (
        data_01[0] != CHAR_MAX ||
        data_01[1] != CHAR_MAX
    ) {
        return false;
    }

    data_01 = talloc_realloc ( data_01, sizeof ( char ) * 5 );
    if ( data_01 == NULL ) {
        return false;
    }
    // realloc can change chunk pointer
    chunk_01 = talloc_chunk_from_data ( data_01 );

    if (
        data_01[0] != CHAR_MAX ||
        data_01[1] != CHAR_MAX
    ) {
        return false;
    }

    return true;
}

bool test_move ()
{
    if (
        talloc_move ( trivium, data_00 ) != 0 ||
        chunk_trivium->parent != chunk_00 ||

        talloc_move ( trivium, data_012 ) != 0 ||
        chunk_trivium->parent != chunk_012
    ) {
        return false;
    }

    return true;
}

void set_data ()
{
    * data_0    = INT8_MAX;
    * data_00   = UINT16_MAX;
    data_01[0] = CHAR_MAX;
    data_01[1] = CHAR_MAX;
    data_01[2] = CHAR_MAX;
    data_01[3] = CHAR_MAX;
    data_01[4] = CHAR_MAX;
    * data_02   = UINT32_MAX;
    * data_010  = SIZE_MAX;
    * data_011  = SIZE_MAX;
    * data_012  = SIZE_MAX;
}

void set_chunks ()
{
    chunk_root    = talloc_chunk_from_data ( root );
    chunk_0       = talloc_chunk_from_data ( data_0 );
    chunk_00      = talloc_chunk_from_data ( data_00 );
    chunk_01      = talloc_chunk_from_data ( data_01 );
    chunk_02      = talloc_chunk_from_data ( data_02 );
    chunk_010     = talloc_chunk_from_data ( data_010 );
    chunk_011     = talloc_chunk_from_data ( data_011 );
    chunk_012     = talloc_chunk_from_data ( data_012 );
    chunk_trivium = talloc_chunk_from_data ( trivium );
}

bool test_chunks ()
{
    if (
        chunk_root    == NULL ||
        chunk_0       == NULL ||
        chunk_00      == NULL ||
        chunk_01      == NULL ||
        chunk_02      == NULL ||
        chunk_010     == NULL ||
        chunk_011     == NULL ||
        chunk_012     == NULL ||
        chunk_trivium == NULL
    ) {
        return false;
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

            chunk_012->parent      == chunk_01      &&
            chunk_012->prev        == NULL          &&
            chunk_012->next        == chunk_011     &&
            chunk_012->first_child == chunk_trivium &&

            chunk_trivium->parent      == chunk_012 &&
            chunk_trivium->prev        == NULL      &&
            chunk_trivium->next        == NULL      &&
            chunk_trivium->first_child == NULL
        )
    ) {
        return false;
    }
    return true;
}

bool test_chunks_without_data_01 ()
{
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
        return false;
    }
    return true;
}

bool test_data_without_data_01 ()
{
    if (
        ! (
            * data_0  == INT8_MAX   &&
            * data_00 == UINT16_MAX &&
            * data_02 == UINT32_MAX
        )
    ) {
        return false;
    }
    return true;
}

#ifdef TALLOC_DEBUG
bool test_history_event ( uint8_t index, uint8_t mode, talloc_chunk * chunk )
{
    talloc_event * event = talloc_dynarr_get ( history, index );
    if ( event == NULL ) {
        return false;
    }
    return event->mode == mode && event->chunk == chunk;
}

bool test_history()
{
    if (
        ! (
            history->length == 23 &&
            test_history_event ( 0, ADD_MODE, chunk_root )    &&
            test_history_event ( 1, ADD_MODE, chunk_0 )       &&
            test_history_event ( 2, ADD_MODE, chunk_00 )      &&
            test_history_event ( 3, ADD_MODE, chunk_01 )      &&
            test_history_event ( 4, ADD_MODE, chunk_02 )      &&
            test_history_event ( 5, ADD_MODE, chunk_010 )     &&
            test_history_event ( 6, ADD_MODE, chunk_011 )     &&
            test_history_event ( 7, ADD_MODE, chunk_012 )     &&
            test_history_event ( 8, ADD_MODE, chunk_trivium ) &&

            test_history_event ( 9,  UPDATE_MODE, chunk_01 ) &&
            test_history_event ( 10, UPDATE_MODE, chunk_01 ) &&
            test_history_event ( 11, UPDATE_MODE, chunk_01 ) &&

            test_history_event ( 12, MOVE_MODE, chunk_trivium ) &&
            test_history_event ( 13, MOVE_MODE, chunk_trivium ) &&

            test_history_event ( 14, DELETE_MODE, chunk_01 )      &&
            test_history_event ( 15, DELETE_MODE, chunk_012 )     &&
            test_history_event ( 16, DELETE_MODE, chunk_trivium ) &&
            test_history_event ( 17, DELETE_MODE, chunk_011 )     &&
            test_history_event ( 18, DELETE_MODE, chunk_010 )     &&

            test_history_event ( 19, DELETE_MODE, chunk_root ) &&
            test_history_event ( 20, DELETE_MODE, chunk_0 )    &&
            test_history_event ( 21, DELETE_MODE, chunk_02 )   &&
            test_history_event ( 22, DELETE_MODE, chunk_00 )
        )
    ) {
        return false;
    }
    return true;
}
#endif

int main ()
{
    if ( !init() ) {
        free_data();
        return 1;
    }
    if ( !alloc() ) {
        free_data();
        return 2;
    }

    if ( !test_alloc() ) {
        free_data();
        return 3;
    }
    if ( !test_realloc() ) {
        free_data();
        return 4;
    }

    set_chunks();
    if ( !test_move() ) {
        free_data();
        return 5;
    }

    set_data();
    set_chunks();

    if ( !test_chunks() ) {
        free_data();
        return 6;
    }

    if ( talloc_free ( data_01 ) != 0 ) {
        return 7;
    }

    if ( !test_chunks_without_data_01() ) {
        free_data();
        return 8;
    }

    if ( !test_data_without_data_01() ) {
        free_data();
        return 9;
    }

    if ( talloc_free ( root ) != 0 ) {
        return 10;
    }
    root = NULL;

#ifdef TALLOC_DEBUG
    if ( !test_history() ) {
        free_data();
        return 11;
    }
#endif

    if ( !free_data() ) {
        return 12;
    }

    return 0;
}


