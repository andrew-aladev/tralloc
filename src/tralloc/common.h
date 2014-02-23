// This file is part of tralloc. Copyright (C) 2013 Andrew Aladjev aladjev.andrew@gmail.com
// tralloc is free software: you can redistribute it and/or modify it under the terms of the GNU General Lesser Public License as published by the Free Software Foundation, either version 3 of the License, or (at your option) any later version.
// tralloc is distributed in the hope that it will be useful, but WITHOUT ANY WARRANTY; without even the implied warranty of MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU General Lesser Public License for more details.
// You should have received a copy of the GNU General Lesser Public License along with tralloc. If not, see <http://www.gnu.org/licenses/>.

#ifndef TRALLOC_COMMON_H
#define TRALLOC_COMMON_H

#include "types.h"


inline
size_t tralloc_predict_chunk_length ( tralloc_extensions extensions )
{
    size_t extensions_length = 0;

#if defined(TRALLOC_LENGTH)
    if ( extensions & TRALLOC_EXTENSION_LENGTH ) {
        extensions_length += sizeof ( _tralloc_length );
    }
#endif

#if defined(TRALLOC_DESTRUCTOR)
    if ( extensions & TRALLOC_EXTENSION_DESTRUCTORS ) {
        extensions_length += sizeof ( _tralloc_destructors );
    }
#endif

#if defined(TRALLOC_REFERENCE)
    if ( extensions & TRALLOC_EXTENSION_REFERENCES ) {
        extensions_length += sizeof ( _tralloc_references );
    } else if ( extensions & TRALLOC_EXTENSION_REFERENCE ) {
        extensions_length += sizeof ( _tralloc_reference );
    }
#endif

#if defined(TRALLOC_POOL)
    if ( extensions & TRALLOC_EXTENSION_POOL ) {
        extensions_length += sizeof ( _tralloc_pool );
    } else if ( extensions & TRALLOC_EXTENSION_POOL_CHILD ) {
        extensions_length += sizeof ( _tralloc_pool_child );
    }
#endif

    return extensions_length + sizeof ( _tralloc_chunk );
}

inline
tralloc_context * _tralloc_get_context_from_chunk ( _tralloc_chunk * chunk )
{
    return ( tralloc_context * ) ( ( uintptr_t ) chunk + sizeof ( _tralloc_chunk ) );
}

inline
_tralloc_chunk * _tralloc_get_chunk_from_context ( tralloc_context * context )
{
    return ( _tralloc_chunk * ) ( ( uintptr_t ) context - sizeof ( _tralloc_chunk ) );
}


#endif
