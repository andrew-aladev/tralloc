// This file is part of tralloc. Copyright (C) 2013 Andrew Aladjev aladjev.andrew@gmail.com
// tralloc is free software: you can redistribute it and/or modify it under the terms of the GNU General Lesser Public License as published by the Free Software Foundation, either version 3 of the License, or (at your option) any later version.
// tralloc is distributed in the hope that it will be useful, but WITHOUT ANY WARRANTY; without even the implied warranty of MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU General Lesser Public License for more details.
// You should have received a copy of the GNU General Lesser Public License along with tralloc. If not, see <http://www.gnu.org/licenses/>.

#include "chunk.h"
#include "main.h"
#include "events.h"

#if defined(TRALLOC_THREADS)
#   include "threads/chunk.h"
#endif

#include <stdlib.h>
#include <string.h>


tralloc_error _tralloc_debug_new_chunk ( _tralloc_chunk * chunk, size_t chunk_length, size_t length, const char * file, size_t line )
{
    chunk->chunk_length = chunk_length;
    chunk->length       = length;

    chunk->initialized_in_file = strdup ( file );
    if ( chunk->initialized_in_file == NULL ) {
        return TRALLOC_ERROR_MALLOC_FAILED;
    }
    chunk->initialized_at_line = line;

#   if defined(TRALLOC_THREADS)
    tralloc_error result = _tralloc_debug_threads_new_chunk ( chunk );
    if ( result != 0 ) {
        return result;
    }
#   endif

    return _tralloc_debug_add_event ( chunk, chunk_length, length );
}

tralloc_error _tralloc_debug_resize_chunk ( _tralloc_chunk * chunk, size_t old_length, size_t length )
{
    tralloc_error result = _tralloc_debug_set_length ( chunk, length );
    if ( result != 0 ) {
        return result;
    }

#   if defined(TRALLOC_THREADS)
    result = _tralloc_debug_threads_resize_chunk ( chunk );
    if ( result != 0 ) {
        return result;
    }
#   endif

    return _tralloc_debug_resize_event ( chunk, old_length, length );
}

tralloc_error _tralloc_debug_move_chunk ( _tralloc_chunk * chunk, _tralloc_chunk * old_parent_chunk )
{

#   if defined(TRALLOC_THREADS)
    tralloc_error result = _tralloc_debug_threads_move_chunk ( chunk );
    if ( result != 0 ) {
        return result;
    }
#   endif

    return _tralloc_debug_move_event ( chunk, old_parent_chunk );
}

tralloc_error _tralloc_debug_free_chunk ( _tralloc_chunk * chunk )
{
    if ( chunk->initialized_in_file != NULL ) {
        free ( chunk->initialized_in_file );
    }

#   if defined(TRALLOC_THREADS)
    tralloc_error result = _tralloc_debug_threads_free_chunk ( chunk );
    if ( result != 0 ) {
        return result;
    }
#   endif

    return _tralloc_debug_free_event ( chunk );
}
