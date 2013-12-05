// This file is part of talloc2. Copyright (C) 2013 Andrew Aladjev aladjev.andrew@gmail.com
// talloc2 is free software: you can redistribute it and/or modify it under the terms of the GNU General Public License as published by the Free Software Foundation, either version 3 of the License, or (at your option) any later version.
// talloc2 is distributed in the hope that it will be useful, but WITHOUT ANY WARRANTY; without even the implied warranty of MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU General Public License for more details.
// You should have received a copy of the GNU General Public License along with talloc2. If not, see <http://www.gnu.org/licenses/>.

#include <talloc2/tree.h>

#if defined(TALLOC_DEBUG)
#include <talloc2/events.h>
#endif

#if defined(TALLOC_REFERENCE)
#include <talloc2/reference/main.h>
#endif

#include <stdbool.h>
#include <math.h>

static void *         root;
static talloc_chunk * root_chunk;

static uint8_t *      data_1;
static talloc_chunk * data_1_chunk;

static uint16_t *     data_2;
static char *         data_3;
static uint32_t *     data_4;
static talloc_chunk * data_2_chunk;
static talloc_chunk * data_3_chunk;
static talloc_chunk * data_4_chunk;

static size_t *       data_5;
static double *       data_6;
static float  *       data_7;
static talloc_chunk * data_5_chunk;
static talloc_chunk * data_6_chunk;
static talloc_chunk * data_7_chunk;

static void *         trivium;
static talloc_chunk * trivium_chunk;

#if defined(TALLOC_REFERENCE)
static void **        trivium_reference_1;
static talloc_chunk * trivium_reference_1_chunk;

static void **        trivium_reference_2;
static talloc_chunk * trivium_reference_2_chunk;

static int8_t *       data_8;
static int16_t *      data_9;
static int32_t *      data_10;
static talloc_chunk * data_8_chunk;
static talloc_chunk * data_9_chunk;
static talloc_chunk * data_10_chunk;
#endif

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
            1
           /|\
       4    3    2
           /|\
   7        6        5
   |        |        |
trivium trivium_1 trivium_2
            |
            8
           /|\
         10   9
*/

bool test_alloc()
{
    root = talloc_new ( NULL );
    if ( root == NULL ) {
        return false;
    }

    data_1 = talloc ( root, sizeof ( uint8_t ) );
    if ( data_1 == NULL ) {
        return false;
    }
    * data_1 = 123;

    data_2 = talloc_zero ( data_1, sizeof ( uint16_t ) * 4 );
    data_3 = talloc      ( data_1, sizeof ( char ) * 3 );
    data_4 = talloc_zero ( data_1, sizeof ( uint32_t ) * 2 );
    if (
        data_2 == NULL || data_2[0] != 0 || data_2[1] != 0 || data_2[2] != 0 || data_2[3] != 0 ||
        data_3 == NULL ||
        data_4 == NULL || data_4[0] != 0 || data_4[1] != 0
    ) {
        return false;
    }
    data_2[0] = 012;
    data_2[1] = 345;
    data_2[2] = 678;
    data_2[3] = 901;

    data_3[0] = 'q';
    data_3[1] = 'w';
    data_3[2] = 'e';

    data_4[0] = 12345;
    data_4[1] = 67890;

    data_5 = talloc      ( data_3, sizeof ( size_t ) * 3 );
    data_6 = talloc_zero ( data_3, sizeof ( double ) );
    data_7 = talloc      ( data_3, sizeof ( float ) * 2 );
    if (
        data_5 == NULL ||
        data_6 == NULL || * data_6 != 0 ||
        data_7 == NULL
    ) {
        return false;
    }
    data_5[0] = 123456789;
    data_5[1] = 987654321;
    data_5[2] = 123456789;

    * data_6 = 0.0123456789;

    data_7[0] = 0.01234;
    data_7[1] = 0.56789;

    trivium = talloc_new ( data_7 );
    if ( trivium == NULL ) {
        return false;
    }

#if defined(TALLOC_REFERENCE)
    trivium_reference_1 = talloc_add_reference ( trivium, data_6 );
    trivium_reference_2 = talloc_add_reference ( trivium, data_5 );
    if ( trivium_reference_1 == NULL || trivium_reference_2 == NULL ) {
        return false;
    }

    data_8  = talloc      ( trivium_reference_1, sizeof ( int8_t ) * 2 );
    data_9  = talloc      ( data_8, sizeof ( int16_t ) * 3 );
    data_10 = talloc_zero ( data_8, sizeof ( int32_t ) * 2 );
    if (
        data_8  == NULL ||
        data_9  == NULL ||
        data_10 == NULL || data_10[0] != 0 || data_10[1] != 0
    ) {
        return false;
    }
    data_8[0] = - 1;
    data_8[1] = 2;

    data_9[0] = 4000;
    data_9[1] = - 5000;
    data_9[2] = 6000;

    data_10[0] = - 123456;
    data_10[1] = 789012;
#endif

    return true;
}

