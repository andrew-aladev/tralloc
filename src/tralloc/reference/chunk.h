// This file is part of tralloc. Copyright (C) 2013 Andrew Aladjev aladjev.andrew@gmail.com
// tralloc is free software: you can redistribute it and/or modify it under the terms of the GNU General Lesser Public License as published by the Free Software Foundation, either version 3 of the License, or (at your option) any later version.
// tralloc is distributed in the hope that it will be useful, but WITHOUT ANY WARRANTY; without even the implied warranty of MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU General Lesser Public License for more details.
// You should have received a copy of the GNU General Lesser Public License along with tralloc. If not, see <http://www.gnu.org/licenses/>.

#ifndef TRALLOC_REFERENCE_CHUNK_H
#define TRALLOC_REFERENCE_CHUNK_H

#include "../tree/common.h"


inline
void _tralloc_reference_update_chunk ( _tralloc_chunk * reference_chunk )
{
    _tralloc_reference * reference  = _tralloc_reference_from_chunk ( reference_chunk );
    _tralloc_chunk     * prev_chunk = reference->prev;
    _tralloc_chunk     * next_chunk = reference->next;
    if ( prev_chunk == NULL ) {
        _tralloc_references * references = _tralloc_references_from_chunk ( reference->references );
        references->first_reference      = reference_chunk;
    } else {
        reference = _tralloc_reference_from_chunk ( prev_chunk );
        reference->next = reference_chunk;
    }
    if ( next_chunk != NULL ) {
        reference = _tralloc_reference_from_chunk ( next_chunk );
        reference->prev = reference_chunk;
    }
}

inline
uint8_t _tralloc_reference_free_chunk ( _tralloc_chunk * chunk )
{
    _tralloc_reference * reference  = _tralloc_reference_from_chunk ( chunk );
    _tralloc_chunk *     prev_chunk = reference->prev;
    _tralloc_chunk *     next_chunk = reference->next;

    if ( prev_chunk == NULL ) {
        _tralloc_chunk * references_chunk = reference->references;
        _tralloc_references * references  = _tralloc_references_from_chunk ( references_chunk );
        references->first_reference       = next_chunk;

        if ( next_chunk == NULL ) {
            if ( references_chunk->parent == NULL ) {
                return _tralloc_free_chunk ( references_chunk );
            }
        }
    } else {
        reference = _tralloc_reference_from_chunk ( prev_chunk );
        reference->next = next_chunk;
    }
    if ( next_chunk != NULL ) {
        reference = _tralloc_reference_from_chunk ( next_chunk );
        reference->prev = prev_chunk;
    }

    return 0;
}


#endif
