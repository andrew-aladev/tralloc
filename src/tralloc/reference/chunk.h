// This file is part of tralloc. Copyright (C) 2013 Andrew Aladjev aladjev.andrew@gmail.com
// tralloc is free software: you can redistribute it and/or modify it under the terms of the GNU General Lesser Public License as published by the Free Software Foundation, either version 3 of the License, or (at your option) any later version.
// tralloc is distributed in the hope that it will be useful, but WITHOUT ANY WARRANTY; without even the implied warranty of MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU General Lesser Public License for more details.
// You should have received a copy of the GNU General Lesser Public License along with tralloc. If not, see <http://www.gnu.org/licenses/>.

#ifndef TRALLOC_REFERENCE_CHUNK_H
#define TRALLOC_REFERENCE_CHUNK_H

#include "../tree.h"

#if defined(TRALLOC_DEBUG)
#include "../events.h"
#endif

#include <stdlib.h>

inline
tralloc_reference * tralloc_reference_process_new_chunk ( tralloc_reference * reference, const tralloc_context * parent_context, size_t length )
{
    tralloc_chunk * reference_chunk = tralloc_chunk_from_reference ( reference );
    reference_chunk->mode           = TRALLOC_MODE_REFERENCE;

#if defined(TRALLOC_DEBUG)
    reference_chunk->chunk_length = sizeof ( tralloc_reference ) + sizeof ( tralloc_chunk );
    reference_chunk->length       = length;
#endif

    tralloc_add_chunk ( parent_context, reference_chunk );

    return reference;
}

inline
tralloc_reference * tralloc_reference_malloc_chunk ( const tralloc_context * parent_context, size_t length )
{
    tralloc_reference * reference = malloc ( sizeof ( tralloc_reference ) + sizeof ( tralloc_chunk ) + length );
    if ( reference == NULL ) {
        return NULL;
    }
    return tralloc_reference_process_new_chunk ( reference, parent_context, length );
}

inline
tralloc_reference * tralloc_reference_calloc_chunk ( const tralloc_context * parent_context, size_t length )
{
    tralloc_reference * reference = calloc ( 1, sizeof ( tralloc_reference ) + sizeof ( tralloc_chunk ) + length );
    if ( reference == NULL ) {
        return NULL;
    }
    return tralloc_reference_process_new_chunk ( reference, parent_context, length );
}

inline
void tralloc_reference_update ( tralloc_reference * reference )
{
    tralloc_reference * prev = reference->prev;
    if ( prev == NULL ) {
        tralloc_extensions * extensions = reference->parent_extensions;
        extensions->first_reference     = reference;
    } else {
        prev->next = reference;
    }
    tralloc_reference * next = reference->next;
    if ( next != NULL ) {
        next->prev = reference;
    }
}

inline
tralloc_chunk * tralloc_reference_realloc_chunk ( tralloc_chunk * reference_chunk, size_t length )
{
    tralloc_reference * old_reference = tralloc_reference_from_chunk ( reference_chunk );
    tralloc_reference * new_reference = realloc ( old_reference, sizeof ( tralloc_reference ) + sizeof ( tralloc_chunk ) + length );
    if ( new_reference == NULL ) {
        return NULL;
    }

    reference_chunk = tralloc_chunk_from_reference ( new_reference );

#if defined(TRALLOC_DEBUG)
    reference_chunk->length = length;
#endif

    if ( old_reference != new_reference ) {
        // now pointers to old_reference is invalid
        // each pointer to old_reference should be replaced with new_reference
        tralloc_reference_update ( new_reference );
    }

    return reference_chunk;
}

inline
void tralloc_reference_update_extensions ( tralloc_extensions * extensions )
{
    tralloc_reference * reference = extensions->first_reference;
    while ( reference != NULL ) {
        reference->parent_extensions = extensions;
        reference = reference->next;
    }
}

uint8_t tralloc_reference_free_chunk ( tralloc_chunk * chunk );

#endif
