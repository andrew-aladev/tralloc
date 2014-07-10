// This file is part of tralloc. Copyright (C) 2013 Andrew Aladjev aladjev.andrew@gmail.com
// tralloc is free software: you can redistribute it and/or modify it under the terms of the GNU General Lesser Public License as published by the Free Software Foundation, either version 3 of the License, or (at your option) any later version.
// tralloc is distributed in the hope that it will be useful, but WITHOUT ANY WARRANTY; without even the implied warranty of MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU General Lesser Public License for more details.
// You should have received a copy of the GNU General Lesser Public License along with tralloc. If not, see <http://www.gnu.org/licenses/>.

#include <tralloc/debug/chunk.h>

#if defined ( TRALLOC_THREADS )
#   include <tralloc/threads/spinlock.h>
#endif


tralloc_error _tralloc_debug_get_length ( _tralloc_chunk * chunk, size_t * length )
{

#   if defined ( TRALLOC_THREADS )
    tralloc_error result = _tralloc_spinlock_lock ( &chunk->length_lock );
    if ( result != 0 ) {
        return result;
    }
#   endif

    * length = chunk->length;

#   if defined ( TRALLOC_THREADS )
    result = _tralloc_spinlock_unlock ( &chunk->length_lock );
    if ( result != 0 ) {
        return result;
    }
#   endif

    return 0;
}

tralloc_error _tralloc_debug_set_length ( _tralloc_chunk * chunk, size_t length )
{

#   if defined ( TRALLOC_THREADS )
    tralloc_error result = _tralloc_spinlock_lock ( &chunk->length_lock );
    if ( result != 0 ) {
        return result;
    }
#   endif

    chunk->length = length;

#   if defined ( TRALLOC_THREADS )
    result = _tralloc_spinlock_unlock ( &chunk->length_lock );
    if ( result != 0 ) {
        return result;
    }
#   endif

    return 0;
}
