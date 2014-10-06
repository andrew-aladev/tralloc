// This file is part of tralloc. Copyright (C) 2013 Andrew Aladjev aladjev.andrew@gmail.com
// tralloc is free software: you can redistribute it and/or modify it under the terms of the GNU General Lesser Public License as published by the Free Software Foundation, either version 3 of the License, or (at your option) any later version.
// tralloc is distributed in the hope that it will be useful, but WITHOUT ANY WARRANTY; without even the implied warranty of MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU General Lesser Public License for more details.
// You should have received a copy of the GNU General Lesser Public License along with tralloc. If not, see <http://www.gnu.org/licenses/>.

#include <tralloc/destructors/clear.h>
#include <tralloc/destructors/chunk.h>
#include <tralloc/common.h>

#if defined ( TRALLOC_DEBUG_STATS )
#   include <tralloc/debug/stats/main.h>
#endif

#include <stdlib.h>


tralloc_error tralloc_clear_destructors ( tralloc_context * context )
{
    if ( context == NULL ) {
        return TRALLOC_ERROR_REQUIRED_ARGUMENT_IS_NULL;
    }
    _tralloc_chunk * chunk = _tralloc_get_chunk_from_context ( context );

    if ( ! ( chunk->extensions & TRALLOC_EXTENSION_DESTRUCTORS ) ) {
        return TRALLOC_ERROR_NO_SUCH_EXTENSION;
    }

    _tralloc_destructors * destructors = _tralloc_get_destructors_from_chunk ( chunk );
    _tralloc_destructor * destructor   = destructors->first_destructor;
    if ( destructor == NULL ) {
        return 0;
    }

#   if defined ( TRALLOC_DEBUG_STATS )
    size_t destructors_count = 0;
#   endif

    _tralloc_destructor * next_destructor;
    while ( destructor != NULL ) {
        next_destructor = destructor->next;
        free ( destructor );
        destructor = next_destructor;

#       if defined ( TRALLOC_DEBUG_STATS )
        destructors_count ++;
#       endif
    }
    destructors->first_destructor = NULL;
    destructors->last_destructor  = NULL;

#   if defined ( TRALLOC_DEBUG_STATS )
    tralloc_error result = _tralloc_debug_stats_subtract_overhead_length ( sizeof ( _tralloc_destructor ) * destructors_count );
    if ( result != 0 ) {
        return result;
    }
#   endif

    return 0;
}
