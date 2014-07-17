// This file is part of tralloc. Copyright (C) 2013 Andrew Aladjev aladjev.andrew@gmail.com
// tralloc is free software: you can redistribute it and/or modify it under the terms of the GNU General Lesser Public License as published by the Free Software Foundation, either version 3 of the License, or (at your option) any later version.
// tralloc is distributed in the hope that it will be useful, but WITHOUT ANY WARRANTY; without even the implied warranty of MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU General Lesser Public License for more details.
// You should have received a copy of the GNU General Lesser Public License along with tralloc. If not, see <http://www.gnu.org/licenses/>.

#define _TRALLOC_INCLUDED_FROM_TREE_ALLOC_C
#include <tralloc/tree/alloc.h>
#include <tralloc/tree/chunk.h>
#include <tralloc/common.h>

#if defined ( TRALLOC_DEBUG )
#   include <tralloc/debug/events.h>
#endif

#if defined ( TRALLOC_THREADS )
#   include <tralloc/threads/chunk.h>
#   include <tralloc/threads/lock/subtree.h>
#   include <tralloc/threads/lock/children.h>
#endif

#if defined ( TRALLOC_LENGTH )
#   include <tralloc/length/chunk.h>
#endif

#if defined ( TRALLOC_DESTRUCTOR )
#   include <tralloc/destructor/chunk.h>
#   include <tralloc/destructor/destructors.h>
#endif

#if defined ( TRALLOC_REFERENCE )
#   include <tralloc/reference/chunk.h>
#   include <tralloc/reference/references.h>
#   include <tralloc/reference/reference.h>
#endif

#if defined ( TRALLOC_POOL )
#   include <tralloc/pool/chunk.h>
#   include <tralloc/pool/pool.h>
#   include <tralloc/pool/pool_child.h>
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

#if defined ( TRALLOC_DEBUG_LOG )
tralloc_error _tralloc_debug_log_new_with_extensions_with_allocator ( const char * file, size_t line, tralloc_context * parent_context, tralloc_context ** child_context, tralloc_extensions _TRALLOC_UNUSED ( extensions ), size_t length, _allocator allocator )
#else
tralloc_error _tralloc_new_with_extensions_with_allocator ( tralloc_context * parent_context, tralloc_context ** child_context, tralloc_extensions _TRALLOC_UNUSED ( extensions ), size_t length, _allocator allocator )
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

#   if defined ( TRALLOC_THREADS )
    tralloc_bool have_subtree_lock  = extensions & TRALLOC_EXTENSION_LOCK_SUBTREE;
    tralloc_bool have_children_lock = extensions & TRALLOC_EXTENSION_LOCK_CHILDREN;
#   endif

#   if defined ( TRALLOC_LENGTH )
    tralloc_bool have_length = extensions & TRALLOC_EXTENSION_LENGTH;
#   endif

#   if defined ( TRALLOC_DESTRUCTOR )
    tralloc_bool have_destructors = extensions & TRALLOC_EXTENSION_DESTRUCTORS;
#   endif

#   if defined ( TRALLOC_REFERENCE )
    // Chunk can have both "TRALLOC_EXTENSION_REFERENCES" and "TRALLOC_EXTENSION_REFERENCE".

    tralloc_bool have_references = extensions & TRALLOC_EXTENSION_REFERENCES;
    tralloc_bool have_reference  = extensions & TRALLOC_EXTENSION_REFERENCE;
#   endif

#   if defined ( TRALLOC_POOL )
    // Chunk can't have both "TRALLOC_EXTENSION_POOL" and "TRALLOC_EXTENSION_POOL_CHILD".
    // "TRALLOC_EXTENSION_POOL_CHILD" will be set automatically.

    _tralloc_pool * parent_pool;
    tralloc_bool have_pool = extensions & TRALLOC_EXTENSION_POOL;
    tralloc_bool have_pool_child;

    if ( have_pool ) {
        have_pool_child = TRALLOC_FALSE;
        extensions      &= ~ ( TRALLOC_EXTENSION_POOL_CHILD );
    } else {
        if (
            parent_chunk != NULL &&
            ( parent_pool = _tralloc_pool_child_get_pool_from_chunk ( parent_chunk ) ) != NULL
        ) {
            have_pool_child = TRALLOC_TRUE;
            extensions      |= TRALLOC_EXTENSION_POOL_CHILD;
        } else {
            have_pool_child = TRALLOC_FALSE;
            extensions      &= ~ ( TRALLOC_EXTENSION_POOL_CHILD );
        }
    }
