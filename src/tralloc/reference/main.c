// This file is part of tralloc. Copyright (C) 2013 Andrew Aladjev aladjev.andrew@gmail.com
// tralloc is free software: you can redistribute it and/or modify it under the terms of the GNU General Lesser Public License as published by the Free Software Foundation, either version 3 of the License, or (at your option) any later version.
// tralloc is distributed in the hope that it will be useful, but WITHOUT ANY WARRANTY; without even the implied warranty of MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU General Lesser Public License for more details.
// You should have received a copy of the GNU General Lesser Public License along with tralloc. If not, see <http://www.gnu.org/licenses/>.

#include "main.h"
#include "../tree.h"


static inline
void _tralloc_reference_detach ( _tralloc_reference * reference )
{
    _tralloc_chunk * old_references_chunk = reference->references;
    _tralloc_chunk * prev_chunk = reference->prev;
    _tralloc_chunk * next_chunk = reference->next;

    reference->references = NULL;
    reference->prev       = NULL;
    reference->next       = NULL;

    if ( prev_chunk != NULL ) {
        reference = _tralloc_reference_from_chunk ( prev_chunk );
        reference->next = next_chunk;
    } else if ( old_references_chunk != NULL ) {
        _tralloc_references * old_references = _tralloc_references_from_chunk ( old_references_chunk );
        old_references->first_reference      = next_chunk;
    }
    if ( next_chunk != NULL ) {
        reference = _tralloc_reference_from_chunk ( next_chunk );
        reference->prev = prev_chunk;
    }
}

static inline
void _tralloc_reference_attach ( _tralloc_chunk * reference_chunk, _tralloc_reference * reference, _tralloc_chunk * references_chunk )
{
    _tralloc_chunk * old_references_chunk = reference->references;
    _tralloc_chunk * prev_chunk = reference->prev;
    _tralloc_chunk * next_chunk = reference->next;

    reference->references = references_chunk;
    reference->prev       = NULL;

    if ( prev_chunk != NULL ) {
        _tralloc_reference * prev_reference = _tralloc_reference_from_chunk ( prev_chunk );
        prev_reference->next = next_chunk;
    } else if ( old_references_chunk != NULL ) {
        _tralloc_references * old_references = _tralloc_references_from_chunk ( old_references_chunk );
        old_references->first_reference      = next_chunk;
    }
    if ( next_chunk != NULL ) {
        _tralloc_reference * next_reference = _tralloc_reference_from_chunk ( next_chunk );
        next_reference->prev = prev_chunk;
    }

    _tralloc_references * references       = _tralloc_references_from_chunk ( references_chunk );
    _tralloc_chunk * first_reference_chunk = references->first_reference;
    if ( first_reference_chunk != NULL ) {
        _tralloc_reference * first_reference = _tralloc_reference_from_chunk ( first_reference_chunk );
        first_reference->prev = reference_chunk;
        reference->next = first_reference_chunk;
    } else {
        reference->next = NULL;
    }
    references->first_reference = reference_chunk;
}

tralloc_error tralloc_move_reference ( tralloc_context * child_context, tralloc_context * parent_context )
{
    if ( child_context == NULL ) {
        return TRALLOC_ERROR_CONTEXT_IS_NULL;
    }
    if ( child_context == parent_context ) {
        return TRALLOC_ERROR_CHILD_EQUALS_PARENT;
    }
    _tralloc_chunk * child_chunk = _tralloc_chunk_from_context ( child_context );

    if ( ( child_chunk->extensions & TRALLOC_HAVE_REFERENCE ) == 0 ) {
        return TRALLOC_ERROR_NO_SUCH_EXTENSION;
    }

    if ( parent_context == NULL ) {
        _tralloc_reference * reference    = _tralloc_reference_from_chunk ( child_chunk );
        _tralloc_chunk * references_chunk = reference->references;
        if ( references_chunk == NULL ) {
            return TRALLOC_ERROR_CHILD_HAS_SAME_PARENT;
        }
        _tralloc_reference_detach ( reference );
    } else {
        _tralloc_reference * reference        = _tralloc_reference_from_chunk ( child_chunk );
        _tralloc_chunk * references_chunk     = reference->references;
        _tralloc_chunk * new_references_chunk = _tralloc_chunk_from_context ( parent_context );
        if ( ( new_references_chunk->extensions & TRALLOC_HAVE_REFERENCES ) == 0 ) {
            return TRALLOC_ERROR_NO_SUCH_EXTENSION;
        }
        if ( references_chunk == new_references_chunk ) {
            return TRALLOC_ERROR_CHILD_HAS_SAME_PARENT;
        }
        _tralloc_reference_attach ( child_chunk, reference, new_references_chunk );
    }

    return 0;
}


tralloc_error tralloc_clear_references ( tralloc_context * chunk_context )
{
    if ( chunk_context == NULL ) {
        return TRALLOC_ERROR_CONTEXT_IS_NULL;
    }
    _tralloc_chunk * chunk = _tralloc_chunk_from_context ( chunk_context );

    if ( ( chunk->extensions & TRALLOC_HAVE_REFERENCES ) == 0 ) {
        return TRALLOC_ERROR_NO_SUCH_EXTENSION;
    }

    _tralloc_references * references = _tralloc_references_from_chunk ( chunk );
    _tralloc_chunk * reference_chunk = references->first_reference;
    _tralloc_chunk * next_reference_chunk;

    tralloc_error result, error = 0;

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
