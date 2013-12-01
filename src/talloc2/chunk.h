// This file is part of talloc2. Copyright (C) 2013 Andrew Aladjev aladjev.andrew@gmail.com
// talloc2 is free software: you can redistribute it and/or modify it under the terms of the GNU General Lesser Public License as published by the Free Software Foundation, either version 3 of the License, or (at your option) any later version.
// talloc2 is distributed in the hope that it will be useful, but WITHOUT ANY WARRANTY; without even the implied warranty of MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU General Lesser Public License for more details.
// You should have received a copy of the GNU General Lesser Public License along with talloc2. If not, see <http://www.gnu.org/licenses/>.

#ifndef TALLOC_CHUNK_H
#define TALLOC_CHUNK_H

#include "tree.h"

#if defined(TALLOC_DEBUG)
#include "events.h"
#endif

#include <stdlib.h>

inline
talloc_chunk * talloc_usual_malloc_chunk ( const void * parent_data, size_t length )
{
    talloc_chunk * chunk = ( talloc_chunk * ) malloc ( sizeof ( talloc_chunk ) + length );
    if ( chunk == NULL ) {
        return NULL;
    }

#if defined(TALLOC_DEBUG)
    chunk->chunk_length = sizeof ( talloc_chunk );
    chunk->length       = length;
#endif

    talloc_add_chunk ( parent_data, chunk );
    return chunk;
}

inline
talloc_chunk * talloc_usual_calloc_chunk ( const void * parent_data, size_t length )
{
    talloc_chunk * chunk = ( talloc_chunk * ) calloc ( 1, sizeof ( talloc_chunk ) + length );
    if ( chunk == NULL ) {
        return NULL;
    }

#if defined(TALLOC_DEBUG)
    chunk->chunk_length = sizeof ( talloc_chunk );
    chunk->length       = length;
#endif

    talloc_add_chunk ( parent_data, chunk );
    return chunk;
}

inline
talloc_chunk * talloc_usual_realloc_chunk ( talloc_chunk * chunk, size_t length )
{
    talloc_chunk * new_chunk = ( talloc_chunk * ) realloc ( chunk, sizeof ( talloc_chunk ) + length );
    if ( new_chunk == NULL ) {
        return NULL;
    }

#if defined(TALLOC_DEBUG)
    chunk->length = length;
#endif

    return new_chunk;
}

inline
uint8_t talloc_usual_free_chunk ( talloc_chunk * chunk )
{
    uint8_t result, error = 0;

#if defined(TALLOC_DEBUG)
    if ( ( result = talloc_on_free ( chunk ) ) != 0 ) {
        error = result;
    }
#endif

    if ( ( result = talloc_free_chunk_children ( chunk ) ) != 0 ) {
        error = result;
    }

    free ( chunk );
    return error;
}

#endif