// This file is part of talloc2. Copyright (C) 2013 Andrew Aladjev aladjev.andrew@gmail.com
// talloc2 is free software: you can redistribute it and/or modify it under the terms of the GNU General Lesser Public License as published by the Free Software Foundation, either version 3 of the License, or (at your option) any later version.
// talloc2 is distributed in the hope that it will be useful, but WITHOUT ANY WARRANTY; without even the implied warranty of MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU General Lesser Public License for more details.
// You should have received a copy of the GNU General Lesser Public License along with talloc2. If not, see <http://www.gnu.org/licenses/>.

#ifndef TALLOC_COMMON_H
#define TALLOC_COMMON_H

#include "types.h"

inline
talloc_context * talloc_context_from_chunk ( talloc_chunk * chunk )
{
    return ( talloc_context * ) ( ( uintptr_t ) chunk + sizeof ( talloc_chunk ) );
}

inline
talloc_chunk * talloc_chunk_from_context ( const talloc_context * context )
{
    return ( talloc_chunk * ) ( ( uintptr_t ) context - sizeof ( talloc_chunk ) );
}

#ifdef TALLOC_EXTENSIONS
inline
talloc_chunk * talloc_chunk_from_extensions ( talloc_extensions * extensions )
{
    return ( talloc_chunk * ) ( ( uintptr_t ) extensions + sizeof ( talloc_extensions ) );
}

inline
talloc_extensions * talloc_extensions_from_chunk ( talloc_chunk * chunk )
{
    return ( talloc_extensions * ) ( ( uintptr_t ) chunk - sizeof ( talloc_extensions ) );
}
#endif

#ifdef TALLOC_REFERENCE
inline
talloc_chunk * talloc_chunk_from_reference ( talloc_reference * reference )
{
    return ( talloc_chunk * ) ( ( uintptr_t ) reference + sizeof ( talloc_reference ) );
}

inline
talloc_reference * talloc_reference_from_chunk ( talloc_chunk * chunk )
{
    return ( talloc_reference * ) ( ( uintptr_t ) chunk - sizeof ( talloc_reference ) );
}
#endif

#endif
