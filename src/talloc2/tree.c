// This file is part of talloc2. Copyright (C) 2013 Andrew Aladjev aladjev.andrew@gmail.com
// talloc2 is free software: you can redistribute it and/or modify it under the terms of the GNU General Lesser Public License as published by the Free Software Foundation, either version 3 of the License, or (at your option) any later version.
// talloc2 is distributed in the hope that it will be useful, but WITHOUT ANY WARRANTY; without even the implied warranty of MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU General Lesser Public License for more details.
// You should have received a copy of the GNU General Lesser Public License along with talloc2. If not, see <http://www.gnu.org/licenses/>.

#include <stdlib.h>
#include <string.h>
#include <stdbool.h>

#include "tree.h"
#include "chunk.h"

#if defined(TALLOC_EVENTS)
#include "events.h"
#endif

#if defined(TALLOC_EXT)
#include "ext/chunk.h"
#endif

#if defined(TALLOC_REFERENCE)
#include "reference/chunk.h"
#endif

static inline
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

#if defined(TALLOC_EVENTS)
    return talloc_on_add ( child );
#else
    return 0;
#endif

}

void __attribute__ ( ( noinline ) ) * talloc ( const void * parent_data, size_t length )
{
    talloc_chunk * chunk;

#if defined(TALLOC_EXT)
    chunk = talloc_ext_chunk_malloc ( length );
#else
    chunk = talloc_usual_chunk_malloc ( length );
#endif

    if ( talloc_add_chunk ( parent_data, chunk ) != 0 ) {
#if defined(TALLOC_EXT)
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

#if defined(TALLOC_EXT)
    chunk = talloc_ext_chunk_calloc ( length );
#else
    chunk = talloc_usual_chunk_calloc ( length );
#endif

    if ( talloc_add_chunk ( parent_data, chunk ) != 0 ) {
#if defined(TALLOC_EXT)
        talloc_ext_chunk_free ( chunk );
#else
        talloc_usual_chunk_free ( chunk );
#endif
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
        parent->first_child = chunk;
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

    talloc_chunk * new_chunk;
#if defined(TALLOC_EXT)
    new_chunk = talloc_ext_chunk_realloc ( old_chunk, length );
#else
    new_chunk = talloc_usual_chunk_realloc ( old_chunk, length );
#endif

    if ( new_chunk == NULL ) {
        return NULL;
    }
    if ( old_chunk != new_chunk ) {
        // now pointers to old_chunk is invalid
        // each pointer to old_chunk should be replaced with new_chunk
        talloc_update_chunk ( new_chunk );
    }

#if defined(TALLOC_EVENTS)
    if ( talloc_on_update ( new_chunk ) != 0 ) {
        return NULL;
    }
#endif

    return talloc_data_from_chunk ( new_chunk );
}

uint8_t talloc_move ( const void * child_data, const void * parent_data )
{
    if ( child_data == NULL ) {
        return 1;
    }
    talloc_chunk * child = talloc_chunk_from_data ( child_data );
    talloc_detach_chunk ( child );

    if ( parent_data == NULL ) {
        child->parent = NULL;
    } else {
        talloc_chunk * new_parent = talloc_chunk_from_data ( parent_data );
        talloc_set_child_chunk ( new_parent, child );
    }

#if defined(TALLOC_EVENTS)
    return talloc_on_move ( child );
#else
    return 0;
#endif

}

static
uint8_t talloc_free_chunk_recursive ( talloc_chunk * chunk )
{
    uint8_t result, error = 0;
    talloc_chunk * child  = chunk->first_child;

#if defined(TALLOC_EVENTS)
    if ( ( result = talloc_on_del ( chunk ) ) != 0 ) {
        error = result;
    }
#endif

#if defined(TALLOC_REFERENCE)
    if ( chunk->mode == TALLOC_MODE_EXT ) {
        if ( ( result = talloc_ext_chunk_free ( chunk ) ) != 0 ) {
            error = result;
        }
    } else {
        talloc_reference_chunk_free ( chunk );
    }
#elif defined(TALLOC_EXT)
    if ( ( result = talloc_ext_chunk_free ( chunk ) ) != 0 ) {
        error = result;
    }
#else
    talloc_usual_chunk_free ( chunk );
#endif

    talloc_chunk * next_child;
    while ( child != NULL ) {
        next_child = child->next;
        if ( ( result = talloc_free_chunk_recursive ( child ) ) != 0 ) {
            error = result;
        }
        child = next_child;
    }

    return error;
}

uint8_t talloc_free_chunk ( talloc_chunk * chunk )
{
    talloc_detach_chunk ( chunk );
    return talloc_free_chunk_recursive ( chunk );
}

extern inline void    talloc_set_child_chunk ( talloc_chunk * parent, talloc_chunk * child );
extern inline void    talloc_detach_chunk    ( talloc_chunk * chunk );
extern inline void *  talloc_new             ( const void * parent_data );
extern inline uint8_t talloc_free            ( void * root_data );