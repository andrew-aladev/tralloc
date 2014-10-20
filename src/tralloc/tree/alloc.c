// This file is part of tralloc. Copyright (C) 2013 Andrew Aladjev aladjev.andrew@gmail.com
// tralloc is free software: you can redistribute it and/or modify it under the terms of the GNU General Lesser Public License as published by the Free Software Foundation, either version 3 of the License, or (at your option) any later version.
// tralloc is distributed in the hope that it will be useful, but WITHOUT ANY WARRANTY; without even the implied warranty of MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU General Lesser Public License for more details.
// You should have received a copy of the GNU General Lesser Public License along with tralloc. If not, see <http://www.gnu.org/licenses/>.

#define _TRALLOC_INCLUDED_FROM_TREE_ALLOC_C
#include <tralloc/tree/alloc.h>
#include <tralloc/tree/free.h>
#include <tralloc/tree/chunk.h>
#include <tralloc/common.h>

#if defined ( TRALLOC_THREADS )
#   include <tralloc/tree/locks/subtree.h>
#   include <tralloc/tree/locks/children.h>
#endif

#if defined ( TRALLOC_LENGTH )
#   include <tralloc/length/main.h>
#endif

#if defined ( TRALLOC_DESTRUCTORS )
#   include <tralloc/destructors/chunk.h>
#   include <tralloc/destructors/destructors.h>
#endif

#if defined ( TRALLOC_REFERENCES )
#   include <tralloc/references/chunk.h>
#   include <tralloc/references/references.h>
#   include <tralloc/references/reference.h>
#endif

#if defined ( TRALLOC_POOL )
#   include <tralloc/pool/chunk.h>
#   include <tralloc/pool/pool.h>
#   include <tralloc/pool/pool_child.h>
#   if defined ( TRALLOC_THREADS )
#       include <tralloc/pool/lock.h>
#   endif
#endif

#if defined ( TRALLOC_DEBUG )
#   include <tralloc/debug/events.h>
#endif

#if defined ( TRALLOC_DEBUG_LOG )
#   include <string.h>
#endif

#if defined ( TRALLOC_DEBUG_THREADS )
#   include <tralloc/debug/threads/lock.h>
#endif

#include <stdlib.h>


#if defined ( TRALLOC_EXTENSIONS )

typedef struct _tralloc_alloc_extensions_environment_type {
    tralloc_extensions extensions;

#   if defined ( TRALLOC_DEBUG_EXTENSIONS )
    tralloc_extensions forced_extensions;
#   endif

#   if defined ( TRALLOC_THREADS )
    tralloc_bool have_subtree_lock;
    tralloc_bool have_children_lock;
#   endif

#   if defined ( TRALLOC_LENGTH )
    tralloc_bool have_length;
#   endif

#   if defined ( TRALLOC_DESTRUCTORS )
    tralloc_bool have_destructors;
#   endif

#   if defined ( TRALLOC_REFERENCES )
    tralloc_bool have_references;
    tralloc_bool have_reference;
#   endif

#   if defined ( TRALLOC_POOL )
    _tralloc_pool * parent_pool;
    _tralloc_pool_child * prev_pool_child;
    _tralloc_pool_child * next_pool_child;

    tralloc_bool have_pool;
    tralloc_bool have_pool_child;

#   if defined ( TRALLOC_THREADS )
    tralloc_bool have_pool_lock;
#   endif

#   endif

} _tralloc_alloc_extensions_environment;

