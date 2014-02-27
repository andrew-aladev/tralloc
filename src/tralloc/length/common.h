// This file is part of tralloc. Copyright (C) 2013 Andrew Aladjev aladjev.andrew@gmail.com
// tralloc is free software: you can redistribute it and/or modify it under the terms of the GNU General Lesser Public License as published by the Free Software Foundation, either version 3 of the License, or (at your option) any later version.
// tralloc is distributed in the hope that it will be useful, but WITHOUT ANY WARRANTY; without even the implied warranty of MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU General Lesser Public License for more details.
// You should have received a copy of the GNU General Lesser Public License along with tralloc. If not, see <http://www.gnu.org/licenses/>.

#ifndef TRALLOC_LENGTH_COMMON_H
#define TRALLOC_LENGTH_COMMON_H

#include "../macro.h"
#include "../types.h"

#undef INLINE
#ifdef _TRALLOC_LENGTH_COMMON_INCLUDED_FROM_OBJECT
#    define INLINE INLINE_IN_OBJECT
#else
#    define INLINE INLINE_IN_HEADER
#endif


INLINE
_tralloc_length * _tralloc_get_length_from_chunk ( _tralloc_chunk * chunk )
{
    return ( _tralloc_length * ) ( ( uintptr_t ) chunk - sizeof ( _tralloc_length ) );
}


#endif
