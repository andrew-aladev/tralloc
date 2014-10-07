// This file is part of tralloc. Copyright (C) 2013 Andrew Aladjev aladjev.andrew@gmail.com
// tralloc is free software: you can redistribute it and/or modify it under the terms of the GNU General Lesser Public License as published by the Free Software Foundation, either version 3 of the License, or (at your option) any later version.
// tralloc is distributed in the hope that it will be useful, but WITHOUT ANY WARRANTY; without even the implied warranty of MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU General Lesser Public License for more details.
// You should have received a copy of the GNU General Lesser Public License along with tralloc. If not, see <http://www.gnu.org/licenses/>.

#include <tralloc/debug/callbacks/main.h>

#if defined ( TRALLOC_THREADS )
#   include <tralloc/debug/callbacks/lock.h>
#endif


static void * _tralloc_debug_callback_before_add_data = NULL;
static void * _tralloc_debug_callback_after_add_data  = NULL;
static tralloc_debug_callback_before_add _tralloc_debug_callback_before_add_callback = NULL;
static tralloc_debug_callback_after_add  _tralloc_debug_callback_after_add_callback  = NULL;

tralloc_error tralloc_debug_callback_set_add_data ( void * before_add, void * after_add )
{

#   if defined ( TRALLOC_THREADS )
    tralloc_error result = _tralloc_debug_callback_wrlock_add ();
    if ( result != 0 ) {
        return result;
    }
#   endif

    _tralloc_debug_callback_before_add_data = before_add;
    _tralloc_debug_callback_after_add_data  = after_add;

#   if defined ( TRALLOC_THREADS )
    result = _tralloc_debug_callback_unlock_add ();
    if ( result != 0 ) {
        return result;
    }
#   endif

    return 0;
}

tralloc_error tralloc_debug_callback_set_add_functions ( tralloc_debug_callback_before_add before_add, tralloc_debug_callback_after_add after_add )
{

#   if defined ( TRALLOC_THREADS )
    tralloc_error result = _tralloc_debug_callback_wrlock_add ();
    if ( result != 0 ) {
        return result;
    }
#   endif

    _tralloc_debug_callback_before_add_callback = before_add;
    _tralloc_debug_callback_after_add_callback  = after_add;

#   if defined ( TRALLOC_THREADS )
    result = _tralloc_debug_callback_unlock_add ();
    if ( result != 0 ) {
        return result;
    }
#   endif

    return 0;
}

tralloc_error _tralloc_debug_callback_before_add_chunk ( _tralloc_chunk * parent_chunk, tralloc_extensions extensions, size_t length )
{

#   if defined ( TRALLOC_THREADS )
    tralloc_error result = _tralloc_debug_callback_rdlock_add ();
    if ( result != 0 ) {
        return result;
    }
#   endif

    void * data = _tralloc_debug_callback_before_add_data;
    tralloc_debug_callback_before_add function = _tralloc_debug_callback_before_add_callback;

#   if defined ( TRALLOC_THREADS )
    result = _tralloc_debug_callback_unlock_add ();
    if ( result != 0 ) {
        return result;
    }
#   endif

    if ( function != NULL ) {
        return function ( data, parent_chunk, extensions, length );
    }
    return 0;
}

tralloc_error _tralloc_debug_callback_after_add_chunk ( _tralloc_chunk * chunk )
{

#   if defined ( TRALLOC_THREADS )
    tralloc_error result = _tralloc_debug_callback_rdlock_add ();
    if ( result != 0 ) {
        return result;
    }
#   endif

    void * data = _tralloc_debug_callback_after_add_data;
    tralloc_debug_callback_after_add function = _tralloc_debug_callback_after_add_callback;

#   if defined ( TRALLOC_THREADS )
    result = _tralloc_debug_callback_unlock_add ();
    if ( result != 0 ) {
        return result;
    }
#   endif

    if ( function != NULL ) {
        return function ( data, chunk );
    }
    return 0;
}


