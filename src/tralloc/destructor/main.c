// This file is part of tralloc. Copyright (C) 2013 Andrew Aladjev aladjev.andrew@gmail.com
// tralloc is free software: you can redistribute it and/or modify it under the terms of the GNU General Lesser Public License as published by the Free Software Foundation, either version 3 of the License, or (at your option) any later version.
// tralloc is distributed in the hope that it will be useful, but WITHOUT ANY WARRANTY; without even the implied warranty of MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU General Lesser Public License for more details.
// You should have received a copy of the GNU General Lesser Public License along with tralloc. If not, see <http://www.gnu.org/licenses/>.

#include "main.h"

void _tralloc_clear_destructors ( _tralloc_chunk * chunk )
{
    _tralloc_destructors * destructors = _tralloc_destructors_from_chunk ( chunk );
    _tralloc_destructor * destructor   = destructors->first_destructor;
    if ( destructor == NULL ) {
        return;
    }

    _tralloc_destructor * next_destructor;
    while ( destructor != NULL ) {
        next_destructor = destructor->next;
        free ( destructor );
        destructor = next_destructor;
    }
    destructors->first_destructor = NULL;
    destructors->last_destructor  = NULL;
}

extern inline uint8_t tralloc_clear_destructors ( const tralloc_context * chunk_context );


uint8_t _tralloc_append_destructor ( _tralloc_chunk * chunk, tralloc_destructor_function function, void * user_data )
{
    _tralloc_destructor * destructor = malloc ( sizeof ( _tralloc_destructor ) );
    if ( destructor == NULL ) {
        return 1;
    }
    destructor->function  = function;
    destructor->user_data = user_data;
    destructor->next      = NULL;

    _tralloc_destructors * destructors    = _tralloc_destructors_from_chunk ( chunk );
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

extern inline uint8_t tralloc_append_destructor ( const tralloc_context * chunk_context, tralloc_destructor_function function, void * user_data );


uint8_t _tralloc_prepend_destructor ( _tralloc_chunk * chunk, tralloc_destructor_function function, void * user_data )
{
    _tralloc_destructor * destructor = malloc ( sizeof ( _tralloc_destructor ) );
    if ( destructor == NULL ) {
        return 1;
    }
    destructor->function  = function;
    destructor->user_data = user_data;

    _tralloc_destructors * destructors     = _tralloc_destructors_from_chunk ( chunk );
    _tralloc_destructor * first_destructor = destructors->first_destructor;
    if ( first_destructor == NULL ) {
        destructors->first_destructor = destructor;
        destructors->last_destructor  = destructor;
        return 0;
    }

    destructor->next              = first_destructor;
    destructors->first_destructor = destructor;

    return 0;
}

extern inline uint8_t tralloc_prepend_destructor ( const tralloc_context * chunk_context, tralloc_destructor_function function, void * user_data );


void _tralloc_delete_destructors_by_comparator ( _tralloc_chunk * chunk, _tralloc_destructor_comparator comparator, tralloc_destructor_function function, void * user_data )
{
    _tralloc_destructors * destructors     = _tralloc_destructors_from_chunk ( chunk );
    _tralloc_destructor * first_destructor = destructors->first_destructor;
    if ( first_destructor == NULL ) {
        return;
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
}

extern inline bool    _tralloc_destructor_comparator_by_function ( _tralloc_destructor * destructor, tralloc_destructor_function function, void * user_data );
extern inline bool    _tralloc_destructor_comparator_by_data     ( _tralloc_destructor * destructor, tralloc_destructor_function function, void * user_data );
extern inline bool    _tralloc_destructor_comparator_strict      ( _tralloc_destructor * destructor, tralloc_destructor_function function, void * user_data );
extern inline uint8_t _tralloc_delete_destructors                ( const tralloc_context * chunk_context, _tralloc_destructor_comparator comparator, tralloc_destructor_function function, void * user_data );
extern inline uint8_t tralloc_delete_destructor                  ( const tralloc_context * chunk_context, tralloc_destructor_function function, void * user_data );
extern inline uint8_t tralloc_delete_destructor_by_function      ( const tralloc_context * chunk_context, tralloc_destructor_function function );
extern inline uint8_t tralloc_delete_destructor_by_data          ( const tralloc_context * chunk_context, void * user_data );
