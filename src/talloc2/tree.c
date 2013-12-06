// This file is part of talloc2. Copyright (C) 2013 Andrew Aladjev aladjev.andrew@gmail.com
// talloc2 is free software: you can redistribute it and/or modify it under the terms of the GNU General Lesser Public License as published by the Free Software Foundation, either version 3 of the License, or (at your option) any later version.
// talloc2 is distributed in the hope that it will be useful, but WITHOUT ANY WARRANTY; without even the implied warranty of MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU General Lesser Public License for more details.
// You should have received a copy of the GNU General Lesser Public License along with talloc2. If not, see <http://www.gnu.org/licenses/>.

#include <stdlib.h>
#include <string.h>
#include <stdbool.h>

#include "tree.h"
#include "chunk.h"

#if defined(TALLOC_DEBUG)
#include "events.h"
#endif

#if defined(TALLOC_EXTENSIONS)
#include "extensions/chunk.h"
#endif

#if defined(TALLOC_REFERENCE)
#include "reference/chunk.h"
#endif

uint8_t talloc_add_chunk ( const void * parent_data, talloc_chunk * child )
{
    child->first_child = NULL;

    if ( parent_data != NULL ) {
        talloc_chunk * parent = talloc_chunk_from_data ( parent_data );
        talloc_set_child_chunk ( parent, child );
    } else {
        child->parent = NULL;
        child->prev   = NULL;
        child->next   = NULL;
    }

#if defined(TALLOC_DEBUG)
    return talloc_on_add ( child );
#else
    return 0;
#endif

}

void * talloc ( const void * parent_data, size_t length )
{
    talloc_chunk * chunk;

#if defined(TALLOC_EXTENSIONS)
    chunk = talloc_extensions_malloc_chunk ( parent_data, length );
#else
    chunk = talloc_usual_malloc_chunk ( parent_data, length );
#endif

    if ( chunk == NULL ) {
        return NULL;
    }

    return talloc_data_from_chunk ( chunk );
}

void * talloc_zero ( const void * parent_data, size_t length )
{
    talloc_chunk * chunk;

#if defined(TALLOC_EXTENSIONS)
    chunk = talloc_extensions_calloc_chunk ( parent_data, length );
#else
    chunk = talloc_usual_calloc_chunk ( parent_data, length );
#endif

    if ( chunk == NULL ) {
        return NULL;
    }

    return talloc_data_from_chunk ( chunk );
}

static inline
void talloc_update_chunk ( talloc_chunk * chunk )
{
    talloc_chunk * prev = chunk->prev;
    if ( prev == NULL ) {
        talloc_chunk * parent = chunk->parent;
        if ( parent != NULL ) {
            parent->first_child = chunk;
        }
    } else {
        prev->next = chunk;
    }
    talloc_chunk * next = chunk->next;
    if ( next != NULL ) {
        next->prev = chunk;
    }

    talloc_chunk * next_child = chunk->first_child;
    while ( next_child != NULL ) {
        next_child->parent = chunk;
        next_child = next_child->next;
    }
}

void * talloc_realloc ( const void * chunk_data, size_t length )
{
    if ( chunk_data == NULL ) {
        return NULL;
    }
    talloc_chunk * old_chunk = talloc_chunk_from_data ( chunk_data );

#if defined(TALLOC_REFERENCE)
    if ( old_chunk->mode == TALLOC_MODE_REFERENCE ) {
        return NULL;
    }
#endif

#if defined(TALLOC_DEBUG)
    size_t old_length = old_chunk->length;
#endif

    talloc_chunk * new_chunk;
#if defined(TALLOC_EXTENSIONS)
    new_chunk = talloc_extensions_realloc_chunk ( old_chunk, length );
#else
    new_chunk = talloc_usual_realloc_chunk ( old_chunk, length );
#endif

    if ( new_chunk == NULL ) {
        return NULL;
    }
    if ( old_chunk != new_chunk ) {
        // now pointers to old_chunk is invalid
        // each pointer to old_chunk should be replaced with new_chunk
        talloc_update_chunk ( new_chunk );
    }

#if defined(TALLOC_DEBUG)
    if ( talloc_on_resize ( new_chunk, old_length ) != 0 ) {
        return NULL;
    }
#endif

    return talloc_data_from_chunk ( new_chunk );
}

uint8_t talloc_move ( const void * child_data, const void * parent_data )
{
    if ( child_data == NULL || parent_data == child_data ) {
        return 1;
    }
    talloc_chunk * child = talloc_chunk_from_data ( child_data );

#if defined(TALLOC_REFERENCE)
    if ( child->mode == TALLOC_MODE_REFERENCE ) {
        if ( parent_data == NULL ) {
            return 2;
        }
    }
#endif

#if defined(TALLOC_DEBUG)
    talloc_chunk * old_parent;
#endif

    if ( parent_data == NULL ) {
        if ( child->parent == NULL ) {
            return 0;
        }

#if defined(TALLOC_DEBUG)
        old_parent = child->parent;
#endif

        talloc_detach_chunk ( child );
        child->parent = NULL;
    } else {
        talloc_chunk * new_parent = talloc_chunk_from_data ( parent_data );
        if ( child->parent == new_parent ) {
            return 0;
        }

#if defined(TALLOC_DEBUG)
        old_parent = child->parent;
#endif

        talloc_detach_chunk ( child );
        talloc_set_child_chunk ( new_parent, child );
    }

#if defined(TALLOC_DEBUG)
    return talloc_on_move ( child, old_parent );
#else
    return 0;
#endif

}

uint8_t talloc_free_chunk ( talloc_chunk * chunk )
{

#if defined(TALLOC_REFERENCE)
    if ( chunk->mode == TALLOC_MODE_EXTENSIONS ) {
        return talloc_extensions_free_chunk ( chunk );
    } else {
        return talloc_reference_free_chunk ( chunk );
    }
#elif defined(TALLOC_EXTENSIONS)
    return talloc_extensions_free_chunk ( chunk );
#else
    return talloc_usual_free_chunk ( chunk );
#endif

}

extern inline void    talloc_set_child_chunk     ( talloc_chunk * parent, talloc_chunk * child );
extern inline void    talloc_detach_chunk        ( talloc_chunk * chunk );
extern inline void *  talloc_new                 ( const void * parent_data );
extern inline uint8_t talloc_free                ( void * root_data );
extern inline uint8_t talloc_free_chunk_children ( talloc_chunk * chunk );
