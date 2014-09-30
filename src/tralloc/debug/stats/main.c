// This file is part of tralloc. Copyright (C) 2013 Andrew Aladjev aladjev.andrew@gmail.com
// tralloc is free software: you can redistribute it and/or modify it under the terms of the GNU General Lesser Public License as published by the Free Software Foundation, either version 3 of the License, or (at your option) any later version.
// tralloc is distributed in the hope that it will be useful, but WITHOUT ANY WARRANTY; without even the implied warranty of MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU General Lesser Public License for more details.
// You should have received a copy of the GNU General Lesser Public License along with tralloc. If not, see <http://www.gnu.org/licenses/>.

#include <tralloc/common.h>
#include <tralloc/debug/stats/main.h>

#if defined ( TRALLOC_THREADS )
#   include <tralloc/debug/stats/lock.h>
#endif


static size_t _tralloc_debug_stats_chunks_count = 0;

static inline
tralloc_error _tralloc_debug_stats_add_chunks_count ( size_t chunks_count )
{

#   if defined ( TRALLOC_THREADS )
    tralloc_error result = _tralloc_debug_stats_wrlock_chunks_count ();
    if ( result != 0 ) {
        return result;
    }
#   endif

    _tralloc_debug_stats_chunks_count += chunks_count;

#   if defined ( TRALLOC_THREADS )
    result = _tralloc_debug_stats_unlock_chunks_count ();
    if ( result != 0 ) {
        return result;
    }
#   endif

    return 0;
}

static inline
tralloc_error _tralloc_debug_stats_subtract_chunks_count ( size_t chunks_count )
{

#   if defined ( TRALLOC_THREADS )
    tralloc_error result = _tralloc_debug_stats_wrlock_chunks_count ();
    if ( result != 0 ) {
        return result;
    }
#   endif

    _tralloc_debug_stats_chunks_count -= chunks_count;

#   if defined ( TRALLOC_THREADS )
    result = _tralloc_debug_stats_unlock_chunks_count ();
    if ( result != 0 ) {
        return result;
    }
#   endif

    return 0;
}

tralloc_error tralloc_debug_stats_get_chunks_count ( size_t * chunks_count )
{

#   if defined ( TRALLOC_THREADS )
    tralloc_error result = _tralloc_debug_stats_rdlock_chunks_count ();
    if ( result != 0 ) {
        return result;
    }
#   endif

    * chunks_count = _tralloc_debug_stats_chunks_count;

#   if defined ( TRALLOC_THREADS )
    result = _tralloc_debug_stats_unlock_chunks_count ();
    if ( result != 0 ) {
        return result;
    }
#   endif

    return 0;
}


static size_t _tralloc_debug_stats_overhead_length = 0;

tralloc_error _tralloc_debug_stats_add_overhead_length ( size_t overhead_length )
{

#   if defined ( TRALLOC_THREADS )
    tralloc_error result = _tralloc_debug_stats_wrlock_overhead_length ();
    if ( result != 0 ) {
        return result;
    }
#   endif

    _tralloc_debug_stats_overhead_length += overhead_length;

#   if defined ( TRALLOC_THREADS )
    result = _tralloc_debug_stats_unlock_overhead_length ();
    if ( result != 0 ) {
        return result;
    }
#   endif

    return 0;
}

tralloc_error _tralloc_debug_stats_subtract_overhead_length ( size_t overhead_length )
{

#   if defined ( TRALLOC_THREADS )
    tralloc_error result = _tralloc_debug_stats_wrlock_overhead_length ();
    if ( result != 0 ) {
        return result;
    }
#   endif

    _tralloc_debug_stats_overhead_length -= overhead_length;

#   if defined ( TRALLOC_THREADS )
    result = _tralloc_debug_stats_unlock_overhead_length ();
    if ( result != 0 ) {
        return result;
    }
#   endif

    return 0;
}

tralloc_error tralloc_debug_stats_get_overhead_length ( size_t * overhead_length )
{

#   if defined ( TRALLOC_THREADS )
    tralloc_error result = _tralloc_debug_stats_rdlock_overhead_length ();
    if ( result != 0 ) {
        return result;
    }
#   endif

    * overhead_length = _tralloc_debug_stats_overhead_length;

#   if defined ( TRALLOC_THREADS )
    result = _tralloc_debug_stats_unlock_overhead_length ();
    if ( result != 0 ) {
        return result;
    }
#   endif

    return 0;
}


static size_t _tralloc_debug_stats_length = 0;

static inline
tralloc_error _tralloc_debug_stats_add_length ( size_t length )
{

#   if defined ( TRALLOC_THREADS )
    tralloc_error result = _tralloc_debug_stats_wrlock_length ();
    if ( result != 0 ) {
        return result;
    }
#   endif

    _tralloc_debug_stats_length += length;

#   if defined ( TRALLOC_THREADS )
    result = _tralloc_debug_stats_unlock_length ();
    if ( result != 0 ) {
        return result;
    }
#   endif

    return 0;
}

static inline
tralloc_error _tralloc_debug_stats_subtract_length ( size_t length )
{

#   if defined ( TRALLOC_THREADS )
    tralloc_error result = _tralloc_debug_stats_wrlock_length ();
    if ( result != 0 ) {
        return result;
    }
#   endif

    _tralloc_debug_stats_length -= length;

#   if defined ( TRALLOC_THREADS )
    result = _tralloc_debug_stats_unlock_length ();
    if ( result != 0 ) {
        return result;
    }
#   endif

    return 0;
}

tralloc_error tralloc_debug_stats_get_length ( size_t * length )
{

#   if defined ( TRALLOC_THREADS )
    tralloc_error result = _tralloc_debug_stats_rdlock_length ();
    if ( result != 0 ) {
        return result;
    }
#   endif

    * length = _tralloc_debug_stats_length;

#   if defined ( TRALLOC_THREADS )
    result = _tralloc_debug_stats_unlock_length ();
    if ( result != 0 ) {
        return result;
    }
#   endif

    return 0;
}

tralloc_error _tralloc_debug_stats_after_add_chunk ( _tralloc_chunk * chunk, size_t length )
{
    tralloc_error result;
    if (
        ( result = _tralloc_debug_stats_add_chunks_count ( 1 ) ) != 0 ||
        ( result = _tralloc_debug_stats_add_length ( length ) )  != 0 ||
        ( result = _tralloc_debug_stats_add_overhead_length ( tralloc_predict_chunk_length ( chunk->extensions ) ) ) != 0
    ) {
        return result;
    }
    return 0;
}

tralloc_error _tralloc_debug_stats_after_resize_chunk ( size_t old_length, size_t length )
{
    if ( length > old_length ) {
        return _tralloc_debug_stats_add_length ( length - old_length );
    } else if ( length < old_length ) {
        return _tralloc_debug_stats_subtract_length ( old_length - length );
    }
    return 0;
}

tralloc_error _tralloc_debug_stats_before_free_chunk ( _tralloc_chunk * chunk )
{
    tralloc_error error = 0, result;

    result = _tralloc_debug_stats_subtract_chunks_count ( 1 );
    if ( result != 0 ) {
        error = result;
    }

    result = _tralloc_debug_stats_subtract_length ( chunk->length );
    if ( result != 0 ) {
        error = result;
    }

    result = _tralloc_debug_stats_subtract_overhead_length ( tralloc_predict_chunk_length ( chunk->extensions ) );
    if ( result != 0 ) {
        error = result;
    }

    return error;
}
