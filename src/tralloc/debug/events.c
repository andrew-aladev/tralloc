// This file is part of tralloc. Copyright (C) 2013 Andrew Aladjev aladjev.andrew@gmail.com
// tralloc is free software: you can redistribute it and/or modify it under the terms of the GNU General Lesser Public License as published by the Free Software Foundation, either version 3 of the License, or (at your option) any later version.
// tralloc is distributed in the hope that it will be useful, but WITHOUT ANY WARRANTY; without even the implied warranty of MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU General Lesser Public License for more details.
// You should have received a copy of the GNU General Lesser Public License along with tralloc. If not, see <http://www.gnu.org/licenses/>.

#include "events.h"
#include "main.h"


#if defined(TRALLOC_THREADS)
static pthread_mutex_t _user_data_mutex = PTHREAD_MUTEX_INITIALIZER;
#endif

static void * _user_data;

#if defined(TRALLOC_THREADS)
static pthread_mutex_t _on_add_mutex    = PTHREAD_MUTEX_INITIALIZER;
static pthread_mutex_t _on_resize_mutex = PTHREAD_MUTEX_INITIALIZER;
static pthread_mutex_t _on_move_mutex   = PTHREAD_MUTEX_INITIALIZER;
static pthread_mutex_t _on_free_mutex   = PTHREAD_MUTEX_INITIALIZER;
#endif

static _tralloc_debug_callback_on_add    _on_add;
static _tralloc_debug_callback_on_resize _on_resize;
static _tralloc_debug_callback_on_move   _on_move;
static _tralloc_debug_callback_on_free   _on_free;

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

tralloc_error _tralloc_debug_set_callbacks (
    _tralloc_debug_callback_on_add on_add,
    _tralloc_debug_callback_on_resize on_resize,
    _tralloc_debug_callback_on_move on_move,
    _tralloc_debug_callback_on_free on_free
)
{

#   if defined(TRALLOC_THREADS)
    if ( pthread_mutex_lock ( &_on_add_mutex ) != 0 ) {
        return TRALLOC_ERROR_MUTEX_FAILED;
    }
#   endif

    _on_add = on_add;

#   if defined(TRALLOC_THREADS)
    if (
        pthread_mutex_unlock ( &_on_add_mutex )    != 0 ||
        pthread_mutex_lock   ( &_on_resize_mutex ) != 0
    ) {
        return TRALLOC_ERROR_MUTEX_FAILED;
    }
#   endif

    _on_resize = on_resize;

#   if defined(TRALLOC_THREADS)
    if (
        pthread_mutex_unlock ( &_on_resize_mutex ) != 0 ||
        pthread_mutex_lock   ( &_on_move_mutex )   != 0
    ) {
        return TRALLOC_ERROR_MUTEX_FAILED;
    }
#   endif

    _on_move = on_move;

#   if defined(TRALLOC_THREADS)
    if (
        pthread_mutex_unlock ( &_on_move_mutex ) != 0 ||
        pthread_mutex_lock   ( &_on_free_mutex ) != 0
    ) {
        return TRALLOC_ERROR_MUTEX_FAILED;
    }
#   endif

    _on_free = on_free;

#   if defined(TRALLOC_THREADS)
    if ( pthread_mutex_unlock ( &_on_free_mutex ) != 0 ) {
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

tralloc_error _tralloc_debug_add_event ( _tralloc_chunk * chunk, size_t chunk_length, size_t length )
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
    if ( pthread_mutex_lock ( &_on_add_mutex ) != 0 ) {
        return TRALLOC_ERROR_MUTEX_FAILED;
    }
#   endif

    if ( _on_add != NULL ) {

#       if defined(TRALLOC_THREADS)
        if ( pthread_mutex_lock ( &_user_data_mutex ) != 0 ) {
            return TRALLOC_ERROR_MUTEX_FAILED;
        }
#       endif

        tralloc_error result = _on_add ( _user_data, chunk );

#       if defined(TRALLOC_THREADS)
        if (
            pthread_mutex_unlock ( &_user_data_mutex ) != 0 ||
            pthread_mutex_unlock ( &_on_add_mutex )    != 0
        ) {
            return TRALLOC_ERROR_MUTEX_FAILED;
        }
#       endif

        return result;
    } else {

#       if defined(TRALLOC_THREADS)
        if ( pthread_mutex_unlock ( &_on_add_mutex ) != 0 ) {
            return TRALLOC_ERROR_MUTEX_FAILED;
        }
#       endif

        return 0;
    }
}

tralloc_error _tralloc_debug_resize_event ( _tralloc_chunk * chunk, size_t old_length, size_t length )
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
    if ( pthread_mutex_lock ( &_on_resize_mutex ) != 0 ) {
        return TRALLOC_ERROR_MUTEX_FAILED;
    }
#   endif

    if ( _on_resize != NULL ) {

#       if defined(TRALLOC_THREADS)
        if ( pthread_mutex_lock ( &_user_data_mutex ) != 0 ) {
            return TRALLOC_ERROR_MUTEX_FAILED;
        }
#       endif

        tralloc_error result = _on_resize ( _user_data, chunk, old_length );

#       if defined(TRALLOC_THREADS)
        if (
            pthread_mutex_unlock ( &_user_data_mutex ) != 0 ||
            pthread_mutex_unlock ( &_on_resize_mutex ) != 0
        ) {
            return TRALLOC_ERROR_MUTEX_FAILED;
        }
#       endif

        return result;
    } else {

#       if defined(TRALLOC_THREADS)
        if ( pthread_mutex_unlock ( &_on_resize_mutex ) != 0 ) {
            return TRALLOC_ERROR_MUTEX_FAILED;
        }
#       endif

        return 0;
    }
}

