// This file is part of tralloc. Copyright (C) 2013 Andrew Aladjev aladjev.andrew@gmail.com
// tralloc is free software: you can redistribute it and/or modify it under the terms of the GNU General Lesser Public License as published by the Free Software Foundation, either version 3 of the License, or (at your option) any later version.
// tralloc is distributed in the hope that it will be useful, but WITHOUT ANY WARRANTY; without even the implied warranty of MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU General Lesser Public License for more details.
// You should have received a copy of the GNU General Lesser Public License along with tralloc. If not, see <http://www.gnu.org/licenses/>.

#define _TRALLOC_INCLUDED_FROM_TREE_CHUNK_C
#include <tralloc/tree/chunk.h>


void _tralloc_chunk_attach ( _tralloc_chunk * chunk, _tralloc_chunk * new_parent_chunk )
{
    _tralloc_chunk * parent = chunk->parent;
    _tralloc_chunk * prev   = chunk->prev;
    _tralloc_chunk * next   = chunk->next;

    chunk->parent = new_parent_chunk;
    chunk->prev   = NULL;

    if ( prev != NULL ) {
        prev->next = next;
    } else if ( parent != NULL ) {
        parent->first_child = next;
    }
    if ( next != NULL ) {
        next->prev = prev;
    }

    if ( new_parent_chunk == NULL ) {
        chunk->next = NULL;
    } else {
        _tralloc_chunk * new_first_child = new_parent_chunk->first_child;
        if ( new_first_child != NULL ) {
            new_first_child->prev = chunk;
            chunk->next = new_first_child;
        } else {
            chunk->next = NULL;
        }
        new_parent_chunk->first_child = chunk;
    }
}

void _tralloc_chunk_update ( _tralloc_chunk * chunk )
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
