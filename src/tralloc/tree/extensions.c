// This file is part of tralloc. Copyright (C) 2013 Andrew Aladjev aladjev.andrew@gmail.com
// tralloc is free software: you can redistribute it and/or modify it under the terms of the GNU General Lesser Public License as published by the Free Software Foundation, either version 3 of the License, or (at your option) any later version.
// tralloc is distributed in the hope that it will be useful, but WITHOUT ANY WARRANTY; without even the implied warranty of MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU General Lesser Public License for more details.
// You should have received a copy of the GNU General Lesser Public License along with tralloc. If not, see <http://www.gnu.org/licenses/>.

#include "common.h"
#include "extensions.h"

#include "../chunk.h"

#if defined(TRALLOC_LENGTH)
#include "../length/chunk.h"
#endif

#if defined(TRALLOC_DESTRUCTOR)
#include "../destructor/chunk.h"
#endif

#if defined(TRALLOC_REFERENCE)
#include "../reference/head_chunk.h"
#include "../reference/chunk.h"
#endif

#include <stdlib.h>
#include <stdbool.h>


typedef tralloc_error ( * _allocator ) ( void ** data, size_t length );

static inline
tralloc_error _malloc ( void ** data, size_t length )
{
    void * _data = malloc ( length );
    if ( _data == NULL ) {
        return TRALLOC_ERROR_MALLOC_FAILED;
    } else {
        * data = _data;
        return 0;
    }
}

static inline
tralloc_error _calloc ( void ** data, size_t length )
{
    void * _data = calloc ( 1, length );
    if ( _data == NULL ) {
        return TRALLOC_ERROR_CALLOC_FAILED;
    } else {
        * data = _data;
        return 0;
    }
}

static inline
tralloc_error _tralloc_with_extensions_with_allocator ( tralloc_context * parent_context, tralloc_context ** child_context, tralloc_extensions extensions, size_t length, _allocator allocator )
{
    if ( child_context == NULL ) {
        return TRALLOC_ERROR_REQUIRED_ARGUMENT_IS_NULL;
    }

    size_t extensions_length = 0;

#if defined(TRALLOC_LENGTH)
    bool have_length = extensions & TRALLOC_EXTENSION_LENGTH;
    if ( have_length ) {
        extensions_length += sizeof ( _tralloc_length );
    }
#endif

#if defined(TRALLOC_DESTRUCTOR)
    bool have_destructors = extensions & TRALLOC_EXTENSION_DESTRUCTORS;
    if ( have_destructors ) {
        extensions_length += sizeof ( _tralloc_destructors );
    }
#endif

#if defined(TRALLOC_REFERENCE)
    bool have_references = extensions & TRALLOC_EXTENSION_REFERENCES;
    bool have_reference  = extensions & TRALLOC_EXTENSION_REFERENCE;
    if ( have_references ) {
        extensions_length += sizeof ( _tralloc_references );
        if ( have_reference ) {
            extensions &= ~ ( TRALLOC_EXTENSION_REFERENCE );
        }
    } else if ( have_reference ) {
        extensions_length += sizeof ( _tralloc_reference );
    }
#endif

    size_t chunk_length = sizeof ( _tralloc_chunk ) + extensions_length;
    void * memory;
    tralloc_error result = allocator ( &memory, chunk_length + length );
    if ( result != 0 ) {
        return result;
    }

    _tralloc_chunk * chunk = ( _tralloc_chunk * ) ( ( uintptr_t ) memory + extensions_length );
    chunk->extensions      = extensions;

#if defined(TRALLOC_DEBUG)
    chunk->chunk_length = chunk_length;
    chunk->length       = length;
#endif

#if defined(TRALLOC_LENGTH)
    if ( have_length ) {
        _tralloc_length_set ( chunk, length );
    }
#endif

#if defined(TRALLOC_DESTRUCTOR)
    if ( have_destructors ) {
        _tralloc_destructors_new_chunk ( chunk );
    }
#endif

#if defined(TRALLOC_REFERENCE)
    if ( have_references ) {
        _tralloc_references_new_chunk ( chunk );
    } else if ( have_reference ) {
        _tralloc_reference_new_chunk ( chunk );
    }
#endif

    result = _tralloc_add_chunk ( parent_context, chunk );
    if ( result != 0 ) {
        return result;
    }
    * child_context = _tralloc_context_from_chunk ( chunk );
    return 0;
}

tralloc_error tralloc_with_extensions ( tralloc_context * parent_context, tralloc_context ** child_context, tralloc_extensions extensions, size_t length )
{
    return _tralloc_with_extensions_with_allocator ( parent_context, child_context, extensions, length, _malloc );
}

extern inline tralloc_error tralloc_with_extensions ( tralloc_context * parent_context, tralloc_context ** child_context, tralloc_extensions extensions, size_t length );
extern inline tralloc_error tralloc                 ( tralloc_context * parent_context, tralloc_context ** child_context, size_t length );


tralloc_error tralloc_zero_with_extensions ( tralloc_context * parent_context, tralloc_context ** child_context, tralloc_extensions extensions, size_t length )
{
    return _tralloc_with_extensions_with_allocator ( parent_context, child_context, extensions, length, _calloc );
}

