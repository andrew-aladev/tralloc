// This file is part of tralloc. Copyright (C) 2013 Andrew Aladjev aladjev.andrew@gmail.com
// tralloc is free software: you can redistribute it and/or modify it under the terms of the GNU General Lesser Public License as published by the Free Software Foundation, either version 3 of the License, or (at your option) any later version.
// tralloc is distributed in the hope that it will be useful, but WITHOUT ANY WARRANTY; without even the implied warranty of MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU General Lesser Public License for more details.
// You should have received a copy of the GNU General Lesser Public License along with tralloc. If not, see <http://www.gnu.org/licenses/>.

#include "main.h"
#include "chunk.h"


#if defined(TRALLOC_THREADS)
static pthread_mutex_t _user_data_mutex = PTHREAD_MUTEX_INITIALIZER;
#endif

static void * _user_data;

tralloc_error _tralloc_debug_set_user_data ( void * user_data )
{

#   if defined(TRALLOC_THREADS)
    if ( pthread_mutex_lock ( &_user_data_mutex ) != 0 ) {
        return TRALLOC_ERROR_MUTEX_FAILED;
    }
#   endif

    _user_data = user_data;

#   if defined(TRALLOC_THREADS)
    if ( pthread_mutex_unlock ( &_user_data_mutex ) != 0 ) {
        return TRALLOC_ERROR_MUTEX_FAILED;
    }
#   endif

    return 0;
}


#if defined(TRALLOC_THREADS)
static pthread_mutex_t _before_add_chunk_mutex = PTHREAD_MUTEX_INITIALIZER;
static pthread_mutex_t _after_add_chunk_mutex  = PTHREAD_MUTEX_INITIALIZER;

static pthread_mutex_t _before_resize_chunk_mutex = PTHREAD_MUTEX_INITIALIZER;
static pthread_mutex_t _after_resize_chunk_mutex  = PTHREAD_MUTEX_INITIALIZER;

static pthread_mutex_t _before_move_chunk_mutex = PTHREAD_MUTEX_INITIALIZER;
static pthread_mutex_t _after_move_chunk_mutex  = PTHREAD_MUTEX_INITIALIZER;

static pthread_mutex_t _before_free_chunk_mutex = PTHREAD_MUTEX_INITIALIZER;
#endif

static _tralloc_debug_callback_before_add_chunk _before_add_chunk;
static _tralloc_debug_callback_after_add_chunk  _after_add_chunk;

static _tralloc_debug_callback_before_resize_chunk _before_resize_chunk;
static _tralloc_debug_callback_after_resize_chunk  _after_resize_chunk;

static _tralloc_debug_callback_before_move_chunk _before_move_chunk;
static _tralloc_debug_callback_after_move_chunk  _after_move_chunk;

static _tralloc_debug_callback_before_free_chunk _before_free_chunk;

tralloc_error _tralloc_debug_set_callbacks (
    _tralloc_debug_callback_before_add_chunk before_add_chunk,
    _tralloc_debug_callback_after_add_chunk  after_add_chunk,

    _tralloc_debug_callback_before_resize_chunk before_resize_chunk,
    _tralloc_debug_callback_after_resize_chunk  after_resize_chunk,

    _tralloc_debug_callback_before_move_chunk before_move_chunk,
    _tralloc_debug_callback_after_move_chunk  after_move_chunk,

    _tralloc_debug_callback_before_free_chunk before_free_chunk
)
{

#   if defined(TRALLOC_THREADS)
    if (
        pthread_mutex_lock ( &_before_add_chunk_mutex ) != 0 ||
        pthread_mutex_lock ( &_after_add_chunk_mutex )  != 0
    ) {
        return TRALLOC_ERROR_MUTEX_FAILED;
    }
#   endif

    _before_add_chunk = before_add_chunk;
    _after_add_chunk  = after_add_chunk;

#   if defined(TRALLOC_THREADS)
    if (
        pthread_mutex_unlock ( &_before_add_chunk_mutex )    != 0 ||
        pthread_mutex_unlock ( &_after_add_chunk_mutex )     != 0 ||
        pthread_mutex_lock   ( &_before_resize_chunk_mutex ) != 0 ||
        pthread_mutex_lock   ( &_after_resize_chunk_mutex )  != 0
    ) {
        return TRALLOC_ERROR_MUTEX_FAILED;
    }
#   endif

    _before_resize_chunk = before_resize_chunk;
    _after_resize_chunk  = after_resize_chunk;

#   if defined(TRALLOC_THREADS)
    if (
        pthread_mutex_unlock ( &_before_resize_chunk_mutex ) != 0 ||
        pthread_mutex_unlock ( &_after_resize_chunk_mutex )  != 0 ||
        pthread_mutex_lock   ( &_before_move_chunk_mutex )   != 0 ||
        pthread_mutex_lock   ( &_after_move_chunk_mutex )    != 0
    ) {
        return TRALLOC_ERROR_MUTEX_FAILED;
    }
#   endif

    _before_move_chunk = before_move_chunk;
    _after_move_chunk  = after_move_chunk;

#   if defined(TRALLOC_THREADS)
    if (
        pthread_mutex_unlock ( &_before_move_chunk_mutex ) != 0 ||
        pthread_mutex_unlock ( &_after_move_chunk_mutex )  != 0 ||
        pthread_mutex_lock   ( &_before_free_chunk_mutex ) != 0
    ) {
        return TRALLOC_ERROR_MUTEX_FAILED;
    }
#   endif

    _before_free_chunk = before_free_chunk;

#   if defined(TRALLOC_THREADS)
    if ( pthread_mutex_unlock ( &_before_free_chunk_mutex ) != 0 ) {
        return TRALLOC_ERROR_MUTEX_FAILED;
    }
#   endif

    return 0;
}


