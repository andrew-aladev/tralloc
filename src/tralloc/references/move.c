// This file is part of tralloc. Copyright (C) 2013 Andrew Aladjev aladjev.andrew@gmail.com
// tralloc is free software: you can redistribute it and/or modify it under the terms of the GNU General Lesser Public License as published by the Free Software Foundation, either version 3 of the License, or (at your option) any later version.
// tralloc is distributed in the hope that it will be useful, but WITHOUT ANY WARRANTY; without even the implied warranty of MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU General Lesser Public License for more details.
// You should have received a copy of the GNU General Lesser Public License along with tralloc. If not, see <http://www.gnu.org/licenses/>.

#include <tralloc/references/move.h>
#include <tralloc/references/chunk.h>


static inline
void _tralloc_detach_reference ( _tralloc_reference * reference )
{
    _tralloc_references * old_references = reference->references;
    _tralloc_reference * prev = reference->prev;
    _tralloc_reference * next = reference->next;

    reference->references = NULL;
    reference->prev       = NULL;
    reference->next       = NULL;

    if ( prev != NULL ) {
        prev->next = next;
    } else if ( old_references != NULL ) {
        old_references->first_reference = next;
    }
    if ( next != NULL ) {
        next->prev = prev;
    }
}

static inline
void _tralloc_attach_reference ( _tralloc_reference * reference, _tralloc_references * new_references )
{
    _tralloc_references * old_references = reference->references;
    _tralloc_reference * prev = reference->prev;
    _tralloc_reference * next = reference->next;

    reference->references = new_references;
    reference->prev       = NULL;

    if ( prev != NULL ) {
        prev->next = next;
    } else if ( old_references != NULL ) {
        old_references->first_reference = next;
    }
    if ( next != NULL ) {
        next->prev = prev;
    }

    _tralloc_reference * first_reference = new_references->first_reference;
    if ( first_reference != NULL ) {
        first_reference->prev = reference;
        reference->next = first_reference;
    } else {
        reference->next = NULL;
    }
    new_references->first_reference = reference;
}

tralloc_error tralloc_move_reference ( tralloc_context * child_context, tralloc_context * parent_context )
{
    if ( child_context == NULL ) {
        return TRALLOC_ERROR_REQUIRED_ARGUMENT_IS_UNDEFINED;
    }
    if ( child_context == parent_context ) {
        return TRALLOC_ERROR_CHILD_EQUALS_PARENT;
    }
    _tralloc_chunk * child_chunk = _tralloc_context_get_chunk ( child_context );

    if ( ! ( child_chunk->extensions & TRALLOC_EXTENSION_REFERENCE ) ) {
        return TRALLOC_ERROR_NO_SUCH_EXTENSION;
    }

    if ( parent_context == NULL ) {
        _tralloc_reference * reference = _tralloc_get_reference_from_chunk ( child_chunk );
        if ( reference->references == NULL ) {
            return TRALLOC_ERROR_CHILD_HAS_SAME_PARENT;
        }
        _tralloc_detach_reference ( reference );
    } else {
        _tralloc_reference * reference        = _tralloc_get_reference_from_chunk ( child_chunk );
        _tralloc_references * old_references  = reference->references;
        _tralloc_chunk * new_references_chunk = _tralloc_context_get_chunk ( parent_context );
        if ( ! ( new_references_chunk->extensions & TRALLOC_EXTENSION_REFERENCES ) ) {
            return TRALLOC_ERROR_NO_SUCH_EXTENSION;
        }
        _tralloc_references * new_references = _tralloc_get_references_from_chunk ( new_references_chunk );
        if ( old_references == new_references ) {
            return TRALLOC_ERROR_CHILD_HAS_SAME_PARENT;
        }
        _tralloc_attach_reference ( reference, new_references );
    }

    return 0;
}
