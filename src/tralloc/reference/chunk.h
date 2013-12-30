// This file is part of tralloc. Copyright (C) 2013 Andrew Aladjev aladjev.andrew@gmail.com
// tralloc is free software: you can redistribute it and/or modify it under the terms of the GNU General Lesser Public License as published by the Free Software Foundation, either version 3 of the License, or (at your option) any later version.
// tralloc is distributed in the hope that it will be useful, but WITHOUT ANY WARRANTY; without even the implied warranty of MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU General Lesser Public License for more details.
// You should have received a copy of the GNU General Lesser Public License along with tralloc. If not, see <http://www.gnu.org/licenses/>.

#ifndef TRALLOC_REFERENCE_CHUNK_H
#define TRALLOC_REFERENCE_CHUNK_H

#include "../chunk.h"
#include "head_chunk.h"

#if defined(TRALLOC_DEBUG)
#include "../events.h"
#endif

inline
void _tralloc_reference_update_chunk ( _tralloc_chunk * chunk )
{
    _tralloc_reference * reference  = _tralloc_reference_from_chunk ( chunk );
    _tralloc_chunk     * prev_chunk = reference->prev;
    if ( prev_chunk == NULL ) {
        _tralloc_chunk * references_chunk = reference->references;
        references->first_reference       = reference;
    } else {
        prev->next = reference;
    }
    _tralloc_reference * next = reference->next;
    if ( next != NULL ) {
        next->prev = reference;
    }
}

inline
uint8_t _tralloc_reference_free_chunk ( _tralloc_chunk * chunk )
{
    _tralloc_reference * reference   = _tralloc_reference_from_chunk ( chunk );
    _tralloc_references * references = reference->references;

    _tralloc_reference * prev = reference->prev;
    _tralloc_reference * next = reference->next;

    if ( prev == NULL ) {
        references->first_reference = next;

        if ( next == NULL ) {
            _tralloc_chunk * parent_chunk = _tralloc_chunk_from_references ( references );
            if ( parent_chunk->parent == NULL ) {
                if ( ( result = _tralloc_references_free_chunk ( parent_chunk ) ) != 0 ) {
                    error = result;
                }
            }
        }
    } else {
        prev->next = next;
    }
    if ( next != NULL ) {
        next->prev = prev;
    }

    if ( ( result = _tralloc_free_chunk_children ( chunk ) ) != 0 ) {
        error = result;
    }

    free ( reference );
    return error;
}

#endif