static
void _tralloc_alloc_prepare_extensions_environment ( _tralloc_chunk * parent_chunk, tralloc_extensions extensions, size_t total_length, _tralloc_alloc_extensions_environment * extensions_environment )
{
    extensions_environment->extensions = extensions;

#   if defined ( TRALLOC_DEBUG_EXTENSIONS )
    extensions_environment->forced_extensions = 0;
    // "forced_extensions ^= EXTENSION" means "toggle forced extension":
    // If "EXTENSION" was not forced - it becomes forced
    // If "EXTENSION" was forced     - it becomes not forced
#   endif

#   if defined ( TRALLOC_THREADS )
    extensions_environment->have_subtree_lock  = extensions_environment->extensions & TRALLOC_EXTENSION_LOCK_SUBTREE;
    extensions_environment->have_children_lock = extensions_environment->extensions & TRALLOC_EXTENSION_LOCK_CHILDREN;
#   endif

#   if defined ( TRALLOC_DEBUG_THREADS )
    if ( !extensions_environment->have_subtree_lock ) {
        // "TRALLOC_EXTENSION_LOCK_SUBTREE" will be forced enabled.
        extensions_environment->have_subtree_lock  = TRALLOC_TRUE;
        extensions_environment->extensions        |= TRALLOC_EXTENSION_LOCK_SUBTREE;

#       if defined ( TRALLOC_DEBUG_EXTENSIONS )
        extensions_environment->forced_extensions ^= TRALLOC_EXTENSION_LOCK_SUBTREE;
#       endif

    }
    if ( !extensions_environment->have_children_lock ) {
        // "TRALLOC_EXTENSION_LOCK_CHILDREN" will be forced enabled.
        extensions_environment->have_children_lock  = TRALLOC_TRUE;
        extensions_environment->extensions         |= TRALLOC_EXTENSION_LOCK_CHILDREN;

#       if defined ( TRALLOC_DEBUG_EXTENSIONS )
        extensions_environment->forced_extensions ^= TRALLOC_EXTENSION_LOCK_CHILDREN;
#       endif

    }
#   endif

#   if defined ( TRALLOC_LENGTH )
    extensions_environment->have_length = extensions_environment->extensions & TRALLOC_EXTENSION_LENGTH;
#   endif

#   if defined ( TRALLOC_DEBUG_LENGTH )
    if ( !extensions_environment->have_length ) {
        // "TRALLOC_EXTENSION_LENGTH" will be forced enabled.
        extensions_environment->have_length  = TRALLOC_TRUE;
        extensions_environment->extensions  |= TRALLOC_EXTENSION_LENGTH;

#       if defined ( TRALLOC_DEBUG_EXTENSIONS )
        extensions_environment->forced_extensions ^= TRALLOC_EXTENSION_LENGTH;
#       endif

    }
#   endif

#   if defined ( TRALLOC_DESTRUCTORS )
    extensions_environment->have_destructors = extensions_environment->extensions & TRALLOC_EXTENSION_DESTRUCTORS;
#   endif

#   if defined ( TRALLOC_REFERENCES )
    extensions_environment->have_references = extensions_environment->extensions & TRALLOC_EXTENSION_REFERENCES;
    extensions_environment->have_reference  = extensions_environment->extensions & TRALLOC_EXTENSION_REFERENCE;
#   endif

#   if defined ( TRALLOC_POOL )
    // Chunk can't have both "TRALLOC_EXTENSION_POOL" and "TRALLOC_EXTENSION_POOL_CHILD".
    extensions_environment->have_pool       = extensions_environment->extensions & TRALLOC_EXTENSION_POOL;
    extensions_environment->have_pool_child = extensions_environment->extensions & TRALLOC_EXTENSION_POOL_CHILD;

    if ( extensions_environment->have_pool ) {
        if ( extensions_environment->have_pool_child ) {
            // "TRALLOC_EXTENSION_POOL_CHILD" will be forced disabled.
            extensions_environment->have_pool_child  = TRALLOC_FALSE;
            extensions_environment->extensions      &= ~TRALLOC_EXTENSION_POOL_CHILD;

#           if defined ( TRALLOC_DEBUG_EXTENSIONS )
            extensions_environment->forced_extensions ^= TRALLOC_EXTENSION_POOL_CHILD;
#           endif

        }
        extensions_environment->parent_pool = NULL;
    } else {
        if ( parent_chunk == NULL ) {
            extensions_environment->parent_pool = NULL;
        } else {
            extensions_environment->parent_pool = _tralloc_get_closest_pool_from_chunk ( parent_chunk );
        }

        if ( extensions_environment->parent_pool == NULL ) {
            if ( extensions_environment->have_pool_child ) {
                // "TRALLOC_EXTENSION_POOL_CHILD" will be forced disabled.
                extensions_environment->have_pool_child  = TRALLOC_FALSE;
                extensions_environment->extensions      &= ~TRALLOC_EXTENSION_POOL_CHILD;

#               if defined ( TRALLOC_DEBUG_EXTENSIONS )
                extensions_environment->forced_extensions ^= TRALLOC_EXTENSION_POOL_CHILD;
#               endif

            }
        } else {
            if ( !extensions_environment->have_pool_child ) {
                // "TRALLOC_EXTENSION_POOL_CHILD" will be forced enabled.
                extensions_environment->have_pool_child  = TRALLOC_TRUE;
                extensions_environment->extensions      |= TRALLOC_EXTENSION_POOL_CHILD;

#               if defined ( TRALLOC_DEBUG_EXTENSIONS )
                extensions_environment->forced_extensions ^= TRALLOC_EXTENSION_POOL_CHILD;
#               endif

            }
        }
    }

#   if defined ( TRALLOC_THREADS )
    extensions_environment->have_pool_lock = extensions_environment->extensions & TRALLOC_EXTENSION_LOCK_POOL;

    if ( extensions_environment->have_pool_lock ) {

#       if defined ( TRALLOC_DEBUG_THREADS )
        if ( !extensions_environment->have_pool_lock ) {
            // "TRALLOC_EXTENSION_LOCK_POOL" will be forced enabled.
            extensions_environment->have_pool_lock  = TRALLOC_TRUE;
            extensions_environment->extensions     |= TRALLOC_EXTENSION_LOCK_POOL;

#           if defined ( TRALLOC_DEBUG_EXTENSIONS )
            extensions_environment->forced_extensions ^= TRALLOC_EXTENSION_LOCK_POOL;
#           endif

        }
#       endif

    } else {
        if ( extensions_environment->have_pool_lock ) {
            // "TRALLOC_EXTENSION_LOCK_POOL" will be forced disabled.
            extensions_environment->have_pool_lock  = TRALLOC_FALSE;
            extensions_environment->extensions     &= ~TRALLOC_EXTENSION_LOCK_POOL;

#           if defined ( TRALLOC_DEBUG )
            extensions_environment->forced_extensions ^= TRALLOC_EXTENSION_LOCK_POOL;
#           endif
        }
    }
#   endif

#   endif

#   if defined ( TRALLOC_POOL )
    if ( extensions_environment->have_pool_child ) {
        if ( !_tralloc_can_alloc_from_pool ( extensions_environment->parent_pool, total_length + _tralloc_get_extensions_length ( extensions_environment->extensions ) ) ) {
            // "parent_pool" can't alloc enough bytes.
            // "TRALLOC_EXTENSION_POOL_CHILD" will be forced disabled.
            extensions_environment->have_pool_child  = TRALLOC_FALSE;
            extensions_environment->extensions      &= ~TRALLOC_EXTENSION_POOL_CHILD;

#           if defined ( TRALLOC_DEBUG )
            extensions_environment->forced_extensions ^= TRALLOC_EXTENSION_LOCK_POOL;
#           endif

        }
    }
#   endif

}

