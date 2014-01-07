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
    if (
        tralloc_new  ( NULL, NULL )    != TRALLOC_ERROR_CONTEXT_IS_NULL ||
        tralloc      ( NULL, NULL, 0 ) != TRALLOC_ERROR_CONTEXT_IS_NULL ||
        tralloc_zero ( NULL, NULL, 0 ) != TRALLOC_ERROR_CONTEXT_IS_NULL
    ) {
        return false;
    }

    if ( tralloc ( tr, ( tralloc_context ** ) &tr->data_1, sizeof ( uint8_t ) ) != 0 ) {
        return false;
    }
    * tr->data_1 = 123;

    if (
        tralloc_zero ( tr->data_1, ( tralloc_context ** ) &tr->data_2, sizeof ( uint16_t ) * 4 ) != 0 ||
        tr->data_2[0] != 0 || tr->data_2[1] != 0 || tr->data_2[2] != 0 || tr->data_2[3] != 0
    ) {
        return false;
    }
    tr->data_2[0] = 012;
    tr->data_2[1] = 345;
    tr->data_2[2] = 678;
    tr->data_2[3] = 901;

    if ( tralloc ( tr->data_1, ( tralloc_context ** ) &tr->data_3, sizeof ( char ) * 3 ) != 0 ) {
        return false;
    }
    tr->data_3[0] = 'q';
    tr->data_3[1] = 'w';
    tr->data_3[2] = 'e';

    if (
        tralloc_zero ( tr->data_1, ( tralloc_context ** ) &tr->data_4, sizeof ( uint32_t ) * 2 ) != 0 ||
        tr->data_4 == NULL || tr->data_4[0] != 0 || tr->data_4[1] != 0
    ) {
        return false;
    }
    tr->data_4[0] = 12345;
    tr->data_4[1] = 67890;

    if ( tralloc ( tr->data_3, ( tralloc_context ** ) &tr->data_5, sizeof ( size_t ) * 3 ) != 0 ) {
        return false;
    }
    tr->data_5[0] = 123456789;
    tr->data_5[1] = 987654321;
    tr->data_5[2] = 123456789;

    if (
        tralloc_zero ( tr->data_3, ( tralloc_context ** ) &tr->data_6, sizeof ( double ) ) != 0 ||
        * tr->data_6 != 0
    ) {
        return false;
    }
    * tr->data_6 = 0.0123456789;

    if ( tralloc ( tr->data_3, ( tralloc_context ** ) &tr->data_7, sizeof ( float ) * 2 ) != 0 ) {
        return false;
    }
    tr->data_7[0] = 0.01234;
    tr->data_7[1] = 0.56789;

#if defined(TRALLOC_REFERENCE)

    if (
        tralloc_with_extensions_new  ( NULL, NULL, 0 )    != TRALLOC_ERROR_CONTEXT_IS_NULL ||
        tralloc_with_extensions      ( NULL, NULL, 0, 0 ) != TRALLOC_ERROR_CONTEXT_IS_NULL ||
        tralloc_zero_with_extensions ( NULL, NULL, 0, 0 ) != TRALLOC_ERROR_CONTEXT_IS_NULL
    ) {
        return false;
    }

    if ( tralloc_with_extensions_new ( tr->data_7, ( tralloc_context ** ) &tr->trivium, TRALLOC_HAVE_REFERENCES ) != 0 ) {
        return false;
    }

    if ( tralloc_reference_new ( tr->trivium, tr->data_6, ( tralloc_context ** ) &tr->trivium_reference_1 ) != 0 ) {
        return false;
    }

    if ( tralloc_reference ( tr->trivium, tr->data_5, ( tralloc_context ** ) &tr->trivium_reference_2, sizeof ( uint16_t ) * 2 ) != 0 ) {
        return false;
    }
    tr->trivium_reference_2[0] = 3000;
    tr->trivium_reference_2[1] = 4000;

    if (
        tralloc_reference_zero ( tr->trivium, tr->trivium_reference_2, ( tralloc_context ** ) &tr->trivium_reference_3, sizeof ( uint32_t ) ) != 0 ||
        * tr->trivium_reference_3 != 0
    ) {
        return false;
    }
    * tr->trivium_reference_3 = 500000;

    if ( tralloc_with_extensions ( tr->trivium_reference_1, ( tralloc_context ** ) &tr->data_8, 0, sizeof ( int8_t ) * 2 ) != 0 ) {
        return false;
    }
    tr->data_8[0] = - 1;
    tr->data_8[1] = 2;

    if ( tralloc ( tr->data_8, ( tralloc_context ** ) &tr->data_9, sizeof ( int16_t ) * 3 ) != 0 ) {
        return false;
    }
    tr->data_9[0] = 4000;
    tr->data_9[1] = - 5000;
    tr->data_9[2] = 6000;

    if (
        tralloc_zero_with_extensions ( tr->data_8, ( tralloc_context ** ) &tr->data_10, sizeof ( int32_t ) * 2, 0 ) != 0 ||
        tr->data_10[0] != 0 || tr->data_10[1] != 0
    ) {
        return false;
    }
    tr->data_10[0] = - 123456;
    tr->data_10[1] = 789012;

#else

    if ( tralloc_new ( tr->data_7, ( tralloc_context ** ) &tr->trivium ) != 0 ) {
        return false;
    }

#endif

    return true;
}
