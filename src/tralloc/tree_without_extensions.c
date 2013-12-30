// This file is part of tralloc. Copyright (C) 2013 Andrew Aladjev aladjev.andrew@gmail.com
// tralloc is free software: you can redistribute it and/or modify it under the terms of the GNU General Lesser Public License as published by the Free Software Foundation, either version 3 of the License, or (at your option) any later version.
// tralloc is distributed in the hope that it will be useful, but WITHOUT ANY WARRANTY; without even the implied warranty of MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU General Lesser Public License for more details.
// You should have received a copy of the GNU General Lesser Public License along with tralloc. If not, see <http://www.gnu.org/licenses/>.

#include <stdlib.h>

#include "tree.h"
#include "chunk.h"

typedef void * ( * _allocator ) ( size_t length );

static inline
void * _malloc ( size_t length )
{
    return malloc ( length );
}

static inline
void * _calloc ( size_t length )
{
    return calloc ( 1, length );
}


static inline
tralloc_context * _tralloc_with_allocator ( const tralloc_context * parent_context, size_t length, _allocator allocator )
{
    _tralloc_chunk * chunk = ( _tralloc_chunk * ) allocator ( sizeof ( _tralloc_chunk ) + length );
    if ( chunk == NULL ) {
        return NULL;
    }

#if defined(TRALLOC_DEBUG)
    chunk->chunk_length = sizeof ( _tralloc_chunk );
    chunk->length       = length;
#endif

    _tralloc_add_chunk ( parent_context, chunk );
    return _tralloc_context_from_chunk ( chunk );
}

tralloc_context * tralloc ( const tralloc_context * parent_context, size_t length )
{
    return _tralloc_with_allocator ( parent_context, length, _malloc );
}

tralloc_context * tralloc_zero ( const tralloc_context * parent_context, size_t length )
{
    return _tralloc_with_allocator ( parent_context, length, _calloc );
}


_tralloc_chunk * _tralloc_realloc ( _tralloc_chunk * old_chunk, size_t length )
{

#if defined(TRALLOC_DEBUG)
    size_t old_length = old_chunk->length;
#endif

    _tralloc_chunk * new_chunk = realloc ( old_chunk, sizeof ( _tralloc_chunk ) + length );
    if ( new_chunk == NULL ) {
        return NULL;
    }

    if ( old_chunk == new_chunk ) {
        old_chunk->length = length;

#if defined(TRALLOC_DEBUG)
        if ( _tralloc_on_resize ( old_chunk, old_length ) != 0 ) {
            return NULL;
        }
#endif

        return old_chunk;
    } else {
        new_chunk->length = length;
        _tralloc_usual_update_chunk ( new_chunk );

#if defined(TRALLOC_DEBUG)
        if ( _tralloc_on_resize ( new_chunk, old_length ) != 0 ) {
            return NULL;
        }
#endif

        return new_chunk;
    }
}

extern inline tralloc_context * tralloc_realloc ( const tralloc_context * chunk_context, size_t length );


uint8_t _tralloc_free_chunk ( _tralloc_chunk * chunk )
{
    uint8_t result, error = 0;

#if defined(TRALLOC_DEBUG)
    if ( ( result = _tralloc_on_free ( chunk ) ) != 0 ) {
        error = result;
    }
#endif

    if ( ( result = _tralloc_free_chunk_children ( chunk ) ) != 0 ) {
        error = result;
    }

    free ( chunk );
    return error;
}
