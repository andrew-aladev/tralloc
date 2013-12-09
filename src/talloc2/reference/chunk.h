// This file is part of talloc2. Copyright (C) 2013 Andrew Aladjev aladjev.andrew@gmail.com
// talloc2 is free software: you can redistribute it and/or modify it under the terms of the GNU General Lesser Public License as published by the Free Software Foundation, either version 3 of the License, or (at your option) any later version.
// talloc2 is distributed in the hope that it will be useful, but WITHOUT ANY WARRANTY; without even the implied warranty of MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU General Lesser Public License for more details.
// You should have received a copy of the GNU General Lesser Public License along with talloc2. If not, see <http://www.gnu.org/licenses/>.

#ifndef TALLOC_REFERENCE_CHUNK_H
#define TALLOC_REFERENCE_CHUNK_H

#include "../tree.h"

#if defined(TALLOC_DEBUG)
#include "../events.h"
#endif

#include <stdlib.h>

inline
talloc_reference * talloc_reference_process_new_chunk ( talloc_reference * reference, const void * parent_data, size_t length )
{
    talloc_chunk * reference_chunk = talloc_chunk_from_reference ( reference );
    reference_chunk->mode          = TALLOC_MODE_REFERENCE;

#if defined(TALLOC_DEBUG)
    reference_chunk->chunk_length = sizeof ( talloc_reference ) + sizeof ( talloc_chunk );
    reference_chunk->length       = length;
#endif

    talloc_add_chunk ( parent_data, reference_chunk );

    return reference;
}

inline
talloc_reference * talloc_reference_malloc_chunk ( const void * parent_data, size_t length )
{
    talloc_reference * reference = malloc ( sizeof ( talloc_reference ) + sizeof ( talloc_chunk ) + length );
    if ( reference == NULL ) {
        return NULL;
    }
    return talloc_reference_process_new_chunk ( reference, parent_data, length );
}

inline
talloc_reference * talloc_reference_calloc_chunk ( const void * parent_data, size_t length )
{
    talloc_reference * reference = calloc ( 1, sizeof ( talloc_reference ) + sizeof ( talloc_chunk ) + length );
    if ( reference == NULL ) {
        return NULL;
    }
    return talloc_reference_process_new_chunk ( reference, parent_data, length );
}

inline
void talloc_reference_update ( talloc_reference * reference )
{
    talloc_reference * prev = reference->prev;
    if ( prev == NULL ) {
        talloc_extensions * extensions = reference->parent_extensions;
        extensions->first_reference    = reference;
    } else {
        prev->next = reference;
    }
    talloc_reference * next = reference->next;
    if ( next != NULL ) {
        next->prev = reference;
    }
}

inline
talloc_chunk * talloc_reference_realloc_chunk ( talloc_chunk * reference_chunk, size_t length )
{
    talloc_reference * old_reference = talloc_reference_from_chunk ( reference_chunk );
    talloc_reference * new_reference = realloc ( old_reference, sizeof ( talloc_reference ) + sizeof ( talloc_chunk ) + length );
    if ( new_reference == NULL ) {
        return NULL;
    }

    reference_chunk = talloc_chunk_from_reference ( new_reference );

#if defined(TALLOC_DEBUG)
    reference_chunk->length = length;
#endif

    if ( old_reference != new_reference ) {
        // now pointers to old_reference is invalid
        // each pointer to old_reference should be replaced with new_reference
        talloc_reference_update ( new_reference );
    }

    return reference_chunk;
}

inline
void talloc_reference_update_extensions ( talloc_extensions * extensions )
{
    talloc_reference * reference = extensions->first_reference;
    while ( reference != NULL ) {
        reference->parent_extensions = extensions;
        reference = reference->next;
    }
}

uint8_t talloc_reference_free_chunk ( talloc_chunk * chunk );

#endif


