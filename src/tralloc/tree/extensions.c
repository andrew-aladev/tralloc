// This file is part of tralloc. Copyright (C) 2013 Andrew Aladjev aladjev.andrew@gmail.com
// tralloc is free software: you can redistribute it and/or modify it under the terms of the GNU General Lesser Public License as published by the Free Software Foundation, either version 3 of the License, or (at your option) any later version.
// tralloc is distributed in the hope that it will be useful, but WITHOUT ANY WARRANTY; without even the implied warranty of MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU General Lesser Public License for more details.
// You should have received a copy of the GNU General Lesser Public License along with tralloc. If not, see <http://www.gnu.org/licenses/>.

#include "common.h"
#include "extensions.h"

#include "../chunk.h"

#if defined(TRALLOC_DESTRUCTOR)
#include "../destructor/chunk.h"
#endif

#if defined(TRALLOC_REFERENCE)
#include "../reference/head_chunk.h"
#include "../reference/chunk.h"
#endif

#include <stdlib.h>
#include <stdbool.h>


typedef void * ( * _allocator ) ( size_t length );

static inline
void * _malloc ( size_t length )
{
    return malloc ( length );
}

static inline
void * _calloc ( size_t length )
{
    return calloc ( 1, length );
}

static inline
_tralloc_chunk * _tralloc_with_extensions_with_allocator ( tralloc_context * parent_context, uint8_t extensions, size_t length, _allocator allocator )
{
    uint8_t extensions_length = 0;

#if defined(TRALLOC_DESTRUCTOR)
    bool destructors = ( extensions & TRALLOC_HAVE_DESTRUCTORS ) != 0;
    if ( destructors ) {
        extensions_length += sizeof ( _tralloc_destructors );
    }
#endif

#if defined(TRALLOC_REFERENCE)
    bool references = ( extensions & TRALLOC_HAVE_REFERENCES ) != 0;
    bool reference  = ( extensions & TRALLOC_HAVE_REFERENCE )  != 0;
    if ( references ) {
        extensions_length += sizeof ( _tralloc_references );
        if ( reference ) {
            extensions &= ~ ( TRALLOC_HAVE_REFERENCE );
        }
    } else if ( reference ) {
        extensions_length += sizeof ( _tralloc_reference );
    }
#endif

    uint8_t chunk_length = sizeof ( _tralloc_chunk ) + extensions_length;
    void * memory        = allocator ( chunk_length + length );
    if ( memory == NULL ) {
        return NULL;
    }

    _tralloc_chunk * chunk = ( _tralloc_chunk * ) ( ( uintptr_t ) memory + extensions_length );
    chunk->extensions      = extensions;

#if defined(TRALLOC_DEBUG)
    chunk->chunk_length = chunk_length;
    chunk->length       = length;
#endif

#if defined(TRALLOC_DESTRUCTOR)
    if ( destructors ) {
        _tralloc_destructors_new_chunk ( chunk );
    }
#endif

#if defined(TRALLOC_REFERENCE)
    if ( references ) {
        _tralloc_references_new_chunk ( chunk );
    }
#endif

    _tralloc_add_chunk ( parent_context, chunk );
    return chunk;
}

_tralloc_chunk * _tralloc_with_extensions ( tralloc_context * parent_context, uint8_t extensions, size_t length )
{
    return _tralloc_with_extensions_with_allocator ( parent_context, extensions, length, _malloc );
}

extern inline tralloc_context * tralloc_with_extensions ( tralloc_context * parent_context, uint8_t extensions, size_t length );
extern inline tralloc_context * tralloc                 ( tralloc_context * parent_context, size_t length );


_tralloc_chunk * _tralloc_zero_with_extensions ( tralloc_context * parent_context, uint8_t extensions, size_t length )
{
    return _tralloc_with_extensions_with_allocator ( parent_context, extensions, length, _calloc );
}

extern inline tralloc_context * tralloc_zero_with_extensions ( tralloc_context * parent_context, uint8_t extensions, size_t length );
extern inline tralloc_context * tralloc_zero                 ( tralloc_context * parent_context, size_t length );