bool test_realloc ()
{
    void * null = talloc_realloc ( NULL, 1 );
    if ( null != NULL ) {
        return false;
    }

    data_1 = talloc_realloc ( data_1, sizeof ( uint8_t ) * 2 );
    if ( data_1 == NULL || data_1[0] != 123 ) {
        return false;
    }

    data_2 = talloc_realloc ( data_2, sizeof ( uint16_t ) * 20 );
    data_3 = talloc_realloc ( data_3, sizeof ( char ) );
    data_4 = talloc_realloc ( data_4, sizeof ( uint32_t ) * 30 );
    if (
        data_2 == NULL || data_2[0] != 012   || data_2[1] != 345 || data_2[2] != 678 || data_2[3] != 901 ||
        data_3 == NULL || * data_3 != 'q'    ||
        data_4 == NULL || data_4[0] != 12345 || data_4[1] != 67890
    ) {
        return false;
    }

    data_5 = talloc_realloc ( data_5, sizeof ( size_t ) * 2 );
    data_6 = talloc_realloc ( data_6, sizeof ( double ) * 10 );
    data_7 = talloc_realloc ( data_7, sizeof ( float ) );
    if (
        data_5 == NULL || data_5[0] != 123456789 || data_5[1] != 987654321 ||
        data_6 == NULL || !compare_double ( data_6[0], 0.0123456789 ) ||
        data_7 == NULL || !compare_float  ( data_7[0], 0.01234 )
    ) {
        return false;
    }

#if defined(TALLOC_REFERENCE)
    if (
        talloc_realloc ( trivium_reference_1, sizeof ( float ) * 2 )  != NULL ||
        talloc_realloc ( trivium_reference_1, sizeof ( size_t ) * 3 ) != NULL
    ) {
        return false;
    }

    data_8  = talloc_realloc ( data_8,  sizeof ( int8_t ) );
    data_9  = talloc_realloc ( data_9,  sizeof ( int16_t ) * 2 );
    data_10 = talloc_realloc ( data_10, sizeof ( int32_t ) );
    if (
        data_8  == NULL || data_8[0]  != - 1    ||
        data_9  == NULL || data_9[0]  != 4000   || data_9[1] != - 5000 ||
        data_10 == NULL || data_10[0] != - 123456
    ) {
        return false;
    }
#endif

    root_chunk    = talloc_chunk_from_data ( root );
    data_1_chunk  = talloc_chunk_from_data ( data_1 );
    data_2_chunk  = talloc_chunk_from_data ( data_2 );
    data_3_chunk  = talloc_chunk_from_data ( data_3 );
    data_4_chunk  = talloc_chunk_from_data ( data_4 );
    data_5_chunk  = talloc_chunk_from_data ( data_5 );
    data_6_chunk  = talloc_chunk_from_data ( data_6 );
    data_7_chunk  = talloc_chunk_from_data ( data_7 );
    trivium_chunk = talloc_chunk_from_data ( trivium );

#if defined(TALLOC_REFERENCE)
    trivium_reference_1_chunk = talloc_chunk_from_data ( trivium_reference_1 );
    trivium_reference_2_chunk = talloc_chunk_from_data ( trivium_reference_2 );
    data_8_chunk              = talloc_chunk_from_data ( data_8 );
    data_9_chunk              = talloc_chunk_from_data ( data_9 );
    data_10_chunk             = talloc_chunk_from_data ( data_10 );
#endif

    return true;
}

bool test_move ()
{
    if (
        talloc_move ( data_3, data_4 ) != 0            ||
        data_3_chunk->parent           != data_4_chunk ||

        talloc_move ( data_3, data_4 ) == 0 ||

        talloc_move ( data_3, NULL ) != 0    ||
        data_3_chunk->parent         != NULL ||

        talloc_move ( data_3, NULL ) == 0 ||

        talloc_move ( data_3, data_1 ) != 0 ||
        data_3_chunk->parent           != data_1_chunk
    ) {
        return false;
    }

#if defined(TALLOC_REFERENCE)
    if (
        talloc_move ( trivium_reference_1, data_5 ) != 0 ||
        trivium_reference_1_chunk->parent           != data_5_chunk ||

        talloc_move ( trivium_reference_1, NULL ) == 0 ||
        talloc_move ( trivium_reference_2, NULL ) == 0 ||

        talloc_move ( trivium_reference_2, data_5 ) == 0
    ) {
        return false;
    }
#endif

    return true;
}

