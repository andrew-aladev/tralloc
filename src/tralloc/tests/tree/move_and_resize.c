// This file is part of tralloc. Copyright (C) 2013 Andrew Aladjev aladjev.andrew@gmail.com
// tralloc is free software: you can redistribute it and/or modify it under the terms of the GNU General Public License as published by the Free Software Foundation, either version 3 of the License, or (at your option) any later version.
// tralloc is distributed in the hope that it will be useful, but WITHOUT ANY WARRANTY; without even the implied warranty of MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU General Public License for more details.
// You should have received a copy of the GNU General Public License along with tralloc. If not, see <http://www.gnu.org/licenses/>.

#include <tralloc/tests/common/main.h>
#include <tralloc/tests/tree/common.h>
#include <tralloc/tree.h>


tralloc_bool test_tree_move_and_resize ( test_tree_data * tr )
{
    if (
        tralloc_realloc ( ( tralloc_context ** ) &tr->data_1, sizeof ( uint8_t ) * 15 ) != 0 ||
        tr->data_1[0] != 123 ||

        tralloc_realloc ( ( tralloc_context ** ) &tr->data_2, sizeof ( uint8_t ) * 20 ) != 0 ||
        tr->data_2[0] != 012 || tr->data_2[1] != 34 || tr->data_2[2] != 56 || tr->data_2[3] != 78 ||

        tralloc_realloc ( ( tralloc_context ** ) &tr->data_3, sizeof ( char ) ) != 0 ||
        * tr->data_3 != 'q' ||

        tralloc_realloc ( ( tralloc_context ** ) &tr->data_4, sizeof ( int ) * 30 ) != 0 ||
        tr->data_4[0] != -102 || tr->data_4[1] != 103 ||

        tralloc_realloc ( ( tralloc_context ** ) &tr->data_5, sizeof ( size_t ) * 2 ) != 0 ||
        tr->data_5[0] != 123 || tr->data_5[1] != 213 ||

        tralloc_realloc ( ( tralloc_context ** ) &tr->data_6, sizeof ( float ) * 10 ) != 0 ||
        !compare_float ( tr->data_6[0], 0.012345 ) ||

        tralloc_realloc ( ( tralloc_context ** ) &tr->data_7, sizeof ( float ) ) != 0 ||
        !compare_float ( tr->data_7[0], 0.01234 )
    ) {
        return TRALLOC_FALSE;
    }
    return TRALLOC_TRUE;
}
