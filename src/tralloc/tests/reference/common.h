// This file is part of tralloc. Copyright (C) 2013 Andrew Aladjev aladjev.andrew@gmail.com
// tralloc is free software: you can redistribute it and/or modify it under the terms of the GNU General Public License as published by the Free Software Foundation, either version 3 of the License, or (at your option) any later version.
// tralloc is distributed in the hope that it will be useful, but WITHOUT ANY WARRANTY; without even the implied warranty of MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU General Public License for more details.
// You should have received a copy of the GNU General Public License along with tralloc. If not, see <http://www.gnu.org/licenses/>.

#ifndef TRALLOC_TESTS_REFERENCE_COMMON_H
#define TRALLOC_TESTS_REFERENCE_COMMON_H

#include <tralloc/types.h>


typedef struct test_reference_tree_type {
    int8_t *          common;
    float *           shared;
    uint8_t *         data_1;
    uint8_t *         data_2;
    char *            data_3;
    int *             common_1;
    tralloc_context * common_2;
    size_t *          common_3;
    tralloc_context * shared_1;
    float *           shared_2;
} test_reference_tree;

tralloc_bool test_reference_errors          ( tralloc_context * ctx );
tralloc_bool test_reference_add             ( test_reference_tree * tr );
tralloc_bool test_reference_move_and_resize ( test_reference_tree * tr );
tralloc_bool test_reference_free_subtree    ( test_reference_tree * tr );

int test_reference ( test_reference_tree * ctx );


#endif
