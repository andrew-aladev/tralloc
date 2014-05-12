// This file is part of tralloc. Copyright (C) 2013 Andrew Aladjev aladjev.andrew@gmail.com
// tralloc is free software: you can redistribute it and/or modify it under the terms of the GNU General Lesser Public License as published by the Free Software Foundation, either version 3 of the License, or (at your option) any later version.
// tralloc is distributed in the hope that it will be useful, but WITHOUT ANY WARRANTY; without even the implied warranty of MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU General Lesser Public License for more details.
// You should have received a copy of the GNU General Lesser Public License along with tralloc. If not, see <http://www.gnu.org/licenses/>.

#include "callback.h"


#if defined(TRALLOC_THREADS)
static pthread_mutex_t _before_add_mutex = PTHREAD_MUTEX_INITIALIZER;
static pthread_mutex_t _after_add_mutex  = PTHREAD_MUTEX_INITIALIZER;
#endif

static void * _before_add_data = NULL;
static void * _after_add_data  = NULL;
static tralloc_debug_callback_before_add _before_add = NULL;
static tralloc_debug_callback_after_add  _after_add  = NULL;

tralloc_error tralloc_debug_callback_set_add_data ( void * before, void * after )
{

#   if defined(TRALLOC_THREADS)
    if ( pthread_mutex_lock ( &_before_add_mutex ) != 0 ) {
        return TRALLOC_ERROR_MUTEX_FAILED;
    }
#   endif

    _before_add_data = before;

#   if defined(TRALLOC_THREADS)
    if ( pthread_mutex_unlock ( &_before_add_mutex ) != 0 || pthread_mutex_lock ( &_after_add_mutex ) != 0 ) {
        return TRALLOC_ERROR_MUTEX_FAILED;
    }
#   endif

    _after_add_data = after;

#   if defined(TRALLOC_THREADS)
    if ( pthread_mutex_unlock ( &_after_add_mutex ) != 0 ) {
        return TRALLOC_ERROR_MUTEX_FAILED;
    }
#   endif

    return 0;
}

tralloc_error tralloc_debug_callback_set_add_functions ( tralloc_debug_callback_before_add before, tralloc_debug_callback_after_add after )
{

#   if defined(TRALLOC_THREADS)
    if ( pthread_mutex_lock ( &_before_add_mutex ) != 0 ) {
        return TRALLOC_ERROR_MUTEX_FAILED;
    }
#   endif

    _before_add = before;

#   if defined(TRALLOC_THREADS)
    if ( pthread_mutex_unlock ( &_before_add_mutex ) != 0 || pthread_mutex_lock ( &_after_add_mutex ) != 0 ) {
        return TRALLOC_ERROR_MUTEX_FAILED;
    }
#   endif

    _after_add = after;

#   if defined(TRALLOC_THREADS)
    if ( pthread_mutex_unlock ( &_after_add_mutex ) != 0 ) {
        return TRALLOC_ERROR_MUTEX_FAILED;
    }
#   endif

    return 0;
}

tralloc_error _tralloc_debug_callback_before_add_chunk ( _tralloc_chunk * parent_chunk, tralloc_extensions extensions, size_t chunk_length, size_t length )
{

#   if defined(TRALLOC_THREADS)
    if ( pthread_mutex_lock ( &_before_add_mutex ) != 0 ) {
        return TRALLOC_ERROR_MUTEX_FAILED;
    }
#   endif

    tralloc_error error = 0;

    if ( _before_add != NULL ) {
        tralloc_error result = _before_add ( _before_add_data, parent_chunk, extensions, chunk_length, length );
        if ( result != 0 ) {
            error = result;
        }
    }

#   if defined(TRALLOC_THREADS)
    if ( pthread_mutex_unlock ( &_before_add_mutex ) != 0 ) {
        return TRALLOC_ERROR_MUTEX_FAILED;
    }
#   endif

    return error;
}

tralloc_error _tralloc_debug_callback_after_add_chunk ( _tralloc_chunk * chunk, size_t chunk_length, size_t length, const char * file, size_t line )
{

#   if defined(TRALLOC_THREADS)
    if ( pthread_mutex_lock ( &_after_add_mutex ) != 0 ) {
        return TRALLOC_ERROR_MUTEX_FAILED;
    }
#   endif

    tralloc_error error = 0;

    if ( _after_add != NULL ) {
        tralloc_error result = _after_add ( _after_add_data, chunk, chunk_length, length, file, line );
        if ( result != 0 ) {
            error = result;
        }
    }

#   if defined(TRALLOC_THREADS)
    if ( pthread_mutex_unlock ( &_after_add_mutex ) != 0 ) {
        return TRALLOC_ERROR_MUTEX_FAILED;
    }
#   endif

    return error;
}


#if defined(TRALLOC_THREADS)
static pthread_mutex_t _before_resize_mutex = PTHREAD_MUTEX_INITIALIZER;
static pthread_mutex_t _after_resize_mutex  = PTHREAD_MUTEX_INITIALIZER;
#endif

