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

#include <talloc/helpers.h>
#include <talloc/events.h>
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

static void     * root, * trivium;
static int8_t   * data_0;
static uint16_t * data_00;
static char     * data_01;
static uint32_t * data_02;
static size_t   * data_010, * data_011, * data_012;

talloc_chunk * chunk_root, * chunk_trivium,
             * chunk_0, * chunk_00, * chunk_01, * chunk_02,
             * chunk_010, * chunk_011, * chunk_012;

#ifdef TALLOC_DEBUG
static talloc_dynarr * history;

enum {
    ADD_MODE,
    UPDATE_MODE,
    DELETE_MODE
};

typedef struct talloc_event_t {
    talloc_chunk * chunk;
    uint8_t mode;
} talloc_event;

void on_add ( talloc_chunk * chunk )
{
    talloc_event * event = malloc ( sizeof ( talloc_event ) );
    event->mode  = ADD_MODE;
    event->chunk = chunk;
    talloc_dynarr_append ( history, event );
}
void on_update ( talloc_chunk * chunk )
{
    talloc_event * event = malloc ( sizeof ( talloc_event ) );
    event->mode  = UPDATE_MODE;
    event->chunk = chunk;
    talloc_dynarr_append ( history, event );
}
void on_del ( talloc_chunk * chunk )
{
    talloc_event * event = malloc ( sizeof ( talloc_event ) );
    event->mode  = DELETE_MODE;
    event->chunk = chunk;
    talloc_dynarr_append ( history, event );
}
#endif

void init ()
{
#ifdef TALLOC_DEBUG
    // all history will be available here
    history = talloc_dynarr_new ( 16 );
    talloc_set_callback ( on_add, on_update, on_del );
#endif
}

void alloc ()
{
    root     = talloc_new ( NULL );
    data_0   = talloc ( root,    sizeof ( int8_t ) );
    data_00  = talloc_zero ( data_0,  sizeof ( uint16_t ) );
    data_01  = talloc ( data_0,  sizeof ( char ) * 3 );
    data_02  = talloc_zero ( data_0,  sizeof ( uint32_t ) );
    data_010 = talloc ( data_01, sizeof ( size_t ) );
    data_011 = talloc_zero ( data_01, sizeof ( size_t ) );
    data_012 = talloc ( data_01, sizeof ( size_t ) );
    trivium  = talloc_new ( data_012 );
}

void free_data ()
{
    if ( root != NULL ) {
        talloc_free ( root );
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
    data_01     = talloc_realloc ( data_01, sizeof ( char ) * 4 );
    // realloc can change chunk pointer
    chunk_01    = talloc_chunk_from_data ( data_01 );

    if (
        ! (
            null       == NULL     &&
            data_01    != NULL     &&
            data_01[0] == CHAR_MAX &&
            data_01[1] == CHAR_MAX &&
            data_01[2] == CHAR_MAX
        )
    ) {
        return false;
    }

    data_01[3] = CHAR_MAX;
    data_01    = talloc_realloc ( data_01, sizeof ( char ) * 2 );
    chunk_01   = talloc_chunk_from_data ( data_01 );

    if (
        ! (
            data_01    != NULL     &&
            data_01[0] == CHAR_MAX &&
            data_01[1] == CHAR_MAX
        )
    ) {
        return false;
    }

    data_01  = talloc_realloc ( data_01, sizeof ( char ) * 5 );
    chunk_01 = talloc_chunk_from_data ( data_01 );

    if (
        ! (
            data_01    != NULL     &&
            data_01[0] == CHAR_MAX &&
            data_01[1] == CHAR_MAX
        )
    ) {
        return false;
    }

    return true;
}

void set_data ()
{
    *data_0    = INT8_MAX;
    *data_00   = UINT16_MAX;
    data_01[0] = CHAR_MAX;
    data_01[1] = CHAR_MAX;
    data_01[2] = CHAR_MAX;
    data_01[3] = CHAR_MAX;
    data_01[4] = CHAR_MAX;
    *data_02   = UINT32_MAX;
    *data_010  = SIZE_MAX;
    *data_011  = SIZE_MAX;
    *data_012  = SIZE_MAX;
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
        ! (
            chunk_root    != NULL &&
            chunk_0       != NULL &&
            chunk_00      != NULL &&
            chunk_01      != NULL &&
            chunk_02      != NULL &&
            chunk_010     != NULL &&
            chunk_011     != NULL &&
            chunk_012     != NULL &&
            chunk_trivium != NULL
        )
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
            *data_0  == INT8_MAX   &&
            *data_00 == UINT16_MAX &&
            *data_02 == UINT32_MAX
        )
    ) {
        return false;
    }
    return true;
}

