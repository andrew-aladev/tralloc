// This file is part of tralloc. Copyright (C) 2013 Andrew Aladjev aladjev.andrew@gmail.com
// tralloc is free software: you can redistribute it and/or modify it under the terms of the GNU General Lesser Public License as published by the Free Software Foundation, either version 3 of the License, or (at your option) any later version.
// tralloc is distributed in the hope that it will be useful, but WITHOUT ANY WARRANTY; without even the implied warranty of MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU General Lesser Public License for more details.
// You should have received a copy of the GNU General Lesser Public License along with tralloc. If not, see <http://www.gnu.org/licenses/>.

#define _TRALLOC_TREE_FREE_INCLUDED_FROM_OBJECT
#include "free.h"
#include "chunk.h"

#if defined(TRALLOC_DEBUG)
#   include "../debug/chunk.h"
#endif

#if defined(TRALLOC_DESTRUCTOR)
#   include "../destructor/chunk.h"
#endif

#if defined(TRALLOC_REFERENCE)
#   include "../reference/head_chunk.h"
#   include "../reference/chunk.h"
#endif

#if defined(TRALLOC_POOL)
#   include "../pool/chunk.h"
#   include "../pool/head_chunk.h"
#endif

#include <stdlib.h>


static inline
tralloc_bool _tralloc_can_free_chunk ( _tralloc_chunk * chunk )
{

#   if defined(TRALLOC_REFERENCE)
    if ( chunk->extensions & TRALLOC_EXTENSION_REFERENCES ) {
        if ( ! _tralloc_references_can_free_chunk ( chunk ) ) {
            return TRALLOC_FALSE;
        }
    }
#   endif

#   if defined(TRALLOC_POOL)
    if ( chunk->extensions & TRALLOC_EXTENSION_POOL ) {
        if ( ! _tralloc_pool_can_free_chunk ( chunk ) ) {
            return TRALLOC_FALSE;
        }
    }
#   endif

    return TRALLOC_TRUE;
}

static inline
tralloc_bool _tralloc_can_free_chunk_children ( _tralloc_chunk * chunk )
{

#   if defined(TRALLOC_REFERENCE)
    if ( chunk->extensions & TRALLOC_EXTENSION_REFERENCES ) {
        if ( ! _tralloc_references_can_free_chunk_children ( chunk ) ) {
            return TRALLOC_FALSE;
        }
    }
#   endif

#   if defined(TRALLOC_POOL)
    if ( chunk->extensions & TRALLOC_EXTENSION_POOL ) {
        if ( ! _tralloc_pool_can_free_chunk_children ( chunk ) ) {
            return TRALLOC_FALSE;
        }
    }
#   endif

    return TRALLOC_TRUE;
}

static inline
tralloc_error _tralloc_free_chunk ( _tralloc_chunk * chunk )
{
    tralloc_error error = 0, result;
    size_t extensions_length = 0;

#   if defined(TRALLOC_LENGTH)
    if ( chunk->extensions & TRALLOC_EXTENSION_LENGTH ) {
        extensions_length += sizeof ( _tralloc_length );
    }
#   endif

#   if defined(TRALLOC_DESTRUCTOR)
    if ( chunk->extensions & TRALLOC_EXTENSION_DESTRUCTORS ) {
        result = _tralloc_destructors_free_chunk ( chunk );
        if ( result != 0 ) {
            error = result;
        }
        extensions_length += sizeof ( _tralloc_destructors );
    }
#   endif

#   if defined(TRALLOC_REFERENCE)
    if ( chunk->extensions & TRALLOC_EXTENSION_REFERENCES ) {
        extensions_length += sizeof ( _tralloc_references );
    } else if ( chunk->extensions & TRALLOC_EXTENSION_REFERENCE ) {
        result = _tralloc_reference_free_chunk ( chunk );
        if ( result != 0 ) {
            error = result;
        }
        extensions_length += sizeof ( _tralloc_reference );
    }
#   endif

#   if defined(TRALLOC_POOL)
    tralloc_bool have_pool_child = chunk->extensions & TRALLOC_EXTENSION_POOL_CHILD;
    if ( chunk->extensions & TRALLOC_EXTENSION_POOL ) {
        extensions_length += sizeof ( _tralloc_pool );
    } else if ( have_pool_child ) {
        result = _tralloc_pool_child_free_chunk ( chunk );
        if ( result != 0 ) {
            error = result;
        }
    }
#   endif

#   if defined(TRALLOC_DEBUG)
    result = _tralloc_debug_free_chunk ( chunk );
    if ( result != 0 ) {
        error = result;
    }
#   endif

#   if defined(TRALLOC_POOL)
    if ( have_pool_child ) {
        return error;
    }
#   endif

    free ( ( void * ) ( ( uintptr_t ) chunk - extensions_length ) );

    return error;
}

