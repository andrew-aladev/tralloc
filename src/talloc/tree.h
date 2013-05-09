// This file is part of talloc. Copyright (C) 2013 Andrew Aladjev aladjev.andrew@gmail.com
// talloc is free software: you can redistribute it and/or modify it under the terms of the GNU General Lesser Public License as published by the Free Software Foundation, either version 3 of the License, or (at your option) any later version.
// talloc is distributed in the hope that it will be useful, but WITHOUT ANY WARRANTY; without even the implied warranty of MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU General Lesser Public License for more details.
// You should have received a copy of the GNU General Lesser Public License along with talloc. If not, see <http://www.gnu.org/licenses/>.

#ifndef TALLOC_TREE_H
#define TALLOC_TREE_H

#include <stddef.h>
#include <stdint.h>

// typedef uint8_t ( * talloc_destructor ) ( uintptr_t data );

// typedef struct talloc_extensions_t {
//     talloc_destructor destructor;
// } talloc_extensions;

// only pointers here. this struct is aligned by sizeof(uintptr_t)
typedef struct talloc_chunk_t {
    struct talloc_chunk_t * parent;
    struct talloc_chunk_t * first_child;
    struct talloc_chunk_t * prev;
    struct talloc_chunk_t * next;
    //talloc_extensions * extensions;
} talloc_chunk;

void *         talloc_data_from_chunk ( talloc_chunk * chunk );
talloc_chunk * talloc_chunk_from_data ( const void * data );

void *  talloc_new ( const void * parent_data );
void *  talloc ( const void * parent_data, size_t length );
uint8_t talloc_free ( void * root_data );

#ifdef DEBUG
typedef void ( * talloc_callback ) ( talloc_chunk * chunk );
void talloc_set_callback ( talloc_callback on_add, talloc_callback on_del );
#endif

#endif
