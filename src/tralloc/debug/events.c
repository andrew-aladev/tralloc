// This file is part of tralloc. Copyright (C) 2013 Andrew Aladjev aladjev.andrew@gmail.com
// tralloc is free software: you can redistribute it and/or modify it under the terms of the GNU General Lesser Public License as published by the Free Software Foundation, either version 3 of the License, or (at your option) any later version.
// tralloc is distributed in the hope that it will be useful, but WITHOUT ANY WARRANTY; without even the implied warranty of MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU General Lesser Public License for more details.
// You should have received a copy of the GNU General Lesser Public License along with tralloc. If not, see <http://www.gnu.org/licenses/>.

#include <tralloc/debug/events.h>
#include <tralloc/macro.h>

#if defined ( TRALLOC_DEBUG_STATS )
#   include <tralloc/debug/stats/main.h>
#endif

#if defined ( TRALLOC_DEBUG_CALLBACKS )
#   include <tralloc/debug/callbacks/main.h>
#endif

#if defined ( TRALLOC_DEBUG_THREADS )
#   include <tralloc/debug/threads/main.h>
#endif

#if defined ( TRALLOC_DEBUG_LOG )
#   include <stdlib.h>
#   include <string.h>
#endif


tralloc_error _tralloc_debug_before_add_chunk ( _tralloc_chunk * _TRALLOC_UNUSED ( parent_chunk ), tralloc_extensions _TRALLOC_UNUSED ( extensions ), size_t _TRALLOC_UNUSED ( length ) )
{
    tralloc_error _TRALLOC_UNUSED ( result );

#   if defined ( TRALLOC_DEBUG_THREADS )
    result = _tralloc_debug_threads_before_add_chunk ( parent_chunk, extensions );
    if ( result != 0 ) {
        return result;
    }
#   endif

#   if defined ( TRALLOC_DEBUG_CALLBACKS )
    result = _tralloc_debug_callback_before_add_chunk ( parent_chunk, extensions, length );
    if ( result != 0 ) {
        return result;
    }
#   endif

    return 0;
}

#if defined ( TRALLOC_DEBUG_LOG )
tralloc_error _tralloc_debug_after_add_chunk ( _tralloc_chunk * chunk, size_t length, const char * file, size_t line )
#else
tralloc_error _tralloc_debug_after_add_chunk ( _tralloc_chunk * chunk, size_t length )
#endif
{
    tralloc_error _TRALLOC_UNUSED ( result );

    chunk->length = length;

#   if defined ( TRALLOC_DEBUG_LOG )
    chunk->initialized_in_file = strdup ( file );
    if ( chunk->initialized_in_file == NULL ) {
        return TRALLOC_ERROR_MALLOC_FAILED;
    }
    chunk->initialized_at_line = line;
#   endif

#   if defined ( TRALLOC_DEBUG_THREADS )
    result = _tralloc_debug_threads_after_add_chunk ( chunk );
    if ( result != 0 ) {

#       if defined ( TRALLOC_DEBUG_LOG )
        free ( chunk->initialized_in_file );
#       endif

        return result;
    }
#   endif

#   if defined ( TRALLOC_DEBUG_STATS )
    result = _tralloc_debug_stats_after_add_chunk ( chunk );
    if ( result != 0 ) {

#       if defined ( TRALLOC_DEBUG_LOG )
        free ( chunk->initialized_in_file );
#       endif

        return result;
    }
#   endif

#   if defined ( TRALLOC_DEBUG_CALLBACKS )
    result = _tralloc_debug_callback_after_add_chunk ( chunk );
    if ( result != 0 ) {

#       if defined ( TRALLOC_DEBUG_LOG )
        free ( chunk->initialized_in_file );
#       endif

        return result;
    }
#   endif

    return 0;
}

tralloc_error _tralloc_debug_before_resize_chunk ( _tralloc_chunk * _TRALLOC_UNUSED ( chunk ) )
{
    tralloc_error _TRALLOC_UNUSED ( result );

#   if defined ( TRALLOC_DEBUG_THREADS )
    result = _tralloc_debug_threads_before_resize_chunk ( chunk );
    if ( result != 0 ) {
        return result;
    }
#   endif

#   if defined ( TRALLOC_DEBUG_CALLBACKS )
    result = _tralloc_debug_callback_before_resize_chunk ( chunk );
    if ( result != 0 ) {
        return result;
    }
#   endif

    return 0;
}

tralloc_error _tralloc_debug_after_resize_chunk ( _tralloc_chunk * chunk, size_t old_length, size_t length )
{
    tralloc_error _TRALLOC_UNUSED ( result );

    chunk->length = length;

#   if defined ( TRALLOC_DEBUG_THREADS )
    result = _tralloc_debug_threads_after_resize_chunk ( chunk );
    if ( result != 0 ) {
        chunk->length = old_length;
        return result;
    }
#   endif

#   if defined ( TRALLOC_DEBUG_STATS )
    result = _tralloc_debug_stats_after_resize_chunk ( old_length, length );
    if ( result != 0 ) {
        chunk->length = old_length;
        return result;
    }
#   endif

#   if defined ( TRALLOC_DEBUG_CALLBACKS )
    result = _tralloc_debug_callback_after_resize_chunk ( chunk, old_length );
    if ( result != 0 ) {
        chunk->length = old_length;
        return result;
    }
#   endif

    return 0;
}