#if defined(TRALLOC_THREADS)
static pthread_mutex_t _chunks_count_mutex = PTHREAD_MUTEX_INITIALIZER;
#endif

static size_t _chunks_count = 0;

static inline
tralloc_error _add_chunks_count ( size_t length )
{

#   if defined(TRALLOC_THREADS)
    if ( pthread_mutex_lock ( &_chunks_count_mutex ) != 0 ) {
        return TRALLOC_ERROR_MUTEX_FAILED;
    }
#   endif

    _chunks_count += length;

#   if defined(TRALLOC_THREADS)
    if ( pthread_mutex_unlock ( &_chunks_count_mutex ) != 0 ) {
        return TRALLOC_ERROR_MUTEX_FAILED;
    }
#   endif

    return 0;
}

static inline
tralloc_error _subtract_chunks_count ( size_t length )
{

#   if defined(TRALLOC_THREADS)
    if ( pthread_mutex_lock ( &_chunks_count_mutex ) != 0 ) {
        return TRALLOC_ERROR_MUTEX_FAILED;
    }
#   endif

    _chunks_count -= length;

#   if defined(TRALLOC_THREADS)
    if ( pthread_mutex_unlock ( &_chunks_count_mutex ) != 0 ) {
        return TRALLOC_ERROR_MUTEX_FAILED;
    }
#   endif

    return 0;
}

tralloc_error tralloc_debug_get_chunks_count ( size_t * length )
{

#   if defined(TRALLOC_THREADS)
    if ( pthread_mutex_lock ( &_chunks_count_mutex ) != 0 ) {
        return TRALLOC_ERROR_MUTEX_FAILED;
    }
#   endif

    * length = _chunks_count;

#   if defined(TRALLOC_THREADS)
    if ( pthread_mutex_unlock ( &_chunks_count_mutex ) != 0 ) {
        return TRALLOC_ERROR_MUTEX_FAILED;
    }
#   endif

    return 0;
}


#if defined(TRALLOC_THREADS)
static pthread_mutex_t _chunks_overhead_length_mutex = PTHREAD_MUTEX_INITIALIZER;
#endif

static size_t _chunks_overhead_length = 0;

tralloc_error _tralloc_debug_add_chunks_overhead_length ( size_t length )
{

#   if defined(TRALLOC_THREADS)
    if ( pthread_mutex_lock ( &_chunks_overhead_length_mutex ) != 0 ) {
        return TRALLOC_ERROR_MUTEX_FAILED;
    }
#   endif

    _chunks_overhead_length += length;

#   if defined(TRALLOC_THREADS)
    if ( pthread_mutex_unlock ( &_chunks_overhead_length_mutex ) != 0 ) {
        return TRALLOC_ERROR_MUTEX_FAILED;
    }
#   endif

    return 0;
}

