// This file is part of tralloc. Copyright (C) 2013 Andrew Aladjev aladjev.andrew@gmail.com
// tralloc is free software: you can redistribute it and/or modify it under the terms of the GNU General Lesser Public License as published by the Free Software Foundation, either version 3 of the License, or (at your option) any later version.
// tralloc is distributed in the hope that it will be useful, but WITHOUT ANY WARRANTY; without even the implied warranty of MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU General Lesser Public License for more details.
// You should have received a copy of the GNU General Lesser Public License along with tralloc. If not, see <http://www.gnu.org/licenses/>.

#include "destructor.h"

static inline
tralloc_destructor * _new_destructor ( tralloc_destructor_function function, void * user_data )
{
    tralloc_destructor * destructor = malloc ( sizeof ( tralloc_destructor ) );
    if ( destructor == NULL ) {
        return NULL;
    }
    destructor->function  = function;
    destructor->user_data = user_data;
    return destructor;
}

uint8_t tralloc_append_destructor ( const tralloc_context * chunk_context, tralloc_destructor_function function, void * user_data )
{
    if ( chunk_context == NULL ) {
        return 1;
    }
    tralloc_destructor * destructor = _new_destructor ( function, user_data );
    if ( destructor == NULL ) {
        return 2;
    }
    destructor->next = NULL;

    tralloc_chunk * chunk                  = _tralloc_chunk_from_context ( chunk_context );
    tralloc_destructors_list * destructors = chunk->destructors;
    if ( destructors == NULL ) {
        destructors = malloc ( sizeof ( tralloc_destructors_list ) );
        if ( destructors == NULL ) {
            free ( destructor );
            return 3;
        }
        destructors->first_destructor = destructor;
        destructors->last_destructor  = destructor;
        chunk->destructors            = destructors;
        return 0;
    }

    tralloc_destructor * last_destructor = destructors->last_destructor;
    last_destructor->next                = destructor;
    destructors->last_destructor         = destructor;

    return 0;
}

uint8_t tralloc_prepend_destructor ( const tralloc_context * chunk_context, tralloc_destructor_function function, void * user_data )
{
    if ( chunk_context == NULL ) {
        return 1;
    }
    tralloc_destructor * destructor = _new_destructor ( function, user_data );
    if ( destructor == NULL ) {
        return 2;
    }

    tralloc_chunk * chunk                  = _tralloc_chunk_from_context ( chunk_context );
    tralloc_destructors_list * destructors = chunk->destructors;
    if ( destructors == NULL ) {
        destructors = malloc ( sizeof ( tralloc_destructors_list ) );
        if ( destructors == NULL ) {
            free ( destructor );
            return 3;
        }
        destructor->next              = NULL;
        destructors->first_destructor = destructor;
        destructors->last_destructor  = destructor;
        chunk->destructors            = destructors;
        return 0;
    }
    
    tralloc_destructor * first_destructor = destructors->first_destructor;
    destructor->next                      = first_destructor;
    destructors->first_destructor         = destructor;

    return 0;
}

static inline
bool _destructor_comparator_by_function ( tralloc_destructor * destructor, tralloc_destructor_function function, void * user_data )
{
    return destructor->function == function;
}

static inline
bool _destructor_comparator_by_data ( tralloc_destructor * destructor, tralloc_destructor_function function, void * user_data )
{
    return destructor->user_data == user_data;
}

static inline
bool _destructor_comparator_strict ( tralloc_destructor * destructor, tralloc_destructor_function function, void * user_data )
{
    return destructor->function == function && destructor->user_data == user_data;
}

typedef bool ( * destructor_comparator ) ( tralloc_destructor * destructor, tralloc_destructor_function function, void * user_data );

static inline
void _delete_destructors_by_comparator ( tralloc_chunk * chunk, destructor_comparator comparator, tralloc_destructor_function function, void * user_data )
{
    tralloc_destructors_list * destructors = chunk->destructors;
    if ( destructors == NULL ) {
        return;
    }
    tralloc_destructor * first_destructor = destructors->first_destructor;
    tralloc_destructor * last_destructor  = NULL;
    tralloc_destructor * destructor       = first_destructor;
    tralloc_destructor * next_destructor;

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
    if ( last_destructor == NULL ) {
        free ( destructors );
        chunk->destructors = NULL;
        return;
    }
    if ( destructors->first_destructor != first_destructor ) {
        destructors->first_destructor = first_destructor;
    }
    if ( destructors->last_destructor != last_destructor ) {
        destructors->last_destructor = last_destructor;
    }
}

static inline
uint8_t _delete_destructors ( const tralloc_context * chunk_context, destructor_comparator comparator, tralloc_destructor_function function, void * user_data )
{
    if ( chunk_context == NULL ) {
        return 1;
    }
    tralloc_chunk * chunk = _tralloc_chunk_from_context ( chunk_context );

    _delete_destructors_by_comparator ( chunk, comparator, function, user_data );
    return 0;
}

uint8_t tralloc_del_destructor ( const tralloc_context * chunk_context, tralloc_destructor_function function, void * user_data )
{
    return _delete_destructors ( chunk_context, _destructor_comparator_strict, function, user_data );
}

uint8_t tralloc_del_destructor_by_function ( const tralloc_context * chunk_context, tralloc_destructor_function function )
{
    return _delete_destructors ( chunk_context, _destructor_comparator_by_function, function, NULL );
}

uint8_t tralloc_del_destructor_by_data ( const tralloc_context * chunk_context, void * user_data )
{
    return _delete_destructors ( chunk_context, _destructor_comparator_by_data, NULL, user_data );
}

extern inline uint8_t tralloc_clear_destructors       ( const tralloc_context * chunk_context );
extern inline void    _tralloc_destructor_free_silent ( tralloc_destructors_list * destructors );
extern inline uint8_t _tralloc_destructor_free        ( tralloc_chunk * chunk );
