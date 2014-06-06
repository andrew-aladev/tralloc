// This file is part of tralloc. Copyright (C) 2013 Andrew Aladjev aladjev.andrew@gmail.com
// tralloc is free software: you can redistribute it and/or modify it under the terms of the GNU General Lesser Public License as published by the Free Software Foundation, either version 3 of the License, or (at your option) any later version.
// tralloc is distributed in the hope that it will be useful, but WITHOUT ANY WARRANTY; without even the implied warranty of MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU General Lesser Public License for more details.
// You should have received a copy of the GNU General Lesser Public License along with tralloc. If not, see <http://www.gnu.org/licenses/>.

#define _TRALLOC_TREE_ALLOC_INCLUDED_FROM_OBJECT
#include <tralloc/tree/alloc.h>
#include <tralloc/tree/chunk.h>
#include <tralloc/common.h>

#if defined(TRALLOC_DEBUG)
#   include <tralloc/debug/events.h>
#endif

#if defined(TRALLOC_THREADS)
#   include <tralloc/threads/chunk.h>
#   include <tralloc/threads/mutex.h>
#endif

#if defined(TRALLOC_LENGTH)
#   include <tralloc/length/chunk.h>
#endif

#if defined(TRALLOC_DESTRUCTOR)
#   include <tralloc/destructor/chunk.h>
#endif

#if defined(TRALLOC_REFERENCE)
#   include <tralloc/reference/head_chunk.h>
#   include <tralloc/reference/chunk.h>
#endif

#if defined(TRALLOC_POOL)
#   include <tralloc/pool/chunk.h>
#   include <tralloc/pool/head_chunk.h>
#endif

#include <stdlib.h>


typedef tralloc_error ( * _allocator ) ( void ** data, size_t length );

static inline
tralloc_error _tralloc_malloc ( void ** data, size_t length )
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
tralloc_error _tralloc_calloc ( void ** data, size_t length )
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

#if defined(TRALLOC_DEBUG_LOG)
tralloc_error _tralloc_with_extensions_with_allocator ( const char * file, size_t line, tralloc_context * parent_context, tralloc_context ** child_context, tralloc_extensions _TRALLOC_UNUSED ( extensions ), size_t length, _allocator allocator )
#else
tralloc_error _tralloc_with_extensions_with_allocator ( tralloc_context * parent_context, tralloc_context ** child_context, tralloc_extensions _TRALLOC_UNUSED ( extensions ), size_t length, _allocator allocator )
#endif

