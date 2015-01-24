// This file is part of tralloc. Copyright (C) 2013 Andrew Aladjev aladjev.andrew@gmail.com
// tralloc is free software: you can redistribute it and/or modify it under the terms of the GNU General Lesser Public License as published by the Free Software Foundation, either version 3 of the License, or (at your option) any later version.
// tralloc is distributed in the hope that it will be useful, but WITHOUT ANY WARRANTY; without even the implied warranty of MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU General Lesser Public License for more details.
// You should have received a copy of the GNU General Lesser Public License along with tralloc. If not, see <http://www.gnu.org/licenses/>.

#include <tralloc/destructors/data.h>
#include <tralloc/context.h>

#if defined ( TRALLOC_DEBUG_STATS )
#   include <tralloc/debug/stats/main.h>
#endif

#include <stdlib.h>


static
tralloc_bool _tralloc_destructor_comparator_by_function ( _tralloc_destructor * destructor, tralloc_destructor_function function, void * _TRALLOC_UNUSED ( user_data ) )
{
    return destructor->function == function;
}

static
tralloc_bool _tralloc_destructor_comparator_by_data ( _tralloc_destructor * destructor, tralloc_destructor_function _TRALLOC_UNUSED ( function ), void * user_data )
{
    return destructor->user_data == user_data;
}

static
tralloc_bool _tralloc_destructor_comparator_strict ( _tralloc_destructor * destructor, tralloc_destructor_function function, void * user_data )
{
    return destructor->function == function && destructor->user_data == user_data;
}

typedef tralloc_bool ( * _tralloc_destructor_comparator ) ( _tralloc_destructor * destructor, tralloc_destructor_function function, void * user_data );

static
tralloc_error _tralloc_destructors_delete_by_comparator ( tralloc_context * context, _tralloc_destructor_comparator comparator, tralloc_destructor_function function, void * user_data )
{
    if ( context == NULL ) {
        return TRALLOC_ERROR_REQUIRED_ARGUMENT_IS_UNDEFINED;
    }
    _tralloc_chunk * chunk = _tralloc_context_get_chunk ( context );

    if ( ! _tralloc_extensions_have_extension ( chunk->extensions, TRALLOC_EXTENSION_DESTRUCTORS ) ) {
        return TRALLOC_ERROR_NO_SUCH_EXTENSION;
    }

    _tralloc_destructors * destructors     = _tralloc_chunk_get_destructors ( chunk );
    _tralloc_destructor * first_destructor = destructors->first_destructor;
    if ( first_destructor == NULL ) {
        return 0;
    }

#   if defined ( TRALLOC_DEBUG_STATS )
    size_t destructors_count = 0;
#   endif

    _tralloc_destructor * last_destructor  = NULL;
    _tralloc_destructor * destructor       = first_destructor;
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

#           if defined ( TRALLOC_DEBUG_STATS )
            destructors_count ++;
#           endif
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

#   if defined ( TRALLOC_DEBUG_STATS )
    tralloc_error result = _tralloc_debug_stats_subtract_overhead_length ( sizeof ( _tralloc_destructor ) * destructors_count );
    if ( result != 0 ) {
        return result;
    }
#   endif

    return 0;
}

tralloc_error tralloc_destructors_delete ( tralloc_context * context, tralloc_destructor_function function, void * user_data )
{
    return _tralloc_destructors_delete_by_comparator ( context, _tralloc_destructor_comparator_strict, function, user_data );
}

tralloc_error tralloc_destructors_delete_by_function ( tralloc_context * context, tralloc_destructor_function function )
{
    return _tralloc_destructors_delete_by_comparator ( context, _tralloc_destructor_comparator_by_function, function, NULL );
}

tralloc_error tralloc_destructors_delete_by_data ( tralloc_context * context, void * user_data )
{
    return _tralloc_destructors_delete_by_comparator ( context, _tralloc_destructor_comparator_by_data, NULL, user_data );
}
