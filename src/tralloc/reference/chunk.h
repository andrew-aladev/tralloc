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
_tralloc_chunk * _tralloc_reference_process_new_chunk ( _tralloc_chunk * chunk, const tralloc_context * parent_context )
{
    chunk->mode = TRALLOC_MODE_REFERENCE;
    return _tralloc_usual_process_new_chunk ( chunk, parent_context );
}

inline
_tralloc_chunk * _tralloc_reference_malloc_chunk ( const tralloc_context * parent_context, size_t length )
{
    _tralloc_reference * reference = malloc ( sizeof ( _tralloc_reference ) + sizeof ( _tralloc_chunk ) + length );
    if ( reference == NULL ) {
        return NULL;
    }
    _tralloc_chunk * chunk = _tralloc_chunk_from_reference ( reference );

#if defined(TRALLOC_DEBUG)
    chunk->chunk_length = sizeof ( _tralloc_reference ) + sizeof ( _tralloc_chunk );
    chunk->length       = length;
#endif

    return _tralloc_reference_process_new_chunk ( chunk, parent_context );
}

inline
_tralloc_chunk * _tralloc_reference_calloc_chunk ( const tralloc_context * parent_context, size_t length )
{
    _tralloc_reference * reference = calloc ( 1, sizeof ( _tralloc_reference ) + sizeof ( _tralloc_chunk ) + length );
    if ( reference == NULL ) {
        return NULL;
    }
    _tralloc_chunk * chunk = _tralloc_chunk_from_reference ( reference );

#if defined(TRALLOC_DEBUG)
    chunk->chunk_length = sizeof ( _tralloc_reference ) + sizeof ( _tralloc_chunk );
    chunk->length       = length;
#endif

    return _tralloc_reference_process_new_chunk ( chunk, parent_context );
}

inline
void _tralloc_reference_update ( _tralloc_reference * reference )
{
    _tralloc_reference * prev = reference->prev;
    if ( prev == NULL ) {
        _tralloc_references * references = reference->references;
        references->first_reference      = reference;
    } else {
        prev->next = reference;
    }
    _tralloc_reference * next = reference->next;
    if ( next != NULL ) {
        next->prev = reference;
    }
}

inline
_tralloc_chunk * _tralloc_reference_realloc_chunk ( _tralloc_chunk * chunk, size_t length )
{
    _tralloc_reference * old_reference = _tralloc_reference_from_chunk ( chunk );
    _tralloc_reference * new_reference = realloc ( old_reference, sizeof ( _tralloc_reference ) + sizeof ( _tralloc_chunk ) + length );
    if ( new_reference == NULL ) {
        return NULL;
    }

    chunk = _tralloc_chunk_from_reference ( new_reference );

#if defined(TRALLOC_DEBUG)
    chunk->length = length;
#endif

    if ( old_reference != new_reference ) {
        // now pointers to old_reference is invalid
        // each pointer to old_reference should be replaced with new_reference
        _tralloc_reference_update ( new_reference );
    }

    return chunk;
}

inline
uint8_t _tralloc_reference_free_chunk ( _tralloc_chunk * chunk )
{
    uint8_t result, error = 0;

#if defined(TRALLOC_DEBUG)
    if ( ( result = _tralloc_on_free ( chunk ) ) != 0 ) {
        error = result;
    }
#endif

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