static void * _tralloc_debug_callback_before_move_data = NULL;
static void * _tralloc_debug_callback_after_move_data  = NULL;
static tralloc_debug_callback_before_move _tralloc_debug_callback_before_move_callback = NULL;
static tralloc_debug_callback_after_move  _tralloc_debug_callback_after_move_callback  = NULL;

tralloc_error tralloc_debug_callback_set_move_data ( void * before_move, void * after_move )
{

#   if defined ( TRALLOC_THREADS )
    tralloc_error result = _tralloc_debug_callback_wrlock_move ();
    if ( result != 0 ) {
        return result;
    }
#   endif

    _tralloc_debug_callback_before_move_data = before_move;
    _tralloc_debug_callback_after_move_data  = after_move;

#   if defined ( TRALLOC_THREADS )
    result = _tralloc_debug_callback_unlock_move ();
    if ( result != 0 ) {
        return result;
    }
#   endif

    return 0;
}

tralloc_error tralloc_debug_callback_set_move_functions ( tralloc_debug_callback_before_move before_move, tralloc_debug_callback_after_move after_move )
{

#   if defined ( TRALLOC_THREADS )
    tralloc_error result = _tralloc_debug_callback_wrlock_move ();
    if ( result != 0 ) {
        return result;
    }
#   endif

    _tralloc_debug_callback_before_move_callback = before_move;
    _tralloc_debug_callback_after_move_callback  = after_move;

#   if defined ( TRALLOC_THREADS )
    result = _tralloc_debug_callback_unlock_move ();
    if ( result != 0 ) {
        return result;
    }
#   endif

    return 0;
}

tralloc_error _tralloc_debug_callback_before_move_chunk ( _tralloc_chunk * chunk )
{

#   if defined ( TRALLOC_THREADS )
    tralloc_error result = _tralloc_debug_callback_rdlock_move ();
    if ( result != 0 ) {
        return result;
    }
#   endif

    void * data = _tralloc_debug_callback_before_move_data;
    tralloc_debug_callback_before_move function = _tralloc_debug_callback_before_move_callback;

#   if defined ( TRALLOC_THREADS )
    result = _tralloc_debug_callback_unlock_move ();
    if ( result != 0 ) {
        return result;
    }
#   endif

    if ( function != NULL ) {
        return function ( data, chunk );
    }
    return 0;
}

tralloc_error _tralloc_debug_callback_after_move_chunk ( _tralloc_chunk * chunk, _tralloc_chunk * old_parent_chunk )
{

#   if defined ( TRALLOC_THREADS )
    tralloc_error result = _tralloc_debug_callback_rdlock_move ();
    if ( result != 0 ) {
        return result;
    }
#   endif

    void * data = _tralloc_debug_callback_after_move_data;
    tralloc_debug_callback_after_move function = _tralloc_debug_callback_after_move_callback;

#   if defined ( TRALLOC_THREADS )
    result = _tralloc_debug_callback_unlock_move ();
    if ( result != 0 ) {
        return result;
    }
#   endif

    if ( function != NULL ) {
        return function ( data, chunk, old_parent_chunk );
    }
    return 0;
}


static void * _tralloc_debug_callback_before_resize_data = NULL;
static void * _tralloc_debug_callback_after_resize_data  = NULL;
static tralloc_debug_callback_before_resize _tralloc_debug_callback_before_resize_callback = NULL;
static tralloc_debug_callback_after_resize  _tralloc_debug_callback_after_resize_callback  = NULL;

tralloc_error tralloc_debug_callback_set_resize_data ( void * before_resize, void * after_resize )
{

#   if defined ( TRALLOC_THREADS )
    tralloc_error result = _tralloc_debug_callback_wrlock_resize ();
    if ( result != 0 ) {
        return result;
    }
#   endif

    _tralloc_debug_callback_before_resize_data = before_resize;
    _tralloc_debug_callback_after_resize_data  = after_resize;

#   if defined ( TRALLOC_THREADS )
    result = _tralloc_debug_callback_unlock_resize ();
    if ( result != 0 ) {
        return result;
    }
#   endif

    return 0;
}

