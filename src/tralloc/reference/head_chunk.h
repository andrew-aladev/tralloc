// This file is part of tralloc. Copyright (C) 2013 Andrew Aladjev aladjev.andrew@gmail.com
// tralloc is free software: you can redistribute it and/or modify it under the terms of the GNU General Lesser Public License as published by the Free Software Foundation, either version 3 of the License, or (at your option) any later version.
// tralloc is distributed in the hope that it will be useful, but WITHOUT ANY WARRANTY; without even the implied warranty of MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU General Lesser Public License for more details.
// You should have received a copy of the GNU General Lesser Public License along with tralloc. If not, see <http://www.gnu.org/licenses/>.

#ifndef TRALLOC_REFERENCE_HEAD_CHUNK_H
#define TRALLOC_REFERENCE_HEAD_CHUNK_H

#include "../common.h"
#include "../macro.h"
#include <stdbool.h>


inline
void _tralloc_references_new_chunk ( _tralloc_chunk * chunk )
{
    _tralloc_references * references = _tralloc_get_references_from_chunk ( chunk );
    references->first_reference = NULL;
    references->extensions      = chunk->extensions;
    references->autofree        = false;
}

void _tralloc_references_update_chunk   ( _tralloc_chunk * chunk );
bool _tralloc_references_can_free_chunk ( _tralloc_chunk * chunk );

inline
bool _tralloc_references_can_free_chunk_children ( _tralloc_chunk * UNUSED ( chunk ) )
{
    return false;
}

#endif
