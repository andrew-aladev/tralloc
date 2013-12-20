// This file is part of tralloc. Copyright (C) 2013 Andrew Aladjev aladjev.andrew@gmail.com
// tralloc is free software: you can redistribute it and/or modify it under the terms of the GNU General Lesser Public License as published by the Free Software Foundation, either version 3 of the License, or (at your option) any later version.
// tralloc is distributed in the hope that it will be useful, but WITHOUT ANY WARRANTY; without even the implied warranty of MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU General Lesser Public License for more details.
// You should have received a copy of the GNU General Lesser Public License along with tralloc. If not, see <http://www.gnu.org/licenses/>.

#ifndef TRALLOC_CHUNK_H
#define TRALLOC_CHUNK_H

#include "tree.h"

#if defined(TRALLOC_DEBUG)
#include "events.h"
#endif

#if defined(TRALLOC_DESTRUCTOR)
#include "destructor.h"
#endif

#include <stdlib.h>

inline
tralloc_chunk * tralloc_usual_process_new_chunk ( tralloc_chunk * chunk, const tralloc_context * parent_context )
{

#if defined(TRALLOC_DESTRUCTOR)
    chunk->first_destructor = NULL;
#endif

    tralloc_add_chunk ( parent_context, chunk );
    return chunk;
}

inline
tralloc_chunk * tralloc_usual_malloc_chunk ( const tralloc_context * parent_context, size_t length )
{
    tralloc_chunk * chunk = ( tralloc_chunk * ) malloc ( sizeof ( tralloc_chunk ) + length );
    if ( chunk == NULL ) {
        return NULL;
    }

#if defined(TRALLOC_DEBUG)
    chunk->chunk_length = sizeof ( tralloc_chunk );
    chunk->length       = length;
#endif

    return tralloc_usual_process_new_chunk ( chunk, parent_context );
}

inline
tralloc_chunk * tralloc_usual_calloc_chunk ( const tralloc_context * parent_context, size_t length )
{
    tralloc_chunk * chunk = ( tralloc_chunk * ) calloc ( 1, sizeof ( tralloc_chunk ) + length );
    if ( chunk == NULL ) {
        return NULL;
    }

#if defined(TRALLOC_DEBUG)
    chunk->chunk_length = sizeof ( tralloc_chunk );
    chunk->length       = length;
#endif

    return tralloc_usual_process_new_chunk ( chunk, parent_context );
}

inline
tralloc_chunk * tralloc_usual_realloc_chunk ( tralloc_chunk * chunk, size_t length )
{
    tralloc_chunk * new_chunk = ( tralloc_chunk * ) realloc ( chunk, sizeof ( tralloc_chunk ) + length );
    if ( new_chunk == NULL ) {
        return NULL;
    }

#if defined(TRALLOC_DEBUG)
    new_chunk->length = length;
#endif

    return new_chunk;
}

inline
uint8_t tralloc_usual_process_free_chunk ( tralloc_chunk * chunk )
{
    uint8_t result, error = 0;

#if defined(TRALLOC_DEBUG)
    if ( ( result = tralloc_on_free ( chunk ) ) != 0 ) {
        error = result;
    }
#endif

#if defined(TRALLOC_DESTRUCTOR)
    if ( ( result = tralloc_destructor_free ( chunk ) ) != 0 ) {
        error = result;
    }
#endif

    if ( ( result = tralloc_free_chunk_children ( chunk ) ) != 0 ) {
        error = result;
    }

    return error;
}

inline
uint8_t tralloc_usual_free_chunk ( tralloc_chunk * chunk )
{
    uint8_t error = tralloc_usual_process_free_chunk ( chunk );
    free ( chunk );
    return error;
}

#endif