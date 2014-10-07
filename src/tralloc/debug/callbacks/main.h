// This file is part of tralloc. Copyright (C) 2013 Andrew Aladjev aladjev.andrew@gmail.com
// tralloc is free software: you can redistribute it and/or modify it under the terms of the GNU General Lesser Public License as published by the Free Software Foundation, either version 3 of the License, or (at your option) any later version.
// tralloc is distributed in the hope that it will be useful, but WITHOUT ANY WARRANTY; without even the implied warranty of MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU General Lesser Public License for more details.
// You should have received a copy of the GNU General Lesser Public License along with tralloc. If not, see <http://www.gnu.org/licenses/>.

#if !defined ( TRALLOC_DEBUG_CALLBACKS_MAIN_H )
#define TRALLOC_DEBUG_CALLBACKS_MAIN_H

#include "../../types.h"


typedef tralloc_error ( * tralloc_debug_callback_before_add ) ( void * data, _tralloc_chunk * parent_chunk, tralloc_extensions extensions, size_t length );
typedef tralloc_error ( * tralloc_debug_callback_after_add )  ( void * data, _tralloc_chunk * chunk );

typedef tralloc_error ( * tralloc_debug_callback_before_resize ) ( void * data, _tralloc_chunk * chunk );
typedef tralloc_error ( * tralloc_debug_callback_after_resize )  ( void * data, _tralloc_chunk * chunk, size_t old_length );

typedef tralloc_error ( * tralloc_debug_callback_before_move ) ( void * data, _tralloc_chunk * chunk );
typedef tralloc_error ( * tralloc_debug_callback_after_move )  ( void * data, _tralloc_chunk * chunk, _tralloc_chunk * old_parent_chunk );

typedef tralloc_error ( * tralloc_debug_callback_before_free_subtree )           ( void * data, _tralloc_chunk * chunk );
typedef tralloc_error ( * tralloc_debug_callback_before_free_chunk )             ( void * data, _tralloc_chunk * chunk );
typedef tralloc_error ( * tralloc_debug_callback_before_refuse_to_free_subtree ) ( void * data, _tralloc_chunk * chunk );
typedef tralloc_error ( * tralloc_debug_callback_after_refuse_to_free_subtree )  ( void * data, _tralloc_chunk * chunk );
typedef tralloc_error ( * tralloc_debug_callback_before_refuse_to_free_chunk )   ( void * data, _tralloc_chunk * chunk );
typedef tralloc_error ( * tralloc_debug_callback_after_refuse_to_free_chunk )    ( void * data, _tralloc_chunk * chunk );

tralloc_error tralloc_debug_callback_set_add_data      ( void * before_add, void * after_add );
tralloc_error tralloc_debug_callback_set_add_functions ( tralloc_debug_callback_before_add before_add, tralloc_debug_callback_after_add after_add );
tralloc_error _tralloc_debug_callback_before_add_chunk ( _tralloc_chunk * parent_chunk, tralloc_extensions extensions, size_t length );
tralloc_error _tralloc_debug_callback_after_add_chunk  ( _tralloc_chunk * chunk );

tralloc_error tralloc_debug_callback_set_resize_data      ( void * before_resize, void * after_resize );
tralloc_error tralloc_debug_callback_set_resize_functions ( tralloc_debug_callback_before_resize before_resize, tralloc_debug_callback_after_resize after_resize );
tralloc_error _tralloc_debug_callback_before_resize_chunk ( _tralloc_chunk * chunk );
tralloc_error _tralloc_debug_callback_after_resize_chunk  ( _tralloc_chunk * chunk, size_t old_length );

tralloc_error tralloc_debug_callback_set_move_data      ( void * before_move, void * after_move );
tralloc_error tralloc_debug_callback_set_move_functions ( tralloc_debug_callback_before_move before_move, tralloc_debug_callback_after_move after_move );
tralloc_error _tralloc_debug_callback_before_move_chunk ( _tralloc_chunk * chunk );
tralloc_error _tralloc_debug_callback_after_move_chunk  ( _tralloc_chunk * chunk, _tralloc_chunk * old_parent_chunk );

tralloc_error tralloc_debug_callback_set_free_data (
    void * before_free_subtree,
    void * before_free_chunk,
    void * before_refuse_to_free_subtree,
    void * after_refuse_to_free_subtree,
    void * before_refuse_to_free_chunk,
    void * after_refuse_to_free_chunk
);
tralloc_error tralloc_debug_callback_set_free_functions (
    tralloc_debug_callback_before_free_subtree           before_free_subtree,
    tralloc_debug_callback_before_free_chunk             before_free_chunk,
    tralloc_debug_callback_before_refuse_to_free_subtree before_refuse_to_free_subtree,
    tralloc_debug_callback_after_refuse_to_free_subtree  after_refuse_to_free_subtree,
    tralloc_debug_callback_before_refuse_to_free_chunk   before_refuse_to_free_chunk,
    tralloc_debug_callback_after_refuse_to_free_chunk    after_refuse_to_free_chunk
);
tralloc_error _tralloc_debug_callback_before_free_subtree           ( _tralloc_chunk * chunk );
tralloc_error _tralloc_debug_callback_before_free_chunk             ( _tralloc_chunk * chunk );
tralloc_error _tralloc_debug_callback_before_refuse_to_free_subtree ( _tralloc_chunk * chunk );
tralloc_error _tralloc_debug_callback_after_refuse_to_free_subtree  ( _tralloc_chunk * chunk );
tralloc_error _tralloc_debug_callback_before_refuse_to_free_chunk   ( _tralloc_chunk * chunk );
tralloc_error _tralloc_debug_callback_after_refuse_to_free_chunk    ( _tralloc_chunk * chunk );


#endif
