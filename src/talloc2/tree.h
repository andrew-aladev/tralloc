// This file is part of talloc2. Copyright (C) 2013 Andrew Aladjev aladjev.andrew@gmail.com
// talloc2 is free software: you can redistribute it and/or modify it under the terms of the GNU General Lesser Public License as published by the Free Software Foundation, either version 3 of the License, or (at your option) any later version.
// talloc2 is distributed in the hope that it will be useful, but WITHOUT ANY WARRANTY; without even the implied warranty of MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU General Lesser Public License for more details.
// You should have received a copy of the GNU General Lesser Public License along with talloc2. If not, see <http://www.gnu.org/licenses/>.

#ifndef TALLOC_TREE_H
#define TALLOC_TREE_H

#include <stddef.h>
#include <stdint.h>

#include "types.h"

inline
void * talloc_memory_from_chunk ( talloc_chunk * chunk )
{
    return ( void * ) ( ( uintptr_t ) chunk - HEAD_CHUNK_OFFSET );
}

inline
void * talloc_chunk_from_memory ( void * memory )
{
    return ( void * ) ( ( uintptr_t ) memory + HEAD_CHUNK_OFFSET );
}

inline
void * talloc_data_from_chunk ( talloc_chunk * chunk )
{
    return ( void * ) ( ( uintptr_t ) chunk + sizeof ( talloc_chunk ) );
}

inline
talloc_chunk * talloc_chunk_from_data ( const void * data )
{
    return ( talloc_chunk * ) ( ( uintptr_t ) data - sizeof ( talloc_chunk ) );
}

void *  talloc ( const void * parent_data, size_t length );

inline
void * talloc_new ( const void * parent_data )
{
    return talloc ( parent_data, 0 );
}

void *  talloc_zero    ( const void * parent_data, size_t length );
void *  talloc_realloc ( const void * child_data, size_t length );
uint8_t talloc_move    ( const void * child_data, const void * parent_data );
uint8_t talloc_free    ( void * root_data );

#endif
