// This file is part of tralloc. Copyright (C) 2013 Andrew Aladjev aladjev.andrew@gmail.com
// tralloc is free software: you can redistribute it and/or modify it under the terms of the GNU General Lesser Public License as published by the Free Software Foundation, either version 3 of the License, or (at your option) any later version.
// tralloc is distributed in the hope that it will be useful, but WITHOUT ANY WARRANTY; without even the implied warranty of MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU General Lesser Public License for more details.
// You should have received a copy of the GNU General Lesser Public License along with tralloc. If not, see <http://www.gnu.org/licenses/>.

#include "chunk.h"
#include "../extensions/chunk.h"

uint8_t tralloc_reference_free_chunk ( tralloc_chunk * reference_chunk )
{
    uint8_t result, error = 0;

#if defined(TRALLOC_DEBUG)
    if ( ( result = tralloc_on_free ( reference_chunk ) ) != 0 ) {
        error = result;
    }
#endif

    tralloc_reference * reference          = tralloc_reference_from_chunk ( reference_chunk );
    tralloc_extensions * parent_extensions = reference->parent_extensions;

    tralloc_reference * prev = reference->prev;
    tralloc_reference * next = reference->next;

    if ( prev == NULL ) {
        parent_extensions->first_reference = next;

        if ( next == NULL ) {
            tralloc_chunk * parent_chunk = tralloc_chunk_from_extensions ( parent_extensions );
            if ( parent_chunk->parent == NULL ) {
                if ( ( result = tralloc_extensions_free_chunk ( parent_chunk ) ) != 0 ) {
                    error = result;
                }
            }
        }
    } else {
        prev->next = next;
    }
    if ( next != NULL ) {
        next->prev = prev;
    }

    if ( ( result = tralloc_free_chunk_children ( reference_chunk ) ) != 0 ) {
        error = result;
    }

    free ( reference );
    return error;
}

extern inline tralloc_reference * tralloc_reference_process_new_chunk ( tralloc_reference * reference, const tralloc_context * parent_context, size_t length );
extern inline tralloc_reference * tralloc_reference_malloc_chunk      ( const tralloc_context * parent_context, size_t length );
extern inline tralloc_reference * tralloc_reference_calloc_chunk      ( const tralloc_context * parent_context, size_t length );
extern inline void                tralloc_reference_update            ( tralloc_reference * reference );
extern inline tralloc_chunk *     tralloc_reference_realloc_chunk     ( tralloc_chunk * reference_chunk, size_t length );
extern inline void                tralloc_reference_update_extensions ( tralloc_extensions * extensions );