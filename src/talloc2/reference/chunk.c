// This file is part of talloc2. Copyright (C) 2013 Andrew Aladjev aladjev.andrew@gmail.com
// talloc2 is free software: you can redistribute it and/or modify it under the terms of the GNU General Lesser Public License as published by the Free Software Foundation, either version 3 of the License, or (at your option) any later version.
// talloc2 is distributed in the hope that it will be useful, but WITHOUT ANY WARRANTY; without even the implied warranty of MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU General Lesser Public License for more details.
// You should have received a copy of the GNU General Lesser Public License along with talloc2. If not, see <http://www.gnu.org/licenses/>.

#include "chunk.h"
#include "../ext/chunk.h"

#if defined(TALLOC_EVENTS)
#include "../events.h"
#endif

#include <stdbool.h>

extern inline talloc_chunk * talloc_reference_new_chunk ( const void * chunk_data );
extern inline void           talloc_reference_update    ( talloc_ext * ext, talloc_chunk * chunk );

uint8_t talloc_reference_free_chunk ( talloc_chunk * chunk )
{
    uint8_t result, error = 0;

#if defined(TALLOC_EVENTS)
    if ( ( result = talloc_on_del ( chunk ) ) != 0 ) {
        error = result;
    }
#endif

    talloc_reference * reference = talloc_reference_from_chunk ( chunk );
    talloc_ext * parent_ext      = reference->parent_ext;

    talloc_reference * prev = reference->prev;
    talloc_reference * next = reference->next;
    
    if ( prev == NULL ) {
        parent_ext->first_reference = next;

        if ( next == NULL ) {
            talloc_chunk * parent_chunk = talloc_chunk_from_ext ( parent_ext );
            if ( parent_chunk->parent == NULL ) {
                if ( ( result = talloc_ext_free_chunk ( parent_chunk ) ) != 0 ) {
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

    free ( reference );
    return error;
}