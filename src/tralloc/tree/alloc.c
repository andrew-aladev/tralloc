// This file is part of tralloc. Copyright (C) 2013 Andrew Aladjev aladjev.andrew@gmail.com
// tralloc is free software: you can redistribute it and/or modify it under the terms of the GNU General Lesser Public License as published by the Free Software Foundation, either version 3 of the License, or (at your option) any later version.
// tralloc is distributed in the hope that it will be useful, but WITHOUT ANY WARRANTY; without even the implied warranty of MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU General Lesser Public License for more details.
// You should have received a copy of the GNU General Lesser Public License along with tralloc. If not, see <http://www.gnu.org/licenses/>.

#define _TRALLOC_INCLUDED_FROM_TREE_ALLOC_C
#include <tralloc/tree/alloc.h>
#include <tralloc/tree/free.h>
#include <tralloc/tree/chunk.h>
#include <tralloc/common.h>

#if defined ( TRALLOC_DEBUG )
#   include <tralloc/debug/events.h>
#endif

#if defined ( TRALLOC_THREADS )
#   include <tralloc/tree/locks/subtree.h>
#   include <tralloc/tree/locks/children.h>
#endif

#if defined ( TRALLOC_LENGTH )
#   include <tralloc/length/chunk.h>
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

#include <stdlib.h>


typedef struct _tralloc_alloc_options_type {
    tralloc_bool zero;

#   if defined ( TRALLOC_EXTENSIONS )
    tralloc_extensions extensions;
#   endif

#   if defined ( TRALLOC_DEBUG_LOG )
    const char * file;
    size_t line;
#   endif

} _tralloc_alloc_options;

#if defined ( TRALLOC_EXTENSIONS )

typedef struct _tralloc_alloc_extensions_environment_type {
    tralloc_extensions extensions;

#   if defined ( TRALLOC_DEBUG )
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

    size_t extensions_length;
} _tralloc_alloc_extensions_environment;

