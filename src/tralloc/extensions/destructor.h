// This file is part of tralloc. Copyright (C) 2013 Andrew Aladjev aladjev.andrew@gmail.com
// tralloc is free software: you can redistribute it and/or modify it under the terms of the GNU General Lesser Public License as published by the Free Software Foundation, either version 3 of the License, or (at your option) any later version.
// tralloc is distributed in the hope that it will be useful, but WITHOUT ANY WARRANTY; without even the implied warranty of MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU General Lesser Public License for more details.
// You should have received a copy of the GNU General Lesser Public License along with tralloc. If not, see <http://www.gnu.org/licenses/>.

#ifndef TRALLOC_EXTENSIONS_DESTRUCTOR_H
#define TRALLOC_EXTENSIONS_DESTRUCTOR_H

#include "chunk.h"
#include <stdbool.h>

inline
void tralloc_destructor_free_silent ( tralloc_extensions * extensions )
{
    tralloc_destructor * destructor = extensions->first_destructor;
    tralloc_destructor * next_destructor;
    while ( destructor != NULL ) {
        next_destructor = destructor->next;
        free ( destructor );
        destructor = next_destructor;
    }
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
    tralloc_extensions * extensions = tralloc_extensions_from_chunk ( tralloc_chunk_from_context ( chunk_context ) );

    tralloc_destructor_free_silent ( extensions );
    extensions->first_destructor = NULL;
    return 0;
}

// If chunk_context is NULL function will return non-zero value.
// Otherwise function will obtain chunk from chunk_context, create destructor from function + user_data and add it to chunk's destructors list.
// Function returns zero or non-zero value if error occurred.
uint8_t tralloc_add_destructor ( const tralloc_context * chunk_context, tralloc_destructor_function function, void * user_data );

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

uint8_t tralloc_destructor_free ( tralloc_chunk * chunk, tralloc_extensions * extensions );

#endif
