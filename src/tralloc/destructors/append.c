// This file is part of tralloc. Copyright (C) 2013 Andrew Aladjev aladjev.andrew@gmail.com
// tralloc is free software: you can redistribute it and/or modify it under the terms of the GNU General Lesser Public License as published by the Free Software Foundation, either version 3 of the License, or (at your option) any later version.
// tralloc is distributed in the hope that it will be useful, but WITHOUT ANY WARRANTY; without even the implied warranty of MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU General Lesser Public License for more details.
// You should have received a copy of the GNU General Lesser Public License along with tralloc. If not, see <http://www.gnu.org/licenses/>.

#include <tralloc/destructors/append.h>
#include <tralloc/destructors/chunk.h>
#include <tralloc/common.h>

#if defined ( TRALLOC_DEBUG )
#   include <tralloc/debug/stats/main.h>
#endif

#include <stdlib.h>


tralloc_error tralloc_append_destructor ( tralloc_context * context, tralloc_destructor_function function, void * user_data )
{
    if ( context == NULL ) {
        return TRALLOC_ERROR_REQUIRED_ARGUMENT_IS_NULL;
    }
    _tralloc_chunk * chunk = _tralloc_get_chunk_from_context ( context );

    if ( ! ( chunk->extensions & TRALLOC_EXTENSION_DESTRUCTORS ) ) {
        return TRALLOC_ERROR_NO_SUCH_EXTENSION;
    }

    _tralloc_destructor * destructor = malloc ( sizeof ( _tralloc_destructor ) );
    if ( destructor == NULL ) {
        return TRALLOC_ERROR_MALLOC_FAILED;
    }

#   if defined ( TRALLOC_DEBUG_STATS )
    tralloc_error result = _tralloc_debug_stats_add_chunks_overhead_length ( sizeof ( _tralloc_destructor ) );
    if ( result != 0 ) {
        free ( destructor );
        return result;
    }
#   endif

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

tralloc_error tralloc_prepend_destructor ( tralloc_context * context, tralloc_destructor_function function, void * user_data )
{
    if ( context == NULL ) {
        return TRALLOC_ERROR_REQUIRED_ARGUMENT_IS_NULL;
    }
    _tralloc_chunk * chunk = _tralloc_get_chunk_from_context ( context );

    if ( ! ( chunk->extensions & TRALLOC_EXTENSION_DESTRUCTORS ) ) {
        return TRALLOC_ERROR_NO_SUCH_EXTENSION;
    }

    _tralloc_destructor * destructor = malloc ( sizeof ( _tralloc_destructor ) );
    if ( destructor == NULL ) {
        return TRALLOC_ERROR_MALLOC_FAILED;
    }

#   if defined ( TRALLOC_DEBUG_STATS )
    tralloc_error result = _tralloc_debug_stats_add_chunks_overhead_length ( sizeof ( _tralloc_destructor ) );
    if ( result != 0 ) {
        free ( destructor );
        return result;
    }
#   endif

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