{
    if ( child_context == NULL ) {
        return TRALLOC_ERROR_REQUIRED_ARGUMENT_IS_NULL;
    }

    tralloc_error result;
    _tralloc_chunk * parent_chunk;

    if ( parent_context == NULL ) {
        parent_chunk = NULL;
    } else {
        parent_chunk = _tralloc_get_chunk_from_context ( parent_context );
    }

    size_t extensions_length = 0;

#   if defined(TRALLOC_THREADS)
    tralloc_bool have_lock_subtree = extensions & TRALLOC_EXTENSION_LOCK_SUBTREE;
    if ( have_lock_subtree ) {
        extensions_length += sizeof ( _tralloc_mutex );
    }

    tralloc_bool have_lock_children = extensions & TRALLOC_EXTENSION_LOCK_CHILDREN;
    if ( have_lock_children ) {
        extensions_length += sizeof ( _tralloc_mutex );
    }
#   endif

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
    // Chunk can have both "TRALLOC_EXTENSION_REFERENCES" and "TRALLOC_EXTENSION_REFERENCE".

    tralloc_bool have_references = extensions & TRALLOC_EXTENSION_REFERENCES;
    if ( have_references ) {
        extensions_length += sizeof ( _tralloc_references );
    }
    tralloc_bool have_reference = extensions & TRALLOC_EXTENSION_REFERENCE;
    if ( have_reference ) {
        extensions_length += sizeof ( _tralloc_reference );
    }
#   endif

#   if defined(TRALLOC_POOL)
    // Chunk can't have both "TRALLOC_EXTENSION_POOL" and "TRALLOC_EXTENSION_POOL_CHILD".
    // "TRALLOC_EXTENSION_POOL_CHILD" will be set automatically.

    _tralloc_pool * parent_pool;
    tralloc_bool have_pool = extensions & TRALLOC_EXTENSION_POOL;
    tralloc_bool have_pool_child;

    if ( have_pool ) {
        have_pool_child   = TRALLOC_FALSE;
        extensions        &= ~ ( TRALLOC_EXTENSION_POOL_CHILD );
        extensions_length += sizeof ( _tralloc_pool );
    } else {
        if (
            parent_chunk != NULL &&
            ( parent_pool = _tralloc_pool_child_get_pool_from_chunk ( parent_chunk ) ) != NULL
        ) {
            have_pool_child   = TRALLOC_TRUE;
            extensions        |= TRALLOC_EXTENSION_POOL_CHILD;
            extensions_length += sizeof ( _tralloc_pool_child );
        } else {
            have_pool_child = TRALLOC_FALSE;
            extensions      &= ~ ( TRALLOC_EXTENSION_POOL_CHILD );
        }
    }
#   endif

    size_t chunk_length = sizeof ( _tralloc_chunk ) + extensions_length;
    size_t total_length = chunk_length + length;

#   if defined(TRALLOC_POOL)
    if ( have_pool_child ) {
        if ( !_tralloc_pool_can_alloc ( parent_pool, total_length ) ) {
            // "TRALLOC_EXTENSION_POOL_CHILD" should be disabled.

            have_pool_child = TRALLOC_FALSE;
            extensions      &= ~ ( TRALLOC_EXTENSION_POOL_CHILD );

            extensions_length -= sizeof ( _tralloc_pool_child );
            chunk_length      -= sizeof ( _tralloc_pool_child );
            total_length      -= sizeof ( _tralloc_pool_child );
        }
    }
#   endif

#   if defined(TRALLOC_DEBUG)
    result = _tralloc_debug_before_add_chunk ( parent_chunk, extensions, chunk_length, length );
    if ( result != 0 ) {
        return result;
    }
#   endif

    void * memory;

#   if defined(TRALLOC_POOL)
    _tralloc_pool_child * prev_pool_child, * next_pool_child;
    if ( have_pool_child ) {
        _tralloc_pool_alloc ( parent_pool, &memory, total_length, allocator == _tralloc_calloc, &prev_pool_child, &next_pool_child );
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

#   if defined(TRALLOC_POOL)
    if ( have_pool_child ) {
        _tralloc_pool_child_new_chunk ( chunk, parent_pool, total_length, prev_pool_child, next_pool_child );
    }
#   endif

#   if defined(TRALLOC_THREADS)
    _tralloc_mutex * subtree_mutex;
    if ( have_lock_subtree ) {
        subtree_mutex = _tralloc_get_subtree_mutex_from_chunk ( chunk );
        result = _tralloc_mutex_new ( subtree_mutex );
        if ( result != 0 ) {

#           if defined(TRALLOC_POOL)
            if ( have_pool_child ) {
                _tralloc_pool_child_free_chunk ( chunk );
            } else {
                free ( memory );
            }
#           else
            free ( memory );
#           endif

            return result;
        }
    }

    _tralloc_mutex * children_mutex;
    if ( have_lock_children ) {
        children_mutex = _tralloc_get_children_mutex_from_chunk ( chunk );
        result = _tralloc_mutex_new ( children_mutex );
        if ( result != 0 ) {

            if ( have_lock_subtree ) {
                _tralloc_mutex_free ( subtree_mutex );
            }

#           if defined(TRALLOC_POOL)
            if ( have_pool_child ) {
                _tralloc_pool_child_free_chunk ( chunk );
            } else {
                free ( memory );
            }
#           else
            free ( memory );
#           endif

            return result;
        }
    }
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
    }
    if ( have_reference ) {
        _tralloc_reference_new_chunk ( chunk );
    }
#   endif

#   if defined(TRALLOC_POOL)
    if ( have_pool ) {
        _tralloc_pool_new_chunk ( chunk, length );
    }
#   endif

    chunk->parent      = NULL;
    chunk->first_child = NULL;
    chunk->prev        = NULL;
    chunk->next        = NULL;

#   if defined(TRALLOC_THREADS)
    tralloc_bool parent_is_locking_children = parent_chunk != NULL && ( parent_chunk->extensions & TRALLOC_EXTENSION_LOCK_CHILDREN );

    _tralloc_mutex * parent_children_mutex;
    if ( parent_is_locking_children ) {
        parent_children_mutex = _tralloc_get_children_mutex_from_chunk ( parent_chunk );
        result = _tralloc_mutex_lock ( parent_children_mutex );
        if ( result != 0 ) {
            if ( have_lock_subtree ) {
                _tralloc_mutex_free ( subtree_mutex );
            }
            if ( have_lock_children ) {
                _tralloc_mutex_free ( children_mutex );
            }

#           if defined(TRALLOC_DESTRUCTOR)
            if ( have_destructors ) {
                _tralloc_destructors_free_chunk ( chunk );
            }
#           endif

#           if defined(TRALLOC_REFERENCE)
            if ( have_reference ) {
                _tralloc_reference_free_chunk ( chunk );
            }
#           endif

#           if defined(TRALLOC_POOL)
            if ( have_pool_child ) {
                _tralloc_pool_child_free_chunk ( chunk );
            } else {
                free ( memory );
            }
#           else
            free ( memory );
#           endif

            return result;
        }
    }
#   endif

    if ( parent_chunk != NULL ) {
        _tralloc_attach_chunk ( chunk, parent_chunk );
    }

#   if defined(TRALLOC_THREADS)
    if ( parent_is_locking_children ) {
        result = _tralloc_mutex_unlock ( parent_children_mutex );
        if ( result != 0 ) {

            if ( parent_chunk != NULL ) {
                _tralloc_attach_chunk ( chunk, NULL );
            }

            if ( have_lock_subtree ) {
                _tralloc_mutex_free ( subtree_mutex );
            }
            if ( have_lock_children ) {
                _tralloc_mutex_free ( children_mutex );
            }

#           if defined(TRALLOC_DESTRUCTOR)
            if ( have_destructors ) {
                _tralloc_destructors_free_chunk ( chunk );
            }
#           endif

#           if defined(TRALLOC_REFERENCE)
            if ( have_reference ) {
                _tralloc_reference_free_chunk ( chunk );
            }
#           endif

#           if defined(TRALLOC_POOL)
            if ( have_pool_child ) {
                _tralloc_pool_child_free_chunk ( chunk );
            } else {
                free ( memory );
            }
#           else
            free ( memory );
#           endif

            return result;
        }
    }
#   endif

#   if defined(TRALLOC_DEBUG)

#   if defined(TRALLOC_DEBUG_LOG)
    result = _tralloc_debug_after_add_chunk ( chunk, chunk_length, length, file, line );
#   else
    result = _tralloc_debug_after_add_chunk ( chunk, chunk_length, length );
#   endif

    if ( result != 0 ) {
        if ( parent_chunk != NULL ) {
            _tralloc_attach_chunk ( chunk, NULL );
        }

#       if defined(TRALLOC_THREADS)
        if ( have_lock_subtree ) {
            _tralloc_mutex_free ( subtree_mutex );
        }
        if ( have_lock_children ) {
            _tralloc_mutex_free ( children_mutex );
        }
#       endif

#       if defined(TRALLOC_DESTRUCTOR)
        if ( have_destructors ) {
            _tralloc_destructors_free_chunk ( chunk );
        }
#       endif

#       if defined(TRALLOC_REFERENCE)
        if ( have_reference ) {
            _tralloc_reference_free_chunk ( chunk );
        }
#       endif

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
#   endif

    * child_context = _tralloc_get_context_from_chunk ( chunk );
    return 0;
}


#if defined(TRALLOC_DEBUG_LOG)

tralloc_error tralloc_with_extensions ( const char * file, size_t line, tralloc_context * parent_context, tralloc_context ** child_context, tralloc_extensions extensions, size_t length )
{
    return _tralloc_with_extensions_with_allocator ( file, line, parent_context, child_context, extensions, length, _tralloc_malloc );
}

tralloc_error tralloc_zero_with_extensions ( const char * file, size_t line, tralloc_context * parent_context, tralloc_context ** child_context, tralloc_extensions extensions, size_t length )
{
    return _tralloc_with_extensions_with_allocator ( file, line, parent_context, child_context, extensions, length, _tralloc_calloc );
}

#else

tralloc_error tralloc_with_extensions ( tralloc_context * parent_context, tralloc_context ** child_context, tralloc_extensions extensions, size_t length )
{
    return _tralloc_with_extensions_with_allocator ( parent_context, child_context, extensions, length, _tralloc_malloc );
}

tralloc_error tralloc_zero_with_extensions ( tralloc_context * parent_context, tralloc_context ** child_context, tralloc_extensions extensions, size_t length )
{
    return _tralloc_with_extensions_with_allocator ( parent_context, child_context, extensions, length, _tralloc_calloc );
}

#endif