tralloc_error tralloc_debug_callback_set_resize_functions ( tralloc_debug_callback_before_resize before_resize, tralloc_debug_callback_after_resize after_resize )
{

#   if defined ( TRALLOC_THREADS )
    tralloc_error result = _tralloc_debug_callback_wrlock_resize ();
    if ( result != 0 ) {
        return result;
    }
#   endif

    _tralloc_debug_callback_before_resize_callback = before_resize;
    _tralloc_debug_callback_after_resize_callback  = after_resize;

#   if defined ( TRALLOC_THREADS )
    result = _tralloc_debug_callback_unlock_resize ();
    if ( result != 0 ) {
        return result;
    }
#   endif

    return 0;
}

tralloc_error _tralloc_debug_callback_before_resize_chunk ( _tralloc_chunk * chunk )
{

#   if defined ( TRALLOC_THREADS )
    tralloc_error result = _tralloc_debug_callback_rdlock_resize ();
    if ( result != 0 ) {
        return result;
    }
#   endif

    void * data = _tralloc_debug_callback_before_resize_data;
    tralloc_debug_callback_before_resize function = _tralloc_debug_callback_before_resize_callback;

#   if defined ( TRALLOC_THREADS )
    result = _tralloc_debug_callback_unlock_resize ();
    if ( result != 0 ) {
        return result;
    }
#   endif

    if ( function != NULL ) {
        return function ( data, chunk );
    }
    return 0;
}

tralloc_error _tralloc_debug_callback_after_resize_chunk ( _tralloc_chunk * chunk, size_t old_length )
{

#   if defined ( TRALLOC_THREADS )
    tralloc_error result = _tralloc_debug_callback_rdlock_resize ();
    if ( result != 0 ) {
        return result;
    }
#   endif

    void * data = _tralloc_debug_callback_after_resize_data;
    tralloc_debug_callback_after_resize function = _tralloc_debug_callback_after_resize_callback;

#   if defined ( TRALLOC_THREADS )
    result = _tralloc_debug_callback_unlock_resize ();
    if ( result != 0 ) {
        return result;
    }
#   endif

    if ( function != NULL ) {
        return function ( data, chunk, old_length );
    }
    return 0;
}


static void * _tralloc_debug_callback_before_free_subtree_data           = NULL;
static void * _tralloc_debug_callback_before_free_chunk_data             = NULL;
static void * _tralloc_debug_callback_before_refuse_to_free_subtree_data = NULL;
static void * _tralloc_debug_callback_after_refuse_to_free_subtree_data  = NULL;
static void * _tralloc_debug_callback_before_refuse_to_free_chunk_data   = NULL;
static void * _tralloc_debug_callback_after_refuse_to_free_chunk_data    = NULL;

static tralloc_debug_callback_before_free_subtree           _tralloc_debug_callback_before_free_subtree_function           = NULL;
static tralloc_debug_callback_before_free_chunk             _tralloc_debug_callback_before_free_chunk_function             = NULL;
static tralloc_debug_callback_before_refuse_to_free_subtree _tralloc_debug_callback_before_refuse_to_free_subtree_function = NULL;
static tralloc_debug_callback_after_refuse_to_free_subtree  _tralloc_debug_callback_after_refuse_to_free_subtree_function  = NULL;
static tralloc_debug_callback_before_refuse_to_free_chunk   _tralloc_debug_callback_before_refuse_to_free_chunk_function   = NULL;
static tralloc_debug_callback_after_refuse_to_free_chunk    _tralloc_debug_callback_after_refuse_to_free_chunk_function    = NULL;

