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

void * talloc ( const void * parent_data, size_t length );

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

inline
uint8_t talloc_free ( void * root_data )
{
    if ( root_data == NULL ) {
        return 0;
    }
    talloc_chunk * chunk = talloc_chunk_from_data ( root_data );
    talloc_detach_chunk ( chunk );
    return talloc_free_chunk ( chunk );
}

uint8_t talloc_add_chunk ( const void * parent_data, talloc_chunk * child );
void *  talloc_zero      ( const void * parent_data, size_t length );
void *  talloc_realloc   ( const void * child_data, size_t length );
uint8_t talloc_move      ( const void * child_data, const void * parent_data );

#endif

