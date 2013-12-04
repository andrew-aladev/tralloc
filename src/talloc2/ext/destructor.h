// This file is part of talloc2. Copyright (C) 2013 Andrew Aladjev aladjev.andrew@gmail.com
// talloc2 is free software: you can redistribute it and/or modify it under the terms of the GNU General Lesser Public License as published by the Free Software Foundation, either version 3 of the License, or (at your option) any later version.
// talloc2 is distributed in the hope that it will be useful, but WITHOUT ANY WARRANTY; without even the implied warranty of MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU General Lesser Public License for more details.
// You should have received a copy of the GNU General Lesser Public License along with talloc2. If not, see <http://www.gnu.org/licenses/>.

#ifndef TALLOC_EXT_DESTRUCTOR_H
#define TALLOC_EXT_DESTRUCTOR_H

#include "chunk.h"
#include <stdbool.h>

inline
void talloc_destructor_free_silent ( talloc_ext * ext )
{
    talloc_destructor * destructor = ext->first_destructor;
    talloc_destructor * next_destructor;
    while ( destructor != NULL ) {
        next_destructor = destructor->next;
        free ( destructor );
        destructor = next_destructor;
    }
}

inline
uint8_t talloc_clear_destructors ( const void * chunk_data )
{
    if ( chunk_data == NULL ) {
        return 1;
    }
    talloc_ext * ext = talloc_ext_from_chunk ( talloc_chunk_from_data ( chunk_data ) );

    talloc_destructor_free_silent ( ext );
    ext->first_destructor = NULL;
    return 0;
}

uint8_t talloc_add_destructor             ( const void * chunk_data, talloc_destructor_function function, void * user_data );
uint8_t talloc_del_destructor             ( const void * chunk_data, talloc_destructor_function function, void * user_data );
uint8_t talloc_del_destructor_by_function ( const void * chunk_data, talloc_destructor_function function );
uint8_t talloc_del_destructor_by_data     ( const void * chunk_data, void * user_data );
uint8_t talloc_destructor_free            ( talloc_chunk * chunk, talloc_ext * ext );

#endif
