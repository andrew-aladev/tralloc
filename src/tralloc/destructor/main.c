// This file is part of tralloc. Copyright (C) 2013 Andrew Aladjev aladjev.andrew@gmail.com
// tralloc is free software: you can redistribute it and/or modify it under the terms of the GNU General Lesser Public License as published by the Free Software Foundation, either version 3 of the License, or (at your option) any later version.
// tralloc is distributed in the hope that it will be useful, but WITHOUT ANY WARRANTY; without even the implied warranty of MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU General Lesser Public License for more details.
// You should have received a copy of the GNU General Lesser Public License along with tralloc. If not, see <http://www.gnu.org/licenses/>.

#define _TRALLOC_DESTRUCTOR_INCLUDED_FROM_OBJECT
#include "main.h"
#include "common.h"
#include "../common.h"
#include <stdlib.h>


tralloc_error tralloc_clear_destructors ( tralloc_context * chunk_context )
{
    if ( chunk_context == NULL ) {
        return TRALLOC_ERROR_REQUIRED_ARGUMENT_IS_NULL;
    }
    _tralloc_chunk * chunk = _tralloc_get_chunk_from_context ( chunk_context );

    if ( ! ( chunk->extensions & TRALLOC_EXTENSION_DESTRUCTORS ) ) {
        return TRALLOC_ERROR_NO_SUCH_EXTENSION;
    }

    _tralloc_destructors * destructors = _tralloc_get_destructors_from_chunk ( chunk );
    _tralloc_destructor * destructor   = destructors->first_destructor;
    if ( destructor == NULL ) {
        return 0;
    }

    _tralloc_destructor * next_destructor;
    while ( destructor != NULL ) {
        next_destructor = destructor->next;
        free ( destructor );
        destructor = next_destructor;
    }
    destructors->first_destructor = NULL;
    destructors->last_destructor  = NULL;
    return 0;
}


tralloc_error tralloc_append_destructor ( tralloc_context * chunk_context, tralloc_destructor_function function, void * user_data )
{
    if ( chunk_context == NULL ) {
        return TRALLOC_ERROR_REQUIRED_ARGUMENT_IS_NULL;
    }
    _tralloc_chunk * chunk = _tralloc_get_chunk_from_context ( chunk_context );

    if ( ! ( chunk->extensions & TRALLOC_EXTENSION_DESTRUCTORS ) ) {
        return TRALLOC_ERROR_NO_SUCH_EXTENSION;
    }

    _tralloc_destructor * destructor = malloc ( sizeof ( _tralloc_destructor ) );
    if ( destructor == NULL ) {
        return TRALLOC_ERROR_MALLOC_FAILED;
    }
    destructor->function  = function;
    destructor->user_data = user_data;
    destructor->next      = NULL;

    _tralloc_destructors * destructors    = _tralloc_get_destructors_from_chunk ( chunk );
    _tralloc_destructor * last_destructor = destructors->last_destructor;
    if ( last_destructor == NULL ) {
        destructors->first_destructor = destructor;
        destructors->last_destructor  = destructor;
        return 0;
    }

    last_destructor->next        = destructor;
    destructors->last_destructor = destructor;
    return 0;
}

tralloc_error tralloc_prepend_destructor ( tralloc_context * chunk_context, tralloc_destructor_function function, void * user_data )
{
    if ( chunk_context == NULL ) {
        return TRALLOC_ERROR_REQUIRED_ARGUMENT_IS_NULL;
    }
    _tralloc_chunk * chunk = _tralloc_get_chunk_from_context ( chunk_context );

    if ( ! ( chunk->extensions & TRALLOC_EXTENSION_DESTRUCTORS ) ) {
        return TRALLOC_ERROR_NO_SUCH_EXTENSION;
    }

    _tralloc_destructor * destructor = malloc ( sizeof ( _tralloc_destructor ) );
    if ( destructor == NULL ) {
        return TRALLOC_ERROR_MALLOC_FAILED;
    }
    destructor->function  = function;
    destructor->user_data = user_data;

    _tralloc_destructors * destructors     = _tralloc_get_destructors_from_chunk ( chunk );
    _tralloc_destructor * first_destructor = destructors->first_destructor;
    if ( first_destructor == NULL ) {
        destructor->next              = NULL;
        destructors->first_destructor = destructor;
        destructors->last_destructor  = destructor;
        return 0;
    }

    destructor->next              = first_destructor;
    destructors->first_destructor = destructor;
    return 0;
}


tralloc_error _tralloc_delete_destructors_by_comparator ( tralloc_context * chunk_context, _tralloc_destructor_comparator comparator, tralloc_destructor_function function, void * user_data )
{
    if ( chunk_context == NULL ) {
        return TRALLOC_ERROR_REQUIRED_ARGUMENT_IS_NULL;
    }
    _tralloc_chunk * chunk = _tralloc_get_chunk_from_context ( chunk_context );

    if ( ! ( chunk->extensions & TRALLOC_EXTENSION_DESTRUCTORS ) ) {
        return TRALLOC_ERROR_NO_SUCH_EXTENSION;
    }

    _tralloc_destructors * destructors     = _tralloc_get_destructors_from_chunk ( chunk );
    _tralloc_destructor * first_destructor = destructors->first_destructor;
    if ( first_destructor == NULL ) {
        return 0;
    }

    _tralloc_destructor * last_destructor = NULL;
    _tralloc_destructor * destructor      = first_destructor;
    _tralloc_destructor * next_destructor;

    while ( destructor != NULL ) {
        next_destructor = destructor->next;
        if ( comparator ( destructor, function, user_data ) ) {
            if ( first_destructor == destructor ) {
                first_destructor = next_destructor;
            }
            free ( destructor );
            if ( last_destructor != NULL ) {
                last_destructor->next = next_destructor;
            }
        } else {
            last_destructor = destructor;
        }
        destructor = next_destructor;
    }
    if ( destructors->first_destructor != first_destructor ) {
        destructors->first_destructor = first_destructor;
    }
    if ( destructors->last_destructor != last_destructor ) {
        destructors->last_destructor = last_destructor;
    }
    return 0;
}
