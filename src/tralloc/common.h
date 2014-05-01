// This file is part of tralloc. Copyright (C) 2013 Andrew Aladjev aladjev.andrew@gmail.com
// tralloc is free software: you can redistribute it and/or modify it under the terms of the GNU General Lesser Public License as published by the Free Software Foundation, either version 3 of the License, or (at your option) any later version.
// tralloc is distributed in the hope that it will be useful, but WITHOUT ANY WARRANTY; without even the implied warranty of MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU General Lesser Public License for more details.
// You should have received a copy of the GNU General Lesser Public License along with tralloc. If not, see <http://www.gnu.org/licenses/>.

#ifndef TRALLOC_COMMON_H
#define TRALLOC_COMMON_H

#include "macro.h"
#include "types.h"

#undef _TRALLOC_INLINE
#ifdef _TRALLOC_COMMON_INCLUDED_FROM_OBJECT
#    define _TRALLOC_INLINE _TRALLOC_INLINE_IN_OBJECT
#else
#    define _TRALLOC_INLINE _TRALLOC_INLINE_IN_HEADER
#endif


const char * tralloc_get_string_for_error ( tralloc_error error );
size_t       tralloc_predict_chunk_length ( tralloc_extensions extensions );

_TRALLOC_INLINE
tralloc_context * _tralloc_get_context_from_chunk ( _tralloc_chunk * chunk )
{
    return ( tralloc_context * ) ( ( uintptr_t ) chunk + sizeof ( _tralloc_chunk ) );
}

_TRALLOC_INLINE
_tralloc_chunk * _tralloc_get_chunk_from_context ( tralloc_context * context )
{
    return ( _tralloc_chunk * ) ( ( uintptr_t ) context - sizeof ( _tralloc_chunk ) );
}


#endif
