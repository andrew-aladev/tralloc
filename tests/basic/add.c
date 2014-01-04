// This file is part of tralloc. Copyright (C) 2013 Andrew Aladjev aladjev.andrew@gmail.com
// tralloc is free software: you can redistribute it and/or modify it under the terms of the GNU General Public License as published by the Free Software Foundation, either version 3 of the License, or (at your option) any later version.
// tralloc is distributed in the hope that it will be useful, but WITHOUT ANY WARRANTY; without even the implied warranty of MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU General Public License for more details.
// You should have received a copy of the GNU General Public License along with tralloc. If not, see <http://www.gnu.org/licenses/>.

#include "common.h"
#include <tralloc/tree.h>

#if defined(TRALLOC_REFERENCE)
#include <tralloc/reference/main.h>
#endif


/*
           tree
            |
            1
           /|\
       4    3    2
           /|\
   7        6        5
   |        |        |\
trivium trivium_1 trivium_3 trivium_2
            |
            8
           /|\
         10   9
*/

bool test_add ( tree * tr )
{
    uint8_t * data_1 = tralloc ( tr, sizeof ( uint8_t ) );
    if ( data_1 == NULL ) {
        return false;
    }
    * data_1   = 123;
    tr->data_1 = data_1;

    uint16_t * data_2 = tralloc_zero ( data_1, sizeof ( uint16_t ) * 4 );
    if ( data_2 == NULL || data_2[0] != 0 || data_2[1] != 0 || data_2[2] != 0 || data_2[3] != 0 ) {
        return false;
    }
    data_2[0]  = 012;
    data_2[1]  = 345;
    data_2[2]  = 678;
    data_2[3]  = 901;
    tr->data_2 = data_2;

    char * data_3 = tralloc ( data_1, sizeof ( char ) * 3 );
    if ( data_3 == NULL ) {
        return false;
    }
    data_3[0]  = 'q';
    data_3[1]  = 'w';
    data_3[2]  = 'e';
    tr->data_3 = data_3;

    uint32_t * data_4 = tralloc_zero ( data_1, sizeof ( uint32_t ) * 2 );
    if ( data_4 == NULL || data_4[0] != 0 || data_4[1] != 0 ) {
        return false;
    }
    data_4[0]  = 12345;
    data_4[1]  = 67890;
    tr->data_4 = data_4;

    size_t * data_5 = tralloc ( data_3, sizeof ( size_t ) * 3 );
    if ( data_5 == NULL ) {
        return false;
    }
    data_5[0]  = 123456789;
    data_5[1]  = 987654321;
    data_5[2]  = 123456789;
    tr->data_5 = data_5;

    double * data_6 = tralloc_zero ( data_3, sizeof ( double ) );
    if ( data_6 == NULL || * data_6 != 0 ) {
        return false;
    }
    * data_6   = 0.0123456789;
    tr->data_6 = data_6;

    float * data_7 = tralloc ( data_3, sizeof ( float ) * 2 );
    if ( data_7 == NULL ) {
        return false;
    }
    data_7[0]  = 0.01234;
    data_7[1]  = 0.56789;
    tr->data_7 = data_7;

#if defined(TRALLOC_REFERENCE)

    void * trivium = tralloc_with_extensions_new ( data_7, TRALLOC_HAVE_REFERENCES );
    if ( trivium == NULL ) {
        return false;
    }
    tr->trivium = trivium;

    void * trivium_reference_1 = tralloc_reference_new ( trivium, data_6 );
    if ( trivium_reference_1 == NULL ) {
        return false;
    }
    tr->trivium_reference_1 = trivium_reference_1;

    uint16_t * trivium_reference_2 = tralloc_reference ( trivium, data_5, sizeof ( uint16_t ) * 2 );
    if ( trivium_reference_2 == NULL ) {
        return false;
    }
    trivium_reference_2[0]  = 3000;
    trivium_reference_2[1]  = 4000;
    tr->trivium_reference_2 = trivium_reference_2;

    uint32_t * trivium_reference_3 = tralloc_reference_zero ( trivium, trivium_reference_2, sizeof ( uint32_t ) );
    if ( trivium_reference_3 == NULL || * trivium_reference_3 != 0 ) {
        return false;
    }
    * trivium_reference_3   = 500000;
    tr->trivium_reference_3 = trivium_reference_3;

    int8_t * data_8 = tralloc ( trivium_reference_1, sizeof ( int8_t ) * 2 );
    if ( data_8 == NULL ) {
        return false;
    }
    data_8[0]  = - 1;
    data_8[1]  = 2;
    tr->data_8 = data_8;

    int16_t * data_9 = tralloc ( data_8, sizeof ( int16_t ) * 3 );
    if ( data_9 == NULL ) {
        return false;
    }
    data_9[0]  = 4000;
    data_9[1]  = - 5000;
    data_9[2]  = 6000;
    tr->data_9 = data_9;

    int32_t * data_10 = tralloc_zero ( data_8, sizeof ( int32_t ) * 2 );
    if ( data_10 == NULL || data_10[0] != 0 || data_10[1] != 0 ) {
        return false;
    }
    data_10[0]  = - 123456;
    data_10[1]  = 789012;
    tr->data_10 = data_10;

#else

    void * trivium = tralloc_new ( data_7 );
    if ( trivium == NULL ) {
        return false;
    }
    tr->trivium = trivium;

#endif

    return true;
}
