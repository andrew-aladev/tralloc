// This file is part of tralloc. Copyright (C) 2013 Andrew Aladjev aladjev.andrew@gmail.com
// tralloc is free software: you can redistribute it and/or modify it under the terms of the GNU General Lesser Public License as published by the Free Software Foundation, either version 3 of the License, or (at your option) any later version.
// tralloc is distributed in the hope that it will be useful, but WITHOUT ANY WARRANTY; without even the implied warranty of MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU General Lesser Public License for more details.
// You should have received a copy of the GNU General Lesser Public License along with tralloc. If not, see <http://www.gnu.org/licenses/>.

#ifndef TRALLOC_EXTENSIONS_CHUNK_H
#define TRALLOC_EXTENSIONS_CHUNK_H

#include "../chunk.h"

#if defined(TRALLOC_DEBUG)
#include "../events.h"
#endif

inline
tralloc_chunk * tralloc_extensions_process_new_chunk ( tralloc_extensions * extensions, tralloc_chunk * chunk, const tralloc_context * parent_context )
{

#if defined(TRALLOC_REFERENCE)
    chunk->mode = TRALLOC_MODE_EXTENSIONS;
    extensions->first_reference = NULL;
#endif

    return tralloc_usual_process_new_chunk ( chunk, parent_context );
}

inline
tralloc_chunk * tralloc_extensions_malloc_chunk ( const tralloc_context * parent_context, size_t length )
{
    tralloc_extensions * extensions = malloc ( sizeof ( tralloc_extensions ) + sizeof ( tralloc_chunk ) + length );
    if ( extensions == NULL ) {
        return NULL;
    }
    tralloc_chunk * chunk = tralloc_chunk_from_extensions ( extensions );

#if defined(TRALLOC_DEBUG)
    chunk->chunk_length = sizeof ( tralloc_extensions ) + sizeof ( tralloc_chunk );
    chunk->length       = length;
#endif

    return tralloc_extensions_process_new_chunk ( extensions, chunk, parent_context );
}

inline
tralloc_chunk * tralloc_extensions_calloc_chunk ( const tralloc_context * parent_context, size_t length )
{
    tralloc_extensions * extensions = calloc ( 1, sizeof ( tralloc_extensions ) + sizeof ( tralloc_chunk ) + length );
    if ( extensions == NULL ) {
        return NULL;
    }
    tralloc_chunk * chunk = tralloc_chunk_from_extensions ( extensions );

#if defined(TRALLOC_DEBUG)
    chunk->chunk_length = sizeof ( tralloc_extensions ) + sizeof ( tralloc_chunk );
    chunk->length       = length;
#endif

    return tralloc_extensions_process_new_chunk ( extensions, chunk, parent_context );
}

#if defined(TRALLOC_REFERENCE)
inline
void tralloc_extensions_update_reference ( tralloc_extensions * extensions )
{
    tralloc_reference * reference = extensions->first_reference;
    while ( reference != NULL ) {
        reference->parent_extensions = extensions;
        reference = reference->next;
    }
}
#endif

inline
tralloc_chunk * tralloc_extensions_realloc_chunk ( tralloc_chunk * chunk, size_t length )
{
    tralloc_extensions * old_extensions = tralloc_extensions_from_chunk ( chunk );
    tralloc_extensions * new_extensions = realloc ( old_extensions, sizeof ( tralloc_extensions ) + sizeof ( tralloc_chunk ) + length );
    if ( new_extensions == NULL ) {
        return NULL;
    }

    chunk = tralloc_chunk_from_extensions ( new_extensions );

#if defined(TRALLOC_DEBUG)
    chunk->length = length;
#endif

#if defined(TRALLOC_REFERENCE)
    if ( old_extensions != new_extensions ) {
        // now pointers to old_extensions is invalid
        // each pointer to old_extensions should be replaced with new_extensions
        tralloc_extensions_update_reference ( new_extensions );
    }
#endif

    return chunk;
}

inline
uint8_t tralloc_extensions_free_chunk ( tralloc_chunk * chunk )
{
    tralloc_extensions * extensions = tralloc_extensions_from_chunk ( chunk );

#if defined(TRALLOC_REFERENCE)
    if ( extensions->first_reference != NULL ) {
        tralloc_detach_chunk ( chunk );
        return 0;
    }
#endif

    uint8_t error = tralloc_usual_process_free_chunk ( chunk );
    free ( extensions );
    return error;
}

#endif
