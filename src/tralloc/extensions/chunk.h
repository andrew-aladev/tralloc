// This file is part of tralloc. Copyright (C) 2013 Andrew Aladjev aladjev.andrew@gmail.com
// tralloc is free software: you can redistribute it and/or modify it under the terms of the GNU General Lesser Public License as published by the Free Software Foundation, either version 3 of the License, or (at your option) any later version.
// tralloc is distributed in the hope that it will be useful, but WITHOUT ANY WARRANTY; without even the implied warranty of MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU General Lesser Public License for more details.
// You should have received a copy of the GNU General Lesser Public License along with tralloc. If not, see <http://www.gnu.org/licenses/>.

#ifndef TRALLOC_EXTENSIONS_CHUNK_H
#define TRALLOC_EXTENSIONS_CHUNK_H

#include "../tree.h"

#if defined(TRALLOC_DEBUG)
#include "../events.h"
#endif

#include <stdlib.h>

inline
tralloc_chunk * tralloc_extensions_process_new_chunk ( tralloc_extensions * extensions, const tralloc_context * parent_context, size_t length )
{

#if defined(TRALLOC_DESTRUCTOR)
    extensions->first_destructor = NULL;
#endif

    tralloc_chunk * extensions_chunk = tralloc_chunk_from_extensions ( extensions );

#if defined(TRALLOC_DEBUG)
    extensions_chunk->chunk_length = sizeof ( tralloc_extensions ) + sizeof ( tralloc_chunk );
    extensions_chunk->length       = length;
#endif

    tralloc_add_chunk ( parent_context, extensions_chunk );

#if defined(TRALLOC_REFERENCE)
    extensions_chunk->mode      = TRALLOC_MODE_EXTENSIONS;
    extensions->first_reference = NULL;
#endif

    return extensions_chunk;
}

inline
tralloc_chunk * tralloc_extensions_malloc_chunk ( const tralloc_context * parent_context, size_t length )
{
    tralloc_extensions * extensions = malloc ( sizeof ( tralloc_extensions ) + sizeof ( tralloc_chunk ) + length );
    if ( extensions == NULL ) {
        return NULL;
    }
    return tralloc_extensions_process_new_chunk ( extensions, parent_context, length );
}

inline
tralloc_chunk * tralloc_extensions_calloc_chunk ( const tralloc_context * parent_context, size_t length )
{
    tralloc_extensions * extensions = calloc ( 1, sizeof ( tralloc_extensions ) + sizeof ( tralloc_chunk ) + length );
    if ( extensions == NULL ) {
        return NULL;
    }
    return tralloc_extensions_process_new_chunk ( extensions, parent_context, length );
}

tralloc_chunk * tralloc_extensions_realloc_chunk ( tralloc_chunk * extensions_chunk, size_t length );
uint8_t         tralloc_extensions_free_chunk    ( tralloc_chunk * extensions_chunk );

#endif
