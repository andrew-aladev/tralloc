// This file is part of tralloc. Copyright (C) 2013 Andrew Aladjev aladjev.andrew@gmail.com
// tralloc is free software: you can redistribute it and/or modify it under the terms of the GNU General Lesser Public License as published by the Free Software Foundation, either version 3 of the License, or (at your option) any later version.
// tralloc is distributed in the hope that it will be useful, but WITHOUT ANY WARRANTY; without even the implied warranty of MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU General Lesser Public License for more details.
// You should have received a copy of the GNU General Lesser Public License along with tralloc. If not, see <http://www.gnu.org/licenses/>.

#include "main.h"
#include "../tree/common.h"
#include "../tree/extensions.h"


typedef _tralloc_chunk * ( * _reference_alloc_chunk ) ( tralloc_context * parent_context, uint8_t extensions, size_t length );

static inline
_tralloc_chunk * _add_reference ( _tralloc_chunk * child_chunk, tralloc_context * parent_context, uint8_t extensions, size_t length, _reference_alloc_chunk allocator )
{
    if ( ( child_chunk->extensions & TRALLOC_HAVE_REFERENCES ) == 0 ) {
        return NULL;
    }

    if ( parent_context != NULL ) {
        _tralloc_chunk * parent_chunk = _tralloc_chunk_from_context ( parent_context );
        if ( parent_chunk->extensions == TRALLOC_HAVE_REFERENCE ) {
            parent_chunk   = parent_chunk->parent;
            parent_context = _tralloc_context_from_chunk ( parent_chunk );
        }

        if ( child_chunk->parent == parent_chunk ) {
            return NULL;
        }
    }

    _tralloc_chunk * chunk = allocator ( parent_context, extensions | TRALLOC_HAVE_REFERENCE, length );
    if ( chunk == NULL ) {
        return NULL;
    }

    _tralloc_reference * reference = _tralloc_reference_from_chunk ( chunk );
    reference->references          = child_chunk;
    reference->prev                = NULL;

    _tralloc_references * references       = _tralloc_references_from_chunk ( child_chunk );
    _tralloc_chunk * first_reference_chunk = references->first_reference;
    if ( first_reference_chunk == NULL ) {
        reference->next = NULL;
    } else {
        reference->next = first_reference_chunk;

        _tralloc_reference * first_reference = _tralloc_reference_from_chunk ( first_reference_chunk );
        first_reference->prev = chunk;
    }
    references->first_reference = chunk;

    return chunk;
}


_tralloc_chunk * _tralloc_reference_with_extensions ( _tralloc_chunk * child_chunk, tralloc_context * parent_context, uint8_t extensions, size_t length )
{
    return _add_reference ( child_chunk, parent_context, extensions, length, _tralloc_with_extensions );
}

extern inline tralloc_context * tralloc_reference_with_extensions ( tralloc_context * child_context, tralloc_context * parent_context, uint8_t extensions, size_t length );
extern inline tralloc_context * tralloc_reference                 ( tralloc_context * child_context, tralloc_context * parent_context, size_t length );


_tralloc_chunk * _tralloc_reference_zero_with_extensions ( _tralloc_chunk * child_chunk, tralloc_context * parent_context, uint8_t extensions, size_t length )
{
    return _add_reference ( child_chunk, parent_context, extensions, length, _tralloc_zero_with_extensions );
}

extern inline tralloc_context * tralloc_reference_zero_with_extensions ( tralloc_context * child_context, tralloc_context * parent_context, uint8_t extensions, size_t length );
extern inline tralloc_context * tralloc_reference_zero                 ( tralloc_context * child_context, tralloc_context * parent_context, size_t length );


extern inline tralloc_context * tralloc_reference_new ( tralloc_context * child_context, tralloc_context * parent_context );


uint8_t _tralloc_clear_references ( _tralloc_chunk * chunk )
{
    uint8_t result, error = 0;

    _tralloc_references * references = _tralloc_references_from_chunk ( chunk );
    _tralloc_chunk * reference_chunk = references->first_reference;
    _tralloc_chunk * next_reference_chunk;

    while ( reference_chunk != NULL ) {
        next_reference_chunk = reference_chunk->next;
        _tralloc_detach_chunk ( chunk );
        if ( ( result = _tralloc_free_chunk ( chunk ) ) != 0 ) {
            error = result;
        }
        reference_chunk = next_reference_chunk;
    }
    references->first_reference = NULL;

    return error;
}

extern inline uint8_t tralloc_clear_references ( tralloc_context * chunk_context );
