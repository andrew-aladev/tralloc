// This file is part of tralloc. Copyright (C) 2013 Andrew Aladjev aladjev.andrew@gmail.com
// tralloc is free software: you can redistribute it and/or modify it under the terms of the GNU General Lesser Public License as published by the Free Software Foundation, either version 3 of the License, or (at your option) any later version.
// tralloc is distributed in the hope that it will be useful, but WITHOUT ANY WARRANTY; without even the implied warranty of MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU General Lesser Public License for more details.
// You should have received a copy of the GNU General Lesser Public License along with tralloc. If not, see <http://www.gnu.org/licenses/>.

#define _TRALLOC_DESTRUCTOR_CHUNK_INCLUDED_FROM_OBJECT
#include "chunk.h"
#include "../common.h"
#include <stdlib.h>


tralloc_error _tralloc_destructor_free_chunk ( _tralloc_chunk * chunk )
{
    _tralloc_destructors * destructors = _tralloc_get_destructors_from_chunk ( chunk );
    if ( destructors == NULL ) {
        return 0;
    }

    tralloc_error result, error = 0;
    tralloc_context * chunk_context  = _tralloc_get_context_from_chunk ( chunk );
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
    return error;
}
