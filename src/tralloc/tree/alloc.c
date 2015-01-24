// This file is part of tralloc. Copyright (C) 2013 Andrew Aladjev aladjev.andrew@gmail.com
// tralloc is free software: you can redistribute it and/or modify it under the terms of the GNU General Lesser Public License as published by the Free Software Foundation, either version 3 of the License, or (at your option) any later version.
// tralloc is distributed in the hope that it will be useful, but WITHOUT ANY WARRANTY; without even the implied warranty of MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU General Lesser Public License for more details.
// You should have received a copy of the GNU General Lesser Public License along with tralloc. If not, see <http://www.gnu.org/licenses/>.

#define _TRALLOC_INCLUDED_FROM_TREE_ALLOC_C
#include <tralloc/tree/alloc.h>
#include <tralloc/tree/free.h>
#include <tralloc/tree/chunk.h>
#include <tralloc/context.h>

#if defined ( TRALLOC_THREADS )
#   include <tralloc/tree/locks/subtree.h>
#   include <tralloc/tree/locks/children.h>
#endif

#if defined ( TRALLOC_LENGTH )
#   include <tralloc/length/data.h>
#endif

#if defined ( TRALLOC_DESTRUCTORS )
#   include <tralloc/destructors/data.h>
#endif

#if defined ( TRALLOC_REFERENCES )
#   include <tralloc/references/references_data.h>
#endif

#if defined ( TRALLOC_POOL )
#   include <tralloc/pool/pool_data.h>
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

typedef struct _tralloc_alloc_environment_type {
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

} _tralloc_alloc_environment;

static
void _TRALLOC_UNUSED_FUNCTION ( _tralloc_alloc_environment_disable_extension ) ( _tralloc_alloc_environment * environment, _tralloc_extension extension )
{
    _tralloc_extensions_disable_extension ( &environment->extensions, extension );

#   if defined ( TRALLOC_DEBUG_EXTENSIONS )
    _tralloc_extensions_toggle_extension ( &environment->forced_extensions, extension );
#   endif

}

static
void _TRALLOC_UNUSED_FUNCTION ( _tralloc_alloc_environment_enable_extension ) ( _tralloc_alloc_environment * environment, _tralloc_extension extension )
{
    _tralloc_extensions_enable_extension ( &environment->extensions, extension );

#   if defined ( TRALLOC_DEBUG_EXTENSIONS )
    _tralloc_extensions_toggle_extension ( &environment->forced_extensions, extension );
#   endif

}

