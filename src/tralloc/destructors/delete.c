// This file is part of tralloc. Copyright (C) 2013 Andrew Aladjev aladjev.andrew@gmail.com
// tralloc is free software: you can redistribute it and/or modify it under the terms of the GNU General Lesser Public License as published by the Free Software Foundation, either version 3 of the License, or (at your option) any later version.
// tralloc is distributed in the hope that it will be useful, but WITHOUT ANY WARRANTY; without even the implied warranty of MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU General Lesser Public License for more details.
// You should have received a copy of the GNU General Lesser Public License along with tralloc. If not, see <http://www.gnu.org/licenses/>.

#define _TRALLOC_INCLUDED_FROM_DESTRUCTORS_DELETE_C
#include <tralloc/destructors/delete.h>
#include <tralloc/destructors/chunk.h>
#include <tralloc/common.h>

#if defined ( TRALLOC_DEBUG_STATS )
#   include <tralloc/debug/stats/main.h>
#endif

#include <stdlib.h>


tralloc_error _tralloc_delete_destructors_by_comparator ( tralloc_context * context, _tralloc_destructor_comparator comparator, tralloc_destructor_function function, void * user_data )
{
    if ( context == NULL ) {
        return TRALLOC_ERROR_REQUIRED_ARGUMENT_IS_UNDEFINED;
    }
    _tralloc_chunk * chunk = _tralloc_get_chunk_from_context ( context );

    if ( ! ( chunk->extensions & TRALLOC_EXTENSION_DESTRUCTORS ) ) {
        return TRALLOC_ERROR_NO_SUCH_EXTENSION;
    }

    _tralloc_destructors * destructors     = _tralloc_get_destructors_from_chunk ( chunk );
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