static
tralloc_error _tralloc_alloc_initialize_extensions ( _tralloc_chunk * chunk, _tralloc_chunk_prototype * chunk_prototype, _tralloc_alloc_extensions_environment * extensions_environment, size_t total_length )
{
    tralloc_error result;

    chunk->extensions = extensions_environment->extensions;

#   if defined ( TRALLOC_DEBUG_EXTENSIONS )
    chunk->forced_extensions = extensions_environment->forced_extensions;
#   endif

#   if defined ( TRALLOC_THREADS )
    _tralloc_subtree_lock * subtree_lock;
    if ( extensions_environment->have_subtree_lock ) {
        subtree_lock = _tralloc_get_subtree_lock_from_chunk ( chunk );
        result = _tralloc_new_subtree_lock ( subtree_lock );
        if ( result != 0 ) {
            return result;
        }
    }

    _tralloc_children_lock * children_lock;
    if ( extensions_environment->have_children_lock ) {
        children_lock = _tralloc_get_children_lock_from_chunk ( chunk );
        result = _tralloc_new_children_lock ( children_lock );
        if ( result != 0 ) {
            if ( extensions_environment->have_subtree_lock ) {
                _tralloc_free_subtree_lock ( subtree_lock );
            }
            return result;
        }
    }

#   if defined ( TRALLOC_POOL )
    _tralloc_pool_lock * pool_lock;
    if ( extensions_environment->have_pool_lock ) {
        pool_lock = _tralloc_get_pool_lock_from_chunk ( chunk );
        result = _tralloc_new_pool_lock ( pool_lock );
        if ( result != 0 ) {
            if ( extensions_environment->have_children_lock ) {
                _tralloc_free_children_lock ( children_lock );
            }
            if ( extensions_environment->have_subtree_lock ) {
                _tralloc_free_subtree_lock ( subtree_lock );
            }
            return result;
        }
    }
#   endif

#   endif

#   if defined ( TRALLOC_DEBUG_THREADS )
    _tralloc_debug_threads_lock * thread_usage_lock = &chunk->thread_usage_lock;
    result = _tralloc_new_debug_threads_lock ( thread_usage_lock );
    if ( result != 0 ) {

#       if defined ( TRALLOC_POOL )
        if ( extensions_environment->have_pool_lock ) {
            _tralloc_free_pool_lock ( pool_lock );
        }
#       endif

        if ( extensions_environment->have_children_lock ) {
            _tralloc_free_children_lock ( children_lock );
        }
        if ( extensions_environment->have_subtree_lock ) {
            _tralloc_free_subtree_lock ( subtree_lock );
        }
        return result;
    }
#   endif

#   if defined ( TRALLOC_DEBUG_LOG )
    chunk->initialized_in_file = strdup ( chunk_prototype->initialized_in_file );
    if ( chunk->initialized_in_file == NULL ) {

#       if defined ( TRALLOC_DEBUG_THREADS )
        _tralloc_free_debug_threads_lock ( thread_usage_lock );
#       endif

#       if defined ( TRALLOC_THREADS )

#       if defined ( TRALLOC_POOL )
        if ( extensions_environment->have_pool_lock ) {
            _tralloc_free_pool_lock ( pool_lock );
        }
#       endif

        if ( extensions_environment->have_children_lock ) {
            _tralloc_free_children_lock ( children_lock );
        }
        if ( extensions_environment->have_subtree_lock ) {
            _tralloc_free_subtree_lock ( subtree_lock );
        }
#       endif

        return TRALLOC_ERROR_MALLOC_FAILED;
    }
    chunk->initialized_at_line = chunk_prototype->initialized_at_line;
#   endif

#   if defined ( TRALLOC_POOL )
    if ( extensions_environment->have_pool ) {
        _tralloc_new_pool ( _tralloc_get_pool_from_chunk ( chunk ), _tralloc_get_context_from_chunk ( chunk ), extensions_environment->extensions, chunk_prototype->length );
    } else if ( extensions_environment->have_pool_child ) {
        _tralloc_new_pool_child ( _tralloc_get_pool_child_from_chunk ( chunk ), extensions_environment->parent_pool, total_length, extensions_environment->prev_pool_child, extensions_environment->next_pool_child );
    }
#   endif

#   if defined ( TRALLOC_LENGTH )
    if ( extensions_environment->have_length ) {
        _tralloc_set_length ( _tralloc_get_length_from_chunk ( chunk ), chunk_prototype->length );
    }
#   endif

#   if defined ( TRALLOC_DESTRUCTORS )
    if ( extensions_environment->have_destructors ) {
        _tralloc_new_destructors ( _tralloc_get_destructors_from_chunk ( chunk ) );
    }
#   endif

#   if defined ( TRALLOC_REFERENCES )
    if ( extensions_environment->have_references ) {
        _tralloc_new_references ( _tralloc_get_references_from_chunk ( chunk ), extensions_environment->extensions );
    }
    if ( extensions_environment->have_reference ) {
        _tralloc_new_reference ( _tralloc_get_reference_from_chunk ( chunk ) );
    }
#   endif

#   if defined ( TRALLOC_DEBUG_THREADS )
    chunk->subtree_usage_status  = _TRALLOC_NOT_USED_BY_THREADS;
    chunk->children_usage_status = _TRALLOC_NOT_USED_BY_THREADS;

#   if defined ( TRALLOC_POOL )
    chunk->pool_usage_status = _TRALLOC_NOT_USED_BY_THREADS;
#   endif

#   endif

    return 0;
}

