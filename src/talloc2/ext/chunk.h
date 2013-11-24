// This file is part of talloc2. Copyright (C) 2013 Andrew Aladjev aladjev.andrew@gmail.com
// talloc2 is free software: you can redistribute it and/or modify it under the terms of the GNU General Lesser Public License as published by the Free Software Foundation, either version 3 of the License, or (at your option) any later version.
// talloc2 is distributed in the hope that it will be useful, but WITHOUT ANY WARRANTY; without even the implied warranty of MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU General Lesser Public License for more details.
// You should have received a copy of the GNU General Lesser Public License along with talloc2. If not, see <http://www.gnu.org/licenses/>.

#ifndef TALLOC_EXT_CHUNK_H
#define TALLOC_EXT_CHUNK_H

#include "destructor.h"

inline
talloc_chunk * talloc_ext_chunk_from_memory ( void * memory )
{
    return ( talloc_chunk * ) ( ( uintptr_t ) memory + sizeof ( talloc_ext ) );
}

inline
void * talloc_memory_from_ext_chunk ( talloc_chunk * chunk )
{
    return ( void * ) ( ( uintptr_t ) chunk - sizeof ( talloc_ext ) );
}

inline
talloc_chunk * talloc_ext_chunk_malloc ( size_t length )
{
    talloc_ext * ext = malloc ( sizeof ( talloc_ext ) + sizeof ( talloc_chunk ) + length );
    if ( ext == NULL ) {
        return NULL;
    } else {
        ext->first_destructor_item = NULL;
        talloc_chunk * chunk       = talloc_ext_chunk_from_memory ( ext );
        chunk->mode                = TALLOC_MODE_EXT;
        return chunk;
    }
}

inline
talloc_chunk * talloc_ext_chunk_calloc ( size_t length )
{
    talloc_ext * ext = calloc ( 1, sizeof ( talloc_ext ) + sizeof ( talloc_chunk ) + length );
    if ( ext == NULL ) {
        return NULL;
    } else {
        ext->first_destructor_item = NULL;
        talloc_chunk * chunk       = talloc_ext_chunk_from_memory ( ext );
        chunk->mode                = TALLOC_MODE_EXT;
        return chunk;
    }
}

inline
talloc_chunk * talloc_ext_chunk_realloc ( talloc_chunk * chunk, size_t length )
{
    void * memory = talloc_memory_from_ext_chunk ( chunk );
    memory        = realloc ( memory, sizeof ( talloc_ext ) + sizeof ( talloc_chunk ) + length );
    if ( memory == NULL ) {
        return NULL;
    } else {
        return talloc_ext_chunk_from_memory ( memory );
    }
}

inline
bool talloc_ext_chunk_free ( talloc_chunk * chunk )
{
    talloc_ext * ext = talloc_memory_from_ext_chunk ( chunk );
    bool result      = talloc_destructor_free ( chunk, ext->first_destructor_item );
    free ( ext );
    return result;
}

#endif
