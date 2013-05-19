// This file is part of talloc. Copyright (C) 2013 Andrew Aladjev aladjev.andrew@gmail.com
// talloc is free software: you can redistribute it and/or modify it under the terms of the GNU General Lesser Public License as published by the Free Software Foundation, either version 3 of the License, or (at your option) any later version.
// talloc is distributed in the hope that it will be useful, but WITHOUT ANY WARRANTY; without even the implied warranty of MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU General Lesser Public License for more details.
// You should have received a copy of the GNU General Lesser Public License along with talloc. If not, see <http://www.gnu.org/licenses/>.

#ifndef TALLOC_TYPES_H
#define TALLOC_TYPES_H

#include "config.h"

#ifdef TALLOC_EXT_DESTRUCTOR
typedef void ( * talloc_destructor ) ( void * parent_data );
#endif

#ifdef TALLOC_EXT

typedef struct talloc_ext_t {
#ifdef TALLOC_EXT_DESTRUCTOR
    talloc_destructor destructor;
#endif
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