static
void _tralloc_alloc_prepare_environment ( _tralloc_chunk * _TRALLOC_UNUSED ( parent_chunk ), tralloc_extensions extensions, size_t _TRALLOC_UNUSED ( total_length ), _tralloc_alloc_environment * environment )
{
    environment->extensions = extensions;

#   if defined ( TRALLOC_DEBUG_EXTENSIONS )
    environment->forced_extensions = 0;
    // If "extension" was not forced - _tralloc_extensions_have_extension ( forced_extensions, extension ) should return false
    // If "extension" was forced     - _tralloc_extensions_have_extension ( forced_extensions, extension ) should return true
#   endif

#   if defined ( TRALLOC_THREADS )
    environment->have_subtree_lock  = _tralloc_extensions_have_extension ( environment->extensions, TRALLOC_EXTENSION_LOCK_SUBTREE );
    environment->have_children_lock = _tralloc_extensions_have_extension ( environment->extensions, TRALLOC_EXTENSION_LOCK_CHILDREN );
#   endif

#   if defined ( TRALLOC_DEBUG_THREADS )
    if ( !environment->have_subtree_lock ) {
        environment->have_subtree_lock = TRALLOC_TRUE;
        _tralloc_alloc_environment_enable_extension ( environment, TRALLOC_EXTENSION_LOCK_SUBTREE );
    }
    if ( !environment->have_children_lock ) {
        environment->have_children_lock = TRALLOC_TRUE;
        _tralloc_alloc_environment_enable_extension ( environment, TRALLOC_EXTENSION_LOCK_CHILDREN );
    }
#   endif

#   if defined ( TRALLOC_LENGTH )
    environment->have_length = _tralloc_extensions_have_extension ( environment->extensions, TRALLOC_EXTENSION_LENGTH );
#   endif

#   if defined ( TRALLOC_DEBUG_LENGTH )
    if ( !environment->have_length ) {
        environment->have_length = TRALLOC_TRUE;
        _tralloc_alloc_environment_enable_extension ( environment, TRALLOC_EXTENSION_LENGTH );
    }
#   endif

#   if defined ( TRALLOC_DESTRUCTORS )
    environment->have_destructors = _tralloc_extensions_have_extension ( environment->extensions, TRALLOC_EXTENSION_DESTRUCTORS );
#   endif

#   if defined ( TRALLOC_REFERENCES )
    environment->have_references = _tralloc_extensions_have_extension ( environment->extensions, TRALLOC_EXTENSION_REFERENCES );
    environment->have_reference  = _tralloc_extensions_have_extension ( environment->extensions, TRALLOC_EXTENSION_REFERENCE );
#   endif

#   if defined ( TRALLOC_POOL )
    // Chunk can't have both "TRALLOC_EXTENSION_POOL" and "TRALLOC_EXTENSION_POOL_CHILD".
    environment->have_pool       = _tralloc_extensions_have_extension ( environment->extensions, TRALLOC_EXTENSION_POOL );
    environment->have_pool_child = _tralloc_extensions_have_extension ( environment->extensions, TRALLOC_EXTENSION_POOL_CHILD );

    if ( environment->have_pool ) {
        if ( environment->have_pool_child ) {
            environment->have_pool_child = TRALLOC_FALSE;
            _tralloc_alloc_environment_disable_extension ( environment, TRALLOC_EXTENSION_POOL_CHILD );
        }
        environment->parent_pool = NULL;
    } else {
        if ( parent_chunk == NULL ) {
            environment->parent_pool = NULL;
        } else {
            environment->parent_pool = _tralloc_chunk_get_closest_pool ( parent_chunk );
        }

        if ( environment->parent_pool == NULL ) {
            if ( environment->have_pool_child ) {
                environment->have_pool_child = TRALLOC_FALSE;
                _tralloc_alloc_environment_disable_extension ( environment, TRALLOC_EXTENSION_POOL_CHILD );
            }
        } else {
            if ( !environment->have_pool_child ) {
                environment->have_pool_child = TRALLOC_TRUE;
                _tralloc_alloc_environment_enable_extension ( environment, TRALLOC_EXTENSION_POOL_CHILD );
            }
        }
    }

#   if defined ( TRALLOC_THREADS )
    environment->have_pool_lock = _tralloc_extensions_have_extension ( environment->extensions, TRALLOC_EXTENSION_LOCK_POOL );

    if ( environment->have_pool_lock ) {

#       if defined ( TRALLOC_DEBUG_THREADS )
        if ( !environment->have_pool_lock ) {
            environment->have_pool_lock = TRALLOC_TRUE;
            _tralloc_alloc_environment_enable_extension ( environment, TRALLOC_EXTENSION_LOCK_POOL );
        }
#       endif

    } else {
        if ( environment->have_pool_lock ) {
            environment->have_pool_lock = TRALLOC_FALSE;
            _tralloc_alloc_environment_disable_extension ( environment, TRALLOC_EXTENSION_LOCK_POOL );
        }
    }
#   endif

#   endif

#   if defined ( TRALLOC_POOL )
    if ( environment->have_pool_child ) {
        if ( !_tralloc_pool_can_alloc ( environment->parent_pool, total_length + _tralloc_extensions_get_length ( environment->extensions ) ) ) {
            // "parent_pool" can't alloc enough bytes.
            environment->have_pool_child = TRALLOC_FALSE;
            _tralloc_alloc_environment_disable_extension ( environment, TRALLOC_EXTENSION_POOL_CHILD );
        }
    }
#   endif

}

