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
tralloc_error _tralloc_alloc ( tralloc_context * parent_context, tralloc_context ** child_context, size_t length, _tralloc_alloc_options * options )
{
    if ( child_context == NULL ) {
        return TRALLOC_ERROR_REQUIRED_ARGUMENT_IS_NULL;
    }

    tralloc_error result;
    _tralloc_chunk * parent_chunk;

#   if defined ( TRALLOC_POOL )
    _tralloc_pool * parent_pool;
#   endif

    if ( parent_context == NULL ) {
        parent_chunk = NULL;

#       if defined ( TRALLOC_POOL )
        parent_pool = NULL;
#       endif
    } else {
        parent_chunk = _tralloc_get_chunk_from_context ( parent_context );

#       if defined ( TRALLOC_POOL )
        parent_pool = _tralloc_get_closest_pool_from_chunk ( parent_chunk );
#       endif
    }

#   if defined ( TRALLOC_DEBUG )
    tralloc_extensions forced_extensions = 0;
    // "forced_extensions ^= EXTENSION" means "toggle forced extension":
    // If "EXTENSION" was not forced - it becomes forced
    // If "EXTENSION" was forced     - it becomes not forced
#   endif

#   if defined ( TRALLOC_THREADS )
    tralloc_bool have_subtree_lock = options->extensions & TRALLOC_EXTENSION_LOCK_SUBTREE;

#   if defined ( TRALLOC_DEBUG_THREADS )
    if ( !have_subtree_lock ) {
        // "TRALLOC_EXTENSION_LOCK_SUBTREE" will be forced enabled.
        have_subtree_lock   = TRALLOC_TRUE;
        options->extensions |= TRALLOC_EXTENSION_LOCK_SUBTREE;
        forced_extensions   ^= TRALLOC_EXTENSION_LOCK_SUBTREE;
    }
#   endif

    tralloc_bool have_children_lock = options->extensions & TRALLOC_EXTENSION_LOCK_CHILDREN;

#   if defined ( TRALLOC_DEBUG_THREADS )
    if ( !have_children_lock ) {
        // "TRALLOC_EXTENSION_LOCK_CHILDREN" will be forced enabled.
        have_children_lock  = TRALLOC_TRUE;
        options->extensions |= TRALLOC_EXTENSION_LOCK_CHILDREN;
        forced_extensions   ^= TRALLOC_EXTENSION_LOCK_CHILDREN;
    }
#   endif

#   endif

#   if defined ( TRALLOC_LENGTH )
    tralloc_bool have_length = options->extensions & TRALLOC_EXTENSION_LENGTH;
#   endif

#   if defined ( TRALLOC_DESTRUCTORS )
    tralloc_bool have_destructors = options->extensions & TRALLOC_EXTENSION_DESTRUCTORS;
#   endif

#   if defined ( TRALLOC_REFERENCES )
    // Chunk can have both "TRALLOC_EXTENSION_REFERENCES" and "TRALLOC_EXTENSION_REFERENCE".
    tralloc_bool have_references = options->extensions & TRALLOC_EXTENSION_REFERENCES;
    tralloc_bool have_reference  = options->extensions & TRALLOC_EXTENSION_REFERENCE;
#   endif

#   if defined ( TRALLOC_POOL )
    // Chunk can't have both "TRALLOC_EXTENSION_POOL" and "TRALLOC_EXTENSION_POOL_CHILD".
    tralloc_bool have_pool       = options->extensions & TRALLOC_EXTENSION_POOL;
    tralloc_bool have_pool_child = options->extensions & TRALLOC_EXTENSION_POOL_CHILD;

    if ( have_pool ) {
        if ( have_pool_child ) {
            // "TRALLOC_EXTENSION_POOL_CHILD" will be forced disabled.
            have_pool_child     = TRALLOC_FALSE;
            options->extensions &= ~TRALLOC_EXTENSION_POOL_CHILD;

#           if defined ( TRALLOC_DEBUG )
            forced_extensions ^= TRALLOC_EXTENSION_POOL_CHILD;
#           endif
        }
    } else {
        if ( parent_pool != NULL ) {
            if ( !have_pool_child ) {
                // "TRALLOC_EXTENSION_POOL_CHILD" will be forced enabled.
                have_pool_child     = TRALLOC_TRUE;
                options->extensions |= TRALLOC_EXTENSION_POOL_CHILD;

#               if defined ( TRALLOC_DEBUG )
                forced_extensions ^= TRALLOC_EXTENSION_POOL_CHILD;
#               endif
            }
        } else {
            if ( have_pool_child ) {
                have_pool_child     = TRALLOC_FALSE;
                options->extensions &= ~TRALLOC_EXTENSION_POOL_CHILD;

#               if defined ( TRALLOC_DEBUG )
                forced_extensions ^= TRALLOC_EXTENSION_POOL_CHILD;
#               endif
            }
        }
    }

#   if defined ( TRALLOC_THREADS )
    tralloc_bool have_pool_lock = options->extensions & TRALLOC_EXTENSION_LOCK_POOL;

    if ( have_pool ) {

#       if defined ( TRALLOC_DEBUG_THREADS )
        if ( !have_pool_lock ) {
            // "TRALLOC_EXTENSION_LOCK_POOL" will be forced enabled.
            have_pool_lock      = TRALLOC_TRUE;
            options->extensions |= TRALLOC_EXTENSION_LOCK_POOL;
            forced_extensions   ^= TRALLOC_EXTENSION_LOCK_POOL;
        }
#       endif

    } else {
        if ( have_pool_lock ) {
            // "TRALLOC_EXTENSION_LOCK_POOL" will be forced disabled.
            have_pool_lock      = TRALLOC_FALSE;
            options->extensions &= ~TRALLOC_EXTENSION_LOCK_POOL;

#           if defined ( TRALLOC_DEBUG )
            forced_extensions ^= TRALLOC_EXTENSION_LOCK_POOL;
#           endif
        }
    }

#   endif

#   endif

    size_t extensions_length = _tralloc_get_extensions_length ( options->extensions );
    size_t chunk_length      = sizeof ( _tralloc_chunk ) + extensions_length;
    size_t total_length      = chunk_length + length;

#   if defined ( TRALLOC_POOL )
    if ( have_pool_child ) {
        if ( !_tralloc_can_alloc_from_pool ( parent_pool, total_length ) ) {
            // "parent_pool" can't alloc "total_length" bytes.
            // "TRALLOC_EXTENSION_POOL_CHILD" will be forced disabled.

            have_pool_child     = TRALLOC_FALSE;
            options->extensions &= ~TRALLOC_EXTENSION_POOL_CHILD;

#           if defined ( TRALLOC_DEBUG )
            forced_extensions ^= TRALLOC_EXTENSION_LOCK_POOL;
#           endif

            extensions_length -= sizeof ( _tralloc_pool_child );
            chunk_length      -= sizeof ( _tralloc_pool_child );
            total_length      -= sizeof ( _tralloc_pool_child );
        }
    }
#   endif

#   if defined ( TRALLOC_DEBUG )
    // Debug should care about thread safety of operations with "parent_chunk" by itself.
    result = _tralloc_debug_before_add_chunk ( parent_chunk, options->extensions, chunk_length, length );
    if ( result != 0 ) {
        return result;
    }
#   endif

    void * memory;

#   if defined ( TRALLOC_POOL )
    _tralloc_pool_child * prev_pool_child, * next_pool_child;
    if ( have_pool_child ) {
        _tralloc_alloc_from_pool ( parent_pool, &memory, total_length, options->zero, &prev_pool_child, &next_pool_child );
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

    _tralloc_chunk * chunk    = ( _tralloc_chunk * ) ( ( uintptr_t ) memory + extensions_length );
    tralloc_context * context = _tralloc_get_context_from_chunk ( chunk );

#   if defined ( TRALLOC_EXTENSIONS )
    chunk->extensions = options->extensions;

#   if defined ( TRALLOC_DEBUG )
    chunk->forced_extensions = forced_extensions;
#   endif
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

            // Subtree lock of "chunk" has been allocated, it should be freed.
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

#   if defined ( TRALLOC_POOL )
    _tralloc_pool_lock * pool_lock;
    if ( have_pool_lock ) {
        pool_lock = _tralloc_get_pool_lock_from_chunk ( chunk );
        result = _tralloc_new_pool_lock ( pool_lock );
        if ( result != 0 ) {
            // It is time to do emergency exit.

            // Subtree lock of "chunk" has been allocated, it should be freed.
            if ( have_subtree_lock ) {
                _tralloc_free_subtree_lock ( subtree_lock );
            }
            // Children lock of "chunk" has been allocated, it should be freed.
            if ( have_children_lock ) {
                _tralloc_free_children_lock ( children_lock );
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

#   endif

#   if defined ( TRALLOC_LENGTH )
    if ( have_length ) {
        _tralloc_length * length_ptr = _tralloc_get_length_from_chunk ( chunk );
        * length_ptr = length;
    }
#   endif

#   if defined ( TRALLOC_DESTRUCTORS )
    _tralloc_destructors * destructors;
    if ( have_destructors ) {
        destructors = _tralloc_get_destructors_from_chunk ( chunk );
        _tralloc_new_destructors ( destructors );
    }
#   endif

#   if defined ( TRALLOC_REFERENCES )
    _tralloc_references * references;
    if ( have_references ) {
        references = _tralloc_get_references_from_chunk ( chunk );
        _tralloc_new_references ( references, options->extensions );
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
        _tralloc_new_pool ( pool, context, options->extensions, length );
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

                // Subtree lock of "chunk" has been allocated, it should be freed.
                if ( have_subtree_lock ) {
                    _tralloc_free_subtree_lock ( subtree_lock );
                }
                // Children lock of "chunk" has been allocated, it should be freed.
                if ( have_children_lock ) {
                    _tralloc_free_children_lock ( children_lock );
                }

#               if defined ( TRALLOC_POOL )
                // Pool lock of "chunk" has been allocated, it should be freed.
                if ( have_pool_lock ) {
                    _tralloc_free_pool_lock ( pool_lock );
                }
#               endif

#               if defined ( TRALLOC_DESTRUCTORS )
                // Destructors of "chunk" has been allocated, it should be freed.
                if ( have_destructors ) {
                    _tralloc_free_destructors ( destructors, context );
                }
#               endif

#               if defined ( TRALLOC_REFERENCES )
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
                // "parent_children_lock" have locked status.

                // Chunk should be detached.
                _tralloc_attach_chunk ( chunk, NULL );

                // Subtree lock of "chunk" has been allocated, it should be freed.
                if ( have_subtree_lock ) {
                    _tralloc_free_subtree_lock ( subtree_lock );
                }
                // Children lock of "chunk" has been allocated, it should be freed.
                if ( have_children_lock ) {
                    _tralloc_free_children_lock ( children_lock );
                }

#               if defined ( TRALLOC_POOL )
                // Pool lock of "chunk" has been allocated, it should be freed.
                if ( have_pool_lock ) {
                    _tralloc_free_pool_lock ( pool_lock );
                }
#               endif

#               if defined ( TRALLOC_DESTRUCTORS )
                // Destructors of "chunk" has been allocated, it should be freed.
                if ( have_destructors ) {
                    _tralloc_free_destructors ( destructors, context );
                }
#               endif

#               if defined ( TRALLOC_REFERENCES )
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
    result = _tralloc_debug_after_add_chunk ( chunk, chunk_length, length, options->file, options->line );
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

        // Subtree lock of "chunk" has been allocated, it should be freed.
        if ( have_subtree_lock ) {
            _tralloc_free_subtree_lock ( subtree_lock );
        }
        // Children lock of "chunk" has been allocated, it should be freed.
        if ( have_children_lock ) {
            _tralloc_free_children_lock ( children_lock );
        }

#       if defined ( TRALLOC_POOL )
        // Pool lock of "chunk" has been allocated, it should be freed.
        if ( have_pool_lock ) {
            _tralloc_free_pool_lock ( pool_lock );
        }
#       endif

#       endif

#       if defined ( TRALLOC_DESTRUCTORS )
        // Destructors of "chunk" has been allocated, it should be freed.
        if ( have_destructors ) {
            _tralloc_free_destructors ( destructors, context );
        }
#       endif

#       if defined ( TRALLOC_REFERENCES )
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


static inline
tralloc_error _tralloc_prepare ( tralloc_context * parent_context, tralloc_context ** child_context, size_t length, _tralloc_alloc_options * options )
{

#   if defined ( TRALLOC_EXTENSIONS )
    options->extensions = 0;
#   endif

    return _tralloc_alloc ( parent_context, child_context, length, options );
}

#if defined ( TRALLOC_EXTENSIONS )
static inline
tralloc_error _tralloc_prepare_with_extensions ( tralloc_context * parent_context, tralloc_context ** child_context, size_t length, tralloc_extensions extensions, _tralloc_alloc_options * options )
{
    options->extensions = extensions;
    return _tralloc_alloc ( parent_context, child_context, length, options );
}
#endif

#if defined ( TRALLOC_DEBUG_LOG )

static inline
tralloc_error _tralloc_debug_log_prepare ( tralloc_context * parent_context, tralloc_context ** child_context, size_t length, _tralloc_alloc_options * options, const char * file, size_t line )
{
    options->file = file;
    options->line = line;
    return _tralloc_prepare ( parent_context, child_context, length, options );
}
tralloc_error _tralloc_debug_log_new ( tralloc_context * parent_context, tralloc_context ** child_context, size_t length, const char * file, size_t line )
{
    _tralloc_alloc_options options;
    options.zero = false;
    return _tralloc_debug_log_prepare ( parent_context, child_context, length, &options, file, line );
}
tralloc_error _tralloc_debug_log_new_zero ( tralloc_context * parent_context, tralloc_context ** child_context, size_t length, const char * file, size_t line )
{
    _tralloc_alloc_options options;
    options.zero = true;
    return _tralloc_debug_log_prepare ( parent_context, child_context, length, &options, file, line );
}

#if defined ( TRALLOC_EXTENSIONS )
static inline
tralloc_error _tralloc_debug_log_prepare_with_extensions ( tralloc_context * parent_context, tralloc_context ** child_context, size_t length, tralloc_extensions extensions, _tralloc_alloc_options * options, const char * file, size_t line )
{
    options->file = file;
    options->line = line;
    return _tralloc_prepare_with_extensions ( parent_context, child_context, length, extensions, options );
}
tralloc_error _tralloc_debug_log_new_with_extensions ( tralloc_context * parent_context, tralloc_context ** child_context, size_t length, tralloc_extensions extensions, const char * file, size_t line )
{
    _tralloc_alloc_options options;
    options.zero = false;
    return _tralloc_debug_log_prepare_with_extensions ( parent_context, child_context, length, extensions, &options, file, line );
}
tralloc_error _tralloc_debug_log_new_zero_with_extensions ( tralloc_context * parent_context, tralloc_context ** child_context, size_t length, tralloc_extensions extensions, const char * file, size_t line )
{
    _tralloc_alloc_options options;
    options.zero = true;
    return _tralloc_debug_log_prepare_with_extensions ( parent_context, child_context, length, extensions, &options, file, line );
}
#endif

#else

tralloc_error tralloc_new ( tralloc_context * parent_context, tralloc_context ** child_context, size_t length )
{
    _tralloc_alloc_options options;
    options.zero = false;
    return _tralloc_prepare ( parent_context, child_context, length, &options );
}
tralloc_error tralloc_new_zero ( tralloc_context * parent_context, tralloc_context ** child_context, size_t length )
{
    _tralloc_alloc_options options;
    options.zero = true;
    return _tralloc_prepare ( parent_context, child_context, length, &options );
}

#if defined ( TRALLOC_EXTENSIONS )
tralloc_error tralloc_new_with_extensions ( tralloc_context * parent_context, tralloc_context ** child_context, size_t length, tralloc_extensions extensions )
{
    _tralloc_alloc_options options;
    options.zero = false;
    return _tralloc_prepare_with_extensions ( parent_context, child_context, length, extensions, &options );
}
tralloc_error tralloc_new_zero_with_extensions ( tralloc_context * parent_context, tralloc_context ** child_context, size_t length, tralloc_extensions extensions )
{
    _tralloc_alloc_options options;
    options.zero = true;
    return _tralloc_prepare_with_extensions ( parent_context, child_context, length, extensions, &options );
}
#endif

#endif
