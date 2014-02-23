// This file is part of tralloc. Copyright (C) 2013 Andrew Aladjev aladjev.andrew@gmail.com
// tralloc is free software: you can redistribute it and/or modify it under the terms of the GNU General Lesser Public License as published by the Free Software Foundation, either version 3 of the License, or (at your option) any later version.
// tralloc is distributed in the hope that it will be useful, but WITHOUT ANY WARRANTY; without even the implied warranty of MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU General Lesser Public License for more details.
// You should have received a copy of the GNU General Lesser Public License along with tralloc. If not, see <http://www.gnu.org/licenses/>.

#include "pure.h"
#include "common.h"

#include "../chunk.h"

#if defined(TRALLOC_DEBUG)
#include "../events.h"
#endif

#include <stdlib.h>


typedef tralloc_error ( * _allocator ) ( void ** data, size_t length );

static inline
tralloc_error _malloc ( void ** data, size_t length )
{
    void * _data = malloc ( length );
    if ( _data == NULL ) {
        return TRALLOC_ERROR_MALLOC_FAILED;
    } else {
        * data = _data;
        return 0;
    }
}

static inline
tralloc_error _calloc ( void ** data, size_t length )
{
    void * _data = calloc ( 1, length );
    if ( _data == NULL ) {
        return TRALLOC_ERROR_CALLOC_FAILED;
    } else {
        * data = _data;
        return 0;
    }
}

static inline
tralloc_error _tralloc_with_allocator ( tralloc_context * parent_context, tralloc_context ** child_context, size_t length, _allocator allocator )
{
    if ( child_context == NULL ) {
        return TRALLOC_ERROR_REQUIRED_ARGUMENT_IS_NULL;
    }

    _tralloc_chunk * chunk;
    tralloc_error result = allocator ( ( void ** ) &chunk, sizeof ( _tralloc_chunk ) + length );
    if ( result != 0 ) {
        return result;
    }

#if defined(TRALLOC_DEBUG)
    chunk->chunk_length = sizeof ( _tralloc_chunk );
    chunk->length       = length;
#endif

    result = _tralloc_add_chunk ( parent_context, chunk );
    if ( result != 0 ) {
        return result;
    }
    * child_context = _tralloc_get_context_from_chunk ( chunk );
    return 0;
}

tralloc_error tralloc ( tralloc_context * parent_context, tralloc_context ** child_context, size_t length )
{
    return _tralloc_with_allocator ( parent_context, child_context, length, _malloc );
}

tralloc_error tralloc_zero ( tralloc_context * parent_context, tralloc_context ** child_context, size_t length )
{
    return _tralloc_with_allocator ( parent_context, child_context, length, _calloc );
}

extern inline tralloc_error tralloc_new ( tralloc_context * parent_context, tralloc_context ** child_context );


tralloc_error tralloc_realloc ( tralloc_context ** chunk_context, size_t length )
{
    if ( chunk_context == NULL ) {
        return TRALLOC_ERROR_REQUIRED_ARGUMENT_IS_NULL;
    }
    tralloc_context * context = * chunk_context;
    if ( context == NULL ) {
        return TRALLOC_ERROR_REQUIRED_ARGUMENT_IS_NULL;
    }
    _tralloc_chunk * old_chunk = _tralloc_get_chunk_from_context ( context );

#if defined(TRALLOC_DEBUG)
    size_t old_length = old_chunk->length;
#endif

    _tralloc_chunk * new_chunk = realloc ( old_chunk, sizeof ( _tralloc_chunk ) + length );
    if ( new_chunk == NULL ) {
        return TRALLOC_ERROR_REALLOC_FAILED;
    }

    if ( old_chunk == new_chunk ) {

#if defined(TRALLOC_DEBUG)
        old_chunk->length = length;
        return _tralloc_on_resize ( old_chunk, old_length );
#endif

    } else {
        _tralloc_usual_update_chunk ( new_chunk );
        * chunk_context = _tralloc_get_context_from_chunk ( new_chunk );

#if defined(TRALLOC_DEBUG)
        new_chunk->length = length;
        return _tralloc_on_resize ( new_chunk, old_length );
#endif

    }
    return 0;
}


extern inline bool _tralloc_can_free_chunk          ( _tralloc_chunk * UNUSED ( chunk ) );
extern inline bool _tralloc_can_free_chunk_children ( _tralloc_chunk * UNUSED ( chunk ) );

tralloc_error _tralloc_free_chunk ( _tralloc_chunk * chunk )
{
    tralloc_error error = 0, result;

#if defined(TRALLOC_DEBUG)
    result = _tralloc_on_free ( chunk );
    if ( result != 0 ) {
        error = result;
    }
#endif

    free ( chunk );
    return error;
}
