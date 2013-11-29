// This file is part of talloc2. Copyright (C) 2013 Andrew Aladjev aladjev.andrew@gmail.com
// talloc2 is free software: you can redistribute it and/or modify it under the terms of the GNU General Lesser Public License as published by the Free Software Foundation, either version 3 of the License, or (at your option) any later version.
// talloc2 is distributed in the hope that it will be useful, but WITHOUT ANY WARRANTY; without even the implied warranty of MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU General Lesser Public License for more details.
// You should have received a copy of the GNU General Lesser Public License along with talloc2. If not, see <http://www.gnu.org/licenses/>.

#include "../data.h"
#include "chunk.h"
#include "destructor.h"
#include <stdbool.h>

static inline
bool talloc_destructor_append ( talloc_ext * ext, talloc_destructor_function function, void * user_data )
{
    talloc_destructor * destructor = malloc ( sizeof ( talloc_destructor ) );
    if ( destructor == NULL ) {
        return false;
    }
    destructor->function  = function;
    destructor->user_data = user_data;
    destructor->next      = NULL;

    talloc_destructor * first_destructor = ext->first_destructor;
    ext->first_destructor = destructor;
    destructor->next      = first_destructor;

    return true;
}

uint8_t talloc_add_destructor ( const void * chunk_data, talloc_destructor_function function, void * user_data )
{
    if ( chunk_data == NULL ) {
        return 1;
    }
    talloc_ext * ext = talloc_ext_from_chunk ( talloc_chunk_from_data ( chunk_data ) );

    if ( !talloc_destructor_append ( ext, function, user_data ) ) {
        return 2;
    }

    return 0;
}

static inline
bool destructor_comparator_by_function ( talloc_destructor * destructor, talloc_destructor_function function, void * user_data )
{
    return destructor->function == function;
}

static inline
bool destructor_comparator_by_data ( talloc_destructor * destructor, talloc_destructor_function function, void * user_data )
{
    return destructor->user_data == user_data;
}

static inline
bool destructor_comparator_strict ( talloc_destructor * destructor, talloc_destructor_function function, void * user_data )
{
    return destructor->function == function && destructor->user_data == user_data;
}

typedef bool ( * destructor_comparator ) ( talloc_destructor * destructor, talloc_destructor_function function, void * user_data );

static inline
void delete_destructors_by_comparator ( talloc_ext * ext, destructor_comparator comparator, talloc_destructor_function function, void * user_data )
{
    talloc_destructor * destructor = ext->first_destructor;
    talloc_destructor * next_destructor;
    talloc_destructor * prev_destructor = NULL;

    while ( destructor != NULL ) {
        next_destructor = destructor->next;
        if ( comparator ( destructor, function, user_data ) ) {
            free ( destructor );
            if ( prev_destructor == NULL ) {
                ext->first_destructor = next_destructor;
            } else {
                prev_destructor->next = next_destructor;
            }
        } else {
            prev_destructor = destructor;
        }
        destructor = next_destructor;
    }
}

static inline
uint8_t delete_destructors ( const void * chunk_data, destructor_comparator comparator, talloc_destructor_function function, void * user_data )
{
    if ( chunk_data == NULL ) {
        return 1;
    }
    talloc_ext * ext = talloc_ext_from_chunk ( talloc_chunk_from_data ( chunk_data ) );

    delete_destructors_by_comparator ( ext, comparator, function, user_data );
    return 0;
}

uint8_t talloc_del_destructor ( const void * chunk_data, talloc_destructor_function function, void * user_data )
{
    return delete_destructors ( chunk_data, destructor_comparator_strict, function, user_data );
}

uint8_t talloc_del_destructor_by_function ( const void * chunk_data, talloc_destructor_function function )
{
    return delete_destructors ( chunk_data, destructor_comparator_by_function, function, NULL );
}

uint8_t talloc_del_destructor_by_data ( const void * chunk_data, void * user_data )
{
    return delete_destructors ( chunk_data, destructor_comparator_by_data, NULL, user_data );
}

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

static inline
uint8_t _destructor_run ( void * chunk_data, talloc_destructor * destructor )
{
    talloc_destructor_function function = destructor->function;
    if ( function == NULL ) {
        return 0;
    }
    return function ( chunk_data, destructor->user_data );
}

uint8_t talloc_destructor_free ( talloc_chunk * chunk, talloc_ext * ext )
{
    void * chunk_data = talloc_data_from_chunk ( chunk );
    talloc_destructor * destructor = ext->first_destructor;
    talloc_destructor * next_destructor;

    uint8_t result, error = 0;
    while ( destructor != NULL ) {
        if ( ( result = _destructor_run ( chunk_data, destructor ) ) != 0 ) {
            error = result;
        }

        next_destructor = destructor->next;
        free ( destructor );
        destructor = next_destructor;
    }
    return error;
}

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
