// This file is part of talloc2. Copyright (C) 2013 Andrew Aladjev aladjev.andrew@gmail.com
// talloc2 is free software: you can redistribute it and/or modify it under the terms of the GNU General Public License as published by the Free Software Foundation, either version 3 of the License, or (at your option) any later version.
// talloc2 is distributed in the hope that it will be useful, but WITHOUT ANY WARRANTY; without even the implied warranty of MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU General Public License for more details.
// You should have received a copy of the GNU General Public License along with talloc2. If not, see <http://www.gnu.org/licenses/>.

#include <talloc2/tree.h>

#if defined(TALLOC_DEBUG)
#include <talloc2/events.h>
#endif

#include <stdbool.h>
#include <math.h>

static void * root;
static talloc_chunk * root_chunk;

static int8_t * data_0;
static talloc_chunk * data_0_chunk;

static uint16_t * data_00;
static char *     data_01;
static uint32_t * data_02;
static talloc_chunk * data_00_chunk;
static talloc_chunk * data_01_chunk;
static talloc_chunk * data_02_chunk;

static size_t * data_010;
static double * data_011;
static float  * data_012;
static talloc_chunk * data_010_chunk;
static talloc_chunk * data_011_chunk;
static talloc_chunk * data_012_chunk;

static void * trivium;
static talloc_chunk * trivium_chunk;

bool compare_float ( float a, float b )
{
    return fabs ( a - b ) < 0.000001;
}

bool compare_double ( double a, double b )
{
    return fabs ( a - b ) < 0.000000000001;
}

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

bool test_alloc()
{
    root = talloc_new ( NULL );
    if ( root == NULL ) {
        return false;
    }

    data_0 = talloc ( root, sizeof ( int8_t ) );
    if ( data_0 == NULL ) {
        return false;
    }
    * data_0 = - 123;

    data_00 = talloc_zero ( data_0, sizeof ( uint16_t ) * 4 );
    data_01 = talloc      ( data_0, sizeof ( char ) * 3 );
    data_02 = talloc_zero ( data_0, sizeof ( uint32_t ) * 2 );
    if (
        data_00 == NULL || data_00[0] != 0 || data_00[1] != 0 || data_00[2] != 0 || data_00[3] != 0 ||
        data_01 == NULL ||
        data_02 == NULL || data_02[0] != 0 || data_02[1] != 0
    ) {
        return false;
    }
    data_00[0] = 012;
    data_00[1] = 345;
    data_00[2] = 678;
    data_00[3] = 901;

    data_01[0] = 'q';
    data_01[1] = 'w';
    data_01[2] = 'e';

    data_02[0] = 12345;
    data_02[1] = 67890;

    data_010 = talloc      ( data_01, sizeof ( size_t ) * 3 );
    data_011 = talloc_zero ( data_01, sizeof ( double ) );
    data_012 = talloc      ( data_01, sizeof ( float ) * 2 );
    if (
        data_010 == NULL ||
        data_011 == NULL || * data_011 != 0 ||
        data_012 == NULL
    ) {
        return false;
    }
    data_010[0] = 123456789;
    data_010[1] = 987654321;
    data_010[2] = 123456789;

    * data_011 = 0.0123456789;

    data_012[0] = 0.01234;
    data_012[1] = 0.56789;

    trivium = talloc_new ( data_012 );
    if ( trivium == NULL ) {
        return false;
    }

    return true;
}

bool test_realloc ()
{
    void * null = talloc_realloc ( NULL, 1 );
    if ( null != NULL ) {
        return false;
    }

    data_0 = talloc_realloc ( data_0, sizeof ( int8_t ) * 2 );
    if ( data_0 == NULL || data_0[0] != - 123 ) {
        return false;
    }

    data_00 = talloc_realloc ( data_00, sizeof ( uint16_t ) * 20 );
    data_01 = talloc_realloc ( data_01, sizeof ( char ) );
    data_02 = talloc_realloc ( data_02, sizeof ( uint32_t ) * 30 );
    if (
        data_00 == NULL || data_00[0] != 012   || data_00[1] != 345 || data_00[2] != 678 || data_00[3] != 901 ||
        data_01 == NULL || * data_01 != 'q'    ||
        data_02 == NULL || data_02[0] != 12345 || data_02[1] != 67890
    ) {
        return false;
    }

    data_010 = talloc_realloc ( data_010, sizeof ( size_t ) * 2 );
    data_011 = talloc_realloc ( data_011, sizeof ( double ) * 10 );
    data_012 = talloc_realloc ( data_012, sizeof ( float ) );
    if (
        data_010 == NULL || data_010[0] != 123456789 || data_010[1] != 987654321 ||
        data_011 == NULL || !compare_double ( data_011[0], 0.0123456789 ) ||
        data_012 == NULL || !compare_float  ( data_012[0], 0.01234 )
    ) {
        return false;
    }

    root_chunk     = talloc_chunk_from_data ( root );
    data_0_chunk   = talloc_chunk_from_data ( data_0 );
    data_00_chunk  = talloc_chunk_from_data ( data_00 );
    data_01_chunk  = talloc_chunk_from_data ( data_01 );
    data_02_chunk  = talloc_chunk_from_data ( data_02 );
    data_010_chunk = talloc_chunk_from_data ( data_010 );
    data_011_chunk = talloc_chunk_from_data ( data_011 );
    data_012_chunk = talloc_chunk_from_data ( data_012 );
    trivium_chunk  = talloc_chunk_from_data ( trivium );

    return true;
}

