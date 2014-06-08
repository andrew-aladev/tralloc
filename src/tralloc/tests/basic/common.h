// This file is part of tralloc. Copyright (C) 2013 Andrew Aladjev aladjev.andrew@gmail.com
// tralloc is free software: you can redistribute it and/or modify it under the terms of the GNU General Public License as published by the Free Software Foundation, either version 3 of the License, or (at your option) any later version.
// tralloc is distributed in the hope that it will be useful, but WITHOUT ANY WARRANTY; without even the implied warranty of MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU General Public License for more details.
// You should have received a copy of the GNU General Public License along with tralloc. If not, see <http://www.gnu.org/licenses/>.

#ifndef TRALLOC_TESTS_BASIC_COMMON
#define TRALLOC_TESTS_BASIC_COMMON

#include <tralloc/types.h>


typedef struct test_basic_tree_type {
    uint8_t *         data_1;
    uint8_t *         data_2;
    char *            data_3;
    int *             data_4;
    size_t *          data_5;
    float *           data_6;
    float  *          data_7;
    tralloc_context * trivium;
} test_basic_tree;

tralloc_bool test_basic_errors          ( tralloc_context * ctx );
tralloc_bool test_basic_add             ( test_basic_tree * tr );
tralloc_bool test_basic_move_and_resize ( test_basic_tree * tr );
tralloc_bool test_basic_free_subtree    ( test_basic_tree * tr );

int test_basic ( tralloc_context * ctx );


#endif
