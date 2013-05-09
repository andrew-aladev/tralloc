// This file is part of talloc. Copyright (C) 2013 Andrew Aladjev aladjev.andrew@gmail.com
// talloc is free software: you can redistribute it and/or modify it under the terms of the GNU General Lesser Public License as published by the Free Software Foundation, either version 3 of the License, or (at your option) any later version.
// talloc is distributed in the hope that it will be useful, but WITHOUT ANY WARRANTY; without even the implied warranty of MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU General Lesser Public License for more details.
// You should have received a copy of the GNU General Lesser Public License along with talloc. If not, see <http://www.gnu.org/licenses/>.

#include "tree.h"

#include <stdlib.h>
#include <string.h>

static const size_t chunk_size = sizeof ( talloc_chunk ) / sizeof ( uintptr_t );

#ifdef DEBUG
static talloc_callback talloc_on_add;
static talloc_callback talloc_on_del;

void talloc_set_callback ( talloc_callback on_add, talloc_callback on_del ) {
    talloc_on_add = on_add;
    talloc_on_del = on_del;
}
#endif

extern inline
void * talloc_data_from_chunk ( talloc_chunk * chunk ) {
    return ( void * ) ( ( uintptr_t ) chunk + chunk_size );
}

extern inline
talloc_chunk * talloc_chunk_from_data ( const void * data ) {
    return ( talloc_chunk * ) ( ( uintptr_t ) data - chunk_size );
}

void * talloc ( const void * parent_data, size_t length ) {
    talloc_chunk * child = malloc ( sizeof ( talloc_chunk ) + length );
    if ( child == NULL ) {
        return NULL;
    }
    memset ( child, 0, sizeof ( talloc_chunk ) );
    void * child_data = talloc_data_from_chunk ( child );

    if ( parent_data != NULL ) {
        talloc_chunk * parent = talloc_chunk_from_data ( parent_data );
        if ( parent == NULL ) {
            free ( child );
            return NULL;
        }

        child->parent = parent;

        talloc_chunk * parent_first_child = parent->first_child;
        if ( parent_first_child != NULL ) {
            parent_first_child->prev = child;
            child->next = parent_first_child;
        }
        parent->first_child = child;
    }

#ifdef DEBUG
    if ( talloc_on_add != NULL ) {
        talloc_on_add ( child );
    }
#endif

    return child_data;
}

extern inline
void * talloc_new ( const void * parent_data ) {
    return talloc ( parent_data, 0 );
}

static inline
void talloc_free_recursive ( talloc_chunk * root ) {
    talloc_chunk * child = root->first_child;
    free ( root );

#ifdef DEBUG
    if ( talloc_on_del != NULL ) {
        talloc_on_del ( root );
    }
#endif

    talloc_chunk * new_child;
    while ( child != NULL ) {
        new_child = child->next;
        talloc_free_recursive ( child );
        child = new_child;
    }
}

uint8_t talloc_free ( void * root_data ) {
    talloc_chunk * root = talloc_chunk_from_data ( root_data );
    if ( root == NULL ) {
        return 1;
    }

    talloc_chunk * prev   = root->prev;
    talloc_chunk * next   = root->next;
    talloc_chunk * parent = root->parent;

    talloc_free_recursive ( root );

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

    return 0;
}
