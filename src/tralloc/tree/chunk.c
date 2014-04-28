// This file is part of tralloc. Copyright (C) 2013 Andrew Aladjev aladjev.andrew@gmail.com
// tralloc is free software: you can redistribute it and/or modify it under the terms of the GNU General Lesser Public License as published by the Free Software Foundation, either version 3 of the License, or (at your option) any later version.
// tralloc is distributed in the hope that it will be useful, but WITHOUT ANY WARRANTY; without even the implied warranty of MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU General Lesser Public License for more details.
// You should have received a copy of the GNU General Lesser Public License along with tralloc. If not, see <http://www.gnu.org/licenses/>.

#include "chunk.h"
#include "../common.h"


void _tralloc_attach_chunk ( _tralloc_chunk * current, _tralloc_chunk * new_parent )
{
    _tralloc_chunk * parent = current->parent;
    _tralloc_chunk * prev   = current->prev;
    _tralloc_chunk * next   = current->next;

    current->parent = new_parent;
    current->prev   = NULL;

    if ( prev != NULL ) {
        prev->next = next;
    } else if ( parent != NULL ) {
        parent->first_child = next;
    }
    if ( next != NULL ) {
        next->prev = prev;
    }

    _tralloc_chunk * new_first_child = new_parent->first_child;
    if ( new_first_child != NULL ) {
        new_first_child->prev = current;
        current->next = new_first_child;
    } else {
        current->next = NULL;
    }
    new_parent->first_child = current;
}

void _tralloc_detach_chunk ( _tralloc_chunk * current )
{
    _tralloc_chunk * prev   = current->prev;
    _tralloc_chunk * next   = current->next;
    _tralloc_chunk * parent = current->parent;

    current->parent = NULL;
    current->prev   = NULL;
    current->next   = NULL;

    if ( prev != NULL ) {
        prev->next = next;
    } else if ( parent != NULL ) {
        parent->first_child = next;
    }
    if ( next != NULL ) {
        next->prev = prev;
    }
}

void _tralloc_update_chunk ( _tralloc_chunk * current )
{
    _tralloc_chunk * prev = current->prev;
    if ( prev == NULL ) {
        _tralloc_chunk * parent = current->parent;
        if ( parent != NULL ) {
            parent->first_child = current;
        }
    } else {
        prev->next = current;
    }
    _tralloc_chunk * next = current->next;
    if ( next != NULL ) {
        next->prev = current;
    }

    _tralloc_chunk * next_child = current->first_child;
    while ( next_child != NULL ) {
        next_child->parent = current;
        next_child = next_child->next;
    }
}
