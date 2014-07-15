// This file is part of tralloc. Copyright (C) 2013 Andrew Aladjev aladjev.andrew@gmail.com
// tralloc is free software: you can redistribute it and/or modify it under the terms of the GNU General Lesser Public License as published by the Free Software Foundation, either version 3 of the License, or (at your option) any later version.
// tralloc is distributed in the hope that it will be useful, but WITHOUT ANY WARRANTY; without even the implied warranty of MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU General Lesser Public License for more details.
// You should have received a copy of the GNU General Lesser Public License along with tralloc. If not, see <http://www.gnu.org/licenses/>.

#define _TRALLOC_INCLUDED_FROM_TREE_FREE_C
#include <tralloc/tree/free.h>
#include <tralloc/tree/chunk.h>

#if defined ( TRALLOC_DEBUG )
#   include <tralloc/debug/events.h>
#endif

#if defined ( TRALLOC_THREADS )
#   include <tralloc/threads/chunk.h>
#   include <tralloc/threads/lock/subtree.h>
#   include <tralloc/threads/lock/children.h>
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


static inline
tralloc_bool _tralloc_can_free_chunk ( _tralloc_chunk * _TRALLOC_UNUSED ( chunk ) )
{

#   if defined ( TRALLOC_REFERENCE )
    if ( chunk->extensions & TRALLOC_EXTENSION_REFERENCES ) {
        _tralloc_references * references = _tralloc_get_references_from_chunk ( chunk );
        if ( ! _tralloc_can_free_references ( references ) ) {
            return TRALLOC_FALSE;
        }
    }
#   endif

#   if defined ( TRALLOC_POOL )
    if ( chunk->extensions & TRALLOC_EXTENSION_POOL ) {
        _tralloc_pool * pool = _tralloc_get_pool_from_chunk ( chunk );
        if ( ! _tralloc_can_free_pool ( pool ) ) {
            return TRALLOC_FALSE;
        }
    }
#   endif

    return TRALLOC_TRUE;
}

static inline
tralloc_bool _tralloc_can_free_chunk_children ( _tralloc_chunk * _TRALLOC_UNUSED ( chunk ) )
{

#   if defined ( TRALLOC_REFERENCE )
    if ( chunk->extensions & TRALLOC_EXTENSION_REFERENCES ) {
        _tralloc_references * references = _tralloc_get_references_from_chunk ( chunk );
        if ( ! _tralloc_references_can_free_children ( references ) ) {
            return TRALLOC_FALSE;
        }
    }
#   endif

#   if defined ( TRALLOC_POOL )
    if ( chunk->extensions & TRALLOC_EXTENSION_POOL ) {
        _tralloc_pool * pool = _tralloc_get_pool_from_chunk ( chunk );
        if ( ! _tralloc_pool_can_free_children ( pool ) ) {
            return TRALLOC_FALSE;
        }
    }
#   endif

    return TRALLOC_TRUE;
}

