// This file is part of talloc2. Copyright (C) 2013 Andrew Aladjev aladjev.andrew@gmail.com
// talloc2 is free software: you can redistribute it and/or modify it under the terms of the GNU General Lesser Public License as published by the Free Software Foundation, either version 3 of the License, or (at your option) any later version.
// talloc2 is distributed in the hope that it will be useful, but WITHOUT ANY WARRANTY; without even the implied warranty of MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU General Lesser Public License for more details.
// You should have received a copy of the GNU General Lesser Public License along with talloc2. If not, see <http://www.gnu.org/licenses/>.

#ifndef TALLOC_EXTENSIONS_CHUNK_H
#define TALLOC_EXTENSIONS_CHUNK_H

#include "../tree.h"

#if defined(TALLOC_DEBUG)
#include "../events.h"
#endif

#include <stdlib.h>

inline
talloc_chunk * talloc_extensions_process_new_chunk ( talloc_extensions * extensions, const talloc_context * parent_context, size_t length )
{

#if defined(TALLOC_DESTRUCTOR)
    extensions->first_destructor = NULL;
#endif

    talloc_chunk * extensions_chunk = talloc_chunk_from_extensions ( extensions );

#if defined(TALLOC_DEBUG)
    extensions_chunk->chunk_length = sizeof ( talloc_extensions ) + sizeof ( talloc_chunk );
    extensions_chunk->length       = length;
#endif

    talloc_add_chunk ( parent_context, extensions_chunk );

#if defined(TALLOC_REFERENCE)
    extensions_chunk->mode      = TALLOC_MODE_EXTENSIONS;
    extensions->first_reference = NULL;
#endif

    return extensions_chunk;
}

inline
talloc_chunk * talloc_extensions_malloc_chunk ( const talloc_context * parent_context, size_t length )
{
    talloc_extensions * extensions = malloc ( sizeof ( talloc_extensions ) + sizeof ( talloc_chunk ) + length );
    if ( extensions == NULL ) {
        return NULL;
    }
    return talloc_extensions_process_new_chunk ( extensions, parent_context, length );
}

inline
talloc_chunk * talloc_extensions_calloc_chunk ( const talloc_context * parent_context, size_t length )
{
    talloc_extensions * extensions = calloc ( 1, sizeof ( talloc_extensions ) + sizeof ( talloc_chunk ) + length );
    if ( extensions == NULL ) {
        return NULL;
    }
    return talloc_extensions_process_new_chunk ( extensions, parent_context, length );
}

talloc_chunk * talloc_extensions_realloc_chunk ( talloc_chunk * extensions_chunk, size_t length );
uint8_t        talloc_extensions_free_chunk    ( talloc_chunk * extensions_chunk );

#endif
