// This file is part of tralloc. Copyright (C) 2013 Andrew Aladjev aladjev.andrew@gmail.com
// tralloc is free software: you can redistribute it and/or modify it under the terms of the GNU General Lesser Public License as published by the Free Software Foundation, either version 3 of the License, or (at your option) any later version.
// tralloc is distributed in the hope that it will be useful, but WITHOUT ANY WARRANTY; without even the implied warranty of MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU General Lesser Public License for more details.
// You should have received a copy of the GNU General Lesser Public License along with tralloc. If not, see <http://www.gnu.org/licenses/>.

#include "events.h"
#include "chunk.h"

#if defined(TRALLOC_DEBUG_STATS)
#   include "stats.h"
#endif

#if defined(TRALLOC_DEBUG_CALLBACKS)
#   include "callbacks.h"
#endif

#if defined(TRALLOC_DEBUG_THREADS)
#   include "threads.h"
#endif

#include <stdlib.h>
#include <string.h>


tralloc_error _tralloc_debug_before_add_chunk ( _tralloc_chunk * parent_chunk, tralloc_extensions extensions, size_t chunk_length, size_t length )
{
    tralloc_error result;

#   if defined(TRALLOC_DEBUG_THREADS)
    result = _tralloc_debug_threads_before_add_chunk ( parent_chunk, extensions );
    if ( result != 0 ) {
        return result;
    }
#   endif

#   if defined(TRALLOC_DEBUG_CALLBACKS)
    result = _tralloc_debug_callback_before_add_chunk ( parent_chunk, extensions, chunk_length, length );
    if ( result != 0 ) {
        return result;
    }
#   endif

    return 0;
}

tralloc_error _tralloc_debug_after_add_chunk ( _tralloc_chunk * chunk, size_t chunk_length, size_t length, const char * file, size_t line )
{
    chunk->chunk_length = chunk_length;
    chunk->length       = length;

#   if defined(TRALLOC_THREADS)
    if ( pthread_spin_init ( &chunk->length_lock, 0 ) != 0 ) {
        return TRALLOC_ERROR_SPINLOCK_FAILED;
    }
#   endif

#   if defined(TRALLOC_DEBUG_LOG)
    chunk->initialized_in_file = strdup ( file );
    if ( chunk->initialized_in_file == NULL ) {
        return TRALLOC_ERROR_MALLOC_FAILED;
    }
    chunk->initialized_at_line = line;
#   endif

    tralloc_error result;

#   if defined(TRALLOC_DEBUG_THREADS)
    result = _tralloc_debug_threads_after_add_chunk ( chunk );
    if ( result != 0 ) {

#       if defined(TRALLOC_DEBUG_LOG)
        free ( chunk->initialized_in_file );
#       endif

        return result;
    }
#   endif

#   if defined(TRALLOC_DEBUG_STATS)
    result = _tralloc_debug_stats_after_add_chunk ( chunk_length, length );
    if ( result != 0 ) {

#       if defined(TRALLOC_DEBUG_LOG)
        free ( chunk->initialized_in_file );
#       endif

        return result;
    }
#   endif

#   if defined(TRALLOC_DEBUG_CALLBACKS)
    result = _tralloc_debug_callback_after_add_chunk ( chunk, chunk_length, length );
    if ( result != 0 ) {

#       if defined(TRALLOC_DEBUG_LOG)
        free ( chunk->initialized_in_file );
#       endif

        return result;
    }
#   endif

    return 0;
}

tralloc_error _tralloc_debug_before_resize_chunk ( _tralloc_chunk * chunk )
{
    tralloc_error result;

#   if defined(TRALLOC_DEBUG_THREADS)
    result = _tralloc_debug_threads_before_resize_chunk ( chunk );
    if ( result != 0 ) {
        return result;
    }
#   endif

#   if defined(TRALLOC_DEBUG_CALLBACKS)
    result = _tralloc_debug_callback_before_resize_chunk ( chunk );
    if ( result != 0 ) {
        return result;
    }
#   endif

    return 0;
}

tralloc_error _tralloc_debug_after_resize_chunk ( _tralloc_chunk * chunk, size_t old_length, size_t length )
{
    tralloc_error result = _tralloc_debug_set_length ( chunk, length );
    if ( result != 0 ) {
        return result;
    }

#   if defined(TRALLOC_DEBUG_THREADS)
    result = _tralloc_debug_threads_after_resize_chunk ( chunk );
    if ( result != 0 ) {
        _tralloc_debug_set_length ( chunk, old_length );
        return result;
    }
#   endif

#   if defined(TRALLOC_DEBUG_STATS)
    result = _tralloc_debug_stats_after_resize_chunk ( old_length, length );
    if ( result != 0 ) {
        _tralloc_debug_set_length ( chunk, old_length );
        return result;
    }
#   endif

#   if defined(TRALLOC_DEBUG_CALLBACKS)
    result = _tralloc_debug_callback_after_resize_chunk ( chunk, old_length );
    if ( result != 0 ) {
        _tralloc_debug_set_length ( chunk, old_length );
        return result;
    }
#   endif

    return 0;
}

