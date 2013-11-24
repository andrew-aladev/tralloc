// This file is part of talloc2. Copyright (C) 2013 Andrew Aladjev aladjev.andrew@gmail.com
// talloc2 is free software: you can redistribute it and/or modify it under the terms of the GNU General Lesser Public License as published by the Free Software Foundation, either version 3 of the License, or (at your option) any later version.
// talloc2 is distributed in the hope that it will be useful, but WITHOUT ANY WARRANTY; without even the implied warranty of MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU General Lesser Public License for more details.
// You should have received a copy of the GNU General Lesser Public License along with talloc2. If not, see <http://www.gnu.org/licenses/>.

#ifndef TALLOC_REFERENCE_MAIN_H
#define TALLOC_REFERENCE_MAIN_H

#include "../tree.h"

inline
talloc_reference * talloc_reference_from_chunk ( talloc_chunk * chunk )
{
    return ( talloc_reference * ) ( ( uintptr_t ) chunk - sizeof ( talloc_reference ) );
}

inline
talloc_chunk * talloc_chunk_from_reference ( talloc_reference * reference )
{
    return ( talloc_chunk * ) ( ( uintptr_t ) reference + sizeof ( talloc_reference ) );
}

#endif
