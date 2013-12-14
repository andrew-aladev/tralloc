// This file is part of talloc2. Copyright (C) 2013 Andrew Aladjev aladjev.andrew@gmail.com
// talloc2 is free software: you can redistribute it and/or modify it under the terms of the GNU General Lesser Public License as published by the Free Software Foundation, either version 3 of the License, or (at your option) any later version.
// talloc2 is distributed in the hope that it will be useful, but WITHOUT ANY WARRANTY; without even the implied warranty of MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU General Lesser Public License for more details.
// You should have received a copy of the GNU General Lesser Public License along with talloc2. If not, see <http://www.gnu.org/licenses/>.

#include "main.h"
#include "../extensions/chunk.h"

typedef talloc_reference * ( * reference_alloc_chunk ) ( const talloc_context * parent_context, size_t length );

static inline
talloc_context * add_reference ( const talloc_context * child_context, const talloc_context * parent_context, size_t length, reference_alloc_chunk allocator )
{
    if ( parent_context == NULL || child_context == NULL || parent_context == child_context ) {
        return NULL;
    }

    talloc_chunk * parent_chunk = talloc_chunk_from_context ( parent_context );
    if ( parent_chunk->mode == TALLOC_MODE_REFERENCE ) {
        talloc_reference * reference = talloc_reference_from_chunk ( parent_chunk );
        parent_chunk                 = talloc_chunk_from_extensions ( reference->parent_extensions );
        parent_context               = talloc_context_from_chunk ( parent_chunk );
    }

    talloc_chunk * child_chunk = talloc_chunk_from_context ( child_context );
    if ( child_chunk->parent == parent_chunk ) {
        return NULL;
    }

    talloc_reference * reference = allocator ( parent_context, length );
    if ( reference == NULL ) {
        return NULL;
    }

    talloc_extensions * child_extensions = talloc_extensions_from_chunk ( child_chunk );

    reference->parent_extensions = child_extensions;
    reference->prev              = NULL;

    talloc_reference * first_reference = child_extensions->first_reference;
    if ( first_reference == NULL ) {
        reference->next = NULL;
    } else {
        reference->next       = first_reference;
        first_reference->prev = reference;
    }
    child_extensions->first_reference = reference;

    return talloc_context_from_chunk ( talloc_chunk_from_reference ( reference ) );
}

talloc_context * talloc_add_reference_with_data ( const talloc_context * child_context, const talloc_context * parent_context, size_t length )
{
    return add_reference ( child_context, parent_context, length, talloc_reference_malloc_chunk );
}

talloc_context * talloc_add_reference_with_zero_data ( const talloc_context * child_context, const talloc_context * parent_context, size_t length )
{
    return add_reference ( child_context, parent_context, length, talloc_reference_calloc_chunk );
}

uint8_t talloc_clear_references ( const talloc_context * chunk_context )
{
    talloc_extensions * extensions = talloc_extensions_from_chunk ( talloc_chunk_from_context ( chunk_context ) );
    talloc_reference * reference   = extensions->first_reference;
    talloc_reference * next_reference;

    uint8_t result, error = 0;
    talloc_chunk * chunk;
    while ( reference != NULL ) {
        next_reference = reference->next;
        chunk = talloc_chunk_from_reference ( reference );
        talloc_detach_chunk ( chunk );
        if ( ( result = talloc_free_chunk ( chunk ) ) != 0 ) {
            error = result;
        }
        reference = next_reference;
    }
    extensions->first_reference = NULL;

    return error;
}

extern inline talloc_context * talloc_add_reference ( const talloc_context * child_context, const talloc_context * parent_context );