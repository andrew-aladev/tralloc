// This file is part of tralloc. Copyright (C) 2013 Andrew Aladjev aladjev.andrew@gmail.com
// tralloc is free software: you can redistribute it and/or modify it under the terms of the GNU General Lesser Public License as published by the Free Software Foundation, either version 3 of the License, or (at your option) any later version.
// tralloc is distributed in the hope that it will be useful, but WITHOUT ANY WARRANTY; without even the implied warranty of MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU General Lesser Public License for more details.
// You should have received a copy of the GNU General Lesser Public License along with tralloc. If not, see <http://www.gnu.org/licenses/>.

#include <tralloc/tree/move.h>
#include <tralloc/tree/chunk.h>
#include <tralloc/common.h>

#if defined(TRALLOC_THREADS)
#   include <tralloc/threads/chunk.h>
#   include <tralloc/threads/mutex.h>
#endif

#if defined(TRALLOC_DEBUG)
#   include <tralloc/debug/events.h>
#endif


tralloc_error tralloc_move ( tralloc_context * child_context, tralloc_context * parent_context )
{
    if ( child_context == NULL ) {
        return TRALLOC_ERROR_REQUIRED_ARGUMENT_IS_NULL;
    }
    if ( child_context == parent_context ) {
        return TRALLOC_ERROR_CHILD_EQUALS_PARENT;
    }
    _tralloc_chunk * child_chunk = _tralloc_get_chunk_from_context ( child_context );

    tralloc_error _TRALLOC_UNUSED ( result );

#   if defined(TRALLOC_DEBUG)
    // Debug should care about thread safety of operations with "child_chunk" by itself.
    result = _tralloc_debug_before_move_chunk ( child_chunk );
    if ( result != 0 ) {
        return result;
    }
#   endif
    
#   if defined(TRALLOC_THREADS)
    // Locking subtree mutex of "child_chunk".
    _tralloc_mutex * child_subtree_mutex;
    tralloc_bool child_have_subtree_mutex = child_chunk->extensions & TRALLOC_EXTENSION_LOCK_SUBTREE;
    if ( child_have_subtree_mutex ) {
        child_subtree_mutex = _tralloc_get_subtree_mutex_from_chunk ( child_chunk );
        result = _tralloc_mutex_lock ( child_subtree_mutex );
        if ( result != 0 ) {
            return result;
        }
    }
#   endif

    // When subtree mutex of "child_chunk" is locked - it is possible to access "child_chunk"'s parent, prev and next.
    _tralloc_chunk * old_parent_chunk = child_chunk->parent;

    _tralloc_chunk * new_parent_chunk;
    if ( parent_context == NULL ) {
        new_parent_chunk = NULL;
    } else {
        new_parent_chunk = _tralloc_get_chunk_from_context ( parent_context );
    }

    if ( old_parent_chunk == new_parent_chunk ) {

#       if defined(TRALLOC_THREADS)
        // It is time to do emergency exit, but subtree mutex of "child_chunk" is locked, it should be unlocked.
        if ( child_have_subtree_mutex ) {
            _tralloc_mutex_unlock ( child_subtree_mutex );
        }
#       endif

        return TRALLOC_ERROR_CHILD_HAS_SAME_PARENT;
    }

    // "child_chunk"'s parent is going to be changed.

#   if defined(TRALLOC_THREADS)
    // Locking children mutex of "old_parent_chunk".
    _tralloc_mutex * old_parent_children_mutex;
    tralloc_bool old_parent_have_children_mutex = old_parent_chunk != NULL && ( old_parent_chunk->extensions & TRALLOC_EXTENSION_LOCK_CHILDREN );
    if ( old_parent_have_children_mutex ) {
        old_parent_children_mutex = _tralloc_get_children_mutex_from_chunk ( old_parent_chunk );
        result = _tralloc_mutex_lock ( old_parent_children_mutex );
        if ( result != 0 ) {
            // It is time to do emergency exit, but subtree mutex of "child_chunk" is locked, it should be unlocked.
            if ( child_have_subtree_mutex ) {
                _tralloc_mutex_unlock ( child_subtree_mutex );
            }
            return result;
        }
    }

    // Locking children mutex of "new_parent_chunk".
    _tralloc_mutex * new_parent_children_mutex;
    tralloc_bool new_parent_have_children_mutex = new_parent_chunk != NULL && ( new_parent_chunk->extensions & TRALLOC_EXTENSION_LOCK_CHILDREN );
    if ( new_parent_have_children_mutex ) {
        new_parent_children_mutex = _tralloc_get_children_mutex_from_chunk ( new_parent_chunk );
        result = _tralloc_mutex_lock ( new_parent_children_mutex );
        if ( result != 0 ) {
            // It is time to do emergency exit.

            // Children mutex of "old_parent_chunk" is locked, it should be unlocked.
            if ( old_parent_have_children_mutex ) {
                _tralloc_mutex_unlock ( old_parent_children_mutex );
            }

            // Subtree mutex of "child_chunk" is locked, it should be unlocked.
            if ( child_have_subtree_mutex ) {
                _tralloc_mutex_unlock ( child_subtree_mutex );
            }
            return result;
        }
    }
#   endif

    _tralloc_attach_chunk ( child_chunk, new_parent_chunk );

#   if defined(TRALLOC_THREADS)
    // Unlocking children mutex of "new_parent_chunk".
    if ( new_parent_have_children_mutex ) {
        result = _tralloc_mutex_unlock ( new_parent_children_mutex );
        if ( result != 0 ) {
            // It is time to do emergency exit.

            // "child_chunk"'s parent was changed, old parent should be reverted.
            _tralloc_attach_chunk ( child_chunk, old_parent_chunk );

            // Children mutex of "old_parent_chunk" is locked, it should be unlocked.
            if ( old_parent_have_children_mutex ) {
                _tralloc_mutex_unlock ( old_parent_children_mutex );
            }

            // Subtree mutex of "child_chunk" is locked, it should be unlocked.
            if ( child_have_subtree_mutex ) {
                _tralloc_mutex_unlock ( child_subtree_mutex );
            }
            return result;
        }
    }

    // Unlocking children mutex of "old_parent_chunk".
    if ( old_parent_have_children_mutex ) {
        result = _tralloc_mutex_unlock ( old_parent_children_mutex );
        if ( result != 0 ) {
            // It is time to do emergency exit.

            // "child_chunk"'s parent was changed, old parent should be reverted.
            _tralloc_attach_chunk ( child_chunk, old_parent_chunk );

            // Subtree mutex of "child_chunk" is locked, it should be unlocked.
            if ( child_have_subtree_mutex ) {
                _tralloc_mutex_unlock ( child_subtree_mutex );
            }
            return result;
        }
    }
    
    // Unlocking subtree mutex of "child_chunk".
    if ( child_have_subtree_mutex ) {
        result = _tralloc_mutex_unlock ( child_subtree_mutex );
        if ( result != 0 ) {
            // It is time to do emergency exit, but "child_chunk"'s parent was changed, old parent should be reverted.
            _tralloc_attach_chunk ( child_chunk, old_parent_chunk );

            return result;
        }
    }
#   endif

#   if defined(TRALLOC_DEBUG)
    // Debug should care about thread safety of operations with "child_chunk" by itself.
    result = _tralloc_debug_after_move_chunk ( child_chunk, old_parent_chunk );
    if ( result != 0 ) {
        // It is time to do emergency exit, but "child_chunk"'s parent was changed, old parent should be reverted.
        _tralloc_attach_chunk ( child_chunk, old_parent_chunk );

        return result;
    }
#   endif

    return 0;
}
