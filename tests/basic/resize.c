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
    tralloc_context * empty = NULL;
    if (
        tralloc_realloc ( NULL, 0 )   != TRALLOC_ERROR_CONTEXT_IS_NULL ||
        tralloc_realloc ( &empty, 0 ) != TRALLOC_ERROR_CONTEXT_IS_NULL
    ) {
        return false;
    }

    if (
        tralloc_realloc ( ( tralloc_context ** ) &tr->data_1, sizeof ( uint8_t ) * 15 ) != 0 ||
        tr->data_1[0] != 123
    ) {
        return false;
    }

    if (
        tralloc_realloc ( ( tralloc_context ** ) &tr->data_2, sizeof ( uint16_t ) * 20 ) != 0 ||
        tr->data_2[0] != 012 || tr->data_2[1] != 345 || tr->data_2[2] != 678 || tr->data_2[3] != 901
    ) {
        return false;
    }

    if (
        tralloc_realloc ( ( tralloc_context ** ) &tr->data_3, sizeof ( char ) ) != 0 ||
        * tr->data_3 != 'q'
    ) {
        return false;
    }

    if (
        tralloc_realloc ( ( tralloc_context ** ) &tr->data_4, sizeof ( uint32_t ) * 30 ) != 0 ||
        tr->data_4[0] != 12345 || tr->data_4[1] != 67890
    ) {
        return false;
    }

    if (
        tralloc_realloc ( ( tralloc_context ** ) &tr->data_5, sizeof ( size_t ) * 2 ) != 0 ||
        tr->data_5[0] != 123456789 || tr->data_5[1] != 987654321
    ) {
        return false;
    }

    if (
        tralloc_realloc ( ( tralloc_context ** ) &tr->data_6, sizeof ( double ) * 10 ) != 0 ||
        !compare_double ( tr->data_6[0], 0.0123456789 )
    ) {
        return false;
    }

    if (
        tralloc_realloc ( ( tralloc_context ** ) &tr->data_7, sizeof ( float ) ) != 0 ||
        !compare_float ( tr->data_7[0], 0.01234 )
    ) {
        return false;
    }

#if defined(TRALLOC_REFERENCE)

    if ( tralloc_realloc ( ( tralloc_context ** ) &tr->trivium_reference_1, sizeof ( float ) * 20 ) != 0 ) {
        return false;
    }

    if (
        tralloc_realloc ( ( tralloc_context ** ) &tr->trivium_reference_2, sizeof ( uint16_t ) ) != 0 ||
        * tr->trivium_reference_2 != 3000
    ) {
        return false;
    }

    if (
        tralloc_realloc ( ( tralloc_context ** ) &tr->trivium_reference_3, sizeof ( uint32_t ) * 10 ) != 0 ||
        tr->trivium_reference_3[0] != 500000
    ) {
        return false;
    }

    if (
        tralloc_realloc ( ( tralloc_context ** ) &tr->data_8, sizeof ( int8_t ) ) != 0 ||
        tr->data_8[0] != - 1
    ) {
        return false;
    }

    if (
        tralloc_realloc ( ( tralloc_context ** ) &tr->data_9, sizeof ( int16_t ) * 2 ) != 0 ||
        tr->data_9[0] != 4000 || tr->data_9[1] != - 5000
    ) {
        return false;
    }

    if (
        tralloc_realloc ( ( tralloc_context ** ) &tr->data_10, sizeof ( int32_t ) ) != 0 ||
        tr->data_10[0] != - 123456
    ) {
        return false;
    }

#endif

    return true;
}
