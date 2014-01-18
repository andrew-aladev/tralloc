// This file is part of tralloc. Copyright (C) 2013 Andrew Aladjev aladjev.andrew@gmail.com
// tralloc is free software: you can redistribute it and/or modify it under the terms of the GNU General Lesser Public License as published by the Free Software Foundation, either version 3 of the License, or (at your option) any later version.
// tralloc is distributed in the hope that it will be useful, but WITHOUT ANY WARRANTY; without even the implied warranty of MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU General Lesser Public License for more details.
// You should have received a copy of the GNU General Lesser Public License along with tralloc. If not, see <http://www.gnu.org/licenses/>.

#ifndef TRALLOC_COMMON_H
#define TRALLOC_COMMON_H

#include "types.h"


inline
tralloc_context * _tralloc_context_from_chunk ( _tralloc_chunk * chunk )
{
    return ( tralloc_context * ) ( ( uintptr_t ) chunk + sizeof ( _tralloc_chunk ) );
}

inline
_tralloc_chunk * _tralloc_chunk_from_context ( tralloc_context * context )
{
    return ( _tralloc_chunk * ) ( ( uintptr_t ) context - sizeof ( _tralloc_chunk ) );
}

#if defined(TRALLOC_LENGTH)
inline
_tralloc_length * _tralloc_length_from_chunk ( _tralloc_chunk * chunk )
{
    return ( _tralloc_length * ) ( ( uintptr_t ) chunk - sizeof ( _tralloc_length ) );
}
#endif

#if defined(TRALLOC_DESTRUCTOR)
inline
_tralloc_destructors * _tralloc_destructors_from_chunk ( _tralloc_chunk * chunk )
{
    size_t offset = sizeof ( _tralloc_destructors );

#if defined(TRALLOC_LENGTH)
    if ( chunk->extensions & TRALLOC_EXTENSION_LENGTH ) {
        offset += sizeof ( _tralloc_length );
    }
#endif

    return ( _tralloc_destructors * ) ( ( uintptr_t ) chunk - offset );
}
#endif

#if defined(TRALLOC_REFERENCE)
inline
_tralloc_references * _tralloc_references_from_chunk ( _tralloc_chunk * chunk )
{
    size_t offset = sizeof ( _tralloc_references );

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

    return ( _tralloc_references * ) ( ( uintptr_t ) chunk - offset );
}

inline
_tralloc_chunk * _tralloc_chunk_from_references ( _tralloc_references * references )
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

inline
_tralloc_reference * _tralloc_reference_from_chunk ( _tralloc_chunk * chunk )
{
    size_t offset = sizeof ( _tralloc_reference );

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

    return ( _tralloc_reference * ) ( ( uintptr_t ) chunk - offset );
}
#endif


#endif
