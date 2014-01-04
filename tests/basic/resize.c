// This file is part of tralloc. Copyright (C) 2013 Andrew Aladjev aladjev.andrew@gmail.com
// tralloc is free software: you can redistribute it and/or modify it under the terms of the GNU General Public License as published by the Free Software Foundation, either version 3 of the License, or (at your option) any later version.
// tralloc is distributed in the hope that it will be useful, but WITHOUT ANY WARRANTY; without even the implied warranty of MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU General Public License for more details.
// You should have received a copy of the GNU General Public License along with tralloc. If not, see <http://www.gnu.org/licenses/>.

#include "common.h"
#include <tralloc/tree.h>
#include <math.h>


bool compare_float ( float a, float b )
{
    return fabs ( a - b ) < 0.000001;
}

bool compare_double ( double a, double b )
{
    return fabs ( a - b ) < 0.000000000001;
}

bool test_resize ( tree * tr )
{
    uint8_t * data_1 = tralloc_realloc ( tr->data_1, sizeof ( uint8_t ) * 15 );
    if ( data_1 == NULL || data_1[0] != 123 ) {
        return false;
    }
    tr->data_1 = data_1;

    uint16_t * data_2 = tralloc_realloc ( tr->data_2, sizeof ( uint16_t ) * 20 );
    if ( data_2 == NULL || data_2[0] != 012 || data_2[1] != 345 || data_2[2] != 678 || data_2[3] != 901 ) {
        return false;
    }
    tr->data_2 = data_2;

    char * data_3 = tralloc_realloc ( tr->data_3, sizeof ( char ) );
    if ( data_3 == NULL || * data_3 != 'q' ) {
        return false;
    }
    tr->data_3 = data_3;

    uint32_t * data_4 = tralloc_realloc ( tr->data_4, sizeof ( uint32_t ) * 30 );
    if ( data_4 == NULL || data_4[0] != 12345 || data_4[1] != 67890 ) {
        return false;
    }
    tr->data_4 = data_4;

    size_t * data_5 = tralloc_realloc ( tr->data_5, sizeof ( size_t ) * 2 );
    if ( data_5 == NULL || data_5[0] != 123456789 || data_5[1] != 987654321 ) {
        return false;
    }
    tr->data_5 = data_5;

    double * data_6 = tralloc_realloc ( tr->data_6, sizeof ( double ) * 10 );
    if ( data_6 == NULL || !compare_double ( data_6[0], 0.0123456789 ) ) {
        return false;
    }
    tr->data_6 = data_6;

    float * data_7 = tralloc_realloc ( tr->data_7, sizeof ( float ) );
    if ( data_7 == NULL || !compare_float ( data_7[0], 0.01234 ) ) {
        return false;
    }
    tr->data_7 = data_7;

#if defined(TRALLOC_REFERENCE)

    float * trivium_reference_1 = tralloc_realloc ( tr->trivium_reference_1, sizeof ( float ) * 20 );
    if ( trivium_reference_1 == NULL ) {
        return false;
    }
    tr->trivium_reference_1 = trivium_reference_1;

    uint16_t * trivium_reference_2 = tralloc_realloc ( tr->trivium_reference_2, sizeof ( uint16_t ) );
    if ( trivium_reference_2 == NULL || * trivium_reference_2 != 3000 ) {
        return false;
    }
    tr->trivium_reference_2 = trivium_reference_2;

    uint32_t * trivium_reference_3 = tralloc_realloc ( tr->trivium_reference_3, sizeof ( uint32_t ) * 10 );
    if ( trivium_reference_3 == NULL || trivium_reference_3[0] != 500000 ) {
        return false;
    }
    tr->trivium_reference_3 = trivium_reference_3;

    int8_t * data_8 = tralloc_realloc ( tr->data_8, sizeof ( int8_t ) );
    if ( data_8 == NULL || data_8[0] != - 1 ) {
        return false;
    }
    tr->data_8 = data_8;

    int16_t * data_9 = tralloc_realloc ( tr->data_9, sizeof ( int16_t ) * 2 );
    if ( data_9 == NULL || data_9[0] != 4000 || data_9[1] != - 5000 ) {
        return false;
    }
    tr->data_9 = data_9;

    int32_t * data_10 = tralloc_realloc ( tr->data_10, sizeof ( int32_t ) );
    if ( data_10 == NULL || data_10[0] != - 123456 ) {
        return false;
    }
    tr->data_10 = data_10;

#endif

    return true;
}