static
void _tralloc_alloc_prepare_extensions_environment ( _tralloc_chunk * parent_chunk, tralloc_extensions extensions, size_t total_length, _tralloc_alloc_extensions_environment * ext_env )
{
    ext_env->extensions = extensions;

#   if defined ( TRALLOC_DEBUG )
    ext_env->forced_extensions = 0;
    // "forced_extensions ^= EXTENSION" means "toggle forced extension":
    // If "EXTENSION" was not forced - it becomes forced
    // If "EXTENSION" was forced     - it becomes not forced
#   endif

#   if defined ( TRALLOC_THREADS )
    ext_env->have_subtree_lock  = ext_env->extensions & TRALLOC_EXTENSION_LOCK_SUBTREE;
    ext_env->have_children_lock = ext_env->extensions & TRALLOC_EXTENSION_LOCK_CHILDREN;
#   endif

#   if defined ( TRALLOC_DEBUG_THREADS )
    if ( !ext_env->have_subtree_lock ) {
        // "TRALLOC_EXTENSION_LOCK_SUBTREE" will be forced enabled.
        ext_env->have_subtree_lock  = TRALLOC_TRUE;
        ext_env->extensions        |= TRALLOC_EXTENSION_LOCK_SUBTREE;
        ext_env->forced_extensions ^= TRALLOC_EXTENSION_LOCK_SUBTREE;
    }
    if ( !ext_env->have_children_lock ) {
        // "TRALLOC_EXTENSION_LOCK_CHILDREN" will be forced enabled.
        ext_env->have_children_lock  = TRALLOC_TRUE;
        ext_env->extensions         |= TRALLOC_EXTENSION_LOCK_CHILDREN;
        ext_env->forced_extensions  ^= TRALLOC_EXTENSION_LOCK_CHILDREN;
    }
#   endif

#   if defined ( TRALLOC_LENGTH )
    ext_env->have_length = ext_env->extensions & TRALLOC_EXTENSION_LENGTH;
#   endif

#   if defined ( TRALLOC_DESTRUCTORS )
    ext_env->have_destructors = ext_env->extensions & TRALLOC_EXTENSION_DESTRUCTORS;
#   endif

#   if defined ( TRALLOC_REFERENCES )
    ext_env->have_references = ext_env->extensions & TRALLOC_EXTENSION_REFERENCES;
    ext_env->have_reference  = ext_env->extensions & TRALLOC_EXTENSION_REFERENCE;
#   endif

#   if defined ( TRALLOC_POOL )
    // Chunk can't have both "TRALLOC_EXTENSION_POOL" and "TRALLOC_EXTENSION_POOL_CHILD".
    ext_env->have_pool       = ext_env->extensions & TRALLOC_EXTENSION_POOL;
    ext_env->have_pool_child = ext_env->extensions & TRALLOC_EXTENSION_POOL_CHILD;

    if ( ext_env->have_pool ) {
        if ( ext_env->have_pool_child ) {
            // "TRALLOC_EXTENSION_POOL_CHILD" will be forced disabled.
            ext_env->have_pool_child  = TRALLOC_FALSE;
            ext_env->extensions      &= ~TRALLOC_EXTENSION_POOL_CHILD;

#           if defined ( TRALLOC_DEBUG )
            ext_env->forced_extensions ^= TRALLOC_EXTENSION_POOL_CHILD;
#           endif
        }
        ext_env->parent_pool = NULL;
    } else {
        if ( parent_chunk == NULL ) {
            ext_env->parent_pool = NULL;
        } else {
            ext_env->parent_pool = _tralloc_get_closest_pool_from_chunk ( parent_chunk );
        }

        if ( ext_env->parent_pool == NULL ) {
            if ( ext_env->have_pool_child ) {
                // "TRALLOC_EXTENSION_POOL_CHILD" will be forced disabled.
                ext_env->have_pool_child  = TRALLOC_FALSE;
                ext_env->extensions      &= ~TRALLOC_EXTENSION_POOL_CHILD;

#               if defined ( TRALLOC_DEBUG )
                ext_env->forced_extensions ^= TRALLOC_EXTENSION_POOL_CHILD;
#               endif
            }
        } else {
            if ( !ext_env->have_pool_child ) {
                // "TRALLOC_EXTENSION_POOL_CHILD" will be forced enabled.
                ext_env->have_pool_child  = TRALLOC_TRUE;
                ext_env->extensions      |= TRALLOC_EXTENSION_POOL_CHILD;

#               if defined ( TRALLOC_DEBUG )
                ext_env->forced_extensions ^= TRALLOC_EXTENSION_POOL_CHILD;
#               endif
            }
        }
    }

#   if defined ( TRALLOC_THREADS )
    ext_env->have_pool_lock = ext_env->extensions & TRALLOC_EXTENSION_LOCK_POOL;

    if ( ext_env->have_pool_lock ) {

#       if defined ( TRALLOC_DEBUG_THREADS )
        if ( !ext_env->have_pool_lock ) {
            // "TRALLOC_EXTENSION_LOCK_POOL" will be forced enabled.
            ext_env->have_pool_lock     = TRALLOC_TRUE;
            ext_env->extensions        |= TRALLOC_EXTENSION_LOCK_POOL;
            ext_env->forced_extensions ^= TRALLOC_EXTENSION_LOCK_POOL;
        }
#       endif
    } else {
        if ( ext_env->have_pool_lock ) {
            // "TRALLOC_EXTENSION_LOCK_POOL" will be forced disabled.
            ext_env->have_pool_lock  = TRALLOC_FALSE;
            ext_env->extensions     &= ~TRALLOC_EXTENSION_LOCK_POOL;

#           if defined ( TRALLOC_DEBUG )
            ext_env->forced_extensions ^= TRALLOC_EXTENSION_LOCK_POOL;
#           endif
        }
    }
#   endif

#   endif

    ext_env->extensions_length = _tralloc_get_extensions_length ( ext_env->extensions );

#   if defined ( TRALLOC_POOL )
    if ( ext_env->have_pool_child ) {
        if ( !_tralloc_can_alloc_from_pool ( ext_env->parent_pool, total_length + ext_env->extensions_length ) ) {
            // "parent_pool" can't alloc enough bytes.
            // "TRALLOC_EXTENSION_POOL_CHILD" will be forced disabled.
            ext_env->have_pool_child  = TRALLOC_FALSE;
            ext_env->extensions      &= ~TRALLOC_EXTENSION_POOL_CHILD;

#           if defined ( TRALLOC_DEBUG )
            ext_env->forced_extensions ^= TRALLOC_EXTENSION_LOCK_POOL;
#           endif

            ext_env->extensions_length = _tralloc_get_extensions_length ( ext_env->extensions );
        }
    }
#   endif

}

