// This file is part of tralloc. Copyright (C) 2013 Andrew Aladjev aladjev.andrew@gmail.com
// tralloc is free software: you can redistribute it and/or modify it under the terms of the GNU General Lesser Public License as published by the Free Software Foundation, either version 3 of the License, or (at your option) any later version.
// tralloc is distributed in the hope that it will be useful, but WITHOUT ANY WARRANTY; without even the implied warranty of MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU General Lesser Public License for more details.
// You should have received a copy of the GNU General Lesser Public License along with tralloc. If not, see <http://www.gnu.org/licenses/>.

#include "realloc.h"
#include "chunk.h"
#include "../common.h"

#if defined(TRALLOC_DEBUG)
#   include "../events.h"
#endif

#if defined(TRALLOC_LENGTH)
#   include "../length/chunk.h"
#endif

#if defined(TRALLOC_DESTRUCTOR)
#   include "../destructor/chunk.h"
#endif

#if defined(TRALLOC_REFERENCE)
#   include "../reference/head_chunk.h"
#   include "../reference/chunk.h"
#endif

#if defined(TRALLOC_POOL)
#   include "../pool/chunk.h"
#   include "../pool/head_chunk.h"
#endif

#include <stdlib.h>
#include <string.h>


tralloc_error tralloc_realloc ( tralloc_context ** chunk_context, size_t length )
{
    if ( chunk_context == NULL ) {
        return TRALLOC_ERROR_REQUIRED_ARGUMENT_IS_NULL;
    }
    tralloc_context * context = * chunk_context;
    if ( context == NULL ) {
        return TRALLOC_ERROR_REQUIRED_ARGUMENT_IS_NULL;
    }
    _tralloc_chunk * old_chunk = _tralloc_get_chunk_from_context ( context );
    tralloc_error result;

#   if defined(TRALLOC_DEBUG)
    size_t old_length;
    result = _tralloc_get_chunk_length ( old_chunk, &old_length );
    if ( result != 0 ) {
        return result;
    }
#   endif

    size_t extensions_length = 0;

#   if defined(TRALLOC_LENGTH)
    tralloc_bool have_length = old_chunk->extensions & TRALLOC_EXTENSION_LENGTH;
    if ( have_length ) {
        extensions_length += sizeof ( _tralloc_length );
    }
#   endif

#   if defined(TRALLOC_DESTRUCTOR)
    if ( old_chunk->extensions & TRALLOC_EXTENSION_DESTRUCTORS ) {
        extensions_length += sizeof ( _tralloc_destructors );
    }
#   endif

#   if defined(TRALLOC_REFERENCE)
    tralloc_bool have_references = old_chunk->extensions & TRALLOC_EXTENSION_REFERENCES;
    tralloc_bool have_reference  = old_chunk->extensions & TRALLOC_EXTENSION_REFERENCE;
    if ( have_references ) {
        extensions_length += sizeof ( _tralloc_references );
    } else if ( have_reference ) {
        extensions_length += sizeof ( _tralloc_reference );
    }
#   endif

#   if defined(TRALLOC_POOL)
    if ( old_chunk->extensions & TRALLOC_EXTENSION_POOL ) {
        return TRALLOC_ERROR_POOL_CANT_BE_REALLOCATED;
    }

    tralloc_bool have_pool_child = old_chunk->extensions & TRALLOC_EXTENSION_POOL_CHILD;
    if ( have_pool_child ) {
        extensions_length += sizeof ( _tralloc_pool_child );
    }
#   endif

    size_t total_length = sizeof ( _tralloc_chunk ) + extensions_length + length;
    void * old_memory   = ( void * ) ( ( uintptr_t ) old_chunk - extensions_length );
    void * new_memory;

#   if defined(TRALLOC_POOL)
    if ( have_pool_child ) {
        new_memory = _tralloc_pool_child_resize ( old_memory, total_length );
        if ( new_memory == NULL ) {
            // pool_child should be deleted
            _tralloc_pool_child * pool_child = _tralloc_get_pool_child_from_chunk ( old_chunk );
            size_t old_total_length = pool_child->length;

            old_chunk->extensions &= ~ ( TRALLOC_EXTENSION_POOL_CHILD );
            extensions_length -= sizeof ( _tralloc_pool_child );
            old_total_length  -= sizeof ( _tralloc_pool_child );
            total_length      -= sizeof ( _tralloc_pool_child );
            old_memory        += sizeof ( _tralloc_pool_child );

            new_memory = malloc ( total_length );
            if ( new_memory == NULL ) {
                return TRALLOC_ERROR_MALLOC_FAILED;
            }
            memmove ( new_memory, old_memory, old_total_length );

            result = _tralloc_pool_child_free_chunk ( old_chunk );
            if ( result != 0 ) {
                free ( new_memory );
                return result;
            }
        }
    } else {
        new_memory = realloc ( old_memory, total_length );
        if ( new_memory == NULL ) {
            return TRALLOC_ERROR_REALLOC_FAILED;
        }
    }
#   else
    new_memory = realloc ( old_memory, total_length );
    if ( new_memory == NULL ) {
        return TRALLOC_ERROR_REALLOC_FAILED;
    }
#   endif

    if ( old_memory == new_memory ) {

#       if defined(TRALLOC_LENGTH)
        if ( have_length ) {
            _tralloc_length_set ( old_chunk, length );
        }
#       endif

#       if defined(TRALLOC_DEBUG)
        result = _tralloc_set_chunk_length ( old_chunk, length );
        if ( result != 0 ) {
            return result;
        }
        return _tralloc_on_resize ( old_chunk, old_length );
#       endif

    } else {
        _tralloc_chunk * new_chunk = ( _tralloc_chunk * ) ( ( uintptr_t ) new_memory + extensions_length );
        _tralloc_update_chunk ( new_chunk );

#       if defined(TRALLOC_LENGTH)
        if ( have_length ) {
            _tralloc_length_set ( new_chunk, length );
        }
#       endif

#       if defined(TRALLOC_REFERENCE)
        if ( have_references ) {
            _tralloc_references_update_chunk ( new_chunk );
        } else if ( have_reference ) {
            _tralloc_reference_update_chunk ( new_chunk );
        }
#       endif

        * chunk_context = _tralloc_get_context_from_chunk ( new_chunk );

#       if defined(TRALLOC_DEBUG)
        result = _tralloc_set_chunk_length ( new_chunk, length );
        if ( result != 0 ) {
            return result;
        }
        return _tralloc_on_resize ( new_chunk, old_length );
#       endif

    }
    return 0;
}