static void * _before_resize_data = NULL;
static void * _after_resize_data  = NULL;
static tralloc_debug_callback_before_resize _before_resize = NULL;
static tralloc_debug_callback_after_resize  _after_resize  = NULL;

tralloc_error tralloc_debug_callback_set_resize_data ( void * before, void * after )
{

#   if defined(TRALLOC_THREADS)
    if ( pthread_mutex_lock ( &_before_resize_mutex ) != 0 ) {
        return TRALLOC_ERROR_MUTEX_FAILED;
    }
#   endif

    _before_resize_data = before;

#   if defined(TRALLOC_THREADS)
    if ( pthread_mutex_unlock ( &_before_resize_mutex ) != 0 || pthread_mutex_lock ( &_after_resize_mutex ) != 0 ) {
        return TRALLOC_ERROR_MUTEX_FAILED;
    }
#   endif

    _after_resize_data = after;

#   if defined(TRALLOC_THREADS)
    if ( pthread_mutex_unlock ( &_after_resize_mutex ) != 0 ) {
        return TRALLOC_ERROR_MUTEX_FAILED;
    }
#   endif

    return 0;
}

tralloc_error tralloc_debug_callback_set_resize_functions ( tralloc_debug_callback_before_resize before, tralloc_debug_callback_after_resize after )
{

#   if defined(TRALLOC_THREADS)
    if ( pthread_mutex_lock ( &_before_resize_mutex ) != 0 ) {
        return TRALLOC_ERROR_MUTEX_FAILED;
    }
#   endif

    _before_resize = before;

#   if defined(TRALLOC_THREADS)
    if ( pthread_mutex_unlock ( &_before_resize_mutex ) != 0 || pthread_mutex_lock ( &_after_resize_mutex ) != 0 ) {
        return TRALLOC_ERROR_MUTEX_FAILED;
    }
#   endif

    _after_resize = after;

#   if defined(TRALLOC_THREADS)
    if ( pthread_mutex_unlock ( &_after_resize_mutex ) != 0 ) {
        return TRALLOC_ERROR_MUTEX_FAILED;
    }
#   endif

    return 0;
}

tralloc_error _tralloc_debug_callback_before_resize_chunk ( _tralloc_chunk * chunk )
{

#   if defined(TRALLOC_THREADS)
    if ( pthread_mutex_lock ( &_before_resize_mutex ) != 0 ) {
        return TRALLOC_ERROR_MUTEX_FAILED;
    }
#   endif

    tralloc_error error = 0;

    if ( _before_resize != NULL ) {
        tralloc_error result = _before_resize ( _before_resize_data, chunk );
        if ( result != 0 ) {
            error = result;
        }
    }

#   if defined(TRALLOC_THREADS)
    if ( pthread_mutex_unlock ( &_before_resize_mutex ) != 0 ) {
        return TRALLOC_ERROR_MUTEX_FAILED;
    }
#   endif

    return error;
}

tralloc_error _tralloc_debug_callback_after_resize_chunk ( _tralloc_chunk * chunk, size_t old_length, size_t length )
{

#   if defined(TRALLOC_THREADS)
    if ( pthread_mutex_lock ( &_after_resize_mutex ) != 0 ) {
        return TRALLOC_ERROR_MUTEX_FAILED;
    }
#   endif

    tralloc_error error = 0;

    if ( _after_resize != NULL ) {
        tralloc_error result = _after_resize ( _after_resize_data, chunk, old_length, length );
        if ( result != 0 ) {
            error = result;
        }
    }

#   if defined(TRALLOC_THREADS)
    if ( pthread_mutex_unlock ( &_after_resize_mutex ) != 0 ) {
        return TRALLOC_ERROR_MUTEX_FAILED;
    }
#   endif

    return error;
}


#if defined(TRALLOC_THREADS)
static pthread_mutex_t _before_move_mutex = PTHREAD_MUTEX_INITIALIZER;
static pthread_mutex_t _after_move_mutex  = PTHREAD_MUTEX_INITIALIZER;
#endif

static void * _before_move_data = NULL;
static void * _after_move_data  = NULL;
static tralloc_debug_callback_before_move _before_move = NULL;
static tralloc_debug_callback_after_move  _after_move  = NULL;

tralloc_error tralloc_debug_callback_set_move_data ( void * before, void * after )
{

#   if defined(TRALLOC_THREADS)
    if ( pthread_mutex_lock ( &_before_move_mutex ) != 0 ) {
        return TRALLOC_ERROR_MUTEX_FAILED;
    }
#   endif

    _before_move_data = before;

#   if defined(TRALLOC_THREADS)
    if ( pthread_mutex_unlock ( &_before_move_mutex ) != 0 || pthread_mutex_lock ( &_after_move_mutex ) != 0 ) {
        return TRALLOC_ERROR_MUTEX_FAILED;
    }
#   endif

    _after_move_data = after;

#   if defined(TRALLOC_THREADS)
    if ( pthread_mutex_unlock ( &_after_move_mutex ) != 0 ) {
        return TRALLOC_ERROR_MUTEX_FAILED;
    }
#   endif

    return 0;
}