tralloc_error _tralloc_debug_before_move_chunk ( _tralloc_chunk * _TRALLOC_UNUSED ( chunk ) )
{
    tralloc_error _TRALLOC_UNUSED ( result );

#   if defined ( TRALLOC_DEBUG_THREADS )
    result = _tralloc_debug_threads_before_move_chunk ( chunk );
    if ( result != 0 ) {
        return result;
    }
#   endif

#   if defined ( TRALLOC_DEBUG_CALLBACKS )
    result = _tralloc_debug_callback_before_move_chunk ( chunk );
    if ( result != 0 ) {
        return result;
    }
#   endif

    return 0;
}

tralloc_error _tralloc_debug_after_move_chunk ( _tralloc_chunk * _TRALLOC_UNUSED ( chunk ), _tralloc_chunk * _TRALLOC_UNUSED ( old_parent_chunk ) )
{
    tralloc_error _TRALLOC_UNUSED ( result );

#   if defined ( TRALLOC_DEBUG_THREADS )
    result = _tralloc_debug_threads_after_move_chunk ( chunk );
    if ( result != 0 ) {
        return result;
    }
#   endif

#   if defined ( TRALLOC_DEBUG_CALLBACKS )
    result = _tralloc_debug_callback_after_move_chunk ( chunk, old_parent_chunk );
    if ( result != 0 ) {
        return result;
    }
#   endif

    return 0;
}

tralloc_error _tralloc_debug_before_free_subtree ( _tralloc_chunk * _TRALLOC_UNUSED ( chunk ) )
{
    tralloc_error error = 0, _TRALLOC_UNUSED ( result );

#   if defined ( TRALLOC_DEBUG_THREADS )
    result = _tralloc_debug_threads_before_free_subtree ( chunk );
    if ( result != 0 ) {
        error = result;
    }
#   endif

#   if defined ( TRALLOC_DEBUG_CALLBACKS )
    result = _tralloc_debug_callback_before_free_subtree ( chunk );
    if ( result != 0 ) {
        error = result;
    }
#   endif

    return error;
}

tralloc_error _tralloc_debug_before_free_chunk ( _tralloc_chunk * chunk )
{
    tralloc_error error = 0, _TRALLOC_UNUSED ( result );

#   if defined ( TRALLOC_DEBUG_THREADS )
    result = _tralloc_debug_threads_before_free_chunk ( chunk );
    if ( result != 0 ) {
        error = result;
    }
#   endif

#   if defined ( TRALLOC_DEBUG_STATS )
    result = _tralloc_debug_stats_before_free_chunk ( chunk );
    if ( result != 0 ) {
        error = result;
    }
#   endif

#   if defined ( TRALLOC_DEBUG_CALLBACKS )
    result = _tralloc_debug_callback_before_free_chunk ( chunk );
    if ( result != 0 ) {
        error = result;
    }
#   endif

#   if defined ( TRALLOC_DEBUG_LOG )
    if ( chunk->initialized_in_file != NULL ) {
        free ( chunk->initialized_in_file );
    }
#   endif

    return error;
}

tralloc_error _tralloc_debug_before_refuse_to_free_subtree ( _tralloc_chunk * _TRALLOC_UNUSED ( chunk ) )
{
    tralloc_error error = 0, _TRALLOC_UNUSED ( result );

#   if defined ( TRALLOC_DEBUG_THREADS )
    result = _tralloc_debug_threads_before_refuse_to_free_subtree ( chunk );
    if ( result != 0 ) {
        error = result;
    }
#   endif

#   if defined ( TRALLOC_DEBUG_CALLBACKS )
    result = _tralloc_debug_callback_before_refuse_to_free_subtree ( chunk );
    if ( result != 0 ) {
        error = result;
    }
#   endif

    return error;
}

tralloc_error _tralloc_debug_after_refuse_to_free_subtree ( _tralloc_chunk * _TRALLOC_UNUSED ( chunk ) )
{

#   if defined ( TRALLOC_DEBUG_CALLBACKS )
    return _tralloc_debug_callback_after_refuse_to_free_subtree ( chunk );
#   endif

    return 0;
}

tralloc_error _tralloc_debug_before_refuse_to_free_chunk ( _tralloc_chunk * _TRALLOC_UNUSED ( chunk ) )
{
    tralloc_error error = 0, _TRALLOC_UNUSED ( result );

#   if defined ( TRALLOC_DEBUG_THREADS )
    result = _tralloc_debug_threads_before_refuse_to_free_chunk ( chunk );
    if ( result != 0 ) {
        error = result;
    }
#   endif

#   if defined ( TRALLOC_DEBUG_CALLBACKS )
    result = _tralloc_debug_callback_before_refuse_to_free_chunk ( chunk );
    if ( result != 0 ) {
        error = result;
    }
#   endif

    return error;
}

tralloc_error _tralloc_debug_after_refuse_to_free_chunk ( _tralloc_chunk * _TRALLOC_UNUSED ( chunk ) )
{

#   if defined ( TRALLOC_DEBUG_CALLBACKS )
    return _tralloc_debug_callback_after_refuse_to_free_chunk ( chunk );
#   endif

    return 0;
}