extern inline tralloc_error tralloc_zero_with_extensions ( tralloc_context * parent_context, tralloc_context ** child_context, tralloc_extensions extensions, size_t length );
extern inline tralloc_error tralloc_zero                 ( tralloc_context * parent_context, tralloc_context ** child_context, size_t length );

extern inline tralloc_error tralloc_new                 ( tralloc_context * parent_context, tralloc_context ** child_context );
extern inline tralloc_error tralloc_with_extensions_new ( tralloc_context * parent_context, tralloc_context ** child_context, tralloc_extensions extensions );


tralloc_error tralloc_realloc ( tralloc_context ** chunk_context, size_t length )
{
    if ( chunk_context == NULL ) {
        return TRALLOC_ERROR_REQUIRED_ARGUMENT_IS_NULL;
    }
    tralloc_context * context = * chunk_context;
    if ( context == NULL ) {
        return TRALLOC_ERROR_REQUIRED_ARGUMENT_IS_NULL;
    }
    _tralloc_chunk * old_chunk = _tralloc_chunk_from_context ( context );

#if defined(TRALLOC_DEBUG)
    size_t old_length = old_chunk->length;
#endif

    size_t extensions_length = 0;

#if defined(TRALLOC_LENGTH)
    bool have_length = old_chunk->extensions & TRALLOC_EXTENSION_LENGTH;
    if ( have_length ) {
        extensions_length += sizeof ( _tralloc_length );
    }
#endif

#if defined(TRALLOC_DESTRUCTOR)
    if ( old_chunk->extensions & TRALLOC_EXTENSION_DESTRUCTORS ) {
        extensions_length += sizeof ( _tralloc_destructors );
    }
#endif

#if defined(TRALLOC_REFERENCE)
    bool have_references = old_chunk->extensions & TRALLOC_EXTENSION_REFERENCES;
    bool have_reference  = old_chunk->extensions & TRALLOC_EXTENSION_REFERENCE;
    if ( have_references ) {
        extensions_length += sizeof ( _tralloc_references );
    } else if ( have_reference ) {
        extensions_length += sizeof ( _tralloc_reference );
    }
#endif

    void * old_memory = ( void * ) ( ( uintptr_t ) old_chunk - extensions_length );
    void * new_memory = realloc ( old_memory, sizeof ( _tralloc_chunk ) + extensions_length + length );
    if ( new_memory == NULL ) {
        return TRALLOC_ERROR_REALLOC_FAILED;
    }

    if ( old_memory == new_memory ) {

#if defined(TRALLOC_LENGTH)
        if ( have_length ) {
            _tralloc_length_set ( old_chunk, length );
        }
#endif

#if defined(TRALLOC_DEBUG)
        old_chunk->length = length;
        return _tralloc_on_resize ( old_chunk, old_length );
#endif

    } else {
        _tralloc_chunk * new_chunk = ( _tralloc_chunk * ) ( ( uintptr_t ) new_memory + extensions_length );
        _tralloc_usual_update_chunk ( new_chunk );

#if defined(TRALLOC_LENGTH)
        if ( have_length ) {
            _tralloc_length_set ( new_chunk, length );
        }
#endif

#if defined(TRALLOC_REFERENCE)
        if ( have_references ) {
            _tralloc_references_update_chunk ( new_chunk );
        } else if ( have_reference ) {
            _tralloc_reference_update_chunk ( new_chunk );
        }
#endif

        * chunk_context = _tralloc_context_from_chunk ( new_chunk );

#if defined(TRALLOC_DEBUG)
        new_chunk->length = length;
        return _tralloc_on_resize ( new_chunk, old_length );
#endif

    }
    return 0;
}


tralloc_error _tralloc_free_chunk ( _tralloc_chunk * chunk )
{

#if defined(TRALLOC_REFERENCE)
    bool have_references = chunk->extensions & TRALLOC_EXTENSION_REFERENCES;
    if ( have_references ) {
        if ( !_tralloc_references_try_free_chunk ( chunk ) ) {
            return 0;
        }
    }
#endif

    tralloc_error result, error = 0;

#if defined(TRALLOC_DEBUG)
    result = _tralloc_on_free ( chunk );
    if ( result != 0 ) {
        error = result;
    }
#endif

    size_t extensions_length = 0;

#if defined(TRALLOC_LENGTH)
    if ( chunk->extensions & TRALLOC_EXTENSION_LENGTH ) {
        extensions_length += sizeof ( _tralloc_length );
    }
#endif

#if defined(TRALLOC_REFERENCE)
    if ( have_references ) {
        extensions_length += sizeof ( _tralloc_references );
    } else if ( chunk->extensions & TRALLOC_EXTENSION_REFERENCE ) {
        result = _tralloc_reference_free_chunk ( chunk );
        if ( result != 0 ) {
            error = result;
        }
        extensions_length += sizeof ( _tralloc_reference );
    }
#endif

#if defined(TRALLOC_DESTRUCTOR)
    if ( chunk->extensions & TRALLOC_EXTENSION_DESTRUCTORS ) {
        result = _tralloc_destructor_free_chunk ( chunk );
        if ( result != 0 ) {
            error = result;
        }
        extensions_length += sizeof ( _tralloc_destructors );
    }
#endif

    result = _tralloc_free_chunk_children ( chunk );
    if ( result != 0 ) {
        error = result;
    }

    void * memory = ( void * ) ( ( uintptr_t ) chunk - extensions_length );
    free ( memory );
    return error;
}