tralloc_error tralloc_debug_callback_set_free_data (
    void * before_free_subtree,
    void * before_free_chunk,
    void * before_refuse_to_free_subtree,
    void * after_refuse_to_free_subtree,
    void * before_refuse_to_free_chunk,
    void * after_refuse_to_free_chunk
)
{
    tralloc_error error = 0;

#   if defined ( TRALLOC_THREADS )
    tralloc_error result = _tralloc_debug_callback_wrlock_free ();
    if ( result != 0 ) {
        error = result;
    }
#   endif

    _tralloc_debug_callback_before_free_subtree_data           = before_free_subtree;
    _tralloc_debug_callback_before_free_chunk_data             = before_free_chunk;
    _tralloc_debug_callback_before_refuse_to_free_subtree_data = before_refuse_to_free_subtree;
    _tralloc_debug_callback_after_refuse_to_free_subtree_data  = after_refuse_to_free_subtree;
    _tralloc_debug_callback_before_refuse_to_free_chunk_data   = before_refuse_to_free_chunk;
    _tralloc_debug_callback_after_refuse_to_free_chunk_data    = after_refuse_to_free_chunk;

#   if defined ( TRALLOC_THREADS )
    result = _tralloc_debug_callback_unlock_free ();
    if ( result != 0 ) {
        error = result;
    }
#   endif

    return error;
}

tralloc_error tralloc_debug_callback_set_free_functions (
    tralloc_debug_callback_before_free_subtree           before_free_subtree,
    tralloc_debug_callback_before_free_chunk             before_free_chunk,
    tralloc_debug_callback_before_refuse_to_free_subtree before_refuse_to_free_subtree,
    tralloc_debug_callback_after_refuse_to_free_subtree  after_refuse_to_free_subtree,
    tralloc_debug_callback_before_refuse_to_free_chunk   before_refuse_to_free_chunk,
    tralloc_debug_callback_after_refuse_to_free_chunk    after_refuse_to_free_chunk
)
{
    tralloc_error error = 0;

#   if defined ( TRALLOC_THREADS )
    tralloc_error result = _tralloc_debug_callback_wrlock_free ();
    if ( result != 0 ) {
        error = result;
    }
#   endif

    _tralloc_debug_callback_before_free_subtree_function           = before_free_subtree;
    _tralloc_debug_callback_before_free_chunk_function             = before_free_chunk;
    _tralloc_debug_callback_before_refuse_to_free_subtree_function = before_refuse_to_free_subtree;
    _tralloc_debug_callback_after_refuse_to_free_subtree_function  = after_refuse_to_free_subtree;
    _tralloc_debug_callback_before_refuse_to_free_chunk_function   = before_refuse_to_free_chunk;
    _tralloc_debug_callback_after_refuse_to_free_chunk_function    = after_refuse_to_free_chunk;

#   if defined ( TRALLOC_THREADS )
    result = _tralloc_debug_callback_unlock_free ();
    if ( result != 0 ) {
        error = result;
    }
#   endif

    return error;
}

tralloc_error _tralloc_debug_callback_before_free_subtree ( _tralloc_chunk * chunk )
{
    tralloc_error error = 0, result;

#   if defined ( TRALLOC_THREADS )
    result = _tralloc_debug_callback_rdlock_free ();
    if ( result != 0 ) {
        error = result;
    }
#   endif

    void * data = _tralloc_debug_callback_before_free_subtree_data;
    tralloc_debug_callback_before_free_subtree function = _tralloc_debug_callback_before_free_subtree_function;

#   if defined ( TRALLOC_THREADS )
    result = _tralloc_debug_callback_unlock_free ();
    if ( result != 0 ) {
        error = result;
    }
#   endif

    if ( function != NULL ) {
        result = function ( data, chunk );
        if ( result != 0 ) {
            error = result;
        }
    }
    return error;
}

tralloc_error _tralloc_debug_callback_before_free_chunk ( _tralloc_chunk * chunk )
{
    tralloc_error error = 0, result;

#   if defined ( TRALLOC_THREADS )
    result = _tralloc_debug_callback_rdlock_free ();
    if ( result != 0 ) {
        error = result;
    }
#   endif

    void * data = _tralloc_debug_callback_before_free_chunk_data;
    tralloc_debug_callback_before_free_chunk function = _tralloc_debug_callback_before_free_chunk_function;

#   if defined ( TRALLOC_THREADS )
    result = _tralloc_debug_callback_unlock_free ();
    if ( result != 0 ) {
        error = result;
    }
#   endif

    if ( function != NULL ) {
        result = function ( data, chunk );
        if ( result != 0 ) {
            error = result;
        }
    }
    return error;
}

