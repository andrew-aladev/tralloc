// This file is part of talloc2. Copyright (C) 2013 Andrew Aladjev aladjev.andrew@gmail.com
// talloc2 is free software: you can redistribute it and/or modify it under the terms of the GNU General Lesser Public License as published by the Free Software Foundation, either version 3 of the License, or (at your option) any later version.
// talloc2 is distributed in the hope that it will be useful, but WITHOUT ANY WARRANTY; without even the implied warranty of MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU General Lesser Public License for more details.
// You should have received a copy of the GNU General Lesser Public License along with talloc2. If not, see <http://www.gnu.org/licenses/>.

#ifndef TALLOC_EXT_CHUNK_H
#define TALLOC_EXT_CHUNK_H

#include "../tree.h"
#include <stdlib.h>

inline
talloc_chunk * talloc_ext_malloc_chunk ( const void * parent_data, size_t length )
{
    talloc_ext * ext = malloc ( sizeof ( talloc_ext ) + sizeof ( talloc_chunk ) + length );
    if ( ext == NULL ) {
        return NULL;
    }

#if defined(TALLOC_EXT_DESTRUCTOR)
    ext->first_destructor = NULL;
#endif

    talloc_chunk * ext_chunk = talloc_chunk_from_ext ( ext );

#if defined(TALLOC_DEBUG)
    ext_chunk->chunk_length = sizeof ( talloc_ext ) + sizeof ( talloc_chunk );
    ext_chunk->length       = length;
#endif

    talloc_add_chunk ( parent_data, ext_chunk );

#if defined(TALLOC_REFERENCE)
    ext_chunk->mode      = TALLOC_MODE_EXT;
    ext->first_reference = NULL;
#endif

    return ext_chunk;
}

inline
talloc_chunk * talloc_ext_calloc_chunk ( const void * parent_data, size_t length )
{
    talloc_ext * ext = calloc ( 1, sizeof ( talloc_ext ) + sizeof ( talloc_chunk ) + length );
    if ( ext == NULL ) {
        return NULL;
    }

#if defined(TALLOC_EXT_DESTRUCTOR)
    ext->first_destructor = NULL;
#endif

    talloc_chunk * ext_chunk = talloc_chunk_from_ext ( ext );

#if defined(TALLOC_DEBUG)
    ext_chunk->chunk_length = sizeof ( talloc_ext ) + sizeof ( talloc_chunk );
    ext_chunk->length       = length;
#endif

    talloc_add_chunk ( parent_data, ext_chunk );

#if defined(TALLOC_REFERENCE)
    ext_chunk->mode          = TALLOC_MODE_EXT;
    ext->first_reference = NULL;
#endif

    return ext_chunk;
}

talloc_chunk * talloc_ext_realloc_chunk ( talloc_chunk * ext_chunk, size_t length );
uint8_t        talloc_ext_free_chunk    ( talloc_chunk * ext_chunk );

#endif
