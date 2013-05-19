// This file is part of talloc. Copyright (C) 2013 Andrew Aladjev aladjev.andrew@gmail.com
// talloc is free software: you can redistribute it and/or modify it under the terms of the GNU General Lesser Public License as published by the Free Software Foundation, either version 3 of the License, or (at your option) any later version.
// talloc is distributed in the hope that it will be useful, but WITHOUT ANY WARRANTY; without even the implied warranty of MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU General Lesser Public License for more details.
// You should have received a copy of the GNU General Lesser Public License along with talloc. If not, see <http://www.gnu.org/licenses/>.

#ifndef TALLOC_TREE_H
#define TALLOC_TREE_H

#include <stddef.h>
#include <stdint.h>

#include "config.h"

typedef struct talloc_chunk_t {
    struct talloc_chunk_t * parent;
    struct talloc_chunk_t * first_child;
    struct talloc_chunk_t * prev;
    struct talloc_chunk_t * next;
#ifdef TALLOC_EXT
    struct talloc_ext_t * ext;
#endif
} talloc_chunk;

void *  talloc ( const void * parent_data, size_t length );
void *  talloc_zero ( const void * parent_data, size_t length );
void *  talloc_realloc ( const void * child_data, size_t length );
uint8_t talloc_free ( void * root_data );

inline
void * talloc_data_from_chunk ( talloc_chunk * chunk ) {
    return ( void * ) ( ( uintptr_t ) chunk + sizeof ( talloc_chunk ) );
}

inline
talloc_chunk * talloc_chunk_from_data ( const void * data ) {
    return ( talloc_chunk * ) ( ( uintptr_t ) data - sizeof ( talloc_chunk ) );
}

#ifdef TALLOC_DEBUG
// You can obtain full global history of talloc work
typedef void ( * talloc_callback ) ( talloc_chunk * chunk );
void talloc_set_callback ( talloc_callback on_add, talloc_callback on_update, talloc_callback on_del );
#endif

#endif
