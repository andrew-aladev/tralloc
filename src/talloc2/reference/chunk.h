// This file is part of talloc2. Copyright (C) 2013 Andrew Aladjev aladjev.andrew@gmail.com
// talloc2 is free software: you can redistribute it and/or modify it under the terms of the GNU General Lesser Public License as published by the Free Software Foundation, either version 3 of the License, or (at your option) any later version.
// talloc2 is distributed in the hope that it will be useful, but WITHOUT ANY WARRANTY; without even the implied warranty of MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU General Lesser Public License for more details.
// You should have received a copy of the GNU General Lesser Public License along with talloc2. If not, see <http://www.gnu.org/licenses/>.

#ifndef TALLOC_REFERENCE_CHUNK_H
#define TALLOC_REFERENCE_CHUNK_H

#include "../common.h"
#include <stdlib.h>

inline
talloc_chunk * talloc_reference_new_chunk ( const void * chunk_data )
{
    void * memory = malloc ( sizeof ( talloc_reference ) + sizeof ( talloc_chunk ) + sizeof ( uintptr_t ) );
    if ( memory == NULL ) {
        return NULL;
    }

    talloc_chunk * chunk = talloc_chunk_from_reference ( memory );
    chunk->mode          = TALLOC_MODE_REFERENCE;

    void ** data = ( void ** ) talloc_data_from_chunk ( chunk );
    * data       = ( void * ) chunk_data;
    return chunk;
}

inline
void talloc_reference_update ( talloc_ext * ext, talloc_chunk * chunk )
{
    void ** data;
    void * chunk_data = talloc_data_from_chunk ( chunk );

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
