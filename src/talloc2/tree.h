// This file is part of talloc2. Copyright (C) 2013 Andrew Aladjev aladjev.andrew@gmail.com
// talloc2 is free software: you can redistribute it and/or modify it under the terms of the GNU General Lesser Public License as published by the Free Software Foundation, either version 3 of the License, or (at your option) any later version.
// talloc2 is distributed in the hope that it will be useful, but WITHOUT ANY WARRANTY; without even the implied warranty of MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU General Lesser Public License for more details.
// You should have received a copy of the GNU General Lesser Public License along with talloc2. If not, see <http://www.gnu.org/licenses/>.

#ifndef TALLOC_TREE_H
#define TALLOC_TREE_H

#include "chunk.h"

#ifdef TALLOC_EXT
#include "ext/chunk.h"
#endif

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

uint8_t talloc_add ( const void * parent_data, talloc_chunk * child );

inline
void * talloc ( const void * parent_data, size_t length )
{

    talloc_chunk * chunk;
#ifdef TALLOC_EXT
    chunk = talloc_ext_chunk_malloc ( length );
#else
    chunk = talloc_usual_chunk_malloc ( length );
#endif

    if ( talloc_add ( parent_data, chunk ) != 0 ) {
#ifdef TALLOC_EXT
        talloc_ext_chunk_free ( chunk );
#else
        talloc_usual_chunk_free ( chunk );
#endif
        return NULL;
    }

    return talloc_data_from_chunk ( chunk );
}

inline
void * talloc_zero ( const void * parent_data, size_t length )
{

    talloc_chunk * chunk;
#ifdef TALLOC_EXT
    chunk = talloc_ext_chunk_calloc ( length );
#else
    chunk = talloc_usual_chunk_calloc ( length );
#endif

    if ( talloc_add ( parent_data, chunk ) != 0 ) {
#ifdef TALLOC_EXT
        talloc_ext_chunk_free ( chunk );
#else
        talloc_usual_chunk_free ( chunk );
#endif
        return NULL;
    }

    return talloc_data_from_chunk ( chunk );
}

inline
void * talloc_new ( const void * parent_data )
{
    return talloc ( parent_data, 0 );
}

void *  talloc_realloc ( const void * child_data, size_t length );
uint8_t talloc_move    ( const void * child_data, const void * parent_data );
uint8_t talloc_free    ( void * root_data );

#endif
