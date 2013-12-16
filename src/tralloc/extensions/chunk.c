// This file is part of tralloc. Copyright (C) 2013 Andrew Aladjev aladjev.andrew@gmail.com
// tralloc is free software: you can redistribute it and/or modify it under the terms of the GNU General Lesser Public License as published by the Free Software Foundation, either version 3 of the License, or (at your option) any later version.
// tralloc is distributed in the hope that it will be useful, but WITHOUT ANY WARRANTY; without even the implied warranty of MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU General Lesser Public License for more details.
// You should have received a copy of the GNU General Lesser Public License along with tralloc. If not, see <http://www.gnu.org/licenses/>.

#include "chunk.h"

#if defined(TRALLOC_DESTRUCTOR)
#include "destructor.h"
#endif

#if defined(TRALLOC_REFERENCE)
#include "../reference/chunk.h"
#endif

tralloc_chunk * tralloc_extensions_realloc_chunk ( tralloc_chunk * extensions_chunk, size_t length )
{
    tralloc_extensions * old_extensions = tralloc_extensions_from_chunk ( extensions_chunk );
    tralloc_extensions * new_extensions = realloc ( old_extensions, sizeof ( tralloc_extensions ) + sizeof ( tralloc_chunk ) + length );
    if ( new_extensions == NULL ) {
        return NULL;
    }

    extensions_chunk = tralloc_chunk_from_extensions ( new_extensions );

#if defined(TRALLOC_DEBUG)
    extensions_chunk->length = length;
#endif

#if defined(TRALLOC_REFERENCE)
    if ( old_extensions != new_extensions ) {
        // now pointers to old_extensions is invalid
        // each pointer to old_extensions should be replaced with new_extensions
        tralloc_reference_update_extensions ( new_extensions );
    }
#endif

    return extensions_chunk;
}

uint8_t tralloc_extensions_free_chunk ( tralloc_chunk * extensions_chunk )
{
    tralloc_extensions * extensions = tralloc_extensions_from_chunk ( extensions_chunk );

#if defined(TRALLOC_REFERENCE)
    if ( extensions->first_reference != NULL ) {
        tralloc_detach_chunk ( extensions_chunk );
        return 0;
    }
#endif

    uint8_t result, error = 0;

#if defined(TRALLOC_DEBUG)
    if ( ( result = tralloc_on_free ( extensions_chunk ) ) != 0 ) {
        error = result;
    }
#endif

#if defined(TRALLOC_DESTRUCTOR)
    if ( ( result = tralloc_destructor_free ( extensions_chunk, extensions ) ) != 0 ) {
        error = result;
    }
#endif

    if ( ( result = tralloc_free_chunk_children ( extensions_chunk ) ) != 0 ) {
        error = result;
    }

    free ( extensions );
    return error;
}

extern inline tralloc_chunk * tralloc_extensions_process_new_chunk ( tralloc_extensions * extensions, const tralloc_context * parent_context, size_t length );
extern inline tralloc_chunk * tralloc_extensions_malloc_chunk      ( const tralloc_context * parent_context, size_t length );
extern inline tralloc_chunk * tralloc_extensions_calloc_chunk      ( const tralloc_context * parent_context, size_t length );