static inline
tralloc_error _tralloc_free_chunk ( _tralloc_chunk * chunk )
{
    tralloc_error error = 0, _TRALLOC_UNUSED ( result );
    tralloc_context * context = _tralloc_get_context_from_chunk ( chunk );

#   if defined ( TRALLOC_THREADS )
    if ( chunk->extensions & TRALLOC_EXTENSION_LOCK_SUBTREE ) {
        void * subtree_lock = _tralloc_get_subtree_lock_from_chunk ( chunk );
        result = _tralloc_free_subtree_lock ( subtree_lock );
        if ( result != 0 ) {
            error = result;
        }
    }

    if ( chunk->extensions & TRALLOC_EXTENSION_LOCK_CHILDREN ) {
        void * children_lock = _tralloc_get_children_lock_from_chunk ( chunk );
        result = _tralloc_free_children_lock ( children_lock );
        if ( result != 0 ) {
            error = result;
        }
    }
#   endif

#   if defined ( TRALLOC_DESTRUCTOR )
    if ( chunk->extensions & TRALLOC_EXTENSION_DESTRUCTORS ) {
        _tralloc_destructors * destructors = _tralloc_get_destructors_from_chunk ( chunk );
        result = _tralloc_free_destructors ( destructors, context );
        if ( result != 0 ) {
            error = result;
        }
    }
#   endif

#   if defined ( TRALLOC_REFERENCE )
    if ( chunk->extensions & TRALLOC_EXTENSION_REFERENCE ) {
        _tralloc_reference * reference = _tralloc_get_reference_from_chunk ( chunk );
        result = _tralloc_free_reference ( reference );
        if ( result != 0 ) {
            error = result;
        }
    }
#   endif

#   if defined ( TRALLOC_POOL )
    tralloc_bool have_pool_child = chunk->extensions & TRALLOC_EXTENSION_POOL_CHILD;
    if ( have_pool_child ) {
        _tralloc_pool_child * pool_child = _tralloc_get_pool_child_from_chunk ( chunk );
        result = _tralloc_free_pool_child ( pool_child );
        if ( result != 0 ) {
            error = result;
        }
    }
#   endif

#   if defined ( TRALLOC_POOL )
    if ( have_pool_child ) {
        return error;
    }
#   endif

    void * memory = ( void * ) ( ( uintptr_t ) chunk - _tralloc_get_extensions_length ( chunk->extensions ) );
    free ( memory );
    return error;
}

/*
The algorithm of subtree traversal.
Chunk numbers is the order of traversal.

        1
       / \
      2   6
     /|\
    3 4 5

Each chunk can:
1. prohibit to free it's children.
2. prohibit to be freed itself.

This algorithm is great, because you don't need to allocate big amount of memory like in stack-based or recursion-based algorithms.
But you can't use the vanilla algorithm to free chunks.
For example:
1. free chunk_1.
2. free chunk_2.
2. you can't access chunk_6 by chunk_2->next.

But you can "straighten" the tree.
The idea is simple : you are accessing "first_child" pointer only when you are going slightly lower,
you don't care about "first_child" when you are going slightly higher and to the right.

So you can create vertical list linked by "first_child" pointer and than free each chunk from this list.

Each single chunk, that can't be freed should be detached. (it should have NULL "parent", "prev", "next" and "first_child" pointers)
Each subtree, that can't be freed should be detached. (the root chunk of subtree should have NULL "parent", "prev", "next" pointers)

*/

#if defined ( TRALLOC_DEBUG )

