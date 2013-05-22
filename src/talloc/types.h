// This file is part of talloc. Copyright (C) 2013 Andrew Aladjev aladjev.andrew@gmail.com
// talloc is free software: you can redistribute it and/or modify it under the terms of the GNU General Lesser Public License as published by the Free Software Foundation, either version 3 of the License, or (at your option) any later version.
// talloc is distributed in the hope that it will be useful, but WITHOUT ANY WARRANTY; without even the implied warranty of MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU General Lesser Public License for more details.
// You should have received a copy of the GNU General Lesser Public License along with talloc. If not, see <http://www.gnu.org/licenses/>.

#ifndef TALLOC_TYPES_H
#define TALLOC_TYPES_H

#include <stdint.h>

#include "config.h"

#ifdef TALLOC_EXT
typedef void ( * talloc_destructor ) ( void * parent_data );

// these are indexes in array, which will be reallocated
// should be sorted by typical usage times. less index - more usage
enum {
    TALLOC_EXT_DESTRUCTOR = 0
};

typedef struct talloc_ext_t {
    void ** data;
    uint8_t length;
} talloc_ext;
#endif

typedef struct talloc_chunk_t {
    struct talloc_chunk_t * parent;
    struct talloc_chunk_t * first_child;
    struct talloc_chunk_t * prev;
    struct talloc_chunk_t * next;
#ifdef TALLOC_EXT
    talloc_ext * ext;
#endif
} talloc_chunk;

#endif
