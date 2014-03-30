// This file is part of tralloc. Copyright (C) 2013 Andrew Aladjev aladjev.andrew@gmail.com
// tralloc is free software: you can redistribute it and/or modify it under the terms of the GNU General Lesser Public License as published by the Free Software Foundation, either version 3 of the License, or (at your option) any later version.
// tralloc is distributed in the hope that it will be useful, but WITHOUT ANY WARRANTY; without even the implied warranty of MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU General Lesser Public License for more details.
// You should have received a copy of the GNU General Lesser Public License along with tralloc. If not, see <http://www.gnu.org/licenses/>.

#include "chunk.h"
#include "../common.h"

#if defined(TRALLOC_DEBUG)
#   include "../events.h"
#endif


void _tralloc_update_chunk ( _tralloc_chunk * chunk )
{
    _tralloc_chunk * prev = chunk->prev;
    if ( prev == NULL ) {
        _tralloc_chunk * parent = chunk->parent;
        if ( parent != NULL ) {
            parent->first_child = chunk;
        }
    } else {
        prev->next = chunk;
    }
    _tralloc_chunk * next = chunk->next;
    if ( next != NULL ) {
        next->prev = chunk;
    }

    _tralloc_chunk * next_child = chunk->first_child;
    while ( next_child != NULL ) {
        next_child->parent = chunk;
        next_child = next_child->next;
    }
}

tralloc_error _tralloc_attach_chunk ( _tralloc_chunk * child, _tralloc_chunk * new_parent )
{
    _tralloc_chunk * parent = child->parent;
    _tralloc_chunk * prev   = child->prev;
    _tralloc_chunk * next   = child->next;
    _tralloc_chunk * new_first_child = new_parent->first_child;

#   if defined(TRALLOC_DEBUG) && defined(TRALLOC_THREADS)

    // child->parent           = new_parent;
    // new_parent->first_child = child;

    if ( pthread_equal ( child->thread_id, new_parent->thread_id ) == 0 ) {
        if ( ! ( child->extensions & TRALLOC_EXTENSION_LOCK_PARENT ) ) {
            return TRALLOC_ERROR_NO_PARENT_LOCK;
        }
        if ( ! ( new_parent->extensions & TRALLOC_EXTENSION_LOCK_FIRST_CHILD ) ) {
            return TRALLOC_ERROR_NO_FIRST_CHILD_LOCK;
        }
    }

    if ( prev != NULL ) {
        if ( next != NULL ) {

            // prev->next = next;
            // next->prev = prev;

            if ( pthread_equal ( prev->thread_id, next->thread_id ) == 0 ) {
                if ( ! ( prev->extensions & TRALLOC_EXTENSION_LOCK_NEXT ) ) {
                    return TRALLOC_ERROR_NO_NEXT_LOCK;
                }
                if ( ! ( next->extensions & TRALLOC_EXTENSION_LOCK_PREV ) ) {
                    return TRALLOC_ERROR_NO_PREV_LOCK;
                }
            }
        }
    } else if ( parent != NULL ) {
        if ( next != NULL ) {

            // parent->first_child = next;

            if ( pthread_equal ( parent->thread_id, next->thread_id ) == 0 ) {
                if ( ! ( next->extensions & TRALLOC_EXTENSION_LOCK_PARENT ) ) {
                    return TRALLOC_ERROR_NO_PARENT_LOCK;
                }
                if ( ! ( parent->extensions & TRALLOC_EXTENSION_LOCK_FIRST_CHILD ) ) {
                    return TRALLOC_ERROR_NO_FIRST_CHILD_LOCK;
                }
            }
        }
    }

    if ( new_first_child != NULL ) {

        // new_first_child->prev = child;
        // child->next = new_first_child;

        if ( pthread_equal ( new_first_child->thread_id, child->thread_id ) == 0 ) {
            if ( ! ( child->extensions & TRALLOC_EXTENSION_LOCK_NEXT ) ) {
                return TRALLOC_ERROR_NO_NEXT_LOCK;
            }
            if ( ! ( new_first_child->extensions & TRALLOC_EXTENSION_LOCK_PREV ) ) {
                return TRALLOC_ERROR_NO_PREV_LOCK;
            }
        }
    }
#   endif

    child->parent = new_parent;
    child->prev   = NULL;

    if ( prev != NULL ) {
        prev->next = next;
    } else if ( parent != NULL ) {
        parent->first_child = next;
    }
    if ( next != NULL ) {
        next->prev = prev;
    }

    if ( new_first_child != NULL ) {
        new_first_child->prev = child;
        child->next = new_first_child;
    } else {
        child->next = NULL;
    }
    new_parent->first_child = child;

    return 0;
}

void _tralloc_detach_chunk ( _tralloc_chunk * chunk )
{
    _tralloc_chunk * prev   = chunk->prev;
    _tralloc_chunk * next   = chunk->next;
    _tralloc_chunk * parent = chunk->parent;

    chunk->parent = NULL;
    chunk->prev   = NULL;
    chunk->next   = NULL;

    if ( prev != NULL ) {
        prev->next = next;
    } else if ( parent != NULL ) {
        parent->first_child = next;
    }
    if ( next != NULL ) {
        next->prev = prev;
    }
}

tralloc_error _tralloc_add_chunk ( tralloc_context * parent_context, _tralloc_chunk * child_chunk )
{
    child_chunk->parent      = NULL;
    child_chunk->first_child = NULL;
    child_chunk->prev        = NULL;
    child_chunk->next        = NULL;

    if ( parent_context != NULL ) {
        tralloc_error result = _tralloc_attach_chunk ( child_chunk, _tralloc_get_chunk_from_context ( parent_context ) );
        if ( result != 0 ) {
            return result;
        }
    }

#   if defined(TRALLOC_DEBUG)
    return _tralloc_on_add ( child_chunk );
#   else
    return 0;
#   endif

}