#   endif

    size_t extensions_length = _tralloc_get_extensions_length ( extensions );
    size_t chunk_length      = sizeof ( _tralloc_chunk ) + extensions_length;
    size_t total_length      = chunk_length + length;

#   if defined ( TRALLOC_POOL )
    if ( have_pool_child ) {
        if ( !_tralloc_can_alloc_from_pool ( parent_pool, total_length ) ) {
            // "parent_pool" can't alloc "total_length" bytes.
            // "TRALLOC_EXTENSION_POOL_CHILD" for new chunk should be disabled.

            have_pool_child = TRALLOC_FALSE;
            extensions      &= ~ ( TRALLOC_EXTENSION_POOL_CHILD );

            extensions_length -= sizeof ( _tralloc_pool_child );
            chunk_length      -= sizeof ( _tralloc_pool_child );
            total_length      -= sizeof ( _tralloc_pool_child );
        }
    }
#   endif

#   if defined ( TRALLOC_DEBUG )
    // Debug should care about thread safety of operations with "parent_chunk" by itself.
    result = _tralloc_debug_before_add_chunk ( parent_chunk, extensions, chunk_length, length );
    if ( result != 0 ) {
        return result;
    }
#   endif

    void * memory;

#   if defined ( TRALLOC_POOL )
    _tralloc_pool_child * prev_pool_child, * next_pool_child;
    if ( have_pool_child ) {
        _tralloc_alloc_from_pool ( parent_pool, &memory, total_length, allocator == _tralloc_calloc, &prev_pool_child, &next_pool_child );
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

    _tralloc_chunk * chunk    = ( _tralloc_chunk * ) ( ( uintptr_t ) memory + extensions_length );
    tralloc_context * context = _tralloc_get_context_from_chunk ( chunk );

#   if defined ( TRALLOC_EXTENSIONS )
    chunk->extensions = extensions;
#   endif

#   if defined ( TRALLOC_POOL )
    _tralloc_pool_child * pool_child;
    if ( have_pool_child ) {
        pool_child = _tralloc_get_pool_child_from_chunk ( chunk );
        _tralloc_new_pool_child ( pool_child, parent_pool, total_length, prev_pool_child, next_pool_child );
    }
#   endif

#   if defined ( TRALLOC_THREADS )
    _tralloc_subtree_lock * subtree_lock;
    if ( have_subtree_lock ) {
        subtree_lock = _tralloc_get_subtree_lock_from_chunk ( chunk );
        result = _tralloc_new_subtree_lock ( subtree_lock );
        if ( result != 0 ) {
            // It is time to do emergency exit.

            // Memory has been allocated in the pool or global memory, it should be freed.
#           if defined ( TRALLOC_POOL )
            if ( have_pool_child ) {
                _tralloc_free_pool_child ( pool_child );
            } else {
                free ( memory );
            }
#           else
            free ( memory );
#           endif

            return result;
        }
    }

    _tralloc_children_lock * children_lock;
    if ( have_children_lock ) {
        children_lock = _tralloc_get_children_lock_from_chunk ( chunk );
        result = _tralloc_new_children_lock ( children_lock );
        if ( result != 0 ) {
            // It is time to do emergency exit.

            // Subtree mutex of "chunk" has been allocated, it should be freed.
            if ( have_subtree_lock ) {
                _tralloc_free_subtree_lock ( subtree_lock );
            }

            // Memory has been allocated in the pool or global memory, it should be freed.
#           if defined ( TRALLOC_POOL )
            if ( have_pool_child ) {
                _tralloc_free_pool_child ( pool_child );
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

#   if defined ( TRALLOC_LENGTH )
    if ( have_length ) {
        _tralloc_length * length_ptr = _tralloc_get_length_from_chunk ( chunk );
        * length_ptr = length;
    }
#   endif

#   if defined ( TRALLOC_DESTRUCTOR )
    _tralloc_destructors * destructors;
    if ( have_destructors ) {
        destructors = _tralloc_get_destructors_from_chunk ( chunk );
        _tralloc_new_destructors ( destructors );
    }
#   endif

#   if defined ( TRALLOC_REFERENCE )
    _tralloc_references * references;
    if ( have_references ) {
        references = _tralloc_get_references_from_chunk ( chunk );
        _tralloc_new_references ( references, extensions );
    }
    _tralloc_reference * reference;
    if ( have_reference ) {
        reference = _tralloc_get_reference_from_chunk ( chunk );
        _tralloc_new_reference ( reference );
    }
#   endif

#   if defined ( TRALLOC_POOL )
    _tralloc_pool * pool;
    if ( have_pool ) {
        pool = _tralloc_get_pool_from_chunk ( chunk );
        _tralloc_new_pool ( pool, context, extensions, length );
    }
#   endif

    chunk->parent      = NULL;
    chunk->first_child = NULL;
    chunk->prev        = NULL;
    chunk->next        = NULL;

#   if defined ( TRALLOC_THREADS )
    tralloc_bool parent_have_children_lock;
    _tralloc_children_lock * parent_children_lock;
#   endif

    if ( parent_chunk != NULL ) {

#       if defined ( TRALLOC_THREADS )

#       if defined ( TRALLOC_DEBUG_THREADS )
        // Locks from extensions are inactive in debug threads mode.
        parent_have_children_lock = TRALLOC_TRUE;
        parent_children_lock = &parent_chunk->children_lock;
#       else
        parent_have_children_lock = parent_chunk->extensions & TRALLOC_EXTENSION_LOCK_CHILDREN;
        if ( parent_have_children_lock ) {
            parent_children_lock = _tralloc_get_children_lock_from_chunk ( parent_chunk );
        }
#       endif

        if ( parent_have_children_lock ) {
            result = _tralloc_wrlock_children ( parent_children_lock );
            if ( result != 0 ) {
                // It is time to do emergency exit.

                // Subtree mutex of "chunk" has been allocated, it should be freed.
                if ( have_subtree_lock ) {
                    _tralloc_free_subtree_lock ( subtree_lock );
                }
                // Children mutex of "chunk" has been allocated, it should be freed.
                if ( have_children_lock ) {
                    _tralloc_free_children_lock ( children_lock );
                }

#               if defined ( TRALLOC_DESTRUCTOR )
                // Destructors of "chunk" has been allocated, it should be freed.
                if ( have_destructors ) {
                    _tralloc_free_destructors ( destructors, context );
                }
#               endif

#               if defined ( TRALLOC_REFERENCE )
                // Reference of "chunk" has been allocated, it should be freed.
                if ( have_reference ) {
                    _tralloc_free_reference ( reference );
                }
#               endif

                // Memory has been allocated in the pool or global memory, it should be freed.
#               if defined ( TRALLOC_POOL )
                if ( have_pool_child ) {
                    _tralloc_free_pool_child ( pool_child );
                } else {
                    free ( memory );
                }
#               else
                free ( memory );
#               endif

                return result;
            }
        }
#       endif

        _tralloc_attach_chunk ( chunk, parent_chunk );

#       if defined ( TRALLOC_THREADS )
        if ( parent_have_children_lock ) {
            result = _tralloc_unlock_children ( parent_children_lock );
            if ( result != 0 ) {
                // It is time to do emergency exit.
                // "parent_children_mutex" have locked status.

                // Chunk should be detached.
                _tralloc_attach_chunk ( chunk, NULL );

                // Subtree mutex of "chunk" has been allocated, it should be freed.
                if ( have_subtree_lock ) {
                    _tralloc_free_subtree_lock ( subtree_lock );
                }
                // Children mutex of "chunk" has been allocated, it should be freed.
                if ( have_children_lock ) {
                    _tralloc_free_children_lock ( children_lock );
                }

#               if defined ( TRALLOC_DESTRUCTOR )
                // Destructors of "chunk" has been allocated, it should be freed.
                if ( have_destructors ) {
                    _tralloc_free_destructors ( destructors, context );
                }
#               endif

#               if defined ( TRALLOC_REFERENCE )
                // Reference of "chunk" has been allocated, it should be freed.
                if ( have_reference ) {
                    _tralloc_free_reference ( reference );
                }
#               endif

                // Memory has been allocated in the pool or global memory, it should be freed.
#               if defined ( TRALLOC_POOL )
                if ( have_pool_child ) {
                    _tralloc_free_pool_child ( pool_child );
                } else {
                    free ( memory );
                }
#               else
                free ( memory );
#               endif

                return result;
            }
        }
#       endif

    }

#   if defined ( TRALLOC_DEBUG )

    // Debug should care about thread safety of operations with "chunk" by itself.
#   if defined ( TRALLOC_DEBUG_LOG )
    result = _tralloc_debug_after_add_chunk ( chunk, chunk_length, length, file, line );
#   else
    result = _tralloc_debug_after_add_chunk ( chunk, chunk_length, length );
#   endif

    if ( result != 0 ) {
        // It is time to do emergency exit.

        tralloc_error _TRALLOC_UNUSED ( error ) = result;

        if ( parent_chunk != NULL ) {

#           if defined ( TRALLOC_THREADS )
            if ( parent_have_children_lock ) {
                result = _tralloc_wrlock_children ( parent_children_lock );
                if ( result != 0 ) {
                    // It is not possible to delete chunk directly.
                    // Chunk will be deleted normally by it's parent.
                    return error;
                }
            }
#           endif

            // Chunk should be detached.
            _tralloc_attach_chunk ( chunk, NULL );

#           if defined ( TRALLOC_THREADS )
            if ( parent_have_children_lock ) {
                _tralloc_unlock_children ( parent_children_lock );
            }
#           endif
        }

#       if defined ( TRALLOC_THREADS )
        // Subtree mutex of "chunk" has been allocated, it should be freed.
        if ( have_subtree_lock ) {
            _tralloc_free_subtree_lock ( subtree_lock );
        }
        // Children mutex of "chunk" has been allocated, it should be freed.
        if ( have_children_lock ) {
            _tralloc_free_children_lock ( children_lock );
        }
#       endif

#       if defined ( TRALLOC_DESTRUCTOR )
        // Destructors of "chunk" has been allocated, it should be freed.
        if ( have_destructors ) {
            _tralloc_free_destructors ( destructors, context );
        }
#       endif

#       if defined ( TRALLOC_REFERENCE )
        // Reference of "chunk" has been allocated, it should be freed.
        if ( have_reference ) {
            _tralloc_free_reference ( reference );
        }
#       endif

        // Memory has been allocated in the pool or global memory, it should be freed.
#       if defined ( TRALLOC_POOL )
        if ( have_pool_child ) {
            _tralloc_free_pool_child ( pool_child );
        } else {
            free ( memory );
        }
#       else
        free ( memory );
#       endif

        return result;
    }
#   endif

    * child_context = context;
    return 0;
}


#if defined ( TRALLOC_DEBUG_LOG )

tralloc_error _tralloc_debug_log_new_with_extensions ( const char * file, size_t line, tralloc_context * parent_context, tralloc_context ** child_context, tralloc_extensions extensions, size_t length )
{
    return _tralloc_debug_log_new_with_extensions_with_allocator ( file, line, parent_context, child_context, extensions, length, _tralloc_malloc );
}

tralloc_error _tralloc_debug_log_new_zero_with_extensions ( const char * file, size_t line, tralloc_context * parent_context, tralloc_context ** child_context, tralloc_extensions extensions, size_t length )
{
    return _tralloc_debug_log_new_with_extensions_with_allocator ( file, line, parent_context, child_context, extensions, length, _tralloc_calloc );
}

#else

tralloc_error tralloc_new_with_extensions ( tralloc_context * parent_context, tralloc_context ** child_context, tralloc_extensions extensions, size_t length )
{
    return _tralloc_new_with_extensions_with_allocator ( parent_context, child_context, extensions, length, _tralloc_malloc );
}

tralloc_error tralloc_new_zero_with_extensions ( tralloc_context * parent_context, tralloc_context ** child_context, tralloc_extensions extensions, size_t length )
{
    return _tralloc_new_with_extensions_with_allocator ( parent_context, child_context, extensions, length, _tralloc_calloc );
}

#endif
