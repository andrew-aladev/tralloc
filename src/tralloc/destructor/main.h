// This file is part of tralloc. Copyright (C) 2013 Andrew Aladjev aladjev.andrew@gmail.com
// tralloc is free software: you can redistribute it and/or modify it under the terms of the GNU General Lesser Public License as published by the Free Software Foundation, either version 3 of the License, or (at your option) any later version.
// tralloc is distributed in the hope that it will be useful, but WITHOUT ANY WARRANTY; without even the implied warranty of MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU General Lesser Public License for more details.
// You should have received a copy of the GNU General Lesser Public License along with tralloc. If not, see <http://www.gnu.org/licenses/>.

#ifndef TRALLOC_DESTRUCTOR_H
#define TRALLOC_DESTRUCTOR_H

#include "../common.h"
#include <stdlib.h>
#include <stdbool.h>


// If chunk_context is NULL function will return non-zero value.
// Otherwise function will obtain chunk from chunk_context and clear it's destructors list.
// Destructors will not be called.
// Function returns zero or non-zero value if error occurred.

void _tralloc_clear_destructors ( _tralloc_chunk * chunk );

inline
uint8_t tralloc_clear_destructors ( const tralloc_context * chunk_context )
{
    if ( chunk_context == NULL ) {
        return 1;
    }
    _tralloc_clear_destructors ( _tralloc_chunk_from_context ( chunk_context ) );
    return 0;
}


// If chunk_context is NULL function will return non-zero value.
// Otherwise function will obtain chunk from chunk_context, create destructor from function + user_data and append it to chunk's destructors list.
// Order of calls are:
// 1. Destructor from root to it's children.
// 2. Free from children to root.
// If you want to use some data in destructor - tralloc it to the chunk_context and pass as user_data.
// Function returns zero or non-zero value if error occurred.

uint8_t _tralloc_append_destructor ( _tralloc_chunk * chunk, tralloc_destructor_function function, void * user_data );

inline
uint8_t tralloc_append_destructor ( const tralloc_context * chunk_context, tralloc_destructor_function function, void * user_data )
{
    if ( chunk_context == NULL ) {
        return 1;
    }
    return _tralloc_append_destructor ( _tralloc_chunk_from_context ( chunk_context ), function, user_data );
}


// Function works the same as "tralloc_append_destructor".
// It will use prepend destructor to destructors list instead of append.

uint8_t _tralloc_prepend_destructor ( _tralloc_chunk * chunk, tralloc_destructor_function function, void * user_data );

inline
uint8_t tralloc_prepend_destructor ( const tralloc_context * chunk_context, tralloc_destructor_function function, void * user_data )
{
    if ( chunk_context == NULL ) {
        return 1;
    }
    return _tralloc_prepend_destructor ( _tralloc_chunk_from_context ( chunk_context ), function, user_data );
}


inline
bool _tralloc_destructor_comparator_by_function ( _tralloc_destructor * destructor, tralloc_destructor_function function, void * user_data )
{
    return destructor->function == function;
}

inline
bool _tralloc_destructor_comparator_by_data ( _tralloc_destructor * destructor, tralloc_destructor_function function, void * user_data )
{
    return destructor->user_data == user_data;
}

inline
bool _tralloc_destructor_comparator_strict ( _tralloc_destructor * destructor, tralloc_destructor_function function, void * user_data )
{
    return destructor->function == function && destructor->user_data == user_data;
}

typedef bool ( * _tralloc_destructor_comparator ) ( _tralloc_destructor * destructor, tralloc_destructor_function function, void * user_data );

void _tralloc_delete_destructors_by_comparator ( _tralloc_chunk * chunk, _tralloc_destructor_comparator comparator, tralloc_destructor_function function, void * user_data );

inline
uint8_t _tralloc_delete_destructors ( const tralloc_context * chunk_context, _tralloc_destructor_comparator comparator, tralloc_destructor_function function, void * user_data )
{
    if ( chunk_context == NULL ) {
        return 1;
    }
    _tralloc_delete_destructors_by_comparator ( _tralloc_chunk_from_context ( chunk_context ), comparator, function, user_data );
    return 0;
}

// If chunk_context is NULL function will return non-zero value.
// Otherwise function will obtain chunk from chunk_context and delete destructor by function + user_data from it's destructors list.
// Function returns zero or non-zero value if error occurred.

inline
uint8_t tralloc_delete_destructor ( const tralloc_context * chunk_context, tralloc_destructor_function function, void * user_data )
{
    return _tralloc_delete_destructors ( chunk_context, _tralloc_destructor_comparator_strict, function, user_data );
}

// If chunk_context is NULL function will return non-zero value.
// Otherwise function will obtain chunk from chunk_context and delete destructor by function from it's destructors list.
// Function returns zero or non-zero value if error occurred.

inline
uint8_t tralloc_delete_destructor_by_function ( const tralloc_context * chunk_context, tralloc_destructor_function function )
{
    return _tralloc_delete_destructors ( chunk_context, _tralloc_destructor_comparator_by_function, function, NULL );
}

// If chunk_context is NULL function will return non-zero value.
// Otherwise function will obtain chunk from chunk_context and delete destructor by user_data from it's destructors list.
// Function returns zero or non-zero value if error occurred.

inline
uint8_t tralloc_delete_destructor_by_data ( const tralloc_context * chunk_context, void * user_data )
{
    return _tralloc_delete_destructors ( chunk_context, _tralloc_destructor_comparator_by_data, NULL, user_data );
}