static
tralloc_error _tralloc_alloc_initialize_extensions ( _tralloc_chunk * chunk, _tralloc_alloc_extensions_environment * ext_env, size_t total_length, size_t data_length )
{
    tralloc_error result;

#   if defined ( TRALLOC_THREADS )
    _tralloc_subtree_lock * subtree_lock;
    if ( ext_env->have_subtree_lock ) {
        subtree_lock = _tralloc_get_subtree_lock_from_chunk ( chunk );
        result = _tralloc_new_subtree_lock ( subtree_lock );
        if ( result != 0 ) {
            return result;
        }
    }

    _tralloc_children_lock * children_lock;
    if ( ext_env->have_children_lock ) {
        children_lock = _tralloc_get_children_lock_from_chunk ( chunk );
        result = _tralloc_new_children_lock ( children_lock );
        if ( result != 0 ) {
            if ( ext_env->have_subtree_lock ) {
                _tralloc_free_subtree_lock ( subtree_lock );
            }
            return result;
        }
    }

#   if defined ( TRALLOC_POOL )
    _tralloc_pool_lock * pool_lock;
    if ( ext_env->have_pool_lock ) {
        pool_lock = _tralloc_get_pool_lock_from_chunk ( chunk );
        result = _tralloc_new_pool_lock ( pool_lock );
        if ( result != 0 ) {
            if ( ext_env->have_subtree_lock ) {
                _tralloc_free_subtree_lock ( subtree_lock );
            }
            if ( ext_env->have_children_lock ) {
                _tralloc_free_children_lock ( children_lock );
            }
            return result;
        }
    }
#   endif

#   endif

#   if defined ( TRALLOC_POOL )
    if ( ext_env->have_pool ) {
        _tralloc_new_pool ( _tralloc_get_pool_from_chunk ( chunk ), _tralloc_get_context_from_chunk ( chunk ), ext_env->extensions, data_length );
    } else if ( ext_env->have_pool_child ) {
        _tralloc_new_pool_child ( _tralloc_get_pool_child_from_chunk ( chunk ), ext_env->parent_pool, total_length, ext_env->prev_pool_child, ext_env->next_pool_child );
    }
#   endif

#   if defined ( TRALLOC_LENGTH )
    if ( ext_env->have_length ) {
        _tralloc_length * length_ptr = _tralloc_get_length_from_chunk ( chunk );
        * length_ptr = data_length;
    }
#   endif

#   if defined ( TRALLOC_DESTRUCTORS )
    if ( ext_env->have_destructors ) {
        _tralloc_new_destructors ( _tralloc_get_destructors_from_chunk ( chunk ) );
    }
#   endif

#   if defined ( TRALLOC_REFERENCES )
    if ( ext_env->have_references ) {
        _tralloc_new_references ( _tralloc_get_references_from_chunk ( chunk ), ext_env->extensions );
    }

    if ( ext_env->have_reference ) {
        _tralloc_new_reference ( _tralloc_get_reference_from_chunk ( chunk ) );
    }
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

static
tralloc_error _tralloc_alloc ( tralloc_context * parent_context, tralloc_context ** child_context, size_t data_length, _tralloc_alloc_options * opts )
{
    if ( child_context == NULL ) {
        return TRALLOC_ERROR_REQUIRED_ARGUMENT_IS_NULL;
    }

    _tralloc_chunk * parent_chunk;
    if ( parent_context == NULL ) {
        parent_chunk = NULL;
    } else {
        parent_chunk = _tralloc_get_chunk_from_context ( parent_context );
    }

    size_t chunk_length = sizeof ( _tralloc_chunk );
    size_t total_length = chunk_length + data_length;

#   if defined ( TRALLOC_EXTENSIONS )
    _tralloc_alloc_extensions_environment ext_env;
    _tralloc_alloc_prepare_extensions_environment ( parent_chunk, opts->extensions, total_length, &ext_env );
    chunk_length += ext_env.extensions_length;
    total_length += ext_env.extensions_length;
#   endif

    tralloc_error result;

#   if defined ( TRALLOC_DEBUG )
    // Debug should care about thread safety of operations with "parent_chunk" by itself.
    result = _tralloc_debug_before_add_chunk ( parent_chunk, ext_env.extensions, chunk_length, data_length );
    if ( result != 0 ) {
        return result;
    }
#   endif

    void * memory;

#   if defined ( TRALLOC_POOL )
    if ( ext_env.have_pool_child ) {
        _tralloc_alloc_from_pool ( ext_env.parent_pool, &memory, total_length, opts->zero, &ext_env.prev_pool_child, &ext_env.next_pool_child );
    } else {
        if ( opts->zero ) {
            result = _tralloc_calloc ( &memory, total_length );
        } else {
            result = _tralloc_malloc ( &memory, total_length );
        }
        if ( result != 0 ) {
            return result;
        }
    }
#   else
    if ( opts->zero ) {
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
    chunk_memory += ext_env.extensions_length;
#   endif

    _tralloc_chunk * chunk    = ( _tralloc_chunk * ) chunk_memory;
    tralloc_context * context = _tralloc_get_context_from_chunk ( chunk );

#   if defined ( TRALLOC_EXTENSIONS )
    chunk->extensions = ext_env.extensions;

#   if defined ( TRALLOC_DEBUG )
    chunk->forced_extensions = ext_env.forced_extensions;
#   endif
#   endif

#   if defined ( TRALLOC_EXTENSIONS )
    result = _tralloc_alloc_initialize_extensions ( chunk, &ext_env, total_length, data_length );
    if ( result != 0 ) {

#       if defined ( TRALLOC_POOL )
        if ( ext_env.have_pool_child ) {
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
#   if defined ( TRALLOC_DEBUG_LOG )
    result = _tralloc_debug_after_add_chunk ( chunk, chunk_length, data_length, opts->file, opts->line );
#   else
    result = _tralloc_debug_after_add_chunk ( chunk, chunk_length, data_length );
#   endif

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


static inline
tralloc_error _tralloc_forward_to_alloc ( tralloc_context * parent_context, tralloc_context ** child_context, size_t data_length, _tralloc_alloc_options * opts )
{

#   if defined ( TRALLOC_EXTENSIONS )
    opts->extensions = 0;
#   endif

    return _tralloc_alloc ( parent_context, child_context, data_length, opts );
}

#if defined ( TRALLOC_EXTENSIONS )
static inline
tralloc_error _tralloc_forward_to_alloc_with_extensions ( tralloc_context * parent_context, tralloc_context ** child_context, size_t data_length, tralloc_extensions extensions, _tralloc_alloc_options * opts )
{
    opts->extensions = extensions;
    return _tralloc_alloc ( parent_context, child_context, data_length, opts );
}
#endif

#if defined ( TRALLOC_DEBUG_LOG )

static inline
tralloc_error _tralloc_debug_log_forward_to_alloc ( tralloc_context * parent_context, tralloc_context ** child_context, size_t data_length, _tralloc_alloc_options * opts, const char * file, size_t line )
{
    opts->file = file;
    opts->line = line;
    return _tralloc_forward_to_alloc ( parent_context, child_context, data_length, opts );
}
tralloc_error _tralloc_debug_log_new ( tralloc_context * parent_context, tralloc_context ** child_context, size_t data_length, const char * file, size_t line )
{
    _tralloc_alloc_options opts;
    opts.zero = false;
    return _tralloc_debug_log_forward_to_alloc ( parent_context, child_context, data_length, &opts, file, line );
}
tralloc_error _tralloc_debug_log_new_zero ( tralloc_context * parent_context, tralloc_context ** child_context, size_t data_length, const char * file, size_t line )
{
    _tralloc_alloc_options opts;
    opts.zero = true;
    return _tralloc_debug_log_forward_to_alloc ( parent_context, child_context, data_length, &opts, file, line );
}

#if defined ( TRALLOC_EXTENSIONS )
static inline
tralloc_error _tralloc_debug_log_forward_to_alloc_with_extensions ( tralloc_context * parent_context, tralloc_context ** child_context, size_t data_length, tralloc_extensions extensions, _tralloc_alloc_options * opts, const char * file, size_t line )
{
    opts->file = file;
    opts->line = line;
    return _tralloc_forward_to_alloc_with_extensions ( parent_context, child_context, data_length, extensions, opts );
}
tralloc_error _tralloc_debug_log_new_with_extensions ( tralloc_context * parent_context, tralloc_context ** child_context, size_t data_length, tralloc_extensions extensions, const char * file, size_t line )
{
    _tralloc_alloc_options opts;
    opts.zero = false;
    return _tralloc_debug_log_forward_to_alloc_with_extensions ( parent_context, child_context, data_length, extensions, &opts, file, line );
}
tralloc_error _tralloc_debug_log_new_zero_with_extensions ( tralloc_context * parent_context, tralloc_context ** child_context, size_t data_length, tralloc_extensions extensions, const char * file, size_t line )
{
    _tralloc_alloc_options opts;
    opts.zero = true;
    return _tralloc_debug_log_forward_to_alloc_with_extensions ( parent_context, child_context, data_length, extensions, &opts, file, line );
}
#endif

#else

tralloc_error tralloc_new ( tralloc_context * parent_context, tralloc_context ** child_context, size_t data_length )
{
    _tralloc_alloc_options opts;
    opts.zero = false;
    return _tralloc_forward_to_alloc ( parent_context, child_context, data_length, &opts );
}
tralloc_error tralloc_new_zero ( tralloc_context * parent_context, tralloc_context ** child_context, size_t data_length )
{
    _tralloc_alloc_options opts;
    opts.zero = true;
    return _tralloc_forward_to_alloc ( parent_context, child_context, data_length, &opts );
}

#if defined ( TRALLOC_EXTENSIONS )
tralloc_error tralloc_new_with_extensions ( tralloc_context * parent_context, tralloc_context ** child_context, size_t data_length, tralloc_extensions extensions )
{
    _tralloc_alloc_options opts;
    opts.zero = false;
    return _tralloc_forward_to_alloc_with_extensions ( parent_context, child_context, data_length, extensions, &opts );
}
tralloc_error tralloc_new_zero_with_extensions ( tralloc_context * parent_context, tralloc_context ** child_context, size_t data_length, tralloc_extensions extensions )
{
    _tralloc_alloc_options opts;
    opts.zero = true;
    return _tralloc_forward_to_alloc_with_extensions ( parent_context, child_context, data_length, extensions, &opts );
}
#endif

#endif