tralloc_error _tralloc_debug_subtract_chunks_overhead_length ( size_t length )
{

#   if defined(TRALLOC_THREADS)
    if ( pthread_mutex_lock ( &_chunks_overhead_length_mutex ) != 0 ) {
        return TRALLOC_ERROR_MUTEX_FAILED;
    }
#   endif

    _chunks_overhead_length -= length;

#   if defined(TRALLOC_THREADS)
    if ( pthread_mutex_unlock ( &_chunks_overhead_length_mutex ) != 0 ) {
        return TRALLOC_ERROR_MUTEX_FAILED;
    }
#   endif

    return 0;
}

tralloc_error tralloc_debug_get_chunks_overhead_length ( size_t * length )
{

#   if defined(TRALLOC_THREADS)
    if ( pthread_mutex_lock ( &_chunks_overhead_length_mutex ) != 0 ) {
        return TRALLOC_ERROR_MUTEX_FAILED;
    }
#   endif

    * length = _chunks_overhead_length;

#   if defined(TRALLOC_THREADS)
    if ( pthread_mutex_unlock ( &_chunks_overhead_length_mutex ) != 0 ) {
        return TRALLOC_ERROR_MUTEX_FAILED;
    }
#   endif

    return 0;
}


#if defined(TRALLOC_THREADS)
static pthread_mutex_t _chunks_length_mutex = PTHREAD_MUTEX_INITIALIZER;
#endif

static size_t _chunks_length = 0;

static inline
tralloc_error _add_chunks_length ( size_t length )
{

#   if defined(TRALLOC_THREADS)
    if ( pthread_mutex_lock ( &_chunks_length_mutex ) != 0 ) {
        return TRALLOC_ERROR_MUTEX_FAILED;
    }
#   endif

    _chunks_length += length;

#   if defined(TRALLOC_THREADS)
    if ( pthread_mutex_unlock ( &_chunks_length_mutex ) != 0 ) {
        return TRALLOC_ERROR_MUTEX_FAILED;
    }
#   endif

    return 0;
}

static inline
tralloc_error _subtract_chunks_length ( size_t length )
{

#   if defined(TRALLOC_THREADS)
    if ( pthread_mutex_lock ( &_chunks_length_mutex ) != 0 ) {
        return TRALLOC_ERROR_MUTEX_FAILED;
    }
#   endif

    _chunks_length -= length;

#   if defined(TRALLOC_THREADS)
    if ( pthread_mutex_unlock ( &_chunks_length_mutex ) != 0 ) {
        return TRALLOC_ERROR_MUTEX_FAILED;
    }
#   endif

    return 0;
}

tralloc_error tralloc_debug_get_chunks_length ( size_t * length )
{

#   if defined(TRALLOC_THREADS)
    if ( pthread_mutex_lock ( &_chunks_length_mutex ) != 0 ) {
        return TRALLOC_ERROR_MUTEX_FAILED;
    }
#   endif

    * length = _chunks_length;

#   if defined(TRALLOC_THREADS)
    if ( pthread_mutex_unlock ( &_chunks_length_mutex ) != 0 ) {
        return TRALLOC_ERROR_MUTEX_FAILED;
    }
#   endif

    return 0;
}


tralloc_error _tralloc_debug_event_before_add_chunk ( _tralloc_chunk * parent_chunk, tralloc_extensions extensions, size_t chunk_length, size_t length )
{

#   if defined(TRALLOC_THREADS)
    if ( pthread_mutex_lock ( &_before_add_chunk_mutex ) != 0 ) {
        return TRALLOC_ERROR_MUTEX_FAILED;
    }
#   endif

    if ( _before_add_chunk != NULL ) {

#       if defined(TRALLOC_THREADS)
        if ( pthread_mutex_lock ( &_user_data_mutex ) != 0 ) {
            return TRALLOC_ERROR_MUTEX_FAILED;
        }
#       endif

        tralloc_error result = _before_add_chunk ( _user_data, parent_chunk, extensions, chunk_length, length );

#       if defined(TRALLOC_THREADS)
        if (
            pthread_mutex_unlock ( &_user_data_mutex ) != 0 ||
            pthread_mutex_unlock ( &_before_add_chunk_mutex )    != 0
        ) {
            return TRALLOC_ERROR_MUTEX_FAILED;
        }
#       endif

        return result;
    } else {

#       if defined(TRALLOC_THREADS)
        if ( pthread_mutex_unlock ( &_before_add_chunk_mutex ) != 0 ) {
            return TRALLOC_ERROR_MUTEX_FAILED;
        }
#       endif

        return 0;
    }
}

