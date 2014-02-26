// This file is part of tralloc. Copyright (C) 2013 Andrew Aladjev aladjev.andrew@gmail.com
// tralloc is free software: you can redistribute it and/or modify it under the terms of the GNU General Lesser Public License as published by the Free Software Foundation, either version 3 of the License, or (at your option) any later version.
// tralloc is distributed in the hope that it will be useful, but WITHOUT ANY WARRANTY; without even the implied warranty of MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU General Lesser Public License for more details.
// You should have received a copy of the GNU General Lesser Public License along with tralloc. If not, see <http://www.gnu.org/licenses/>.

#define TRALLOC_TREE_COMMON_INCLUDED_FROM_OBJECT
#include "../tree.h"

#if defined(TRALLOC_DEBUG)
#include "../events.h"
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

#if defined(TRALLOC_DEBUG)
    _tralloc_chunk * old_parent_chunk;
#endif

    if ( parent_context == NULL ) {
        if ( child_chunk->parent == NULL ) {
            return TRALLOC_ERROR_CHILD_HAS_SAME_PARENT;
        }

#if defined(TRALLOC_DEBUG)
        old_parent_chunk = child_chunk->parent;
#endif

        _tralloc_detach_chunk ( child_chunk );
    } else {
        _tralloc_chunk * new_parent_chunk = _tralloc_get_chunk_from_context ( parent_context );
        if ( child_chunk->parent == new_parent_chunk ) {
            return TRALLOC_ERROR_CHILD_HAS_SAME_PARENT;
        }

#if defined(TRALLOC_DEBUG)
        old_parent_chunk = child_chunk->parent;
#endif

        _tralloc_attach_chunk ( child_chunk, new_parent_chunk );
    }

#if defined(TRALLOC_DEBUG)
    return _tralloc_on_move ( child_chunk, old_parent_chunk );
#else
    return 0;
#endif

}


tralloc_error _tralloc_add_chunk ( tralloc_context * parent_context, _tralloc_chunk * child_chunk )
{
    child_chunk->first_child = NULL;

    if ( parent_context != NULL ) {
        _tralloc_chunk * parent_chunk       = _tralloc_get_chunk_from_context ( parent_context );
        _tralloc_chunk * parent_first_child = parent_chunk->first_child;

        child_chunk->parent = parent_chunk;

        if ( parent_first_child != NULL ) {
            parent_first_child->prev = child_chunk;
            child_chunk->next        = parent_first_child;
        } else {
            child_chunk->next = NULL;
        }
        parent_chunk->first_child = child_chunk;
    } else {
        child_chunk->parent = NULL;
        child_chunk->next   = NULL;
    }

    child_chunk->prev = NULL;

#if defined(TRALLOC_DEBUG)
    return _tralloc_on_add ( child_chunk );
#else
    return 0;
#endif

}

void _tralloc_attach_chunk ( _tralloc_chunk * child, _tralloc_chunk * new_parent )
{
    _tralloc_chunk * parent = child->parent;
    _tralloc_chunk * prev   = child->prev;
    _tralloc_chunk * next   = child->next;

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

    _tralloc_chunk * first_child = new_parent->first_child;
    if ( first_child != NULL ) {
        first_child->prev = child;
        child->next = first_child;
    } else {
        child->next = NULL;
    }
    new_parent->first_child = child;
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


static inline
void _tralloc_silent_detach_chunk ( _tralloc_chunk * chunk )
{
    chunk->parent = NULL;
    chunk->prev   = NULL;
    chunk->next   = NULL;
}

static inline
_tralloc_chunk * _tralloc_get_vertical_list_root ( _tralloc_chunk * root_chunk )
{
    _tralloc_chunk * chunk_to_detach;
    _tralloc_chunk * chunk = root_chunk;

    while ( ! _tralloc_can_free_chunk ( chunk ) ) {
        if ( _tralloc_can_free_chunk_children ( chunk ) && chunk->first_child != NULL ) {
            chunk = chunk->first_child;
        } else {
            if ( chunk == root_chunk ) {
                return NULL;
            }

            while ( chunk->next == NULL ) {
                chunk_to_detach = chunk;
                chunk = chunk->parent;
                _tralloc_silent_detach_chunk ( chunk_to_detach );

                if ( chunk == root_chunk ) {
                    return NULL;
                }
            }
            chunk_to_detach = chunk;
            chunk = chunk->next;
            _tralloc_silent_detach_chunk ( chunk_to_detach );
        }
    }

    return chunk;
}

static inline
void _tralloc_subtree_to_vertical_list ( _tralloc_chunk * list_root_chunk, _tralloc_chunk * root_chunk )
{
    _tralloc_chunk * prev_chunk, * next_chunk, * chunk_to_detach;
    prev_chunk = next_chunk = list_root_chunk;

    while ( TRALLOC_TRUE ) {
        do {
            if ( _tralloc_can_free_chunk_children ( next_chunk ) && next_chunk->first_child != NULL ) {
                next_chunk = next_chunk->first_child;
            } else {
                if ( next_chunk == root_chunk ) {
                    prev_chunk->first_child = NULL;
                    return;
                }

                while ( next_chunk->next == NULL ) {
                    chunk_to_detach = next_chunk;
                    next_chunk = next_chunk->parent;
                    _tralloc_silent_detach_chunk ( chunk_to_detach );

                    if ( next_chunk == root_chunk ) {
                        prev_chunk->first_child = NULL;
                        return;
                    }
                }
                chunk_to_detach = next_chunk;
                next_chunk = next_chunk->next;
                _tralloc_silent_detach_chunk ( chunk_to_detach );
            }
        } while ( ! _tralloc_can_free_chunk ( next_chunk ) );

        prev_chunk->first_child = next_chunk;
        prev_chunk = next_chunk;
    }
}

tralloc_error _tralloc_free_subtree ( _tralloc_chunk * root_chunk )
{
    _tralloc_detach_chunk ( root_chunk );

    _tralloc_chunk * chunk = _tralloc_get_vertical_list_root ( root_chunk );
    if ( chunk == NULL ) {
        return 0;
    }
    _tralloc_subtree_to_vertical_list ( chunk, root_chunk );

    tralloc_error error = 0, result;
    _tralloc_chunk * next_chunk;
    while ( chunk != NULL ) {
        next_chunk = chunk->first_child;

        result = _tralloc_free_chunk ( chunk );
        if ( result != 0 ) {
            error = result;
        }

        chunk = next_chunk;
    }

    return error;
}

tralloc_error _tralloc_free_single ( _tralloc_chunk * chunk )
{
    _tralloc_detach_chunk ( chunk );
    return _tralloc_free_chunk ( chunk );
}
