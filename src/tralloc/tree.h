// This file is part of tralloc. Copyright (C) 2013 Andrew Aladjev aladjev.andrew@gmail.com
// tralloc is free software: you can redistribute it and/or modify it under the terms of the GNU General Lesser Public License as published by the Free Software Foundation, either version 3 of the License, or (at your option) any later version.
// tralloc is distributed in the hope that it will be useful, but WITHOUT ANY WARRANTY; without even the implied warranty of MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU General Lesser Public License for more details.
// You should have received a copy of the GNU General Lesser Public License along with tralloc. If not, see <http://www.gnu.org/licenses/>.

#ifndef TRALLOC_TREE_H
#define TRALLOC_TREE_H

#include "common.h"

#if defined(TRALLOC_DEBUG)
#include "events.h"
#endif

inline
void tralloc_set_child_chunk ( tralloc_chunk * parent, tralloc_chunk * child )
{
    tralloc_chunk * parent_first_child = parent->first_child;

    child->parent = parent;
    if ( parent_first_child != NULL ) {
        parent_first_child->prev = child;
        child->next = parent_first_child;
        child->prev = NULL;
    } else {
        child->next = NULL;
        child->prev = NULL;
    }
    parent->first_child = child;
}

inline
uint8_t tralloc_add_chunk ( const tralloc_context * parent_context, tralloc_chunk * child )
{
    child->first_child = NULL;

    if ( parent_context != NULL ) {
        tralloc_chunk * parent = tralloc_chunk_from_context ( parent_context );
        tralloc_set_child_chunk ( parent, child );
    } else {
        child->parent = NULL;
        child->prev   = NULL;
        child->next   = NULL;
    }

#if defined(TRALLOC_DEBUG)
    return tralloc_on_add ( child );
#else
    return 0;
#endif

}

inline
void tralloc_detach_chunk ( tralloc_chunk * chunk )
{
    tralloc_chunk * prev   = chunk->prev;
    tralloc_chunk * next   = chunk->next;
    tralloc_chunk * parent = chunk->parent;

    if ( prev != NULL ) {
        if ( next != NULL ) {
            prev->next = next;
            next->prev = prev;
        } else {
            prev->next = NULL;
        }
    } else {
        if ( next != NULL ) {
            next->prev = NULL;
        }
        if ( parent != NULL ) {
            parent->first_child = next;
        }
    }

    chunk->parent = NULL;
}

// Function uses malloc to allocate new chunk with size = sizeof ( chunk ) + length.
// If parent_context is NULL function will set new chunk as root independent chunk.
// Otherwise it will obtain parent chunk from parent_context and attach new chunk to parent chunk.
// parent_context can be both usual and reference.
// Function returns pointer to memory (with length size) or NULL if error occurred.
tralloc_context * tralloc ( const tralloc_context * parent_context, size_t length );

// Function works the same as "tralloc".
// It will use calloc instead of malloc to allocate new chunk.
tralloc_context * tralloc_zero ( const tralloc_context * parent_context, size_t length );

// Function uses malloc to allocate new chunk with size = sizeof ( chunk ).
// If parent_context is NULL function will set new chunk as root independent chunk.
// Otherwise it will obtain parent chunk from parent_context and attach new chunk to parent chunk.
// parent_context can be both usual and reference.
// Function returns pointer to memory (with zero size) or NULL if error occurred.
// This memory should not be used for storing information.
inline
tralloc_context * tralloc_new ( const tralloc_context * parent_context )
{
    return tralloc ( parent_context, 0 );
}

uint8_t tralloc_free_chunk ( tralloc_chunk * chunk );

inline
uint8_t tralloc_free_chunk_children ( tralloc_chunk * chunk )
{
    uint8_t result, error = 0;
    tralloc_chunk * child = chunk->first_child;

    tralloc_chunk * next_child;
    while ( child != NULL ) {
        next_child = child->next;
        if ( ( result = tralloc_free_chunk ( child ) ) != 0 ) {
            error = result;
        }
        child = next_child;
    }

    return error;
}

// Function obtains chunk from chunk_context, detaches it from the tree and start delete operation on subtree starting from it.
// chunk_context can be both usual and reference.
// If chunk is usual and it has reference(s) - it will become root independent chunk and delete operation will not go to it's children.
// Otherwise it will be deleted.
// If chunk is reference it will be deleted.
// If linked usual chunk is root independent chunk and it has no references - it will be deleted.
// All chunk's destructors will be invoked on delete.
// Function returns zero if delete operations were successful and non-zero value otherwise.
// Non-zero value meaning is "error code of the last failed operation". Function will not stop on errors.
inline
uint8_t tralloc_free ( const tralloc_context * chunk_context )
{
    if ( chunk_context == NULL ) {
        return 0;
    }
    tralloc_chunk * chunk = tralloc_chunk_from_context ( chunk_context );
    tralloc_detach_chunk ( chunk );
    return tralloc_free_chunk ( chunk );
}

// Function obtains chunk from chunk_context, uses realloc to change size of chunk from current_length to length.
// chunk_context can be both usual and reference.
// Function returns pointer to memory (with length size) or NULL if error occurred.
tralloc_context * tralloc_realloc ( const tralloc_context * chunk_context, size_t length );

// Function obtains child chunk from child_context, parent chunk from parent_context.
// chunk_context and parent_context can be both usual and reference.
// Function detaches child chunk from it's parent and attaches it to new parent chunk.
// Function returns zero if move operation was successful and non-zero value otherwise.
uint8_t tralloc_move ( const tralloc_context * child_context, const tralloc_context * parent_context );

#endif