static inline
void _tralloc_silent_detach_chunk ( _tralloc_chunk * chunk )
{
    chunk->parent = NULL;
    chunk->prev   = NULL;
    chunk->next   = NULL;
}

static inline
_tralloc_chunk * _tralloc_get_vertical_list_root ( _tralloc_chunk * root_chunk )
{
    _tralloc_chunk * chunk_to_detach;
    _tralloc_chunk * chunk = root_chunk;

    while ( ! _tralloc_can_free_chunk ( chunk ) ) {
        if ( _tralloc_can_free_chunk_children ( chunk ) && chunk->first_child != NULL ) {
            chunk = chunk->first_child;
        } else {
            if ( chunk == root_chunk ) {
                return NULL;
            }

            while ( chunk->next == NULL ) {
                chunk_to_detach = chunk;
                chunk = chunk->parent;
                _tralloc_silent_detach_chunk ( chunk_to_detach );

                if ( chunk == root_chunk ) {
                    return NULL;
                }
            }
            chunk_to_detach = chunk;
            chunk = chunk->next;
            _tralloc_silent_detach_chunk ( chunk_to_detach );
        }
    }

    return chunk;
}

static inline
void _tralloc_subtree_to_vertical_list ( _tralloc_chunk * list_root_chunk, _tralloc_chunk * root_chunk )
{
    _tralloc_chunk * prev_chunk, * next_chunk, * chunk_to_detach;
    prev_chunk = next_chunk = list_root_chunk;

    while ( TRALLOC_TRUE ) {
        do {
            if ( _tralloc_can_free_chunk_children ( next_chunk ) && next_chunk->first_child != NULL ) {
                next_chunk = next_chunk->first_child;
            } else {
                if ( next_chunk == root_chunk ) {
                    prev_chunk->first_child = NULL;
                    return;
                }

                while ( next_chunk->next == NULL ) {
                    chunk_to_detach = next_chunk;
                    next_chunk = next_chunk->parent;
                    _tralloc_silent_detach_chunk ( chunk_to_detach );

                    if ( next_chunk == root_chunk ) {
                        prev_chunk->first_child = NULL;
                        return;
                    }
                }
                chunk_to_detach = next_chunk;
                next_chunk = next_chunk->next;
                _tralloc_silent_detach_chunk ( chunk_to_detach );
            }
        } while ( ! _tralloc_can_free_chunk ( next_chunk ) );

        prev_chunk->first_child = next_chunk;
        prev_chunk = next_chunk;
    }
}

tralloc_error _tralloc_free_subtree ( _tralloc_chunk * root_chunk )
{
    _tralloc_detach_chunk ( root_chunk );

    _tralloc_chunk * chunk = _tralloc_get_vertical_list_root ( root_chunk );
    if ( chunk == NULL ) {
        return 0;
    }
    _tralloc_subtree_to_vertical_list ( chunk, root_chunk );

    tralloc_error error = 0, result;
    _tralloc_chunk * next_chunk;
    while ( chunk != NULL ) {
        next_chunk = chunk->first_child;

        result = _tralloc_free_chunk ( chunk );
        if ( result != 0 ) {
            error = result;
        }

        chunk = next_chunk;
    }

    return error;
}

tralloc_error _tralloc_free_single ( _tralloc_chunk * chunk )
{
    _tralloc_detach_chunk ( chunk );
    return _tralloc_free_chunk ( chunk );
}

