// This file is part of tralloc. Copyright (C) 2013 Andrew Aladjev aladjev.andrew@gmail.com
// tralloc is free software: you can redistribute it and/or modify it under the terms of the GNU General Lesser Public License as published by the Free Software Foundation, either version 3 of the License, or (at your option) any later version.
// tralloc is distributed in the hope that it will be useful, but WITHOUT ANY WARRANTY; without even the implied warranty of MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU General Lesser Public License for more details.
// You should have received a copy of the GNU General Lesser Public License along with tralloc. If not, see <http://www.gnu.org/licenses/>.

#if !defined ( TRALLOC_LENGTH_DATA_H )
#define TRALLOC_LENGTH_DATA_H

#include "../length.h"
#include "../extensions.h"

#undef _TRALLOC_INLINE
#if defined ( _TRALLOC_INCLUDED_FROM_LENGTH_DATA_C )
#    define _TRALLOC_INLINE _TRALLOC_INLINE_IN_OBJECT
#else
#    define _TRALLOC_INLINE _TRALLOC_INLINE_IN_HEADER
#endif


struct _tralloc_length_type {
    size_t length;
};

_TRALLOC_INLINE
size_t _tralloc_length_get ( _tralloc_length * length_ptr )
{
    return length_ptr->length;
}

_TRALLOC_INLINE
void _tralloc_length_set ( _tralloc_length * length_ptr, size_t length )
{
    length_ptr->length = length;
}

_TRALLOC_INLINE
_tralloc_length * _tralloc_chunk_get_length ( _tralloc_chunk * chunk )
{
    return ( _tralloc_length * ) ( ( uintptr_t ) chunk - _tralloc_extensions_get_offset_for_extension ( chunk->extensions, TRALLOC_EXTENSION_LENGTH ) );
}


#endif
