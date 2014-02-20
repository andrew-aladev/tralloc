// This file is part of tralloc. Copyright (C) 2013 Andrew Aladjev aladjev.andrew@gmail.com
// tralloc is free software: you can redistribute it and/or modify it under the terms of the GNU General Lesser Public License as published by the Free Software Foundation, either version 3 of the License, or (at your option) any later version.
// tralloc is distributed in the hope that it will be useful, but WITHOUT ANY WARRANTY; without even the implied warranty of MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU General Lesser Public License for more details.
// You should have received a copy of the GNU General Lesser Public License along with tralloc. If not, see <http://www.gnu.org/licenses/>.

#ifndef TRALLOC_TREE_COMMON_H
#define TRALLOC_TREE_COMMON_H

#include "../common.h"

#if defined(TRALLOC_DEBUG)
#include "../events.h"
#endif


tralloc_error tralloc_realloc ( tralloc_context ** chunk_context, size_t length );
tralloc_error tralloc_move    ( tralloc_context * child_context, tralloc_context * parent_context );


inline
tralloc_error _tralloc_add_chunk ( tralloc_context * parent_context, _tralloc_chunk * child_chunk )
{
    child_chunk->first_child = NULL;

    if ( parent_context != NULL ) {
        _tralloc_chunk * parent_chunk       = _tralloc_get_chunk_from_context ( parent_context );
        _tralloc_chunk * parent_first_child = parent_chunk->first_child;

        child_chunk->parent = parent_chunk;

        if ( parent_first_child != NULL ) {
            parent_first_child->prev = child_chunk;
            child_chunk->next              = parent_first_child;
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

inline
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

inline
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


bool _tralloc_free_chunk ( _tralloc_chunk * chunk, tralloc_error * error );

inline
tralloc_error _tralloc_free_subtree ( _tralloc_chunk * root_chunk )
{
    tralloc_error    error = 0;
    _tralloc_chunk * chunk = root_chunk;
    bool can_free_children;

    while ( true ) {
        can_free_children = _tralloc_free_chunk ( chunk, &error );
        if ( can_free_children && chunk->first_child != NULL ) {
            chunk = chunk->first_child;
        } else {
            while ( chunk->next == NULL ) {
                if ( chunk == root_chunk ) {
                    return error;
                }
                chunk = chunk->parent;
            }
            chunk = chunk->next;
        }
    }
}

inline
tralloc_error tralloc_free ( tralloc_context * chunk_context )
{
    if ( chunk_context == NULL ) {
        return TRALLOC_ERROR_REQUIRED_ARGUMENT_IS_NULL;
    }
    _tralloc_chunk * chunk = _tralloc_get_chunk_from_context ( chunk_context );
    _tralloc_detach_chunk ( chunk );
    return _tralloc_free_subtree ( chunk );
}


#endif