tralloc_error _tralloc_debug_move_event ( _tralloc_chunk * chunk, _tralloc_chunk * old_parent_chunk )
{

#   if defined(TRALLOC_THREADS)
    if ( pthread_mutex_lock ( &_on_move_mutex ) != 0 ) {
        return TRALLOC_ERROR_MUTEX_FAILED;
    }
#   endif

    if ( _on_move != NULL ) {

#       if defined(TRALLOC_THREADS)
        if ( pthread_mutex_lock ( &_user_data_mutex ) != 0 ) {
            return TRALLOC_ERROR_MUTEX_FAILED;
        }
#       endif

        tralloc_error result = _on_move ( _user_data, chunk, old_parent_chunk );

#       if defined(TRALLOC_THREADS)
        if (
            pthread_mutex_unlock ( &_user_data_mutex ) != 0 ||
            pthread_mutex_unlock ( &_on_move_mutex )   != 0
        ) {
            return TRALLOC_ERROR_MUTEX_FAILED;
        }
#       endif

        return result;
    } else {

#       if defined(TRALLOC_THREADS)
        if ( pthread_mutex_unlock ( &_on_move_mutex ) != 0 ) {
            return TRALLOC_ERROR_MUTEX_FAILED;
        }
#       endif

        return 0;
    }
}

tralloc_error _tralloc_debug_free_event ( _tralloc_chunk * chunk )
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
    if ( pthread_mutex_lock ( &_on_free_mutex ) != 0 ) {
        return TRALLOC_ERROR_MUTEX_FAILED;
    }
#   endif

    if ( _on_free != NULL ) {

#       if defined(TRALLOC_THREADS)
        if ( pthread_mutex_lock ( &_user_data_mutex ) != 0 ) {
            return TRALLOC_ERROR_MUTEX_FAILED;
        }
#       endif

        tralloc_error result = _on_free ( _user_data, chunk );

#       if defined(TRALLOC_THREADS)
        if (
            pthread_mutex_unlock ( &_user_data_mutex ) != 0 ||
            pthread_mutex_unlock ( &_on_free_mutex )   != 0
        ) {
            return TRALLOC_ERROR_MUTEX_FAILED;
        }
#       endif

        return result;
    } else {

#       if defined(TRALLOC_THREADS)
        if ( pthread_mutex_unlock ( &_on_free_mutex ) != 0 ) {
            return TRALLOC_ERROR_MUTEX_FAILED;
        }
#       endif

        return 0;
    }
}