tralloc_error _tralloc_debug_event_after_add_chunk ( _tralloc_chunk * chunk, size_t chunk_length, size_t length )
{
    tralloc_error result;
    if (
        ( result = _add_chunks_count ( 1 ) ) != 0 ||
        ( result = _tralloc_debug_add_chunks_overhead_length ( chunk_length ) ) != 0 ||
        ( result = _add_chunks_length ( length ) ) != 0
    ) {
        return result;
    }

#   if defined(TRALLOC_THREADS)
    if ( pthread_mutex_lock ( &_after_add_chunk_mutex ) != 0 ) {
        return TRALLOC_ERROR_MUTEX_FAILED;
    }
#   endif

    if ( _after_add_chunk != NULL ) {

#       if defined(TRALLOC_THREADS)
        if ( pthread_mutex_lock ( &_user_data_mutex ) != 0 ) {
            return TRALLOC_ERROR_MUTEX_FAILED;
        }
#       endif

        tralloc_error result = _after_add_chunk ( _user_data, chunk, chunk_length, length );

#       if defined(TRALLOC_THREADS)
        if (
            pthread_mutex_unlock ( &_user_data_mutex ) != 0 ||
            pthread_mutex_unlock ( &_after_add_chunk_mutex )    != 0
        ) {
            return TRALLOC_ERROR_MUTEX_FAILED;
        }
#       endif

        return result;
    } else {

#       if defined(TRALLOC_THREADS)
        if ( pthread_mutex_unlock ( &_after_add_chunk_mutex ) != 0 ) {
            return TRALLOC_ERROR_MUTEX_FAILED;
        }
#       endif

        return 0;
    }
}

tralloc_error _tralloc_debug_event_before_resize_chunk ( _tralloc_chunk * chunk )
{

#   if defined(TRALLOC_THREADS)
    if ( pthread_mutex_lock ( &_before_resize_chunk_mutex ) != 0 ) {
        return TRALLOC_ERROR_MUTEX_FAILED;
    }
#   endif

    if ( _before_resize_chunk != NULL ) {

#       if defined(TRALLOC_THREADS)
        if ( pthread_mutex_lock ( &_user_data_mutex ) != 0 ) {
            return TRALLOC_ERROR_MUTEX_FAILED;
        }
#       endif

        tralloc_error result = _before_resize_chunk ( _user_data, chunk );

#       if defined(TRALLOC_THREADS)
        if (
            pthread_mutex_unlock ( &_user_data_mutex ) != 0 ||
            pthread_mutex_unlock ( &_before_resize_chunk_mutex ) != 0
        ) {
            return TRALLOC_ERROR_MUTEX_FAILED;
        }
#       endif

        return result;
    } else {

#       if defined(TRALLOC_THREADS)
        if ( pthread_mutex_unlock ( &_before_resize_chunk_mutex ) != 0 ) {
            return TRALLOC_ERROR_MUTEX_FAILED;
        }
#       endif

        return 0;
    }
}

tralloc_error _tralloc_debug_event_after_resize_chunk ( _tralloc_chunk * chunk, size_t old_length, size_t length )
{
    tralloc_error result;
    if ( length > old_length ) {
        result = _add_chunks_length ( length - old_length );
        if ( result != 0 ) {
            return result;
        }
    } else if ( length < old_length ) {
        result = _subtract_chunks_length ( old_length - length );
        if ( result != 0 ) {
            return result;
        }
    }

#   if defined(TRALLOC_THREADS)
    if ( pthread_mutex_lock ( &_after_resize_chunk_mutex ) != 0 ) {
        return TRALLOC_ERROR_MUTEX_FAILED;
    }
#   endif

    if ( _after_resize_chunk != NULL ) {

#       if defined(TRALLOC_THREADS)
        if ( pthread_mutex_lock ( &_user_data_mutex ) != 0 ) {
            return TRALLOC_ERROR_MUTEX_FAILED;
        }
#       endif

        tralloc_error result = _after_resize_chunk ( _user_data, chunk, old_length );

#       if defined(TRALLOC_THREADS)
        if (
            pthread_mutex_unlock ( &_user_data_mutex ) != 0 ||
            pthread_mutex_unlock ( &_after_resize_chunk_mutex ) != 0
        ) {
            return TRALLOC_ERROR_MUTEX_FAILED;
        }
#       endif

        return result;
    } else {

#       if defined(TRALLOC_THREADS)
        if ( pthread_mutex_unlock ( &_after_resize_chunk_mutex ) != 0 ) {
            return TRALLOC_ERROR_MUTEX_FAILED;
        }
#       endif

        return 0;
    }
}

