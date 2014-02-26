// This file is part of tralloc. Copyright (C) 2013 Andrew Aladjev aladjev.andrew@gmail.com
// tralloc is free software: you can redistribute it and/or modify it under the terms of the GNU General Public License as published by the Free Software Foundation, either version 3 of the License, or (at your option) any later version.
// tralloc is distributed in the hope that it will be useful, but WITHOUT ANY WARRANTY; without even the implied warranty of MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU General Public License for more details.
// You should have received a copy of the GNU General Public License along with tralloc. If not, see <http://www.gnu.org/licenses/>.

#ifndef TRALLOC_TESTS_REFERENCE_COMMON
#define TRALLOC_TESTS_REFERENCE_COMMON

#include <tralloc/types.h>


typedef struct tree_type {
    uint32_t *        common;
    float *           shared;
    uint8_t *         data_1;
    uint16_t *        data_2;
    char *            data_3;
    int *             common_1;
    tralloc_context * common_2;
    uint8_t *         common_3;
    tralloc_context * shared_1;
    double *          shared_2;
} tree;

tralloc_bool test_errors          ( tralloc_context * ctx );
tralloc_bool test_add             ( tree * tr );
tralloc_bool test_move_and_resize ( tree * tr );
tralloc_bool test_free_subtree    ( tree * tr );


#endif