// Function goes through subtree and call _tralloc_debug_before_free_chunk for each chunk.
static inline
tralloc_error _tralloc_before_free_subtree ( _tralloc_chunk * root_chunk )
{
    tralloc_error error  = 0;
    tralloc_error result = _tralloc_debug_before_free_subtree ( root_chunk );
    if ( result != 0 ) {
        error = result;
    }

    tralloc_bool can_free_chunk, can_free_chunk_children;
    _tralloc_chunk * prev_chunk, * next_chunk;
    _tralloc_chunk * chunk = root_chunk;

    while ( TRALLOC_TRUE ) {
        can_free_chunk = _tralloc_can_free_chunk ( chunk );
        if ( can_free_chunk ) {
            result = _tralloc_debug_before_free_chunk ( chunk );
            if ( result != 0 ) {
                error = result;
            }
        }

        can_free_chunk_children = _tralloc_can_free_chunk_children ( chunk );
        if ( can_free_chunk_children ) {
            if ( chunk->first_child != NULL ) {
                // algorithm can go slightly lower.
                chunk = chunk->first_child;

                // next "chunk" is ready.
                continue;
            }
        } else if ( can_free_chunk ) {
            // "chunk" can be freed and it can have children, but algorithm can't free them.
            // These children chunks should be detached.
            next_chunk = chunk->first_child;
            while ( next_chunk != NULL ) {
                prev_chunk = next_chunk;
                next_chunk = next_chunk->next;
                result = _tralloc_debug_before_refuse_to_free_subtree ( prev_chunk );
                if ( result != 0 ) {
                    error = result;
                }
            }
        }
        // algorithm can't go slightly lower.

        // if "root_chunk" is the only chunk, that algorithm can process - it should stops here.
        if ( chunk == root_chunk ) {
            return error;
        }

        while ( chunk->next == NULL ) {
            // algorithm should go slightly higher.
            prev_chunk = chunk;
            chunk = chunk->parent;
            // "prev_chunk->parent", "prev_chunk->prev" and "prev_chunk->next" is not needed now.

            if ( !can_free_chunk ) {
                // if "prev_chunk" can't be freed - it can be subtree or single chunk.
                // it should be detached.
                if ( can_free_chunk_children ) {
                    result = _tralloc_debug_before_refuse_to_free_chunk ( prev_chunk );
                } else {
                    result = _tralloc_debug_before_refuse_to_free_subtree ( prev_chunk );
                }
                if ( result != 0 ) {
                    error = result;
                }
            }

            if ( chunk == root_chunk ) {
                return error;
            }

            // "chunk" has changed - "can_free_chunk", "can_free_chunk_children" variables should be updated.
            can_free_chunk          = _tralloc_can_free_chunk ( chunk );
            can_free_chunk_children = _tralloc_can_free_chunk_children ( chunk );
        }

        // algorithm can go to the right.
        prev_chunk = chunk;
        chunk = chunk->next;
        // "prev_chunk->parent", "prev_chunk->prev" and "prev_chunk->next" is not needed now.

        if ( !can_free_chunk ) {
            // if "prev_chunk" can't be freed - it can be subtree or single chunk.
            // it should be detached.
            if ( can_free_chunk_children ) {
                result = _tralloc_debug_before_refuse_to_free_chunk ( prev_chunk );
            } else {
                result = _tralloc_debug_before_refuse_to_free_subtree ( prev_chunk );
            }
            if ( result != 0 ) {
                error = result;
            }
        }

        // next "chunk" is ready.
    }
}
#endif

// Chunk's neighbor should not know that it has been detached.
static inline
void _tralloc_detach_chunk_silent ( _tralloc_chunk * chunk )
{
    chunk->parent = NULL;
    chunk->prev   = NULL;
    chunk->next   = NULL;
}

