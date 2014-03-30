// This file is part of tralloc. Copyright (C) 2013 Andrew Aladjev aladjev.andrew@gmail.com
// tralloc is free software: you can redistribute it and/or modify it under the terms of the GNU General Lesser Public License as published by the Free Software Foundation, either version 3 of the License, or (at your option) any later version.
// tralloc is distributed in the hope that it will be useful, but WITHOUT ANY WARRANTY; without even the implied warranty of MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU General Lesser Public License for more details.
// You should have received a copy of the GNU General Lesser Public License along with tralloc. If not, see <http://www.gnu.org/licenses/>.

#define _TRALLOC_TREE_ALLOC_INCLUDED_FROM_OBJECT
#include "alloc.h"
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

    tralloc_error result;
    size_t extensions_length = 0;

#   if defined(TRALLOC_LENGTH)
    tralloc_bool have_length = extensions & TRALLOC_EXTENSION_LENGTH;
    if ( have_length ) {
        extensions_length += sizeof ( _tralloc_length );
    }
#   endif

#   if defined(TRALLOC_DESTRUCTOR)
    tralloc_bool have_destructors = extensions & TRALLOC_EXTENSION_DESTRUCTORS;
    if ( have_destructors ) {
        extensions_length += sizeof ( _tralloc_destructors );
    }
#   endif

#   if defined(TRALLOC_REFERENCE)
    tralloc_bool have_references = extensions & TRALLOC_EXTENSION_REFERENCES;
    tralloc_bool have_reference  = extensions & TRALLOC_EXTENSION_REFERENCE;
    if ( have_references ) {
        if ( have_reference ) {
            return TRALLOC_ERROR_BOTH_REFERENCES_AND_REFERENSE;
        }
        extensions_length += sizeof ( _tralloc_references );
    } else if ( have_reference ) {
        extensions_length += sizeof ( _tralloc_reference );
    }
#   endif

#   if defined(TRALLOC_POOL)
    _tralloc_pool * parent_pool;
    tralloc_bool have_pool = extensions & TRALLOC_EXTENSION_POOL;
    tralloc_bool have_pool_child;
    if ( have_pool ) {
        have_pool_child   = TRALLOC_FALSE;
        extensions        &= ~ ( TRALLOC_EXTENSION_POOL_CHILD );
        extensions_length += sizeof ( _tralloc_pool );
    } else {
        parent_pool = _tralloc_pool_child_get_pool_from_context ( parent_context );
        if ( parent_pool == NULL ) {
            have_pool_child = TRALLOC_FALSE;
            extensions      &= ~ ( TRALLOC_EXTENSION_POOL_CHILD );
        } else {
            have_pool_child   = TRALLOC_TRUE;
            extensions        |= TRALLOC_EXTENSION_POOL_CHILD;
            extensions_length += sizeof ( _tralloc_pool_child );
        }
    }
#   endif

    size_t chunk_length = sizeof ( _tralloc_chunk ) + extensions_length;
    size_t total_length = chunk_length + length;

#   if defined(TRALLOC_POOL)
    if ( have_pool_child ) {
        if ( !_tralloc_pool_can_alloc ( parent_pool, total_length ) ) {
            have_pool_child = TRALLOC_FALSE;
            extensions      &= ~ ( TRALLOC_EXTENSION_POOL_CHILD );

            extensions_length -= sizeof ( _tralloc_pool_child );
            chunk_length      -= sizeof ( _tralloc_pool_child );
            total_length      -= sizeof ( _tralloc_pool_child );
        }
    }
#   endif

    void * memory;

#   if defined(TRALLOC_POOL)
    _tralloc_pool_child * prev_pool_child, * next_pool_child;
    if ( have_pool_child ) {
        _tralloc_pool_alloc ( parent_pool, &memory, total_length, allocator == _calloc, &prev_pool_child, &next_pool_child );
    } else {
        result = allocator ( &memory, total_length );
        if ( result != 0 ) {
            return result;
        }
    }
#   else
    result = allocator ( &memory, total_length );
    if ( result != 0 ) {
        return result;
    }
#   endif

    _tralloc_chunk * chunk = ( _tralloc_chunk * ) ( ( uintptr_t ) memory + extensions_length );

#   if defined(TRALLOC_EXTENSIONS)
    chunk->extensions = extensions;
#   endif

#   if defined(TRALLOC_DEBUG)
    chunk->thread_id    = pthread_self();
    chunk->chunk_length = chunk_length;
    chunk->length       = length;
#   endif

#   if defined(TRALLOC_LENGTH)
    if ( have_length ) {
        _tralloc_length_set ( chunk, length );
    }
#   endif

#   if defined(TRALLOC_DESTRUCTOR)
    if ( have_destructors ) {
        _tralloc_destructors_new_chunk ( chunk );
    }
#   endif

#   if defined(TRALLOC_REFERENCE)
    if ( have_references ) {
        _tralloc_references_new_chunk ( chunk );
    } else if ( have_reference ) {
        _tralloc_reference_new_chunk ( chunk );
    }
#   endif

#   if defined(TRALLOC_POOL)
    if ( have_pool ) {
        _tralloc_pool_new_chunk ( chunk, length );
    } else if ( have_pool_child ) {
        _tralloc_pool_child_new_chunk ( chunk, parent_pool, total_length, prev_pool_child, next_pool_child );
    }
#   endif

    result = _tralloc_add_chunk ( parent_context, chunk );
    if ( result != 0 ) {

#       if defined(TRALLOC_POOL)
        if ( have_pool_child ) {
            _tralloc_pool_child_free_chunk ( chunk );
        } else {
            free ( memory );
        }
#       else
        free ( memory );
#       endif

        return result;
    }

    * child_context = _tralloc_get_context_from_chunk ( chunk );
    return 0;
}

tralloc_error tralloc_with_extensions ( tralloc_context * parent_context, tralloc_context ** child_context, tralloc_extensions extensions, size_t length )
{
    return _tralloc_with_extensions_with_allocator ( parent_context, child_context, extensions, length, _malloc );
}

tralloc_error tralloc_zero_with_extensions ( tralloc_context * parent_context, tralloc_context ** child_context, tralloc_extensions extensions, size_t length )
{
    return _tralloc_with_extensions_with_allocator ( parent_context, child_context, extensions, length, _calloc );
}