// This file is part of tralloc. Copyright (C) 2013 Andrew Aladjev aladjev.andrew@gmail.com
// tralloc is free software: you can redistribute it and/or modify it under the terms of the GNU General Lesser Public License as published by the Free Software Foundation, either version 3 of the License, or (at your option) any later version.
// tralloc is distributed in the hope that it will be useful, but WITHOUT ANY WARRANTY; without even the implied warranty of MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU General Lesser Public License for more details.
// You should have received a copy of the GNU General Lesser Public License along with tralloc. If not, see <http://www.gnu.org/licenses/>.

#include <tralloc/tree/realloc.h>
#include <tralloc/tree/chunk.h>
#include <tralloc/context.h>

#if defined ( TRALLOC_LENGTH )
#   include <tralloc/length/data.h>
#endif

#if defined ( TRALLOC_REFERENCES )
#   include <tralloc/references/chunk.h>
#   include <tralloc/references/references.h>
#   include <tralloc/references/reference.h>
#endif

#if defined ( TRALLOC_POOL )
#   include <tralloc/pool/chunk.h>
#   include <tralloc/pool/pool_child.h>
#endif

#if defined ( TRALLOC_DEBUG )
#   include <tralloc/debug/events.h>
#endif

#include <stdlib.h>
#include <string.h>


tralloc_error tralloc_realloc ( tralloc_context ** chunk_context, size_t length )
{
    tralloc_context * context;
    if ( chunk_context == NULL || ( context = * chunk_context ) == NULL ) {
        return TRALLOC_ERROR_REQUIRED_ARGUMENT_IS_UNDEFINED;
    }

    _tralloc_chunk * old_chunk = _tralloc_context_get_chunk ( context );
    tralloc_error _TRALLOC_UNUSED ( result );

#   if defined ( TRALLOC_DEBUG_LENGTH )
    size_t old_length = _tralloc_length_get ( _tralloc_chunk_get_length ( old_chunk ) );
#   endif

#   if defined ( TRALLOC_DEBUG )
    // Debug should care about thread safety of operations with "old_chunk" by itself.
    result = _tralloc_debug_before_resize_chunk ( old_chunk );
    if ( result != 0 ) {
        return result;
    }
#   endif

#   if defined ( TRALLOC_LENGTH )
    tralloc_bool have_length = _tralloc_extensions_have_extension ( old_chunk->extensions, TRALLOC_EXTENSION_LENGTH );
#   endif

#   if defined ( TRALLOC_REFERENCES )
    tralloc_bool have_references = _tralloc_extensions_have_extension ( old_chunk->extensions, TRALLOC_EXTENSION_REFERENCES );
    tralloc_bool have_reference  = _tralloc_extensions_have_extension ( old_chunk->extensions, TRALLOC_EXTENSION_REFERENCE );
#   endif

#   if defined ( TRALLOC_POOL )
    tralloc_bool have_pool_child = _tralloc_extensions_have_extension ( old_chunk->extensions, TRALLOC_EXTENSION_POOL_CHILD );
    tralloc_bool have_pool       = _tralloc_extensions_have_extension ( old_chunk->extensions, TRALLOC_EXTENSION_POOL );
    if ( have_pool ) {
        // Realloc function can change pool's pointer, so all pool children's pointers should be updated.
        // But this is not possible, so pool should not be resized.
        return TRALLOC_ERROR_POOL_CANT_BE_REALLOCATED;
    }
#   endif

#   if defined ( TRALLOC_EXTENSIONS )
    size_t extensions_length = _tralloc_extensions_get_length ( old_chunk->extensions );
#   else
    size_t extensions_length = 0;
#   endif

    size_t total_length = sizeof ( _tralloc_chunk ) + extensions_length + length;
    void * old_memory   = ( void * ) ( ( uintptr_t ) old_chunk - extensions_length );
    void * new_memory;

#   if defined ( TRALLOC_POOL )
    if ( have_pool_child ) {
        // "pool_child" should be the first in the stack of extensions.
        _tralloc_pool_child * old_pool_child = old_memory;
        new_memory = _tralloc_pool_child_resize ( old_pool_child, total_length );
        if ( new_memory == NULL ) {
            // "old_chunk" can't be resized, because there is not enough memory in pool.
            // "old_chunk"'s data is still valid.

            size_t old_total_length = old_pool_child->length;

            // TRALLOC_EXTENSION_POOL_CHILD will be disabled in "new_chunk".
            extensions_length -= sizeof ( _tralloc_pool_child );
            old_total_length  -= sizeof ( _tralloc_pool_child );
            total_length      -= sizeof ( _tralloc_pool_child );

            // "old_chunk" should be copied to new location with enough memory.
            old_memory = ( void * ) ( ( uintptr_t ) old_memory + sizeof ( _tralloc_pool_child ) );
            new_memory = malloc ( total_length );
            if ( new_memory == NULL ) {
                return TRALLOC_ERROR_MALLOC_FAILED;
            }
            memmove ( new_memory, old_memory, old_total_length );

            result = _tralloc_pool_child_free ( old_pool_child );
            if ( result != 0 ) {
                free ( new_memory );
                return result;
            }

            // TRALLOC_EXTENSION_POOL_CHILD should be disabled in "new_chunk".
            _tralloc_chunk * new_chunk = ( _tralloc_chunk * ) ( ( uintptr_t ) new_memory + extensions_length );
            _tralloc_extensions_disable_extension ( &new_chunk->extensions, TRALLOC_EXTENSION_POOL_CHILD );
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

#       if defined ( TRALLOC_LENGTH )
        if ( have_length ) {
            _tralloc_length_set ( _tralloc_chunk_get_length ( old_chunk ), length );
        }
#       endif

#       if defined ( TRALLOC_DEBUG )
        // Debug should care about thread safety of operations with "old_chunk" by itself.

#       if defined ( TRALLOC_DEBUG_LENGTH )
        return _tralloc_debug_after_resize_chunk ( old_chunk, old_length );
#       else
        return _tralloc_debug_after_resize_chunk ( old_chunk );
#       endif

#       endif

    } else {
        _tralloc_chunk * new_chunk = ( _tralloc_chunk * ) ( ( uintptr_t ) new_memory + extensions_length );

        _tralloc_chunk_update ( new_chunk );

#       if defined ( TRALLOC_LENGTH )
        if ( have_length ) {
            _tralloc_length_set ( _tralloc_chunk_get_length ( new_chunk ), length );
        }
#       endif

#       if defined ( TRALLOC_REFERENCES )
        if ( have_references ) {
            _tralloc_references * references = _tralloc_chunk_get_references ( new_chunk );
            _tralloc_references_update ( references );
        }
        if ( have_reference ) {
            _tralloc_reference * reference = _tralloc_chunk_get_reference ( new_chunk );
            _tralloc_reference_update ( reference );
        }
#       endif

        * chunk_context = _tralloc_chunk_get_context ( new_chunk );

#       if defined ( TRALLOC_DEBUG )
        // Debug should care about thread safety of operations with "new_chunk" by itself.

#       if defined ( TRALLOC_DEBUG_LENGTH )
        return _tralloc_debug_after_resize_chunk ( new_chunk, old_length );
#       else
        return _tralloc_debug_after_resize_chunk ( new_chunk );
#       endif

#       endif

    }

    return 0;
}
