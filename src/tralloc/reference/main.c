// This file is part of tralloc. Copyright (C) 2013 Andrew Aladjev aladjev.andrew@gmail.com
// tralloc is free software: you can redistribute it and/or modify it under the terms of the GNU General Lesser Public License as published by the Free Software Foundation, either version 3 of the License, or (at your option) any later version.
// tralloc is distributed in the hope that it will be useful, but WITHOUT ANY WARRANTY; without even the implied warranty of MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU General Lesser Public License for more details.
// You should have received a copy of the GNU General Lesser Public License along with tralloc. If not, see <http://www.gnu.org/licenses/>.

#include "main.h"
#include "chunk.h"

typedef _tralloc_chunk * ( * _reference_alloc_chunk ) ( const tralloc_context * parent_context, size_t length );

static inline
tralloc_context * _add_reference ( const tralloc_context * child_context, const tralloc_context * parent_context, size_t length, _reference_alloc_chunk allocator )
{
    if ( child_context == NULL || parent_context == child_context ) {
        return NULL;
    }

    _tralloc_chunk * child_chunk = _tralloc_chunk_from_context ( child_context );

    if ( parent_context != NULL ) {
        _tralloc_chunk * parent_chunk = _tralloc_chunk_from_context ( parent_context );
        if ( parent_chunk->mode == TRALLOC_MODE_REFERENCE ) {
            _tralloc_reference * reference = _tralloc_reference_from_chunk  ( parent_chunk );
            parent_chunk                   = _tralloc_chunk_from_references ( reference->references );
            parent_context                 = _tralloc_context_from_chunk    ( parent_chunk );
        }

        if ( child_chunk->parent == parent_chunk ) {
            return NULL;
        }
    }

    _tralloc_chunk * chunk = allocator ( parent_context, length );
    if ( chunk == NULL ) {
        return NULL;
    }

    _tralloc_references * references = _tralloc_references_from_chunk ( child_chunk );
    _tralloc_reference  * reference  = _tralloc_reference_from_chunk  ( chunk );

    reference->references = references;
    reference->prev       = NULL;

    _tralloc_reference * first_reference = references->first_reference;
    if ( first_reference == NULL ) {
        reference->next = NULL;
    } else {
        reference->next       = first_reference;
        first_reference->prev = reference;
    }
    references->first_reference = reference;

    return _tralloc_context_from_chunk ( chunk );
}

tralloc_context * tralloc_reference_with_data ( const tralloc_context * child_context, const tralloc_context * parent_context, size_t length )
{
    return _add_reference ( child_context, parent_context, length, _tralloc_reference_malloc_chunk );
}

tralloc_context * tralloc_reference_with_zero_data ( const tralloc_context * child_context, const tralloc_context * parent_context, size_t length )
{
    return _add_reference ( child_context, parent_context, length, _tralloc_reference_calloc_chunk );
}

uint8_t tralloc_clear_references ( const tralloc_context * chunk_context )
{
    _tralloc_references * references = _tralloc_references_from_chunk ( _tralloc_chunk_from_context ( chunk_context ) );
    _tralloc_reference * reference   = references->first_reference;
    _tralloc_reference * next_reference;

    uint8_t result, error = 0;
    _tralloc_chunk * chunk;
    while ( reference != NULL ) {
        next_reference = reference->next;
        chunk = _tralloc_chunk_from_reference ( reference );
        _tralloc_detach_chunk ( chunk );
        if ( ( result = _tralloc_free_chunk ( chunk ) ) != 0 ) {
            error = result;
        }
        reference = next_reference;
    }
    references->first_reference = NULL;

    return error;
}

extern inline tralloc_context * tralloc_reference ( const tralloc_context * child_context, const tralloc_context * parent_context );