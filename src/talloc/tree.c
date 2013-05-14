// This file is part of talloc. Copyright (C) 2013 Andrew Aladjev aladjev.andrew@gmail.com
// talloc is free software: you can redistribute it and/or modify it under the terms of the GNU General Lesser Public License as published by the Free Software Foundation, either version 3 of the License, or (at your option) any later version.
// talloc is distributed in the hope that it will be useful, but WITHOUT ANY WARRANTY; without even the implied warranty of MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU General Lesser Public License for more details.
// You should have received a copy of the GNU General Lesser Public License along with talloc. If not, see <http://www.gnu.org/licenses/>.

#include "tree.h"

#include <stdlib.h>
#include <string.h>

#ifdef DEBUG
static talloc_callback talloc_on_add;
static talloc_callback talloc_on_update;
static talloc_callback talloc_on_del;

void talloc_set_callback ( talloc_callback on_add, talloc_callback on_update, talloc_callback on_del ) {
    talloc_on_add    = on_add;
    talloc_on_update = on_update;
    talloc_on_del    = on_del;
}
#endif

static inline
talloc_chunk * _malloc ( size_t length ) {
    return malloc ( sizeof ( talloc_chunk ) + length );
}

static inline
talloc_chunk * _calloc ( size_t length ) {
    return calloc ( 1, sizeof ( talloc_chunk ) + length );
}

static inline
void * _realloc ( talloc_chunk * child, size_t length ) {
    return realloc ( child, sizeof ( talloc_chunk ) + length );
}

static inline
void _init ( talloc_chunk * child ) {
    child->parent      = NULL;
    child->prev        = NULL;
    child->next        = NULL;
    child->first_child = NULL;
}

static inline
void _set_child ( talloc_chunk * parent, talloc_chunk * child ) {
    child->parent = parent;

    talloc_chunk * parent_first_child = parent->first_child;
    if ( parent_first_child != NULL ) {
        parent_first_child->prev = child;
        child->next = parent_first_child;
    }
    parent->first_child = child;
}

static inline
uint8_t _add ( const void * parent_data, talloc_chunk * child ) {
    _init ( child );

    if ( parent_data != NULL ) {
        void * parent = talloc_chunk_from_data ( parent_data );
        if ( parent == NULL ) {
            return 1;
        }
        _set_child ( parent, child );
    }

#ifdef DEBUG
    if ( talloc_on_add != NULL ) {
        talloc_on_add ( child );
    }
#endif

    return 0;
}

void * talloc ( const void * parent_data, size_t length ) {
    talloc_chunk * child = _malloc ( length );
    if ( child == NULL ) {
        return NULL;
    }
    if ( _add ( parent_data, child ) ) {
        free ( child );
        return NULL;
    }
    return talloc_data_from_chunk ( child );
}

void * talloc_zero ( const void * parent_data, size_t length ) {
    talloc_chunk * child = _calloc ( length );
    if ( child == NULL ) {
        return NULL;
    }
    if ( _add ( parent_data, child ) ) {
        free ( child );
        return NULL;
    }
    return talloc_data_from_chunk ( child );
}

static inline
uint8_t _update ( talloc_chunk * child ) {
    talloc_chunk * prev = child->prev;
    if ( prev == NULL ) {
        talloc_chunk * parent = child->parent;
        parent->first_child = child;
    } else {
        prev->next = child;
    }
    talloc_chunk * next = child->next;
    if ( next != NULL ) {
        next->prev = child;
    }

    talloc_chunk * next_child = child->first_child;
    while ( next_child != NULL ) {
        next_child->parent = child;
        next_child = next_child->next;
    }

    return 0;
}

void * talloc_realloc ( const void * child_data, size_t length ) {
    if ( child_data == NULL ) {
        return NULL;
    }
    talloc_chunk * old_child = talloc_chunk_from_data ( child_data );
    talloc_chunk * new_child = _realloc ( old_child, length );
    if ( new_child == NULL ) {
        return NULL;
    }
    if ( old_child != new_child && _update ( new_child ) ) {
        return NULL;
    }

#ifdef DEBUG
    if ( talloc_on_update != NULL ) {
        talloc_on_update ( new_child );
    }
#endif

    return talloc_data_from_chunk ( new_child );
}

static inline
void _free_recursive ( talloc_chunk * root ) {
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
        _free_recursive ( child );
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

    _free_recursive ( root );

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

