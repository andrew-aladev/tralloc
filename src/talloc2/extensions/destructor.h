// This file is part of talloc2. Copyright (C) 2013 Andrew Aladjev aladjev.andrew@gmail.com
// talloc2 is free software: you can redistribute it and/or modify it under the terms of the GNU General Lesser Public License as published by the Free Software Foundation, either version 3 of the License, or (at your option) any later version.
// talloc2 is distributed in the hope that it will be useful, but WITHOUT ANY WARRANTY; without even the implied warranty of MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU General Lesser Public License for more details.
// You should have received a copy of the GNU General Lesser Public License along with talloc2. If not, see <http://www.gnu.org/licenses/>.

#ifndef TALLOC_EXTENSIONS_DESTRUCTOR_H
#define TALLOC_EXTENSIONS_DESTRUCTOR_H

#include "chunk.h"
#include <stdbool.h>

inline
void talloc_destructor_free_silent ( talloc_extensions * extensions )
{
    talloc_destructor * destructor = extensions->first_destructor;
    talloc_destructor * next_destructor;
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
uint8_t talloc_clear_destructors ( const talloc_context * chunk_context )
{
    if ( chunk_context == NULL ) {
        return 1;
    }
    talloc_extensions * extensions = talloc_extensions_from_chunk ( talloc_chunk_from_context ( chunk_context ) );

    talloc_destructor_free_silent ( extensions );
    extensions->first_destructor = NULL;
    return 0;
}

// If chunk_context is NULL function will return non-zero value.
// Otherwise function will obtain chunk from chunk_context, create destructor from function + user_data and add it to chunk's destructors list.
// Function returns zero or non-zero value if error occurred.
uint8_t talloc_add_destructor ( const talloc_context * chunk_context, talloc_destructor_function function, void * user_data );

// If chunk_context is NULL function will return non-zero value.
// Otherwise function will obtain chunk from chunk_context and delete destructor by function + user_data from it's destructors list.
// Function returns zero or non-zero value if error occurred.
uint8_t talloc_del_destructor ( const talloc_context * chunk_context, talloc_destructor_function function, void * user_data );

// If chunk_context is NULL function will return non-zero value.
// Otherwise function will obtain chunk from chunk_context and delete destructor by function from it's destructors list.
// Function returns zero or non-zero value if error occurred.
uint8_t talloc_del_destructor_by_function ( const talloc_context * chunk_context, talloc_destructor_function function );

// If chunk_context is NULL function will return non-zero value.
// Otherwise function will obtain chunk from chunk_context and delete destructor by user_data from it's destructors list.
// Function returns zero or non-zero value if error occurred.
uint8_t talloc_del_destructor_by_data ( const talloc_context * chunk_context, void * user_data );

uint8_t talloc_destructor_free ( talloc_chunk * chunk, talloc_extensions * extensions );

#endif