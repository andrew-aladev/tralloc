// This file is part of tralloc. Copyright (C) 2013 Andrew Aladjev aladjev.andrew@gmail.com
// tralloc is free software: you can redistribute it and/or modify it under the terms of the GNU General Lesser Public License as published by the Free Software Foundation, either version 3 of the License, or (at your option) any later version.
// tralloc is distributed in the hope that it will be useful, but WITHOUT ANY WARRANTY; without even the implied warranty of MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU General Lesser Public License for more details.
// You should have received a copy of the GNU General Lesser Public License along with tralloc. If not, see <http://www.gnu.org/licenses/>.

#ifndef TRALLOC_REFERENCE_HEAD_CHUNK_H
#define TRALLOC_REFERENCE_HEAD_CHUNK_H

#include "../chunk.h"
#include "../tree.h"

#if defined(TRALLOC_DEBUG)
#include "../events.h"
#endif

#include <stdbool.h>

inline
void _tralloc_references_new_chunk ( _tralloc_chunk * chunk )
{
    _tralloc_references * references = _tralloc_references_from_chunk ( chunk );
    references->first_reference      = NULL;
}

inline
void _tralloc_references_update_chunk ( _tralloc_chunk * references_chunk )
{
    _tralloc_references * references = _tralloc_references_from_chunk ( references_chunk );
    _tralloc_chunk * reference_chunk = references->first_reference;
    _tralloc_reference * reference;
    while ( reference_chunk != NULL ) {
        reference             = _tralloc_reference_from_chunk ( reference_chunk );
        reference->references = references_chunk;
        reference_chunk       = reference_chunk->next;
    }
}

inline
bool _tralloc_references_try_free_chunk ( _tralloc_chunk * chunk )
{
    _tralloc_references * references = _tralloc_references_from_chunk ( chunk );
    if ( references->first_reference != NULL ) {
        _tralloc_detach_chunk ( chunk );
        return false;
    }
    return true;
}

#endif