bool test_history()
{
#ifdef TALLOC_DEBUG
    talloc_event * event;
    if (
        ! (
            history->length == 21 &&
            ( event = talloc_dynarr_get ( history, 0 ) ) != NULL && event->mode == ADD_MODE && event->chunk == chunk_root    &&
            ( event = talloc_dynarr_get ( history, 1 ) ) != NULL && event->mode == ADD_MODE && event->chunk == chunk_0       &&
            ( event = talloc_dynarr_get ( history, 2 ) ) != NULL && event->mode == ADD_MODE && event->chunk == chunk_00      &&
            ( event = talloc_dynarr_get ( history, 3 ) ) != NULL && event->mode == ADD_MODE && event->chunk == chunk_01      &&
            ( event = talloc_dynarr_get ( history, 4 ) ) != NULL && event->mode == ADD_MODE && event->chunk == chunk_02      &&
            ( event = talloc_dynarr_get ( history, 5 ) ) != NULL && event->mode == ADD_MODE && event->chunk == chunk_010     &&
            ( event = talloc_dynarr_get ( history, 6 ) ) != NULL && event->mode == ADD_MODE && event->chunk == chunk_011     &&
            ( event = talloc_dynarr_get ( history, 7 ) ) != NULL && event->mode == ADD_MODE && event->chunk == chunk_012     &&
            ( event = talloc_dynarr_get ( history, 8 ) ) != NULL && event->mode == ADD_MODE && event->chunk == chunk_trivium &&

            ( event = talloc_dynarr_get ( history, 9 ) )  != NULL && event->mode == UPDATE_MODE && event->chunk == chunk_01 &&
            ( event = talloc_dynarr_get ( history, 10 ) ) != NULL && event->mode == UPDATE_MODE && event->chunk == chunk_01 &&
            ( event = talloc_dynarr_get ( history, 11 ) ) != NULL && event->mode == UPDATE_MODE && event->chunk == chunk_01 &&

            ( event = talloc_dynarr_get ( history, 12 ) ) != NULL && event->mode == DELETE_MODE && event->chunk == chunk_01      &&
            ( event = talloc_dynarr_get ( history, 13 ) ) != NULL && event->mode == DELETE_MODE && event->chunk == chunk_012     &&
            ( event = talloc_dynarr_get ( history, 14 ) ) != NULL && event->mode == DELETE_MODE && event->chunk == chunk_trivium &&
            ( event = talloc_dynarr_get ( history, 15 ) ) != NULL && event->mode == DELETE_MODE && event->chunk == chunk_011     &&
            ( event = talloc_dynarr_get ( history, 16 ) ) != NULL && event->mode == DELETE_MODE && event->chunk == chunk_010     &&

            ( event = talloc_dynarr_get ( history, 17 ) ) != NULL && event->mode == DELETE_MODE && event->chunk == chunk_root &&
            ( event = talloc_dynarr_get ( history, 18 ) ) != NULL && event->mode == DELETE_MODE && event->chunk == chunk_0    &&
            ( event = talloc_dynarr_get ( history, 19 ) ) != NULL && event->mode == DELETE_MODE && event->chunk == chunk_02   &&
            ( event = talloc_dynarr_get ( history, 20 ) ) != NULL && event->mode == DELETE_MODE && event->chunk == chunk_00
        )
    ) {
        return false;
    }
#endif
    return true;
}

int main ()
{
    init();
    alloc();

    if ( !test_alloc() ) {
        fprintf ( stderr, "%s\n", "test_alloc failed" );
        free_data();
        return 1;
    }
    if ( !test_realloc() ) {
        fprintf ( stderr, "%s\n", "test_realloc failed" );
        free_data();
        return 2;
    }

    set_data();
    set_chunks();

    if ( !test_chunks() ) {
        fprintf ( stderr, "%s\n", "test_chunks failed" );
        free_data();
        return 3;
    }

    talloc_free ( data_01 );

    if ( !test_chunks_without_data_01() ) {
        fprintf ( stderr, "%s\n", "test_chunks_without_data_01 failed" );
        free_data();
        return 4;
    }

    if ( !test_data_without_data_01() ) {
        fprintf ( stderr, "%s\n", "test_data_without_data_01 failed" );
        free_data();
        return 5;
    }

    talloc_free ( root );
    root = NULL;

    if ( !test_history() ) {
        fprintf ( stderr, "%s\n", "test_history failed" );
        free_data();
        return 6;
    }

    return 0;
}