tralloc_error tralloc_debug_callback_set_move_functions ( tralloc_debug_callback_before_move before, tralloc_debug_callback_after_move after )
{

#   if defined(TRALLOC_THREADS)
    if ( pthread_mutex_lock ( &_before_move_mutex ) != 0 ) {
        return TRALLOC_ERROR_MUTEX_FAILED;
    }
#   endif

    _before_move = before;

#   if defined(TRALLOC_THREADS)
    if ( pthread_mutex_unlock ( &_before_move_mutex ) != 0 || pthread_mutex_lock ( &_after_move_mutex ) != 0 ) {
        return TRALLOC_ERROR_MUTEX_FAILED;
    }
#   endif

    _after_move = after;

#   if defined(TRALLOC_THREADS)
    if ( pthread_mutex_unlock ( &_after_move_mutex ) != 0 ) {
        return TRALLOC_ERROR_MUTEX_FAILED;
    }
#   endif

    return 0;
}

tralloc_error _tralloc_debug_callback_before_move_chunk ( _tralloc_chunk * chunk )
{

#   if defined(TRALLOC_THREADS)
    if ( pthread_mutex_lock ( &_before_move_mutex ) != 0 ) {
        return TRALLOC_ERROR_MUTEX_FAILED;
    }
#   endif

    tralloc_error error = 0;

    if ( _before_move != NULL ) {
        tralloc_error result = _before_move ( _before_move_data, chunk );
        if ( result != 0 ) {
            error = result;
        }
    }

#   if defined(TRALLOC_THREADS)
    if ( pthread_mutex_unlock ( &_before_move_mutex ) != 0 ) {
        return TRALLOC_ERROR_MUTEX_FAILED;
    }
#   endif

    return error;
}

tralloc_error _tralloc_debug_callback_after_move_chunk ( _tralloc_chunk * chunk, _tralloc_chunk * old_parent_chunk )
{

#   if defined(TRALLOC_THREADS)
    if ( pthread_mutex_lock ( &_after_move_mutex ) != 0 ) {
        return TRALLOC_ERROR_MUTEX_FAILED;
    }
#   endif

    tralloc_error error = 0;

    if ( _after_move != NULL ) {
        tralloc_error result = _after_move ( _after_move_data, chunk );
        if ( result != 0 ) {
            error = result;
        }
    }

#   if defined(TRALLOC_THREADS)
    if ( pthread_mutex_unlock ( &_after_move_mutex ) != 0 ) {
        return TRALLOC_ERROR_MUTEX_FAILED;
    }
#   endif

    return error;
}


#if defined(TRALLOC_THREADS)
static pthread_mutex_t _before_free_subtree_mutex = PTHREAD_MUTEX_INITIALIZER;
static pthread_mutex_t _before_free_chunk_mutex   = PTHREAD_MUTEX_INITIALIZER;
#endif

static void * _before_free_subtree_data = NULL;
static void * _before_free_chunk_data   = NULL;
static tralloc_debug_callback_before_free_subtree _before_free_subtree = NULL;
static tralloc_debug_callback_before_free_chunk   _before_free_chunk   = NULL;

tralloc_error tralloc_debug_callback_set_free_data ( void * before_subtree, void * before_chunk )
{

#   if defined(TRALLOC_THREADS)
    if ( pthread_mutex_lock ( &_before_free_subtree_mutex ) != 0 ) {
        return TRALLOC_ERROR_MUTEX_FAILED;
    }
#   endif

    _before_free_subtree_data = before_subtree;

#   if defined(TRALLOC_THREADS)
    if ( pthread_mutex_unlock ( &_before_free_subtree_mutex ) != 0 || pthread_mutex_lock ( &_before_free_chunk_mutex ) != 0 ) {
        return TRALLOC_ERROR_MUTEX_FAILED;
    }
#   endif

    _before_free_chunk_data = before_chunk;

#   if defined(TRALLOC_THREADS)
    if ( pthread_mutex_unlock ( &_before_free_chunk_mutex ) != 0 ) {
        return TRALLOC_ERROR_MUTEX_FAILED;
    }
#   endif

    return 0;
}

tralloc_error tralloc_debug_callback_set_free_functions (
    tralloc_debug_callback_before_free_subtree before_subtree, tralloc_debug_callback_before_free_chunk before_chunk
)
{

#   if defined(TRALLOC_THREADS)
    if ( pthread_mutex_lock ( &_before_free_subtree_mutex ) != 0 ) {
        return TRALLOC_ERROR_MUTEX_FAILED;
    }
#   endif

    _before_free_subtree = before_subtree;

#   if defined(TRALLOC_THREADS)
    if ( pthread_mutex_unlock ( &_before_free_subtree_mutex ) != 0 || pthread_mutex_lock ( &_before_free_chunk_mutex ) != 0 ) {
        return TRALLOC_ERROR_MUTEX_FAILED;
    }
#   endif

    _before_free_chunk = before_chunk;

#   if defined(TRALLOC_THREADS)
    if ( pthread_mutex_unlock ( &_before_free_chunk_mutex ) != 0 ) {
        return TRALLOC_ERROR_MUTEX_FAILED;
    }
#   endif

    return 0;
}