static
tralloc_error _tralloc_alloc_initialize_extensions ( _tralloc_chunk * chunk, _tralloc_alloc_environment * environment, size_t _TRALLOC_UNUSED ( length ), size_t _TRALLOC_UNUSED ( total_length ) )
{
    tralloc_error _TRALLOC_UNUSED ( result );

    chunk->extensions = environment->extensions;

#   if defined ( TRALLOC_DEBUG_EXTENSIONS )
    chunk->forced_extensions = environment->forced_extensions;
#   endif

#   if defined ( TRALLOC_THREADS )
    _tralloc_subtree_lock * subtree_lock;
    if ( environment->have_subtree_lock ) {
        subtree_lock = _tralloc_chunk_get_subtree_lock ( chunk );
        result = _tralloc_subtree_lock_new ( subtree_lock );
        if ( result != 0 ) {
            return result;
        }
    }

    _tralloc_children_lock * children_lock;
    if ( environment->have_children_lock ) {
        children_lock = _tralloc_chunk_get_children_lock ( chunk );
        result = _tralloc_children_lock_new ( children_lock );
        if ( result != 0 ) {
            if ( environment->have_subtree_lock ) {
                _tralloc_subtree_lock_free ( subtree_lock );
            }
            return result;
        }
    }

#   if defined ( TRALLOC_POOL )
    _tralloc_pool_lock * pool_lock;
    if ( environment->have_pool_lock ) {
        pool_lock = _tralloc_chunk_get_pool_lock ( chunk );
        result = _tralloc_pool_lock_new ( pool_lock );
        if ( result != 0 ) {
            if ( environment->have_children_lock ) {
                _tralloc_children_lock_free ( children_lock );
            }
            if ( environment->have_subtree_lock ) {
                _tralloc_subtree_lock_free ( subtree_lock );
            }
            return result;
        }
    }
#   endif

#   endif

#   if defined ( TRALLOC_DEBUG_THREADS )
    _tralloc_debug_threads_lock * thread_usage_lock = &chunk->thread_usage_lock;
    result = _tralloc_debug_threads_lock_new ( thread_usage_lock );
    if ( result != 0 ) {

#       if defined ( TRALLOC_POOL )
        if ( environment->have_pool_lock ) {
            _tralloc_pool_lock_free ( pool_lock );
        }
#       endif

        if ( environment->have_children_lock ) {
            _tralloc_children_lock_free ( children_lock );
        }
        if ( environment->have_subtree_lock ) {
            _tralloc_subtree_lock_free ( subtree_lock );
        }
        return result;
    }
#   endif

#   if defined ( TRALLOC_POOL )
    if ( environment->have_pool ) {
        _tralloc_pool_new ( _tralloc_chunk_get_pool ( chunk ), _tralloc_chunk_get_context ( chunk ), environment->extensions, length );
    } else if ( environment->have_pool_child ) {
        _tralloc_pool_child_new ( _tralloc_chunk_get_pool_child ( chunk ), environment->parent_pool, total_length, environment->prev_pool_child, environment->next_pool_child );
    }
#   endif

#   if defined ( TRALLOC_LENGTH )
    if ( environment->have_length ) {
        _tralloc_length_set ( _tralloc_chunk_get_length ( chunk ), length );
    }
#   endif

#   if defined ( TRALLOC_DESTRUCTORS )
    if ( environment->have_destructors ) {
        _tralloc_destructors_new ( _tralloc_chunk_get_destructors ( chunk ) );
    }
#   endif

#   if defined ( TRALLOC_REFERENCES )
    if ( environment->have_references ) {
        _tralloc_references_new ( _tralloc_chunk_get_references ( chunk ), environment->extensions );
    }
    if ( environment->have_reference ) {
        _tralloc_reference_new ( _tralloc_chunk_get_reference ( chunk ) );
    }
#   endif

#   if defined ( TRALLOC_DEBUG_THREADS )
    chunk->subtree_usage_status  = _TRALLOC_DEBUG_NOT_USED_BY_THREADS;
    chunk->children_usage_status = _TRALLOC_DEBUG_NOT_USED_BY_THREADS;

#   if defined ( TRALLOC_POOL )
    chunk->pool_usage_status = _TRALLOC_DEBUG_NOT_USED_BY_THREADS;
#   endif

#   endif

    return 0;
}

#endif

static
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

static
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

