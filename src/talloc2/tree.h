// This file is part of talloc2. Copyright (C) 2013 Andrew Aladjev aladjev.andrew@gmail.com
// talloc2 is free software: you can redistribute it and/or modify it under the terms of the GNU General Lesser Public License as published by the Free Software Foundation, either version 3 of the License, or (at your option) any later version.
// talloc2 is distributed in the hope that it will be useful, but WITHOUT ANY WARRANTY; without even the implied warranty of MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU General Lesser Public License for more details.
// You should have received a copy of the GNU General Lesser Public License along with talloc2. If not, see <http://www.gnu.org/licenses/>.

#ifndef TALLOC_TREE_H
#define TALLOC_TREE_H

#include "common.h"

inline
void talloc_set_child_chunk ( talloc_chunk * parent, talloc_chunk * child )
{
    talloc_chunk * parent_first_child = parent->first_child;

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
void talloc_detach_chunk ( talloc_chunk * chunk )
{
    talloc_chunk * prev   = chunk->prev;
    talloc_chunk * next   = chunk->next;
    talloc_chunk * parent = chunk->parent;

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
// If parent_data is NULL function will set new chunk as root independent chunk.
// Otherwise it will obtain parent chunk from parent_data and attach new chunk to parent chunk.
// parent_data can be both usual and reference.
// Function returns pointer to memory (with length size) or NULL if error occurred.
void * talloc ( const void * parent_data, size_t length );

// Function works the same as "talloc". It will use calloc instead of malloc to allocate new chunk.
void *  talloc_zero ( const void * parent_data, size_t length );

// Function uses malloc to allocate new chunk with size = sizeof ( chunk ).
// If parent_data is NULL function will set new chunk as root independent chunk.
// Otherwise it will obtain parent chunk from parent_data and attach new chunk to parent chunk.
// parent_data can be both usual and reference.
// Function returns pointer to memory (with zero size) or NULL if error occurred. This memory should not be used for storing information.
inline
void * talloc_new ( const void * parent_data )
{
    return talloc ( parent_data, 0 );
}

uint8_t talloc_free_chunk ( talloc_chunk * chunk );

inline
uint8_t talloc_free_chunk_children ( talloc_chunk * chunk )
{
    uint8_t result, error = 0;
    talloc_chunk * child  = chunk->first_child;

    talloc_chunk * next_child;
    while ( child != NULL ) {
        next_child = child->next;
        if ( ( result = talloc_free_chunk ( child ) ) != 0 ) {
            error = result;
        }
        child = next_child;
    }

    return error;
}

// Function obtains chunk from chunk_data, detaches it from the tree and start delete operation on subtree starting from it.
// chunk_data can be both usual and reference.
// If chunk is usual and it has reference(s) - it will become root independent chunk and delete operation will not go to it's children.
// Otherwise it will be deleted. All it's destructors will be invoked.
// If chunk is reference it will be deleted. All it's destructors will be invoked.
// If linked usual chunk is root independent chunk and it has no references - it will be deleted. All it's destructors will be invoked.
// Function returns zero if delete operations were successful and non-zero value otherwise.
// Non-zero value meaning is "error code of the last failed operation". Function will not stop on errors.
inline
uint8_t talloc_free ( void * chunk_data )
{
    if ( chunk_data == NULL ) {
        return 0;
    }
    talloc_chunk * chunk = talloc_chunk_from_data ( chunk_data );
    talloc_detach_chunk ( chunk );
    return talloc_free_chunk ( chunk );
}

uint8_t talloc_add_chunk ( const void * parent_data, talloc_chunk * child );

// Function obtains chunk from chunk_data, uses realloc to change size of chunk from current_length to length.
// chunk_data can be both usual and reference.
// Function returns pointer to memory (with length size) or NULL if error occurred.
void * talloc_realloc ( const void * chunk_data, size_t length );

// Function obtains child chunk from child_data, parent chunk from parent_data.
// chunk_data and parent_data can be both usual and reference.
// Function detaches child chunk from it's parent and attaches it to new parent chunk.
// Function returns zero if move operation was successful and non-zero value otherwise.
uint8_t talloc_move ( const void * child_data, const void * parent_data );

#endif
