// This file is part of talloc2. Copyright (C) 2013 Andrew Aladjev aladjev.andrew@gmail.com
// talloc2 is free software: you can redistribute it and/or modify it under the terms of the GNU General Lesser Public License as published by the Free Software Foundation, either version 3 of the License, or (at your option) any later version.
// talloc2 is distributed in the hope that it will be useful, but WITHOUT ANY WARRANTY; without even the implied warranty of MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU General Lesser Public License for more details.
// You should have received a copy of the GNU General Lesser Public License along with talloc2. If not, see <http://www.gnu.org/licenses/>.

#include "chunk.h"

#if defined(TALLOC_DESTRUCTOR)
#include "destructor.h"
#endif

#if defined(TALLOC_REFERENCE)
#include "../reference/chunk.h"
#endif

#if defined(TALLOC_DEBUG)
#include "../events.h"
#endif

extern inline talloc_chunk * talloc_extensions_malloc_chunk ( const void * parent_data, size_t length );
extern inline talloc_chunk * talloc_extensions_calloc_chunk ( const void * parent_data, size_t length );

talloc_chunk * talloc_extensions_realloc_chunk ( talloc_chunk * extensions_chunk, size_t length )
{
    talloc_extensions * old_extensions = talloc_extensions_from_chunk ( extensions_chunk );
    talloc_extensions * new_extensions = realloc ( old_extensions, sizeof ( talloc_extensions ) + sizeof ( talloc_chunk ) + length );
    if ( new_extensions == NULL ) {
        return NULL;
    }

    extensions_chunk = talloc_chunk_from_extensions ( new_extensions );

#if defined(TALLOC_DEBUG)
    extensions_chunk->length = length;
#endif

#if defined(TALLOC_REFERENCE)
    if ( old_extensions != new_extensions ) {
        // now pointers to old_extensions is invalid
        // each pointer to old_extensions should be replaced with new_extensions
        talloc_reference_update ( new_extensions, extensions_chunk );
    }
#endif

    return extensions_chunk;
}

uint8_t talloc_extensions_free_chunk ( talloc_chunk * extensions_chunk )
{
    talloc_extensions * extensions = talloc_extensions_from_chunk ( extensions_chunk );

#if defined(TALLOC_REFERENCE)
    if ( extensions->first_reference != NULL ) {
        talloc_detach_chunk ( extensions_chunk );
        return 0;
    }
#endif

    uint8_t result, error = 0;

#if defined(TALLOC_DEBUG)
    if ( ( result = talloc_on_free ( extensions_chunk ) ) != 0 ) {
        error = result;
    }
#endif

#if defined(TALLOC_DESTRUCTOR)
    if ( ( result = talloc_destructor_free ( extensions_chunk, extensions ) ) != 0 ) {
        error = result;
    }
#endif

    if ( ( result = talloc_free_chunk_children ( extensions_chunk ) ) != 0 ) {
        error = result;
    }

    free ( extensions );
    return error;
}