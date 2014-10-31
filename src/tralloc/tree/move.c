// This file is part of tralloc. Copyright (C) 2013 Andrew Aladjev aladjev.andrew@gmail.com
// tralloc is free software: you can redistribute it and/or modify it under the terms of the GNU General Lesser Public License as published by the Free Software Foundation, either version 3 of the License, or (at your option) any later version.
// tralloc is distributed in the hope that it will be useful, but WITHOUT ANY WARRANTY; without even the implied warranty of MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU General Lesser Public License for more details.
// You should have received a copy of the GNU General Lesser Public License along with tralloc. If not, see <http://www.gnu.org/licenses/>.

#include <tralloc/tree/move.h>
#include <tralloc/tree/chunk.h>
#include <tralloc/common.h>

#if defined ( TRALLOC_THREADS )
#   include <tralloc/tree/locks/subtree.h>
#   include <tralloc/tree/locks/children.h>
#endif

#if defined ( TRALLOC_DEBUG )
#   include <tralloc/debug/events.h>
#endif


// There are 2 locks from "old_parent_chunk" and "new_parent_chunk", that should be locked before moving "chunk".
// This situation can create a deadlock:
//
//    parent_1   parent_2
//       |          |
//    child_1    child_2
//
// "thread_1" wants to move "child_1" to "parent_2".
// "thread_2" wants to move "child_2" to "parent_1".
//
// "thread_1" see:
//     "old_parent_chunk->children_lock" equals "parent_1->children_lock"
//     "new_parent_chunk->children_lock" equals "parent_2->children_lock"
//
// "thread_2" see:
//     "old_parent_chunk->children_lock" equals "parent_2->children_lock"
//     "new_parent_chunk->children_lock" equals "parent_1->children_lock"
//
// Step 1:
//     "thread_1" locks "old_parent_chunk->children_lock" for writing => it locks "parent_1->children_lock" for writing
//     "thread_2" locks "old_parent_chunk->children_lock" for writing => it locks "parent_2->children_lock" for writing
//
// Step 2:
//     "thread_1" is waiting to lock "new_parent_chunk->children_lock" for writing => "parent_2->children_lock" becomes a deadlock
//     "thread_2" is waiting to lock "new_parent_chunk->children_lock" for writing => "parent_1->children_lock" becomes a deadlock
//
// Move operation do not care about what "children_lock" will be locked first : parent_1 or parent_2.
// This situation can be fixed by defining the order of "children_lock".
// The pointer of "chunk" is changing only in alloc and realloc functions.
// These functions can not take part in threads competition, because it runs in single thread and returns pointer to data only after operation.
// So if pointer "parent_1" is less than "parent_2" - it's "children_lock" should be locked first.

