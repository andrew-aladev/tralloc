// This file is part of tralloc. Copyright (C) 2013 Andrew Aladjev aladjev.andrew@gmail.com
// tralloc is free software: you can redistribute it and/or modify it under the terms of the GNU General Lesser Public License as published by the Free Software Foundation, either version 3 of the License, or (at your option) any later version.
// tralloc is distributed in the hope that it will be useful, but WITHOUT ANY WARRANTY; without even the implied warranty of MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU General Lesser Public License for more details.
// You should have received a copy of the GNU General Lesser Public License along with tralloc. If not, see <http://www.gnu.org/licenses/>.

#ifndef TRALLOC_REFERENCE_COMMON_H
#define TRALLOC_REFERENCE_COMMON_H

#include "../types.h"


inline
size_t _tralloc_get_reference_offset ( _tralloc_chunk * chunk )
{
    size_t offset = 0;

#if defined(TRALLOC_LENGTH)
    if ( chunk->extensions & TRALLOC_EXTENSION_LENGTH ) {
        offset += sizeof ( _tralloc_length );
    }
#endif

#if defined(TRALLOC_DESTRUCTOR)
    if ( chunk->extensions & TRALLOC_EXTENSION_DESTRUCTORS ) {
        offset += sizeof ( _tralloc_destructors );
    }
#endif

    return offset;
}

inline
_tralloc_references * _tralloc_get_references_from_chunk ( _tralloc_chunk * chunk )
{
    return ( _tralloc_references * ) ( ( uintptr_t ) chunk - _tralloc_get_reference_offset ( chunk ) - sizeof ( _tralloc_references ) );
}

inline
_tralloc_reference * _tralloc_get_reference_from_chunk ( _tralloc_chunk * chunk )
{
    return ( _tralloc_reference * ) ( ( uintptr_t ) chunk - _tralloc_get_reference_offset ( chunk ) - sizeof ( _tralloc_reference ) );
}

inline
_tralloc_chunk * _tralloc_get_chunk_from_references ( _tralloc_references * references )
{
    size_t offset = sizeof ( _tralloc_references );

#if defined(TRALLOC_LENGTH)
    if ( references->extensions & TRALLOC_EXTENSION_LENGTH ) {
        offset += sizeof ( _tralloc_length );
    }
#endif

#if defined(TRALLOC_DESTRUCTOR)
    if ( references->extensions & TRALLOC_EXTENSION_DESTRUCTORS ) {
        offset += sizeof ( _tralloc_destructors );
    }
#endif

    return ( _tralloc_chunk * ) ( ( uintptr_t ) references + offset );
}


#endif