tralloc_error _tralloc_alloc ( _tralloc_alloc_options * options, tralloc_context ** child_context )
{
    if ( child_context == NULL ) {
        return TRALLOC_ERROR_REQUIRED_ARGUMENT_IS_UNDEFINED;
    }

    _tralloc_chunk * parent_chunk;
    if ( options->parent_context == NULL ) {
        parent_chunk = NULL;
    } else {
        parent_chunk = _tralloc_context_get_chunk ( options->parent_context );
    }

    size_t total_length = sizeof ( _tralloc_chunk ) + options->length;

#   if defined ( TRALLOC_EXTENSIONS )
    _tralloc_alloc_environment environment;
    _tralloc_alloc_prepare_environment ( parent_chunk, options->extensions, total_length, &environment );
    size_t extensions_length = _tralloc_extensions_get_length ( environment.extensions );
    total_length += extensions_length;
#   endif

    _tralloc_chunk_prototype _TRALLOC_UNUSED ( chunk_prototype );
    chunk_prototype.parent = parent_chunk;
    chunk_prototype.length = options->length;

#   if defined ( TRALLOC_EXTENSIONS )
    chunk_prototype.extensions = environment.extensions;
#   endif

#   if defined ( TRALLOC_DEBUG_EXTENSIONS )
    chunk_prototype.forced_extensions = environment.forced_extensions;
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
    if ( environment.have_pool_child ) {
        _tralloc_pool_alloc ( environment.parent_pool, &memory, total_length, options->zero, &environment.prev_pool_child, &environment.next_pool_child );
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
    tralloc_context * context = _tralloc_chunk_get_context ( chunk );

#   if defined ( TRALLOC_DEBUG_LOG )
    chunk->initialized_in_file = strdup ( chunk_prototype.initialized_in_file );
    if ( chunk->initialized_in_file == NULL ) {
#       if defined ( TRALLOC_POOL )
        if ( environment.have_pool_child ) {
            _tralloc_pool_child_free ( _tralloc_chunk_get_pool_child ( chunk ) );
            return TRALLOC_ERROR_MALLOC_FAILED;
        }
#       endif

        free ( memory );
        return TRALLOC_ERROR_MALLOC_FAILED;
    }
    chunk->initialized_at_line = chunk_prototype.initialized_at_line;
#   endif

#   if defined ( TRALLOC_EXTENSIONS )
    result = _tralloc_alloc_initialize_extensions ( chunk, &environment, chunk_prototype.length, total_length );
    if ( result != 0 ) {
#       if defined ( TRALLOC_DEBUG_LOG )
        if ( chunk->initialized_in_file != NULL ) {
            free ( chunk->initialized_in_file );
        }
#       endif

#       if defined ( TRALLOC_POOL )
        if ( environment.have_pool_child ) {
            _tralloc_pool_child_free ( _tralloc_chunk_get_pool_child ( chunk ) );
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
        parent_have_children_lock = _tralloc_extensions_have_extension ( parent_chunk->extensions, TRALLOC_EXTENSION_LOCK_CHILDREN );
        if ( parent_have_children_lock ) {
            parent_children_lock = _tralloc_chunk_get_children_lock ( parent_chunk );
            result = _tralloc_children_lock_wrlock ( parent_children_lock );
            if ( result != 0 ) {
                _tralloc_free_chunk ( chunk );
                return result;
            }
        }
#       endif

        _tralloc_chunk_attach ( chunk, parent_chunk );

#       if defined ( TRALLOC_THREADS )
        if ( parent_have_children_lock ) {
            result = _tralloc_children_lock_unlock ( parent_children_lock );
            if ( result != 0 ) {
                // Chunk should be detached.
                _tralloc_chunk_attach ( chunk, NULL );
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
                result = _tralloc_children_lock_wrlock ( parent_children_lock );
                if ( result != 0 ) {
                    // "chunk" is a child of "parent_chunk".
                    // But "parent_children_lock" should be locked before detach operation.
                    // So it is not possible to delete chunk now.
                    // Chunk will be deleted normally by it's parent.
                    return error;
                }
            }
#           endif

            _tralloc_chunk_attach ( chunk, NULL );

#           if defined ( TRALLOC_THREADS )
            if ( parent_have_children_lock ) {
                _tralloc_children_lock_unlock ( parent_children_lock );
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
