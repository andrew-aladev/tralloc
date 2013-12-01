// This file is part of talloc2. Copyright (C) 2013 Andrew Aladjev aladjev.andrew@gmail.com
// talloc2 is free software: you can redistribute it and/or modify it under the terms of the GNU General Lesser Public License as published by the Free Software Foundation, either version 3 of the License, or (at your option) any later version.
// talloc2 is distributed in the hope that it will be useful, but WITHOUT ANY WARRANTY; without even the implied warranty of MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU General Lesser Public License for more details.
// You should have received a copy of the GNU General Lesser Public License along with talloc2. If not, see <http://www.gnu.org/licenses/>.

#ifndef TALLOC_REFERENCE_CHUNK_H
#define TALLOC_REFERENCE_CHUNK_H

#include "../tree.h"
#include <stdlib.h>

inline
talloc_reference * talloc_reference_malloc_chunk ( const void * parent_data, const void * chunk_data )
{
    talloc_reference * reference = malloc ( sizeof ( talloc_reference ) + sizeof ( talloc_chunk ) + sizeof ( uintptr_t ) );
    if ( reference == NULL ) {
        return NULL;
    }

    talloc_chunk * reference_chunk = talloc_chunk_from_reference ( reference );
    reference_chunk->mode          = TALLOC_MODE_REFERENCE;

#if defined(TALLOC_DEBUG)
    reference_chunk->chunk_length = sizeof ( talloc_reference ) + sizeof ( talloc_chunk );
    reference_chunk->length       = sizeof ( uintptr_t );
#endif

    void ** data = ( void ** ) talloc_data_from_chunk ( reference_chunk );
    * data       = ( void * ) chunk_data;

    talloc_add_chunk ( parent_data, reference_chunk );

    return reference;
}

inline
void talloc_reference_update ( talloc_ext * ext, talloc_chunk * ext_chunk )
{
    void ** data;
    void * chunk_data = talloc_data_from_chunk ( ext_chunk );

    talloc_reference * reference = ext->first_reference;
    while ( reference != NULL ) {
        reference->parent_ext = ext;
        data = ( void ** ) talloc_data_from_chunk ( talloc_chunk_from_reference ( reference ) );
        * data = chunk_data;
        reference = reference->next;
    }
}

uint8_t talloc_reference_free_chunk ( talloc_chunk * chunk );

#endif


