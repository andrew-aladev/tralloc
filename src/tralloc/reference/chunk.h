// This file is part of tralloc. Copyright (C) 2013 Andrew Aladjev aladjev.andrew@gmail.com
// tralloc is free software: you can redistribute it and/or modify it under the terms of the GNU General Lesser Public License as published by the Free Software Foundation, either version 3 of the License, or (at your option) any later version.
// tralloc is distributed in the hope that it will be useful, but WITHOUT ANY WARRANTY; without even the implied warranty of MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU General Lesser Public License for more details.
// You should have received a copy of the GNU General Lesser Public License along with tralloc. If not, see <http://www.gnu.org/licenses/>.

#if !defined ( TRALLOC_REFERENCE_CHUNK_H )
#define TRALLOC_REFERENCE_CHUNK_H

#include "../common.h"

#undef _TRALLOC_INLINE
#if defined ( _TRALLOC_INCLUDED_FROM_REFERENCE_CHUNK_C )
#    define _TRALLOC_INLINE _TRALLOC_INLINE_IN_OBJECT
#else
#    define _TRALLOC_INLINE _TRALLOC_INLINE_IN_HEADER
#endif


_TRALLOC_INLINE
_tralloc_references * _tralloc_get_references_from_chunk ( _tralloc_chunk * chunk )
{
    return ( _tralloc_references * ) ( ( uintptr_t ) chunk - _tralloc_get_offset_for_extension ( chunk->extensions, TRALLOC_EXTENSION_REFERENCES ) );
}

_TRALLOC_INLINE
_tralloc_reference * _tralloc_get_reference_from_chunk ( _tralloc_chunk * chunk )
{
    return ( _tralloc_reference * ) ( ( uintptr_t ) chunk - _tralloc_get_offset_for_extension ( chunk->extensions, TRALLOC_EXTENSION_REFERENCE ) );
}

_TRALLOC_INLINE
_tralloc_chunk * _tralloc_get_chunk_from_references ( _tralloc_references * references )
{
    return ( _tralloc_chunk * ) ( ( uintptr_t ) references + _tralloc_get_offset_for_extension ( references->extensions, TRALLOC_EXTENSION_REFERENCES ) );
}


#endif