extern inline tralloc_context * tralloc_new                 ( tralloc_context * parent_context );
extern inline tralloc_context * tralloc_with_extensions_new ( tralloc_context * parent_context, uint8_t extensions );


_tralloc_chunk * _tralloc_realloc ( _tralloc_chunk * old_chunk, size_t length )
{

#if defined(TRALLOC_DEBUG)
    size_t old_length = old_chunk->length;
#endif

    uint8_t extensions_length = 0;

#if defined(TRALLOC_DESTRUCTOR)
    if ( ( old_chunk->extensions & TRALLOC_HAVE_DESTRUCTORS ) != 0 ) {
        extensions_length += sizeof ( _tralloc_destructors );
    }
#endif

#if defined(TRALLOC_REFERENCE)
    bool references = ( old_chunk->extensions & TRALLOC_HAVE_REFERENCES ) != 0;
    bool reference  = ( old_chunk->extensions & TRALLOC_HAVE_REFERENCE )  != 0;
    if ( references ) {
        extensions_length += sizeof ( _tralloc_references );
    } else if ( reference ) {
        extensions_length += sizeof ( _tralloc_reference );
    }
#endif

    void * old_memory = ( void * ) ( ( uintptr_t ) old_chunk - extensions_length );
    void * new_memory = realloc ( old_memory, sizeof ( _tralloc_chunk ) + extensions_length + length );
    if ( new_memory == NULL ) {
        return NULL;
    }

    if ( old_memory == new_memory ) {
#if defined(TRALLOC_DEBUG)
        old_chunk->length = length;
        if ( _tralloc_on_resize ( old_chunk, old_length ) != 0 ) {
            return NULL;
        }
#endif

        return old_chunk;
    } else {
        _tralloc_chunk * new_chunk = ( _tralloc_chunk * ) ( ( uintptr_t ) new_memory + extensions_length );
        _tralloc_usual_update_chunk ( new_chunk );

#if defined(TRALLOC_REFERENCE)
        if ( references ) {
            _tralloc_references_update_chunk ( new_chunk );
        } else if ( reference ) {
            _tralloc_reference_update_chunk ( new_chunk );
        }
#endif

#if defined(TRALLOC_DEBUG)
        new_chunk->length = length;
        if ( _tralloc_on_resize ( new_chunk, old_length ) != 0 ) {
            return NULL;
        }
#endif

        return new_chunk;
    }
}


uint8_t _tralloc_free_chunk ( _tralloc_chunk * chunk )
{

#if defined(TRALLOC_REFERENCE)
    bool references = ( chunk->extensions & TRALLOC_HAVE_REFERENCES ) != 0;
    if ( references ) {
        if ( !_tralloc_references_try_free_chunk ( chunk ) ) {
            return 0;
        }
    }
#endif

    uint8_t result, error = 0;

#if defined(TRALLOC_DEBUG)
    if ( ( result = _tralloc_on_free ( chunk ) ) != 0 ) {
        error = result;
    }
#endif

    uint8_t extensions_length = 0;

#if defined(TRALLOC_REFERENCE)
    if ( references ) {
        extensions_length += sizeof ( _tralloc_references );
    } else if ( ( chunk->extensions & TRALLOC_HAVE_REFERENCE ) != 0 ) {
        if ( ( result = _tralloc_reference_free_chunk ( chunk ) ) != 0 ) {
            error = result;
        }
        extensions_length += sizeof ( _tralloc_reference );
    }
#endif

#if defined(TRALLOC_DESTRUCTOR)
    if ( ( chunk->extensions & TRALLOC_HAVE_DESTRUCTORS ) != 0 ) {
        if ( ( result = _tralloc_destructor_free_chunk ( chunk ) ) != 0 ) {
            error = result;
        }
        extensions_length += sizeof ( _tralloc_destructors );
    }
#endif

    if ( ( result = _tralloc_free_chunk_children ( chunk ) ) != 0 ) {
        error = result;
    }

    void * memory = ( void * ) ( ( uintptr_t ) chunk - extensions_length );
    free ( memory );
    return error;
}