/*
           root
            |
            1
           /|\
       3    4    2
      /|\
   7        6        5
   |                 |\
trivium          trivium_1 trivium_2
                     |
                     8
                    /|\
                  10   9
*/

bool test_chunks ()
{
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

        data_5_chunk->parent      != data_3_chunk ||
        data_5_chunk->prev        != data_6_chunk ||
        data_5_chunk->next        != NULL         ||

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
        return false;
    }

#if defined(TALLOC_REFERENCE)
    if (
        data_5_chunk->first_child != trivium_reference_1_chunk ||

        trivium_reference_1_chunk->parent      != data_5_chunk              ||
        trivium_reference_1_chunk->prev        != NULL                      ||
        trivium_reference_1_chunk->next        != trivium_reference_2_chunk ||
        trivium_reference_1_chunk->first_child != data_8_chunk              ||

        trivium_reference_2_chunk->parent      != data_5_chunk              ||
        trivium_reference_2_chunk->prev        != trivium_reference_1_chunk ||
        trivium_reference_2_chunk->next        != NULL                      ||
        trivium_reference_2_chunk->first_child != NULL                      ||

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
    if ( data_5_chunk->first_child != NULL ) {
        return false;
    }
#endif

    return true;
}

/*
           root               trivium
            |
            1
           /|\
       3    4    2
      /|\
   7        6        5
                     |
                  trivium_2
*/

#if defined(TALLOC_REFERENCE)
bool test_chunks_without_trivium_and_first_reference ()
{
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

        data_5_chunk->parent      != data_3_chunk              ||
        data_5_chunk->prev        != data_6_chunk              ||
        data_5_chunk->next        != NULL                      ||
        data_5_chunk->first_child != trivium_reference_2_chunk ||

        data_6_chunk->parent      != data_3_chunk ||
        data_6_chunk->prev        != data_7_chunk ||
        data_6_chunk->next        != data_5_chunk ||
        data_6_chunk->first_child != NULL         ||

        data_7_chunk->parent      != data_3_chunk ||
        data_7_chunk->prev        != NULL         ||
        data_7_chunk->next        != data_6_chunk ||
        data_7_chunk->first_child != NULL         ||

        trivium_chunk->parent      != NULL ||
        trivium_chunk->prev        != NULL ||
        trivium_chunk->next        != NULL ||
        trivium_chunk->first_child != NULL ||
        
        trivium_reference_2_chunk->parent      != data_5_chunk ||
        trivium_reference_2_chunk->prev        != NULL         ||
        trivium_reference_2_chunk->next        != NULL         ||
        trivium_reference_2_chunk->first_child != NULL
    ) {
        return false;
    }
    return true;
}
#endif

/*
           root
            |
            1
           / \
         4    2
*/

bool test_chunks_without_data_3 ()
{
    if (
        root_chunk->parent      != NULL         ||
        root_chunk->prev        != NULL         ||
        root_chunk->next        != NULL         ||
        root_chunk->first_child != data_1_chunk ||

        data_1_chunk->parent      != root_chunk   ||
        data_1_chunk->prev        != NULL         ||
        data_1_chunk->next        != NULL         ||
        data_1_chunk->first_child != data_4_chunk ||

        data_2_chunk->parent      != data_1_chunk ||
        data_2_chunk->prev        != data_4_chunk ||
        data_2_chunk->next        != NULL         ||
        data_2_chunk->first_child != NULL         ||

        data_4_chunk->parent      != data_1_chunk  ||
        data_4_chunk->prev        != NULL          ||
        data_4_chunk->next        != data_2_chunk ||
        data_4_chunk->first_child != NULL
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

#if defined(TALLOC_REFERENCE)
    if (
        talloc_free ( trivium )             != 0 ||
        talloc_free ( trivium_reference_1 ) != 0
    ) {
        talloc_free ( root );
        return 5;
    }
    if ( !test_chunks_without_trivium_and_first_reference() ) {
        talloc_free ( root );
        return 6;
    }
#endif

    if ( talloc_free ( data_3 ) != 0 ) {
        talloc_free ( root );
        return 7;
    }
    if ( !test_chunks_without_data_3() ) {
        talloc_free ( root );
        return 8;
    }

    if ( talloc_free ( root ) != 0 ) {
        return 9;
    }

#if defined(TALLOC_DEBUG)
    if (
        talloc_get_objects_count()        != 0 ||
        talloc_get_objects_chunk_length() != 0 ||
        talloc_get_objects_length()       != 0
    ) {
        return 10;
    }
#endif

    return 0;
}
