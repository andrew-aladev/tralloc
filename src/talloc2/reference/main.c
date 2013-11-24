// This file is part of talloc2. Copyright (C) 2013 Andrew Aladjev aladjev.andrew@gmail.com
// talloc2 is free software: you can redistribute it and/or modify it under the terms of the GNU General Lesser Public License as published by the Free Software Foundation, either version 3 of the License, or (at your option) any later version.
// talloc2 is distributed in the hope that it will be useful, but WITHOUT ANY WARRANTY; without even the implied warranty of MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU General Lesser Public License for more details.
// You should have received a copy of the GNU General Lesser Public License along with talloc2. If not, see <http://www.gnu.org/licenses/>.

#include "../tree.h"
#include "../ext/chunk.h"
#include "main.h"

uint8_t talloc_add_reference ( const void * parent_data, const void * child_data )
{
    if ( parent_data == NULL || child_data == NULL ) {
        return 1;
    }
    talloc_chunk * parent_chunk = talloc_chunk_from_data ( parent_data );
    talloc_chunk * child_chunk  = talloc_chunk_from_data ( child_data );
    if ( child_chunk->parent == parent_chunk ) {
        return 0;
    }
    
    talloc_chunk * reference_chunk = talloc_reference_chunk_new ();
    if ( reference_chunk == NULL ) {
        return 2;
    }
    talloc_reference * reference = talloc_memory_from_reference_chunk ( reference_chunk );
    talloc_ext       * child_ext = talloc_memory_from_ext_chunk ( child_chunk );
    reference->parent = child_ext;
    reference->prev   = NULL;

    talloc_reference * first_reference = child_ext->first_reference;
    if ( first_reference == NULL ) {
        reference->next = NULL;
    } else {
        reference->next       = first_reference;
        first_reference->prev = reference;
    }
    child_ext->first_reference = reference;

    talloc_set_child ( parent_chunk, reference_chunk );

    return 0;
}

uint8_t talloc_del_reference ( const void * parent_data, const void * child_data )
{
    if ( parent_data == NULL || child_data == NULL ) {
        return 1;
    }
    
    return 0;
}