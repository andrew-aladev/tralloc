// This file is part of tralloc. Copyright (C) 2013 Andrew Aladjev aladjev.andrew@gmail.com
// tralloc is free software: you can redistribute it and/or modify it under the terms of the GNU General Lesser Public License as published by the Free Software Foundation, either version 3 of the License, or (at your option) any later version.
// tralloc is distributed in the hope that it will be useful, but WITHOUT ANY WARRANTY; without even the implied warranty of MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU General Lesser Public License for more details.
// You should have received a copy of the GNU General Lesser Public License along with tralloc. If not, see <http://www.gnu.org/licenses/>.

#define _TRALLOC_INCLUDED_FROM_DESTRUCTORS_DESTRUCTORS_C
#include <tralloc/destructors/destructors.h>

#if defined ( TRALLOC_DEBUG_STATS )
#   include <tralloc/debug/stats/main.h>
#endif

#include <stdlib.h>


tralloc_error _tralloc_free_destructors ( _tralloc_destructors * destructors, tralloc_context * context )
{
    _tralloc_destructor * destructor = destructors->first_destructor;
    if ( destructor == NULL ) {
        return 0;
    }

#   if defined ( TRALLOC_DEBUG_STATS )
    size_t destructors_count = 0;
#   endif

    tralloc_error result, error = 0;
    _tralloc_destructor * next_destructor;
    tralloc_destructor_function function;

    while ( destructor != NULL ) {
        function = destructor->function;
        if ( function != NULL ) {
            if ( ( result = function ( context, destructor->user_data ) ) != 0 ) {
                error = result;
            }
        }

        next_destructor = destructor->next;
        free ( destructor );
        destructor = next_destructor;

#       if defined ( TRALLOC_DEBUG_STATS )
        destructors_count ++;
#       endif
    }

#   if defined ( TRALLOC_DEBUG_STATS )
    result = _tralloc_debug_stats_subtract_overhead_length ( sizeof ( _tralloc_destructor ) * destructors_count );
    if ( result != 0 ) {
        error = result;
    }
#   endif

    return error;
}