tralloc_error _tralloc_debug_event_before_move_chunk ( _tralloc_chunk * chunk )
{

#   if defined(TRALLOC_THREADS)
    if ( pthread_mutex_lock ( &_before_move_chunk_mutex ) != 0 ) {
        return TRALLOC_ERROR_MUTEX_FAILED;
    }
#   endif

    if ( _before_move_chunk != NULL ) {

#       if defined(TRALLOC_THREADS)
        if ( pthread_mutex_lock ( &_user_data_mutex ) != 0 ) {
            return TRALLOC_ERROR_MUTEX_FAILED;
        }
#       endif

        tralloc_error result = _before_move_chunk ( _user_data, chunk );

#       if defined(TRALLOC_THREADS)
        if (
            pthread_mutex_unlock ( &_user_data_mutex )         != 0 ||
            pthread_mutex_unlock ( &_before_move_chunk_mutex ) != 0
        ) {
            return TRALLOC_ERROR_MUTEX_FAILED;
        }
#       endif

        return result;
    } else {

#       if defined(TRALLOC_THREADS)
        if ( pthread_mutex_unlock ( &_before_move_chunk_mutex ) != 0 ) {
            return TRALLOC_ERROR_MUTEX_FAILED;
        }
#       endif

        return 0;
    }
}

tralloc_error _tralloc_debug_event_after_move_chunk ( _tralloc_chunk * chunk, _tralloc_chunk * old_parent_chunk )
{

#   if defined(TRALLOC_THREADS)
    if ( pthread_mutex_lock ( &_after_move_chunk_mutex ) != 0 ) {
        return TRALLOC_ERROR_MUTEX_FAILED;
    }
#   endif

    if ( _after_move_chunk != NULL ) {

#       if defined(TRALLOC_THREADS)
        if ( pthread_mutex_lock ( &_user_data_mutex ) != 0 ) {
            return TRALLOC_ERROR_MUTEX_FAILED;
        }
#       endif

        tralloc_error result = _after_move_chunk ( _user_data, chunk, old_parent_chunk );

#       if defined(TRALLOC_THREADS)
        if (
            pthread_mutex_unlock ( &_user_data_mutex )        != 0 ||
            pthread_mutex_unlock ( &_after_move_chunk_mutex ) != 0
        ) {
            return TRALLOC_ERROR_MUTEX_FAILED;
        }
#       endif

        return result;
    } else {

#       if defined(TRALLOC_THREADS)
        if ( pthread_mutex_unlock ( &_after_move_chunk_mutex ) != 0 ) {
            return TRALLOC_ERROR_MUTEX_FAILED;
        }
#       endif

        return 0;
    }
}

tralloc_error _tralloc_debug_event_before_free_chunk ( _tralloc_chunk * chunk )
{
    tralloc_error result;
    size_t length;
    if (
        ( result = _subtract_chunks_count ( 1 ) ) != 0 ||
        ( result = _tralloc_debug_subtract_chunks_overhead_length ( chunk->chunk_length ) ) != 0 ||
        ( result = _tralloc_debug_get_length ( chunk, &length ) ) != 0 ||
        ( result = _subtract_chunks_length ( length ) ) != 0
    ) {
        return result;
    }

#   if defined(TRALLOC_THREADS)
    if ( pthread_mutex_lock ( &_before_free_chunk_mutex ) != 0 ) {
        return TRALLOC_ERROR_MUTEX_FAILED;
    }
#   endif

    if ( _before_free_chunk != NULL ) {

#       if defined(TRALLOC_THREADS)
        if ( pthread_mutex_lock ( &_user_data_mutex ) != 0 ) {
            return TRALLOC_ERROR_MUTEX_FAILED;
        }
#       endif

        tralloc_error result = _before_free_chunk ( _user_data, chunk );

#       if defined(TRALLOC_THREADS)
        if (
            pthread_mutex_unlock ( &_user_data_mutex )         != 0 ||
            pthread_mutex_unlock ( &_before_free_chunk_mutex ) != 0
        ) {
            return TRALLOC_ERROR_MUTEX_FAILED;
        }
#       endif

        return result;
    } else {

#       if defined(TRALLOC_THREADS)
        if ( pthread_mutex_unlock ( &_before_free_chunk_mutex ) != 0 ) {
            return TRALLOC_ERROR_MUTEX_FAILED;
        }
#       endif

        return 0;
    }
}
