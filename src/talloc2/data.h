// This file is part of talloc2. Copyright (C) 2013 Andrew Aladjev aladjev.andrew@gmail.com
// talloc2 is free software: you can redistribute it and/or modify it under the terms of the GNU General Lesser Public License as published by the Free Software Foundation, either version 3 of the License, or (at your option) any later version.
// talloc2 is distributed in the hope that it will be useful, but WITHOUT ANY WARRANTY; without even the implied warranty of MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU General Lesser Public License for more details.
// You should have received a copy of the GNU General Lesser Public License along with talloc2. If not, see <http://www.gnu.org/licenses/>.

#ifndef TALLOC_DATA_H
#define TALLOC_DATA_H

#include "types.h"

inline
void * talloc_data_from_chunk ( talloc_chunk * chunk )
{
    return ( void * ) ( ( uintptr_t ) chunk + sizeof ( talloc_chunk ) );
}

inline
talloc_chunk * talloc_chunk_from_data ( const void * data )
{
    return ( talloc_chunk * ) ( ( uintptr_t ) data - sizeof ( talloc_chunk ) );
}

#endif