tralloc_error _tralloc_debug_before_move_chunk ( _tralloc_chunk * chunk )
{
    tralloc_error result;

#   if defined(TRALLOC_DEBUG_THREADS)
    result = _tralloc_debug_threads_before_move_chunk ( chunk );
    if ( result != 0 ) {
        return result;
    }
#   endif

#   if defined(TRALLOC_DEBUG_CALLBACKS)
    result = _tralloc_debug_callback_before_move_chunk ( chunk );
    if ( result != 0 ) {
        return result;
    }
#   endif

    return 0;
}

tralloc_error _tralloc_debug_after_move_chunk ( _tralloc_chunk * chunk, _tralloc_chunk * old_parent_chunk )
{
    tralloc_error result;

#   if defined(TRALLOC_DEBUG_THREADS)
    result = _tralloc_debug_threads_after_move_chunk ( chunk );
    if ( result != 0 ) {
        return result;
    }
#   endif

#   if defined(TRALLOC_DEBUG_CALLBACKS)
    result = _tralloc_debug_callback_after_move_chunk ( chunk, old_parent_chunk );
    if ( result != 0 ) {
        return result;
    }
#   endif

    return 0;
}

tralloc_error _tralloc_debug_before_free_subtree ( _tralloc_chunk * chunk )
{
    tralloc_error result, error = 0;

#   if defined(TRALLOC_DEBUG_THREADS)
    result = _tralloc_debug_threads_before_free_subtree ( chunk );
    if ( result != 0 ) {
        error = result;
    }
#   endif

#   if defined(TRALLOC_DEBUG_CALLBACKS)
    result = _tralloc_debug_callback_before_free_subtree ( chunk );
    if ( result != 0 ) {
        error = result;
    }
#   endif

    return error;
}

tralloc_error _tralloc_debug_before_free_chunk ( _tralloc_chunk * chunk )
{

#   if defined(TRALLOC_THREADS)
    if ( pthread_spin_destroy ( &chunk->length_lock ) != 0 ) {
        return TRALLOC_ERROR_SPINLOCK_FAILED;
    }
#   endif

#   if defined(TRALLOC_DEBUG_LOG)
    if ( chunk->initialized_in_file != NULL ) {
        free ( chunk->initialized_in_file );
    }
#   endif

    tralloc_error result, error = 0;

#   if defined(TRALLOC_DEBUG_THREADS)
    result = _tralloc_debug_threads_before_free_chunk ( chunk );
    if ( result != 0 ) {
        error = result;
    }
#   endif

#   if defined(TRALLOC_DEBUG_STATS)
    result = _tralloc_debug_stats_before_free_chunk ( chunk );
    if ( result != 0 ) {
        error = result;
    }
#   endif

#   if defined(TRALLOC_DEBUG_CALLBACKS)
    result = _tralloc_debug_callback_before_free_chunk ( chunk );
    if ( result != 0 ) {
        error = result;
    }
#   endif

    return error;
}

tralloc_error _tralloc_debug_before_refuse_to_free_subtree ( _tralloc_chunk * chunk )
{
    tralloc_error result, error = 0;

#   if defined(TRALLOC_DEBUG_THREADS)
    result = _tralloc_debug_threads_before_refuse_to_free_subtree ( chunk );
    if ( result != 0 ) {
        error = result;
    }
#   endif

#   if defined(TRALLOC_DEBUG_CALLBACKS)
    result = _tralloc_debug_callback_before_refuse_to_free_subtree ( chunk );
    if ( result != 0 ) {
        error = result;
    }
#   endif

    return error;
}

tralloc_error _tralloc_debug_after_refuse_to_free_subtree ( _tralloc_chunk * chunk )
{

#   if defined(TRALLOC_DEBUG_CALLBACKS)
    return _tralloc_debug_callback_after_refuse_to_free_subtree ( chunk );
#   endif

    return 0;
}

tralloc_error _tralloc_debug_before_refuse_to_free_chunk ( _tralloc_chunk * chunk )
{
    tralloc_error result, error = 0;

#   if defined(TRALLOC_DEBUG_THREADS)
    result = _tralloc_debug_threads_before_refuse_to_free_chunk ( chunk );
    if ( result != 0 ) {
        error = result;
    }
#   endif

#   if defined(TRALLOC_DEBUG_CALLBACKS)
    result = _tralloc_debug_callback_before_refuse_to_free_chunk ( chunk );
    if ( result != 0 ) {
        error = result;
    }
#   endif

    return error;
}

tralloc_error _tralloc_debug_after_refuse_to_free_chunk ( _tralloc_chunk * chunk )
{

#   if defined(TRALLOC_DEBUG_CALLBACKS)
    return _tralloc_debug_callback_after_refuse_to_free_chunk ( chunk );
#   endif

    return 0;
}