/*
           root
            |
            0
           /|\
       01  02   00
      /|\
      012  011  010
       |
    trivium
*/

bool test_move ()
{
    if (
        talloc_move ( data_01, data_02 ) != 0  ||
        data_01_chunk->parent != data_02_chunk ||

        talloc_move ( data_01, NULL ) != 0 ||
        data_01_chunk->parent != NULL      ||

        talloc_move ( data_01, data_0 ) != 0 ||
        data_01_chunk->parent != data_0_chunk
    ) {
        return false;
    }

    return true;
}

bool test_chunks ()
{
    if (
        root_chunk->parent      != NULL         ||
        root_chunk->prev        != NULL         ||
        root_chunk->next        != NULL         ||
        root_chunk->first_child != data_0_chunk ||

        data_0_chunk->parent      != root_chunk    ||
        data_0_chunk->prev        != NULL          ||
        data_0_chunk->next        != NULL          ||
        data_0_chunk->first_child != data_01_chunk ||

        data_00_chunk->parent      != data_0_chunk  ||
        data_00_chunk->prev        != data_02_chunk ||
        data_00_chunk->next        != NULL          ||
        data_00_chunk->first_child != NULL          ||

        data_01_chunk->parent      != data_0_chunk   ||
        data_01_chunk->prev        != NULL           ||
        data_01_chunk->next        != data_02_chunk  ||
        data_01_chunk->first_child != data_012_chunk ||

        data_02_chunk->parent      != data_0_chunk  ||
        data_02_chunk->prev        != data_01_chunk ||
        data_02_chunk->next        != data_00_chunk ||
        data_02_chunk->first_child != NULL          ||

        data_010_chunk->parent      != data_01_chunk  ||
        data_010_chunk->prev        != data_011_chunk ||
        data_010_chunk->next        != NULL           ||
        data_010_chunk->first_child != NULL           ||

        data_011_chunk->parent      != data_01_chunk  ||
        data_011_chunk->prev        != data_012_chunk ||
        data_011_chunk->next        != data_010_chunk ||
        data_011_chunk->first_child != NULL           ||

        data_012_chunk->parent      != data_01_chunk  ||
        data_012_chunk->prev        != NULL           ||
        data_012_chunk->next        != data_011_chunk ||
        data_012_chunk->first_child != trivium_chunk  ||

        trivium_chunk->parent      != data_012_chunk ||
        trivium_chunk->prev        != NULL           ||
        trivium_chunk->next        != NULL           ||
        trivium_chunk->first_child != NULL
    ) {
        return false;
    }
    return true;
}

/*
           root
            |
            0
           / \
         02   00
*/

bool test_chunks_without_data_01 ()
{
    if (
        root_chunk->parent      != NULL         ||
        root_chunk->prev        != NULL         ||
        root_chunk->next        != NULL         ||
        root_chunk->first_child != data_0_chunk ||

        data_0_chunk->parent      != root_chunk    ||
        data_0_chunk->prev        != NULL          ||
        data_0_chunk->next        != NULL          ||
        data_0_chunk->first_child != data_02_chunk ||

        data_00_chunk->parent      != data_0_chunk  ||
        data_00_chunk->prev        != data_02_chunk ||
        data_00_chunk->next        != NULL          ||
        data_00_chunk->first_child != NULL          ||

        data_02_chunk->parent      != data_0_chunk  ||
        data_02_chunk->prev        != NULL          ||
        data_02_chunk->next        != data_00_chunk ||
        data_02_chunk->first_child != NULL
    ) {
        return false;
    }
    return true;
}

int main ()
{
    if ( !test_alloc() ) {
        talloc_free ( root );
        return 1;
    }
    if ( !test_realloc() ) {
        talloc_free ( root );
        return 2;
    }
    if ( !test_move() ) {
        talloc_free ( root );
        return 3;
    }
    if ( !test_chunks() ) {
        talloc_free ( root );
        return 4;
    }
    if ( talloc_free ( data_01 ) != 0 ) {
        talloc_free ( root );
        return 5;
    }
    if ( !test_chunks_without_data_01() ) {
        talloc_free ( root );
        return 6;
    }

    if ( talloc_free ( root ) != 0 ) {
        return 7;
    }

#if defined(TALLOC_DEBUG)
    if (
        talloc_get_objects_count()        != 0 ||
        talloc_get_objects_chunk_length() != 0 ||
        talloc_get_objects_length()       != 0
    ) {
        return 8;
    }
#endif

    return 0;
}
