// This file is part of tralloc. Copyright (C) 2013 Andrew Aladjev aladjev.andrew@gmail.com
// tralloc is free software: you can redistribute it and/or modify it under the terms of the GNU General Lesser Public License as published by the Free Software Foundation, either version 3 of the License, or (at your option) any later version.
// tralloc is distributed in the hope that it will be useful, but WITHOUT ANY WARRANTY; without even the implied warranty of MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU General Lesser Public License for more details.
// You should have received a copy of the GNU General Lesser Public License along with tralloc. If not, see <http://www.gnu.org/licenses/>.

#ifndef TRALLOC_DESTRUCTOR_H
#define TRALLOC_DESTRUCTOR_H

#include "common.h"
#include <stdbool.h>
#include <stdlib.h>

inline
void _tralloc_destructor_free_silent ( _tralloc_destructors * destructors )
{
    if ( destructors == NULL ) {
        return;
    }
    _tralloc_destructor * destructor = destructors->first_destructor;
    _tralloc_destructor * next_destructor;
    while ( destructor != NULL ) {
        next_destructor = destructor->next;
        free ( destructor );
        destructor = next_destructor;
    }
    free ( destructors );
}

// If chunk_context is NULL function will return non-zero value.
// Otherwise function will obtain chunk from chunk_context and clear it's destructors list.
// Destructors will not be called.
// Function returns zero or non-zero value if error occurred.
inline
uint8_t tralloc_clear_destructors ( const tralloc_context * chunk_context )
{
    if ( chunk_context == NULL ) {
        return 1;
    }
    _tralloc_chunk * chunk = _tralloc_chunk_from_context ( chunk_context );
    _tralloc_destructor_free_silent ( chunk->destructors );
    chunk->destructors = NULL;
    return 0;
}

// If chunk_context is NULL function will return non-zero value.
// Otherwise function will obtain chunk from chunk_context, create destructor from function + user_data and append it to chunk's destructors list.
// Order of calls are:
// 1. Destructor from root to it's children.
// 2. Free from children to root.
// If you want to use some data in destructor - tralloc it to the chunk_context and pass as user_data.
// Function returns zero or non-zero value if error occurred.
uint8_t tralloc_append_destructor ( const tralloc_context * chunk_context, tralloc_destructor_function function, void * user_data );

// Function works the same as "tralloc_append_destructor".
// It will use prepend destructor to destructors list instead of append.
uint8_t tralloc_prepend_destructor ( const tralloc_context * chunk_context, tralloc_destructor_function function, void * user_data );

// If chunk_context is NULL function will return non-zero value.
// Otherwise function will obtain chunk from chunk_context and delete destructor by function + user_data from it's destructors list.
// Function returns zero or non-zero value if error occurred.
uint8_t tralloc_del_destructor ( const tralloc_context * chunk_context, tralloc_destructor_function function, void * user_data );

// If chunk_context is NULL function will return non-zero value.
// Otherwise function will obtain chunk from chunk_context and delete destructor by function from it's destructors list.
// Function returns zero or non-zero value if error occurred.
uint8_t tralloc_del_destructor_by_function ( const tralloc_context * chunk_context, tralloc_destructor_function function );

// If chunk_context is NULL function will return non-zero value.
// Otherwise function will obtain chunk from chunk_context and delete destructor by user_data from it's destructors list.
// Function returns zero or non-zero value if error occurred.
uint8_t tralloc_del_destructor_by_data ( const tralloc_context * chunk_context, void * user_data );

inline
uint8_t _tralloc_destructor_free ( _tralloc_chunk * chunk )
{
    _tralloc_destructors * destructors = chunk->destructors;
    if ( destructors == NULL ) {
        return 0;
    }

    uint8_t result, error = 0;
    tralloc_context * chunk_context  = _tralloc_context_from_chunk ( chunk );
    _tralloc_destructor * destructor = destructors->first_destructor;
    _tralloc_destructor * next_destructor;
    tralloc_destructor_function function;

    while ( destructor != NULL ) {
        function = destructor->function;
        if ( function != NULL ) {
            if ( ( result = function ( chunk_context, destructor->user_data ) ) != 0 ) {
                error = result;
            }
        }

        next_destructor = destructor->next;
        free ( destructor );
        destructor = next_destructor;
    }
    free ( destructors );
    return error;
}

#endif