tralloc_error _tralloc_debug_callback_before_refuse_to_free_subtree ( _tralloc_chunk * chunk )
{
    tralloc_error error = 0, result;

#   if defined ( TRALLOC_THREADS )
    result = _tralloc_debug_callback_rdlock_free ();
    if ( result != 0 ) {
        error = result;
    }
#   endif

    void * data = _tralloc_debug_callback_before_refuse_to_free_subtree_data;
    tralloc_debug_callback_before_refuse_to_free_subtree function = _tralloc_debug_callback_before_refuse_to_free_subtree_function;

#   if defined ( TRALLOC_THREADS )
    result = _tralloc_debug_callback_unlock_free ();
    if ( result != 0 ) {
        error = result;
    }
#   endif

    if ( function != NULL ) {
        result = function ( data, chunk );
        if ( result != 0 ) {
            error = result;
        }
    }
    return error;
}

tralloc_error _tralloc_debug_callback_after_refuse_to_free_subtree ( _tralloc_chunk * chunk )
{
    tralloc_error error = 0, result;

#   if defined ( TRALLOC_THREADS )
    result = _tralloc_debug_callback_rdlock_free ();
    if ( result != 0 ) {
        error = result;
    }
#   endif

    void * data = _tralloc_debug_callback_after_refuse_to_free_subtree_data;
    tralloc_debug_callback_after_refuse_to_free_subtree function = _tralloc_debug_callback_after_refuse_to_free_subtree_function;

#   if defined ( TRALLOC_THREADS )
    result = _tralloc_debug_callback_unlock_free ();
    if ( result != 0 ) {
        error = result;
    }
#   endif

    if ( function != NULL ) {
        result = function ( data, chunk );
        if ( result != 0 ) {
            error = result;
        }
    }
    return error;
}

tralloc_error _tralloc_debug_callback_before_refuse_to_free_chunk ( _tralloc_chunk * chunk )
{
    tralloc_error error = 0, result;

#   if defined ( TRALLOC_THREADS )
    result = _tralloc_debug_callback_rdlock_free ();
    if ( result != 0 ) {
        error = result;
    }
#   endif

    void * data = _tralloc_debug_callback_before_refuse_to_free_chunk_data;
    tralloc_debug_callback_before_refuse_to_free_chunk function = _tralloc_debug_callback_before_refuse_to_free_chunk_function;

#   if defined ( TRALLOC_THREADS )
    result = _tralloc_debug_callback_unlock_free ();
    if ( result != 0 ) {
        error = result;
    }
#   endif

    if ( function != NULL ) {
        result = function ( data, chunk );
        if ( result != 0 ) {
            error = result;
        }
    }
    return error;
}

tralloc_error _tralloc_debug_callback_after_refuse_to_free_chunk ( _tralloc_chunk * chunk )
{
    tralloc_error error = 0, result;

#   if defined ( TRALLOC_THREADS )
    result = _tralloc_debug_callback_rdlock_free ();
    if ( result != 0 ) {
        error = result;
    }
#   endif

    void * data = _tralloc_debug_callback_after_refuse_to_free_chunk_data;
    tralloc_debug_callback_after_refuse_to_free_chunk function = _tralloc_debug_callback_after_refuse_to_free_chunk_function;

#   if defined ( TRALLOC_THREADS )
    result = _tralloc_debug_callback_unlock_free ();
    if ( result != 0 ) {
        error = result;
    }
#   endif

    if ( function != NULL ) {
        result = function ( data, chunk );
        if ( result != 0 ) {
            error = result;
        }
    }
    return error;
}
