// This file is part of tralloc. Copyright (C) 2013 Andrew Aladjev aladjev.andrew@gmail.com
// tralloc is free software: you can redistribute it and/or modify it under the terms of the GNU General Lesser Public License as published by the Free Software Foundation, either version 3 of the License, or (at your option) any later version.
// tralloc is distributed in the hope that it will be useful, but WITHOUT ANY WARRANTY; without even the implied warranty of MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU General Lesser Public License for more details.
// You should have received a copy of the GNU General Lesser Public License along with tralloc. If not, see <http://www.gnu.org/licenses/>.

#ifndef TRALLOC_DESTRUCTOR_COMMON_H
#define TRALLOC_DESTRUCTOR_COMMON_H

#include "../types.h"


inline
_tralloc_destructors * _tralloc_get_destructors_from_chunk ( _tralloc_chunk * chunk )
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
