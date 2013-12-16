// This file is part of tralloc. Copyright (C) 2013 Andrew Aladjev aladjev.andrew@gmail.com
// tralloc is free software: you can redistribute it and/or modify it under the terms of the GNU General Lesser Public License as published by the Free Software Foundation, either version 3 of the License, or (at your option) any later version.
// tralloc is distributed in the hope that it will be useful, but WITHOUT ANY WARRANTY; without even the implied warranty of MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU General Lesser Public License for more details.
// You should have received a copy of the GNU General Lesser Public License along with tralloc. If not, see <http://www.gnu.org/licenses/>.

#include "destructor.h"

#if defined(TRALLOC_REFERENCE)
#include "../reference/chunk.h"
#endif

static inline
void prepend_destructor_to_extensions ( tralloc_extensions * extensions, tralloc_destructor * destructor )
{
    tralloc_destructor * first_destructor = extensions->first_destructor;
    extensions->first_destructor          = destructor;
    destructor->next                      = first_destructor;
}

#if defined(TRALLOC_REFERENCE)
static inline
void prepend_destructor_to_reference ( tralloc_reference * reference, tralloc_destructor * destructor )
{
    tralloc_destructor * first_destructor = reference->first_destructor;
    reference->first_destructor           = destructor;
    destructor->next                      = first_destructor;
}
#endif

uint8_t tralloc_add_destructor ( const tralloc_context * chunk_context, tralloc_destructor_function function, void * user_data )
{
    if ( chunk_context == NULL ) {
        return 1;
    }

    tralloc_destructor * destructor = malloc ( sizeof ( tralloc_destructor ) );
    if ( destructor == NULL ) {
        return 1;
    }
    destructor->function  = function;
    destructor->user_data = user_data;
    destructor->next      = NULL;

    tralloc_chunk * chunk = tralloc_chunk_from_context ( chunk_context );

#if defined(TRALLOC_REFERENCE)
    if ( chunk->mode == TRALLOC_MODE_EXTENSIONS ) {
        prepend_destructor_to_extensions ( tralloc_extensions_from_chunk ( chunk ), destructor );
    } else {
        prepend_destructor_to_reference ( tralloc_reference_from_chunk ( chunk ), destructor );
    }
#else
    prepend_destructor_to_extensions ( tralloc_extensions_from_chunk ( chunk ), destructor );
#endif

    return 0;
}

static inline
bool destructor_comparator_by_function ( tralloc_destructor * destructor, tralloc_destructor_function function, void * user_data )
{
    return destructor->function == function;
}

static inline
bool destructor_comparator_by_data ( tralloc_destructor * destructor, tralloc_destructor_function function, void * user_data )
{
    return destructor->user_data == user_data;
}

static inline
bool destructor_comparator_strict ( tralloc_destructor * destructor, tralloc_destructor_function function, void * user_data )
{
    return destructor->function == function && destructor->user_data == user_data;
}

typedef bool ( * destructor_comparator ) ( tralloc_destructor * destructor, tralloc_destructor_function function, void * user_data );

static inline
void delete_destructors_by_comparator ( tralloc_extensions * extensions, destructor_comparator comparator, tralloc_destructor_function function, void * user_data )
{
    tralloc_destructor * destructor = extensions->first_destructor;
    tralloc_destructor * next_destructor;
    tralloc_destructor * prev_destructor = NULL;

    while ( destructor != NULL ) {
        next_destructor = destructor->next;
        if ( comparator ( destructor, function, user_data ) ) {
            free ( destructor );
            if ( prev_destructor == NULL ) {
                extensions->first_destructor = next_destructor;
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
uint8_t delete_destructors ( const tralloc_context * chunk_context, destructor_comparator comparator, tralloc_destructor_function function, void * user_data )
{
    if ( chunk_context == NULL ) {
        return 1;
    }
    tralloc_extensions * extensions = tralloc_extensions_from_chunk ( tralloc_chunk_from_context ( chunk_context ) );

    delete_destructors_by_comparator ( extensions, comparator, function, user_data );
    return 0;
}

uint8_t tralloc_del_destructor ( const tralloc_context * chunk_context, tralloc_destructor_function function, void * user_data )
{
    return delete_destructors ( chunk_context, destructor_comparator_strict, function, user_data );
}

uint8_t tralloc_del_destructor_by_function ( const tralloc_context * chunk_context, tralloc_destructor_function function )
{
    return delete_destructors ( chunk_context, destructor_comparator_by_function, function, NULL );
}

uint8_t tralloc_del_destructor_by_data ( const tralloc_context * chunk_context, void * user_data )
{
    return delete_destructors ( chunk_context, destructor_comparator_by_data, NULL, user_data );
}

uint8_t tralloc_destructor_free ( tralloc_chunk * chunk, tralloc_extensions * extensions )
{
    uint8_t result, error = 0;
    tralloc_context    * chunk_context = tralloc_context_from_chunk ( chunk );
    tralloc_destructor * destructor    = extensions->first_destructor;
    tralloc_destructor * next_destructor;
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

extern inline uint8_t tralloc_clear_destructors      ( const tralloc_context * chunk_context );
extern inline void    tralloc_destructor_free_silent ( tralloc_extensions    * extensions );
