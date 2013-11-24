// This file is part of talloc2. Copyright (C) 2013 Andrew Aladjev aladjev.andrew@gmail.com
// talloc2 is free software: you can redistribute it and/or modify it under the terms of the GNU General Lesser Public License as published by the Free Software Foundation, either version 3 of the License, or (at your option) any later version.
// talloc2 is distributed in the hope that it will be useful, but WITHOUT ANY WARRANTY; without even the implied warranty of MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU General Lesser Public License for more details.
// You should have received a copy of the GNU General Lesser Public License along with talloc2. If not, see <http://www.gnu.org/licenses/>.

#ifndef TALLOC_CHUNK_H
#define TALLOC_CHUNK_H

#include "types.h"
#include <stdlib.h>

inline
talloc_chunk * talloc_usual_chunk_malloc ( size_t length )
{
    return ( talloc_chunk * ) malloc ( sizeof ( talloc_chunk ) + length );
}

inline
talloc_chunk * talloc_usual_chunk_calloc ( size_t length )
{
    return ( talloc_chunk * ) calloc ( 1, sizeof ( talloc_chunk ) + length );
}

inline
talloc_chunk * talloc_usual_chunk_realloc ( talloc_chunk * chunk, size_t length )
{
    return ( talloc_chunk * ) realloc ( chunk, sizeof ( talloc_chunk ) + length );
}

inline
void talloc_usual_chunk_free ( talloc_chunk * chunk )
{
    free ( chunk );
}

#endif
