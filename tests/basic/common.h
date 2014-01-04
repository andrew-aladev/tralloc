// This file is part of tralloc. Copyright (C) 2013 Andrew Aladjev aladjev.andrew@gmail.com
// tralloc is free software: you can redistribute it and/or modify it under the terms of the GNU General Public License as published by the Free Software Foundation, either version 3 of the License, or (at your option) any later version.
// tralloc is distributed in the hope that it will be useful, but WITHOUT ANY WARRANTY; without even the implied warranty of MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU General Public License for more details.
// You should have received a copy of the GNU General Public License along with tralloc. If not, see <http://www.gnu.org/licenses/>.

#ifndef TRALLOC_TESTS_BASIC_COMMON
#define TRALLOC_TESTS_BASIC_COMMON

#include <tralloc/types.h>
#include <stdbool.h>


typedef struct tree_t {
    uint8_t *  data_1;
    uint16_t * data_2;
    char *     data_3;
    uint32_t * data_4;
    size_t *   data_5;
    double *   data_6;
    float  *   data_7;

    void * trivium;

#if defined(TRALLOC_REFERENCE)
    void *     trivium_reference_1;
    uint16_t * trivium_reference_2;
    uint32_t * trivium_reference_3;

    int8_t *  data_8;
    int16_t * data_9;
    int32_t * data_10;
#endif

} tree;

bool test_add          ( tree * tr );
bool test_resize       ( tree * tr );
bool test_move         ( tree * tr );
bool test_free_subtree ( tree * tr );


#endif
