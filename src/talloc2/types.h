// This file is part of talloc2. Copyright (C) 2013 Andrew Aladjev aladjev.andrew@gmail.com
// talloc2 is free software: you can redistribute it and/or modify it under the terms of the GNU General Lesser Public License as published by the Free Software Foundation, either version 3 of the License, or (at your option) any later version.
// talloc2 is distributed in the hope that it will be useful, but WITHOUT ANY WARRANTY; without even the implied warranty of MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU General Lesser Public License for more details.
// You should have received a copy of the GNU General Lesser Public License along with talloc2. If not, see <http://www.gnu.org/licenses/>.

#ifndef TALLOC_TYPES_H
#define TALLOC_TYPES_H

#include <stdint.h>
#include <stddef.h>

#include "config.h"

#ifdef TALLOC_EXT_DESTRUCTOR
typedef uint8_t ( * talloc_destructor ) ( void * child_data, void * user_data );

typedef struct talloc_destructor_item_t {
    struct talloc_destructor_item_t * next;
    talloc_destructor destructor;
    void * user_data;
} talloc_destructor_item;
#endif

#ifdef TALLOC_REFERENCE
typedef struct talloc_reference_t {
    struct talloc_ext_t * parent;
    struct talloc_reference_t * prev;
    struct talloc_reference_t * next;
} talloc_reference;
#endif

#ifdef TALLOC_EXT
typedef struct talloc_ext_t {

#ifdef TALLOC_EXT_DESTRUCTOR
    talloc_destructor_item * first_destructor_item;
#endif

#ifdef TALLOC_REFERENCE
    talloc_reference * first_reference_item;
#endif

} talloc_ext;
#endif

#ifdef TALLOC_MODE
enum {

#ifdef TALLOC_EXT
    TALLOC_MODE_EXT = 0,
#else
    TALLOC_MODE_USUAL = 0,
#endif

#ifdef TALLOC_REFERENCE
    TALLOC_MODE_REFERENCE
#endif

};
#endif

typedef struct talloc_chunk_t {
    struct talloc_chunk_t * parent;
    struct talloc_chunk_t * first_child;
    struct talloc_chunk_t * prev;
    struct talloc_chunk_t * next;

#ifdef TALLOC_MODE
    uint8_t mode;
#endif

} talloc_chunk;

#endif