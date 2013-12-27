// This file is part of tralloc. Copyright (C) 2013 Andrew Aladjev aladjev.andrew@gmail.com
// tralloc is free software: you can redistribute it and/or modify it under the terms of the GNU General Lesser Public License as published by the Free Software Foundation, either version 3 of the License, or (at your option) any later version.
// tralloc is distributed in the hope that it will be useful, but WITHOUT ANY WARRANTY; without even the implied warranty of MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU General Lesser Public License for more details.
// You should have received a copy of the GNU General Lesser Public License along with tralloc. If not, see <http://www.gnu.org/licenses/>.

#ifndef TRALLOC_REFERENCE_HEAD_CHUNK_H
#define TRALLOC_REFERENCE_HEAD_CHUNK_H

#include "../chunk.h"

#if defined(TRALLOC_DEBUG)
#include "../events.h"
#endif

inline
_tralloc_chunk * _tralloc_references_process_new_chunk ( _tralloc_references * references, _tralloc_chunk * chunk, const tralloc_context * parent_context )
{

#if defined(TRALLOC_REFERENCE)
    chunk->mode = TRALLOC_MODE_REFERENCES;
    references->first_reference = NULL;
#endif

    return _tralloc_usual_process_new_chunk ( chunk, parent_context );
}

inline
_tralloc_chunk * _tralloc_references_malloc_chunk ( const tralloc_context * parent_context, size_t length )
{
    _tralloc_references * references = malloc ( sizeof ( _tralloc_references ) + sizeof ( _tralloc_chunk ) + length );
    if ( references == NULL ) {
        return NULL;
    }
    _tralloc_chunk * chunk = _tralloc_chunk_from_references ( references );

#if defined(TRALLOC_DEBUG)
    chunk->chunk_length = sizeof ( _tralloc_references ) + sizeof ( _tralloc_chunk );
    chunk->length       = length;
#endif

    return _tralloc_references_process_new_chunk ( references, chunk, parent_context );
}

inline
_tralloc_chunk * _tralloc_references_calloc_chunk ( const tralloc_context * parent_context, size_t length )
{
    _tralloc_references * references = calloc ( 1, sizeof ( _tralloc_references ) + sizeof ( _tralloc_chunk ) + length );
    if ( references == NULL ) {
        return NULL;
    }
    _tralloc_chunk * chunk = _tralloc_chunk_from_references ( references );

#if defined(TRALLOC_DEBUG)
    chunk->chunk_length = sizeof ( _tralloc_references ) + sizeof ( _tralloc_chunk );
    chunk->length       = length;
#endif

    return _tralloc_references_process_new_chunk ( references, chunk, parent_context );
}

#if defined(TRALLOC_REFERENCE)
inline
void _tralloc_references_update_reference ( _tralloc_references * references )
{
    _tralloc_reference * reference = references->first_reference;
    while ( reference != NULL ) {
        reference->references = references;
        reference = reference->next;
    }
}
#endif

inline
_tralloc_chunk * _tralloc_references_realloc_chunk ( _tralloc_chunk * chunk, size_t length )
{
    _tralloc_references * old_references = _tralloc_references_from_chunk ( chunk );
    _tralloc_references * new_references = realloc ( old_references, sizeof ( _tralloc_references ) + sizeof ( _tralloc_chunk ) + length );
    if ( new_references == NULL ) {
        return NULL;
    }

    chunk = _tralloc_chunk_from_references ( new_references );

#if defined(TRALLOC_DEBUG)
    chunk->length = length;
#endif

#if defined(TRALLOC_REFERENCE)
    if ( old_references != new_references ) {
        // now pointers to old_references is invalid
        // each pointer to old_references should be replaced with new_references
        _tralloc_references_update_reference ( new_references );
    }
#endif

    return chunk;
}

inline
uint8_t _tralloc_references_free_chunk ( _tralloc_chunk * chunk )
{
    _tralloc_references * references = _tralloc_references_from_chunk ( chunk );

#if defined(TRALLOC_REFERENCE)
    if ( references->first_reference != NULL ) {
        _tralloc_detach_chunk ( chunk );
        return 0;
    }
#endif

    uint8_t error = _tralloc_usual_process_free_chunk ( chunk );
    free ( references );
    return error;
}

#endif
