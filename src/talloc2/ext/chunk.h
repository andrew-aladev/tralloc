// This file is part of talloc2. Copyright (C) 2013 Andrew Aladjev aladjev.andrew@gmail.com
// talloc2 is free software: you can redistribute it and/or modify it under the terms of the GNU General Lesser Public License as published by the Free Software Foundation, either version 3 of the License, or (at your option) any later version.
// talloc2 is distributed in the hope that it will be useful, but WITHOUT ANY WARRANTY; without even the implied warranty of MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU General Lesser Public License for more details.
// You should have received a copy of the GNU General Lesser Public License along with talloc2. If not, see <http://www.gnu.org/licenses/>.

#ifndef TALLOC_EXT_CHUNK_H
#define TALLOC_EXT_CHUNK_H

#include "../types.h"
#include <stdlib.h>

#ifdef TALLOC_EXT_DESTRUCTOR
#include "destructor.h"
#endif

inline
talloc_chunk * talloc_chunk_from_ext ( talloc_ext * ext )
{
    return ( talloc_chunk * ) ( ( uintptr_t ) ext + sizeof ( talloc_ext ) );
}

inline
talloc_ext * talloc_ext_from_chunk ( talloc_chunk * chunk )
{
    return ( talloc_ext * ) ( ( uintptr_t ) chunk - sizeof ( talloc_ext ) );
}

inline
talloc_chunk * talloc_ext_chunk_malloc ( size_t length )
{
    talloc_ext * ext = malloc ( sizeof ( talloc_ext ) + sizeof ( talloc_chunk ) + length );
    if ( ext == NULL ) {
        return NULL;
    } else {

#ifdef TALLOC_EXT_DESTRUCTOR
        ext->first_destructor = NULL;
#endif

        talloc_chunk * chunk = talloc_chunk_from_ext ( ext );

#ifdef TALLOC_REFERENCE
        chunk->mode = TALLOC_MODE_EXT;
#endif

        return chunk;
    }
}

inline
talloc_chunk * talloc_ext_chunk_calloc ( size_t length )
{
    talloc_ext * ext = calloc ( 1, sizeof ( talloc_ext ) + sizeof ( talloc_chunk ) + length );
    if ( ext == NULL ) {
        return NULL;
    } else {

#ifdef TALLOC_EXT_DESTRUCTOR
        ext->first_destructor = NULL;
#endif

        talloc_chunk * chunk = talloc_chunk_from_ext ( ext );

#ifdef TALLOC_REFERENCE
        chunk->mode = TALLOC_MODE_EXT;
#endif

        return chunk;
    }
}

inline
talloc_chunk * talloc_ext_chunk_realloc ( talloc_chunk * chunk, size_t length )
{
    talloc_ext * ext = talloc_ext_from_chunk ( chunk );
    ext              = realloc ( ext, sizeof ( talloc_ext ) + sizeof ( talloc_chunk ) + length );
    if ( ext == NULL ) {
        return NULL;
    } else {
        return talloc_chunk_from_ext ( ext );
    }
}

inline
uint8_t talloc_ext_chunk_free ( talloc_chunk * chunk )
{
    talloc_ext * ext = talloc_ext_from_chunk ( chunk );

#ifdef TALLOC_REFERENCE
    ;
#endif

#ifdef TALLOC_EXT_DESTRUCTOR
    uint8_t result = talloc_destructor_free ( chunk, ext );
#endif

    free ( ext );

#ifdef TALLOC_EXT_DESTRUCTOR
    return result;
#else
    return 0;
#endif
}

#endif