// Function provides root chunk of vertical list.
// It can be NULL.
static inline
tralloc_error _tralloc_subtree_to_vertical_list ( _tralloc_chunk * root_chunk, _tralloc_chunk ** list_root_chunk )
{
    tralloc_error error = 0, _TRALLOC_UNUSED ( result );

    tralloc_bool can_free_chunk, can_free_chunk_children;
    _tralloc_chunk * prev_chunk, * next_chunk;
    _tralloc_chunk * chunk = root_chunk;

    _tralloc_chunk * list_chunk = NULL;
    * list_root_chunk           = NULL;

    while ( TRALLOC_TRUE ) {
        can_free_chunk = _tralloc_can_free_chunk ( chunk );
        if ( can_free_chunk ) {
            // current "chunk" can be added to vertical list.
            if ( list_chunk == NULL ) {
                * list_root_chunk = chunk;
            } else {
                list_chunk->first_child = chunk;
            }
            list_chunk = chunk;
        }

        can_free_chunk_children = _tralloc_can_free_chunk_children ( chunk );
        if ( can_free_chunk_children ) {
            if ( chunk->first_child != NULL ) {
                // algorithm can go slightly lower.
                prev_chunk = chunk;
                chunk = chunk->first_child;
                // "prev_chunk->first_child" is not needed now.

                if ( !can_free_chunk ) {
                    // if "prev_chunk" can't be freed - it is definetely single chunk.
                    // it should have NULL "first_child" pointer.
                    prev_chunk->first_child = NULL;
                }

                // next "chunk" is ready.
                continue;
            }
        } else if ( can_free_chunk ) {
            // "chunk" can be freed and it can have children, but algorithm can't free them.
            // These children chunks should be detached.
            next_chunk = chunk->first_child;
            while ( next_chunk != NULL ) {
                prev_chunk = next_chunk;
                next_chunk = next_chunk->next;
                _tralloc_detach_chunk_silent ( prev_chunk );

#               if defined ( TRALLOC_DEBUG )
                result = _tralloc_debug_after_refuse_to_free_subtree ( prev_chunk );
                if ( result != 0 ) {
                    error = result;
                }
#               endif

            }
        }
        // algorithm can't go slightly lower.

        // if "root_chunk" is the only chunk, that algorithm can process - it should stops here.
        if ( chunk == root_chunk ) {
            // vertical list should be closed now.
            if ( list_chunk != NULL ) {
                list_chunk->first_child = NULL;
            }
            return error;
        }

        while ( chunk->next == NULL ) {
            // algorithm should go slightly higher.
            prev_chunk = chunk;
            chunk = chunk->parent;
            // "prev_chunk->parent", "prev_chunk->prev" and "prev_chunk->next" is not needed now.

            if ( !can_free_chunk ) {
                // if "prev_chunk" can't be freed - it can be subtree or single chunk.
                // it should be detached.
                _tralloc_detach_chunk_silent ( prev_chunk );

#               if defined ( TRALLOC_DEBUG )
                if ( can_free_chunk_children ) {
                    result = _tralloc_debug_after_refuse_to_free_chunk ( prev_chunk );
                } else {
                    result = _tralloc_debug_after_refuse_to_free_subtree ( prev_chunk );
                }
                if ( result != 0 ) {
                    error = result;
                }
#               endif

            }

            if ( chunk == root_chunk ) {
                // vertical list should be closed now.
                if ( list_chunk != NULL ) {
                    list_chunk->first_child = NULL;
                }
                return error;
            }

            // "chunk" has changed - "can_free_chunk", "can_free_chunk_children" variables should be updated.
            can_free_chunk          = _tralloc_can_free_chunk ( chunk );
            can_free_chunk_children = _tralloc_can_free_chunk_children ( chunk );
        }

        // algorithm can go to the right.
        prev_chunk = chunk;
        chunk = chunk->next;
        // "prev_chunk->parent", "prev_chunk->prev" and "prev_chunk->next" is not needed now.

        if ( !can_free_chunk ) {
            // if "prev_chunk" can't be freed - it can be subtree or single chunk.
            // it should be detached.
            _tralloc_detach_chunk_silent ( prev_chunk );

#           if defined ( TRALLOC_DEBUG )
            if ( can_free_chunk_children ) {
                result = _tralloc_debug_after_refuse_to_free_chunk ( prev_chunk );
            } else {
                result = _tralloc_debug_after_refuse_to_free_subtree ( prev_chunk );
            }
            if ( result != 0 ) {
                error = result;
            }
#           endif

        }

        // next "chunk" is ready.
    }
}

tralloc_error _tralloc_free_subtree ( _tralloc_chunk * root_chunk )
{
    tralloc_error result, error = 0;

#   if defined ( TRALLOC_DEBUG )
    result = _tralloc_before_free_subtree ( root_chunk );
    if ( result != 0 ) {
        error = result;
    }
#   endif

    _tralloc_attach_chunk ( root_chunk, NULL );

    _tralloc_chunk * prev_chunk, * next_chunk;
    result = _tralloc_subtree_to_vertical_list ( root_chunk, &prev_chunk );
    if ( result != 0 ) {
        error = result;
    }

    while ( prev_chunk != NULL ) {
        next_chunk = prev_chunk->first_child;

        result = _tralloc_free_chunk ( prev_chunk );
        if ( result != 0 ) {
            error = result;
        }

        prev_chunk = next_chunk;
    }

    return error;
}
