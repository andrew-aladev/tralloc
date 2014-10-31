// This file is part of tralloc. Copyright (C) 2013 Andrew Aladjev aladjev.andrew@gmail.com
// tralloc is free software: you can redistribute it and/or modify it under the terms of the GNU General Lesser Public License as published by the Free Software Foundation, either version 3 of the License, or (at your option) any later version.
// tralloc is distributed in the hope that it will be useful, but WITHOUT ANY WARRANTY; without even the implied warranty of MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU General Lesser Public License for more details.
// You should have received a copy of the GNU General Lesser Public License along with tralloc. If not, see <http://www.gnu.org/licenses/>.

#if !defined ( TRALLOC_LENGTH_MAIN_H )
#define TRALLOC_LENGTH_MAIN_H

#include "chunk.h"

#undef _TRALLOC_INLINE
#if defined ( _TRALLOC_INCLUDED_FROM_LENGTH_MAIN_C )
#    define _TRALLOC_INLINE _TRALLOC_INLINE_IN_OBJECT
#else
#    define _TRALLOC_INLINE _TRALLOC_INLINE_IN_HEADER
#endif


_TRALLOC_INLINE
size_t _tralloc_get_length ( _tralloc_length * length_ptr )
{
    return * length_ptr;
}

_TRALLOC_INLINE
void _tralloc_set_length ( _tralloc_length * length_ptr, size_t length )
{
    * length_ptr = length;
}

_TRALLOC_INLINE
tralloc_error tralloc_get_length ( tralloc_context * context, size_t * length_result_ptr )
{
    if ( context == NULL ) {
        return TRALLOC_ERROR_REQUIRED_ARGUMENT_IS_UNDEFINED;
    }
    _tralloc_chunk * chunk = _tralloc_get_chunk_from_context ( context );
    if ( ! ( chunk->extensions & TRALLOC_EXTENSION_LENGTH ) ) {
        return TRALLOC_ERROR_NO_SUCH_EXTENSION;
    }
    * length_result_ptr = _tralloc_get_length ( _tralloc_get_length_from_chunk ( chunk ) );
    return 0;
}


#endif
