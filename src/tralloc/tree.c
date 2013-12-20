// This file is part of tralloc. Copyright (C) 2013 Andrew Aladjev aladjev.andrew@gmail.com
// tralloc is free software: you can redistribute it and/or modify it under the terms of the GNU General Lesser Public License as published by the Free Software Foundation, either version 3 of the License, or (at your option) any later version.
// tralloc is distributed in the hope that it will be useful, but WITHOUT ANY WARRANTY; without even the implied warranty of MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU General Lesser Public License for more details.
// You should have received a copy of the GNU General Lesser Public License along with tralloc. If not, see <http://www.gnu.org/licenses/>.

#include <stdlib.h>
#include <string.h>
#include <stdbool.h>

#include "tree.h"
#include "chunk.h"

#if defined(TRALLOC_EXTENSIONS)
#include "extensions/chunk.h"
#endif

#if defined(TRALLOC_REFERENCE)
#include "reference/chunk.h"
#endif

tralloc_context * tralloc ( const tralloc_context * parent_context, size_t length )
{
    tralloc_chunk * chunk;

#if defined(TRALLOC_EXTENSIONS)
    chunk = tralloc_extensions_malloc_chunk ( parent_context, length );
#else
    chunk = tralloc_usual_malloc_chunk ( parent_context, length );
#endif

    if ( chunk == NULL ) {
        return NULL;
    }

    return tralloc_context_from_chunk ( chunk );
}

tralloc_context * tralloc_zero ( const tralloc_context * parent_context, size_t length )
{
    tralloc_chunk * chunk;

#if defined(TRALLOC_EXTENSIONS)
    chunk = tralloc_extensions_calloc_chunk ( parent_context, length );
#else
    chunk = tralloc_usual_calloc_chunk ( parent_context, length );
#endif

    if ( chunk == NULL ) {
        return NULL;
    }

    return tralloc_context_from_chunk ( chunk );
}

static inline
void tralloc_update_chunk ( tralloc_chunk * chunk )
{
    tralloc_chunk * prev = chunk->prev;
    if ( prev == NULL ) {
        tralloc_chunk * parent = chunk->parent;
        if ( parent != NULL ) {
            parent->first_child = chunk;
        }
    } else {
        prev->next = chunk;
    }
    tralloc_chunk * next = chunk->next;
    if ( next != NULL ) {
        next->prev = chunk;
    }

    tralloc_chunk * next_child = chunk->first_child;
    while ( next_child != NULL ) {
        next_child->parent = chunk;
        next_child = next_child->next;
    }
}

tralloc_context * tralloc_realloc ( const tralloc_context * chunk_context, size_t length )
{
    if ( chunk_context == NULL ) {
        return NULL;
    }
    tralloc_chunk * old_chunk = tralloc_chunk_from_context ( chunk_context );

#if defined(TRALLOC_DEBUG)
    size_t old_length = old_chunk->length;
#endif

    tralloc_chunk * new_chunk;

#if defined(TRALLOC_REFERENCE)
    if ( old_chunk->mode == TRALLOC_MODE_EXTENSIONS ) {
        new_chunk = tralloc_extensions_realloc_chunk ( old_chunk, length );
    } else {
        new_chunk = tralloc_reference_realloc_chunk ( old_chunk, length );
    }
#elif defined(TRALLOC_EXTENSIONS)
    new_chunk = tralloc_extensions_realloc_chunk ( old_chunk, length );
#else
    new_chunk = tralloc_usual_realloc_chunk ( old_chunk, length );
#endif

    if ( new_chunk == NULL ) {
        return NULL;
    }
    if ( old_chunk != new_chunk ) {
        // now pointers to old_chunk is invalid
        // each pointer to old_chunk should be replaced with new_chunk
        tralloc_update_chunk ( new_chunk );
    }

#if defined(TRALLOC_DEBUG)
    if ( tralloc_on_resize ( new_chunk, old_length ) != 0 ) {
        return NULL;
    }
#endif

    return tralloc_context_from_chunk ( new_chunk );
}

uint8_t tralloc_move ( const tralloc_context * child_context, const tralloc_context * parent_context )
{
    if ( child_context == NULL || parent_context == child_context ) {
        return 1;
    }
    tralloc_chunk * child = tralloc_chunk_from_context ( child_context );

#if defined(TRALLOC_DEBUG)
    tralloc_chunk * old_parent;
#endif

    if ( parent_context == NULL ) {
        if ( child->parent == NULL ) {
            return 0;
        }

#if defined(TRALLOC_DEBUG)
        old_parent = child->parent;
#endif

        tralloc_detach_chunk ( child );
        child->parent = NULL;
    } else {
        tralloc_chunk * new_parent = tralloc_chunk_from_context ( parent_context );
        if ( child->parent == new_parent ) {
            return 0;
        }

#if defined(TRALLOC_DEBUG)
        old_parent = child->parent;
#endif

        tralloc_detach_chunk ( child );
        tralloc_set_child_chunk ( new_parent, child );
    }

#if defined(TRALLOC_DEBUG)
    return tralloc_on_move ( child, old_parent );
#else
    return 0;
#endif

}

uint8_t tralloc_free_chunk ( tralloc_chunk * chunk )
{

#if defined(TRALLOC_REFERENCE)
    if ( chunk->mode == TRALLOC_MODE_EXTENSIONS ) {
        return tralloc_extensions_free_chunk ( chunk );
    } else {
        return tralloc_reference_free_chunk ( chunk );
    }
#elif defined(TRALLOC_EXTENSIONS)
    return tralloc_extensions_free_chunk ( chunk );
#else
    return tralloc_usual_free_chunk ( chunk );
#endif

}

extern inline uint8_t           tralloc_add_chunk           ( const tralloc_context * parent_context, tralloc_chunk * child );
extern inline void              tralloc_set_child_chunk     ( tralloc_chunk * parent, tralloc_chunk * child );
extern inline void              tralloc_detach_chunk        ( tralloc_chunk * chunk );
extern inline tralloc_context * tralloc_new                 ( const tralloc_context * parent_context );
extern inline uint8_t           tralloc_free                ( const tralloc_context * root_context );
extern inline uint8_t           tralloc_free_chunk_children ( tralloc_chunk * chunk );
