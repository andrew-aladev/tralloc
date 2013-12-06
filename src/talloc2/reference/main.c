// This file is part of talloc2. Copyright (C) 2013 Andrew Aladjev aladjev.andrew@gmail.com
// talloc2 is free software: you can redistribute it and/or modify it under the terms of the GNU General Lesser Public License as published by the Free Software Foundation, either version 3 of the License, or (at your option) any later version.
// talloc2 is distributed in the hope that it will be useful, but WITHOUT ANY WARRANTY; without even the implied warranty of MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU General Lesser Public License for more details.
// You should have received a copy of the GNU General Lesser Public License along with talloc2. If not, see <http://www.gnu.org/licenses/>.

#include "main.h"
#include "../extensions/chunk.h"

void ** talloc_add_reference ( const void * child_data, const void * parent_data )
{
    if ( parent_data == NULL || child_data == NULL || parent_data == child_data ) {
        return NULL;
    }
    talloc_chunk * parent_chunk = talloc_chunk_from_data ( parent_data );
    talloc_chunk * child_chunk  = talloc_chunk_from_data ( child_data );
    if ( parent_chunk->mode == TALLOC_MODE_REFERENCE ) {
        return NULL;
    }
    if ( child_chunk->parent == parent_chunk ) {
        return NULL;
    }

    talloc_reference * reference = talloc_reference_malloc_chunk ( parent_data, child_data );
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

    return talloc_data_from_chunk ( talloc_chunk_from_reference ( reference ) );
}

uint8_t talloc_clear_references ( const void * chunk_data )
{
    talloc_extensions * extensions = talloc_extensions_from_chunk ( talloc_chunk_from_data ( chunk_data ) );
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