tralloc_error tralloc_move ( tralloc_context * child_context, tralloc_context * parent_context )
{
    if ( child_context == NULL ) {
        return TRALLOC_ERROR_REQUIRED_ARGUMENT_IS_UNDEFINED;
    }
    if ( child_context == parent_context ) {
        return TRALLOC_ERROR_CHILD_EQUALS_PARENT;
    }
    _tralloc_chunk * chunk = _tralloc_get_chunk_from_context ( child_context );

    tralloc_error _TRALLOC_UNUSED ( result );

#   if defined ( TRALLOC_DEBUG )
    // Debug should care about thread safety of operations with "chunk" by itself.
    result = _tralloc_debug_before_move_chunk ( chunk );
    if ( result != 0 ) {
        return result;
    }
#   endif

#   if defined ( TRALLOC_THREADS )
    tralloc_bool have_subtree_lock = chunk->extensions & TRALLOC_EXTENSION_LOCK_SUBTREE;
    _tralloc_subtree_lock * subtree_lock;
    if ( have_subtree_lock ) {
        subtree_lock = _tralloc_get_subtree_lock_from_chunk ( chunk );
        result = _tralloc_wrlock_subtree ( subtree_lock );
        if ( result != 0 ) {
            return result;
        }
    }
#   endif

    // When subtree lock of "chunk" is locked for reading or writing - it is possible to access "chunk->parent", "chunk->prev" and "chunk->next".
    _tralloc_chunk * old_parent_chunk = chunk->parent;

    _tralloc_chunk * new_parent_chunk;
    if ( parent_context == NULL ) {
        new_parent_chunk = NULL;
    } else {
        new_parent_chunk = _tralloc_get_chunk_from_context ( parent_context );
    }

    if ( old_parent_chunk == new_parent_chunk ) {

#       if defined ( TRALLOC_THREADS )
        // It is time to do emergency exit.

        // Subtree lock of "chunk" is locked, it should be unlocked.
        if ( have_subtree_lock ) {
            _tralloc_unlock_subtree ( subtree_lock );
        }
#       endif

        return TRALLOC_ERROR_CHILD_HAS_SAME_PARENT;
    }

    // "chunk->parent" is going to be changed.

#   if defined ( TRALLOC_THREADS )
    tralloc_bool parent_1_have_children_lock;
    tralloc_bool parent_2_have_children_lock;
    _tralloc_children_lock * parent_1_children_lock;
    _tralloc_children_lock * parent_2_children_lock;

    if ( old_parent_chunk < new_parent_chunk ) {
        parent_1_have_children_lock = old_parent_chunk != NULL && ( old_parent_chunk->extensions & TRALLOC_EXTENSION_LOCK_CHILDREN );
        if ( parent_1_have_children_lock ) {
            parent_1_children_lock = _tralloc_get_children_lock_from_chunk ( old_parent_chunk );
        }
        parent_2_have_children_lock = new_parent_chunk != NULL && ( new_parent_chunk->extensions & TRALLOC_EXTENSION_LOCK_CHILDREN );
        if ( parent_2_have_children_lock ) {
            parent_2_children_lock = _tralloc_get_children_lock_from_chunk ( new_parent_chunk );
        }
    } else {
        parent_1_have_children_lock = new_parent_chunk != NULL && ( new_parent_chunk->extensions & TRALLOC_EXTENSION_LOCK_CHILDREN );
        if ( parent_1_have_children_lock ) {
            parent_1_children_lock = _tralloc_get_children_lock_from_chunk ( new_parent_chunk );
        }
        parent_2_have_children_lock = old_parent_chunk != NULL && ( old_parent_chunk->extensions & TRALLOC_EXTENSION_LOCK_CHILDREN );
        if ( parent_2_have_children_lock ) {
            parent_2_children_lock = _tralloc_get_children_lock_from_chunk ( old_parent_chunk );
        }
    }

    if ( parent_1_have_children_lock ) {
        result = _tralloc_wrlock_children ( parent_1_children_lock );
        if ( result != 0 ) {
            // It is time to do emergency exit.

            // Subtree lock of "chunk" is locked, it should be unlocked.
            if ( have_subtree_lock ) {
                _tralloc_unlock_subtree ( subtree_lock );
            }
            return result;
        }
    }

    if ( parent_2_have_children_lock ) {
        result = _tralloc_wrlock_children ( parent_2_children_lock );
        if ( result != 0 ) {
            // It is time to do emergency exit.

            // Subtree lock of "chunk" is locked, it should be unlocked.
            if ( have_subtree_lock ) {
                _tralloc_unlock_subtree ( subtree_lock );
            }
            // "parent_1_children_lock" is locked, it should be unlocked.
            if ( parent_1_have_children_lock ) {
                _tralloc_unlock_children ( parent_1_children_lock );
            }
            return result;
        }
    }
#   endif

    _tralloc_attach_chunk ( chunk, new_parent_chunk );

#   if defined ( TRALLOC_THREADS )
    if ( parent_2_have_children_lock ) {
        result = _tralloc_unlock_children ( parent_2_children_lock );
        if ( result != 0 ) {
            // It is time to do emergency exit.
            // "parent_2_children_lock" have locked status.

            // "chunk->parent" has been changed, old parent should be reverted.
            _tralloc_attach_chunk ( chunk, old_parent_chunk );

            // "parent_1_children_lock" is locked, it should be unlocked.
            if ( parent_1_have_children_lock ) {
                _tralloc_unlock_children ( parent_1_children_lock );
            }
            // Subtree lock of "chunk" is locked, it should be unlocked.
            if ( have_subtree_lock ) {
                _tralloc_unlock_subtree ( subtree_lock );
            }
            return result;
        }
    }

    if ( parent_1_have_children_lock ) {
        result = _tralloc_unlock_children ( parent_1_children_lock );
        if ( result != 0 ) {
            // It is time to do emergency exit.
            // "parent_1_children_lock" have locked status.

            tralloc_error error = result;

            if ( parent_2_have_children_lock ) {
                result = _tralloc_wrlock_children ( parent_2_children_lock );
                if ( result != 0 ) {
                    return error;
                }
            }

            // "chunk->parent" has been changed, old parent should be reverted.
            _tralloc_attach_chunk ( chunk, old_parent_chunk );

            if ( parent_2_have_children_lock ) {
                _tralloc_unlock_children ( parent_2_children_lock );
            }
            // Subtree lock of "chunk" is locked, it should be unlocked.
            if ( have_subtree_lock ) {
                _tralloc_unlock_subtree ( subtree_lock );
            }
            return error;
        }
    }

    if ( have_subtree_lock ) {
        result = _tralloc_unlock_subtree ( subtree_lock );
        if ( result != 0 ) {
            // It is time to do emergency exit.
            // "subtree_lock" have locked status.

            tralloc_error error = result;

            if ( parent_1_have_children_lock ) {
                result = _tralloc_wrlock_children ( parent_1_children_lock );
                if ( result != 0 ) {
                    return error;
                }
            }
            if ( parent_2_have_children_lock ) {
                result = _tralloc_wrlock_children ( parent_2_children_lock );
                if ( result != 0 ) {
                    return error;
                }
            }

            // "chunk->parent" has been changed, old parent should be reverted.
            _tralloc_attach_chunk ( chunk, old_parent_chunk );

            if ( parent_2_have_children_lock ) {
                _tralloc_unlock_children ( parent_2_children_lock );
            }
            if ( parent_1_have_children_lock ) {
                _tralloc_unlock_children ( parent_1_children_lock );
            }

            return error;
        }
    }
#   endif

#   if defined ( TRALLOC_DEBUG )
    // Debug should care about thread safety of operations with "chunk" by itself.
    result = _tralloc_debug_after_move_chunk ( chunk, old_parent_chunk );
    if ( result != 0 ) {
        // It is time to do emergency exit.

        tralloc_error error = result;

#       if defined ( TRALLOC_THREADS )
        if ( have_subtree_lock ) {
            result = _tralloc_wrlock_subtree ( subtree_lock );
            if ( result != 0 ) {
                return error;
            }
        }
        if ( parent_1_have_children_lock ) {
            result = _tralloc_wrlock_children ( parent_1_children_lock );
            if ( result != 0 ) {
                return error;
            }
        }
        if ( parent_2_have_children_lock ) {
            result = _tralloc_wrlock_children ( parent_2_children_lock );
            if ( result != 0 ) {
                return error;
            }
        }
#       endif

        // "chunk->parent" has been changed, old parent should be reverted.
        _tralloc_attach_chunk ( chunk, old_parent_chunk );

#       if defined ( TRALLOC_THREADS )
        if ( have_subtree_lock ) {
            _tralloc_unlock_subtree ( subtree_lock );
        }
        if ( parent_2_have_children_lock ) {
            _tralloc_unlock_children ( parent_2_children_lock );
        }
        if ( parent_1_have_children_lock ) {
            _tralloc_unlock_children ( parent_1_children_lock );
        }
#       endif

        return error;
    }
#   endif

    return 0;
}