#endif

static inline
tralloc_error _tralloc_malloc ( void ** data, size_t total_length )
{
    void * _data = malloc ( total_length );
    if ( _data == NULL ) {
        return TRALLOC_ERROR_MALLOC_FAILED;
    } else {
        * data = _data;
        return 0;
    }
}

static inline
tralloc_error _tralloc_calloc ( void ** data, size_t total_length )
{
    void * _data = calloc ( 1, total_length );
    if ( _data == NULL ) {
        return TRALLOC_ERROR_CALLOC_FAILED;
    } else {
        * data = _data;
        return 0;
    }
}

tralloc_error _tralloc_alloc_internal ( _tralloc_alloc_options * options, tralloc_context ** child_context )
{
    if ( child_context == NULL ) {
        return TRALLOC_ERROR_REQUIRED_ARGUMENT_IS_NULL;
    }

    _tralloc_chunk * parent_chunk;
    if ( options->parent_context == NULL ) {
        parent_chunk = NULL;
    } else {
        parent_chunk = _tralloc_get_chunk_from_context ( options->parent_context );
    }

    size_t total_length = sizeof ( _tralloc_chunk ) + options->length;

#   if defined ( TRALLOC_EXTENSIONS )
    _tralloc_alloc_extensions_environment extensions_environment;
    _tralloc_alloc_prepare_extensions_environment ( parent_chunk, options->extensions, total_length, &extensions_environment );
    size_t extensions_length = _tralloc_get_extensions_length ( extensions_environment.extensions );
    total_length += extensions_length;
#   endif

    _tralloc_chunk_prototype chunk_prototype;
    chunk_prototype.parent = parent_chunk;
    chunk_prototype.length = options->length;

#   if defined ( TRALLOC_EXTENSIONS )
    chunk_prototype.extensions = extensions_environment.extensions;
#   endif

#   if defined ( TRALLOC_DEBUG_EXTENSIONS )
    chunk_prototype.forced_extensions = extensions_environment.forced_extensions;
#   endif

#   if defined ( TRALLOC_DEBUG_LOG )
    chunk_prototype.initialized_in_file = options->file;
    chunk_prototype.initialized_at_line = options->line;
#   endif

    tralloc_error result;

#   if defined ( TRALLOC_DEBUG )
    // Debug should care about thread safety of operations with "parent_chunk" by itself.
    result = _tralloc_debug_before_add_chunk ( &chunk_prototype );
    if ( result != 0 ) {
        return result;
    }
#   endif

    void * memory;

#   if defined ( TRALLOC_POOL )
    if ( extensions_environment.have_pool_child ) {
        _tralloc_alloc_from_pool ( extensions_environment.parent_pool, &memory, total_length, options->zero, &extensions_environment.prev_pool_child, &extensions_environment.next_pool_child );
    } else {
        if ( options->zero ) {
            result = _tralloc_calloc ( &memory, total_length );
        } else {
            result = _tralloc_malloc ( &memory, total_length );
        }
        if ( result != 0 ) {
            return result;
        }
    }
#   else
    if ( options->zero ) {
        result = _tralloc_calloc ( &memory, total_length );
    } else {
        result = _tralloc_malloc ( &memory, total_length );
    }
    if ( result != 0 ) {
        return result;
    }
#   endif

    uintptr_t chunk_memory = ( uintptr_t ) memory;
#   if defined ( TRALLOC_EXTENSIONS )
    chunk_memory += extensions_length;
#   endif

    _tralloc_chunk * chunk    = ( _tralloc_chunk * ) chunk_memory;
    tralloc_context * context = _tralloc_get_context_from_chunk ( chunk );

#   if defined ( TRALLOC_EXTENSIONS )
    result = _tralloc_alloc_initialize_extensions ( chunk, &chunk_prototype, &extensions_environment, total_length );
    if ( result != 0 ) {

#       if defined ( TRALLOC_POOL )
        if ( extensions_environment.have_pool_child ) {
            _tralloc_free_pool_child ( _tralloc_get_pool_child_from_chunk ( chunk ) );
            return result;
        }
#       endif

        free ( memory );
        return result;
    }
#   endif

    chunk->parent      = NULL;
    chunk->first_child = NULL;
    chunk->prev        = NULL;
    chunk->next        = NULL;

    // Now "chunk" is fully initialized.
    // It should be freed by using "_tralloc_free_chunk" function if error occured.

#   if defined ( TRALLOC_THREADS )
    tralloc_bool parent_have_children_lock;
    _tralloc_children_lock * parent_children_lock;
#   endif

    if ( parent_chunk != NULL ) {

#       if defined ( TRALLOC_THREADS )
        parent_have_children_lock = parent_chunk->extensions & TRALLOC_EXTENSION_LOCK_CHILDREN;
        if ( parent_have_children_lock ) {
            parent_children_lock = _tralloc_get_children_lock_from_chunk ( parent_chunk );
            result = _tralloc_wrlock_children ( parent_children_lock );
            if ( result != 0 ) {
                _tralloc_free_chunk ( chunk );
                return result;
            }
        }
#       endif

        _tralloc_attach_chunk ( chunk, parent_chunk );

#       if defined ( TRALLOC_THREADS )
        if ( parent_have_children_lock ) {
            result = _tralloc_unlock_children ( parent_children_lock );
            if ( result != 0 ) {
                // Chunk should be detached.
                _tralloc_attach_chunk ( chunk, NULL );
                _tralloc_free_chunk ( chunk );
                return result;
            }
        }
#       endif
    }

#   if defined ( TRALLOC_DEBUG )
    // Debug should care about thread safety of operations with "chunk" by itself.
    result = _tralloc_debug_after_add_chunk ( chunk );
    if ( result != 0 ) {
        tralloc_error error = result;

        if ( parent_chunk != NULL ) {
            // Chunk should be detached.

#           if defined ( TRALLOC_THREADS )
            if ( parent_have_children_lock ) {
                result = _tralloc_wrlock_children ( parent_children_lock );
                if ( result != 0 ) {
                    // "chunk" is a child of "parent_chunk".
                    // But "parent_children_lock" should be locked before detach operation.
                    // So it is not possible to delete chunk now.
                    // Chunk will be deleted normally by it's parent.
                    return error;
                }
            }
#           endif

            _tralloc_attach_chunk ( chunk, NULL );

#           if defined ( TRALLOC_THREADS )
            if ( parent_have_children_lock ) {
                _tralloc_unlock_children ( parent_children_lock );
            }
#           endif
        }

        _tralloc_free_chunk ( chunk );
        return error;
    }
#   endif

    * child_context = context;
    return 0;
}
