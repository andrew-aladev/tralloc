// This file is part of talloc2. Copyright (C) 2013 Andrew Aladjev aladjev.andrew@gmail.com
// talloc2 is free software: you can redistribute it and/or modify it under the terms of the GNU General Lesser Public License as published by the Free Software Foundation, either version 3 of the License, or (at your option) any later version.
// talloc2 is distributed in the hope that it will be useful, but WITHOUT ANY WARRANTY; without even the implied warranty of MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU General Lesser Public License for more details.
// You should have received a copy of the GNU General Lesser Public License along with talloc2. If not, see <http://www.gnu.org/licenses/>.

#include <stdlib.h>
#include <string.h>
#include <stdbool.h>

#include "tree.h"

#ifdef TALLOC_EVENTS
#include "events.h"
#endif

#ifdef TALLOC_EXT
#include "ext/chunk.h"
#endif

#ifdef TALLOC_REFERENCE
#include "reference/chunk.h"
#endif

extern inline void talloc_set_child ( talloc_chunk * parent, talloc_chunk * child );

static inline
uint8_t _add ( const void * parent_data, talloc_chunk * child )
{
    child->first_child = NULL;

    if ( parent_data != NULL ) {
        void * parent = talloc_chunk_from_data ( parent_data );
        talloc_set_child ( parent, child );
    } else {
        child->parent = NULL;
        child->prev   = NULL;
        child->next   = NULL;
    }

#ifdef TALLOC_EVENTS
    if ( talloc_on_add ( child ) != 0 ) {
        return 1;
    }
#endif

    return 0;
}

void * talloc ( const void * parent_data, size_t length )
{

    talloc_chunk * chunk;
#ifdef TALLOC_EXT
    chunk = talloc_ext_chunk_malloc ( length );
#else
    chunk = talloc_usual_chunk_malloc ( length );
#endif

    if ( _add ( parent_data, chunk ) != 0 ) {
#ifdef TALLOC_EXT
        talloc_ext_chunk_free ( chunk );
#else
        talloc_usual_chunk_free ( chunk );
#endif
        return NULL;
    }

    return talloc_data_from_chunk ( chunk );
}

void * talloc_zero ( const void * parent_data, size_t length )
{

    talloc_chunk * chunk;
#ifdef TALLOC_EXT
    chunk = talloc_ext_chunk_calloc ( length );
#else
    chunk = talloc_usual_chunk_calloc ( length );
#endif

    if ( _add ( parent_data, chunk ) != 0 ) {
#ifdef TALLOC_EXT
        talloc_ext_chunk_free ( chunk );
#else
        talloc_usual_chunk_free ( chunk );
#endif
        return NULL;
    }

    return talloc_data_from_chunk ( chunk );
}

static inline
uint8_t _update ( talloc_chunk * child )
{
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

void * talloc_realloc ( const void * chunk_data, size_t length )
{
    if ( chunk_data == NULL ) {
        return NULL;
    }
    talloc_chunk * old_chunk = talloc_chunk_from_data ( chunk_data );

    talloc_chunk * new_chunk;
#ifdef TALLOC_EXT
    new_chunk = talloc_ext_chunk_realloc ( old_chunk, length );
#else
    new_chunk = talloc_usual_chunk_realloc ( old_chunk, length );
#endif

    if ( new_chunk == NULL ) {
        return NULL;
    }
    if ( old_chunk != new_chunk ) {
        if ( _update ( new_chunk ) != 0 ) {
            return NULL;
        }
    }

#ifdef TALLOC_EVENTS
    if ( talloc_on_update ( new_chunk ) != 0 ) {
        return NULL;
    }
#endif

    return talloc_data_from_chunk ( new_chunk );
}

static inline
void _detach ( talloc_chunk * chunk )
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
}

uint8_t talloc_move ( const void * child_data, const void * parent_data )
{
    if ( child_data == NULL ) {
        return 1;
    }
    talloc_chunk * child = talloc_chunk_from_data ( child_data );
    _detach ( child );

    if ( parent_data == NULL ) {
        child->parent = NULL;
    } else {
        talloc_chunk * new_parent = talloc_chunk_from_data ( parent_data );
        talloc_set_child ( new_parent, child );
    }

#ifdef TALLOC_EVENTS
    if ( talloc_on_move ( child ) != 0 ) {
        return 2;
    }
#endif

    return 0;
}

static
bool free_recursive ( talloc_chunk * root )
{
    bool result          = true;
    talloc_chunk * chunk = root->first_child;

#ifdef TALLOC_EVENTS
    if ( talloc_on_del ( root ) != 0 ) {
        result = false;
    }
#endif

#ifdef TALLOC_MODE
    switch ( root->mode ) {
#ifdef TALLOC_EXT
    case TALLOC_MODE_EXT:
        if ( !talloc_ext_chunk_free ( root ) ) {
            result = false;
        }
        break;
#else
    case TALLOC_MODE_USUAL:
        talloc_usual_chunk_free ( root );
        break;
#endif
#ifdef TALLOC_REFERENCE
    case TALLOC_MODE_REFERENCE:
        talloc_reference_chunk_free ( root );
        break;
#endif
    }
#else
    talloc_usual_chunk_free ( root );
#endif

    talloc_chunk * next_chunk;
    while ( chunk != NULL ) {
        next_chunk = chunk->next;
        if ( !free_recursive ( chunk ) ) {
            result = false;
        }
        chunk = next_chunk;
    }

    return result;
}

uint8_t talloc_free ( void * root_data )
{
    if ( root_data == NULL ) {
        return 0;
    }
    talloc_chunk * root = talloc_chunk_from_data ( root_data );

    _detach ( root );

    if ( !free_recursive ( root ) ) {
        return 1;
    }

    return 0;
}

extern inline void *         talloc_data_from_chunk ( talloc_chunk * chunk );
extern inline talloc_chunk * talloc_chunk_from_data ( const void * data );
extern inline void *         talloc                 ( const void * parent_data, size_t length );
extern inline void *         talloc_zero            ( const void * parent_data, size_t length );
extern inline void *         talloc_new             ( const void * parent_data );