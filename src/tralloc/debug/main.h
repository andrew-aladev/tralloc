// This file is part of tralloc. Copyright (C) 2013 Andrew Aladjev aladjev.andrew@gmail.com
// tralloc is free software: you can redistribute it and/or modify it under the terms of the GNU General Lesser Public License as published by the Free Software Foundation, either version 3 of the License, or (at your option) any later version.
// tralloc is distributed in the hope that it will be useful, but WITHOUT ANY WARRANTY; without even the implied warranty of MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU General Lesser Public License for more details.
// You should have received a copy of the GNU General Lesser Public License along with tralloc. If not, see <http://www.gnu.org/licenses/>.

#ifndef TRALLOC_DEBUG_MAIN_H
#define TRALLOC_DEBUG_MAIN_H

#include "../types.h"


typedef tralloc_error ( * _tralloc_debug_callback_before_add_chunk ) ( void * user_data, _tralloc_chunk * parent_chunk, tralloc_extensions extensions, size_t chunk_length, size_t length );
typedef tralloc_error ( * _tralloc_debug_callback_after_add_chunk )  ( void * user_data, _tralloc_chunk * chunk, size_t chunk_length, size_t length );

typedef tralloc_error ( * _tralloc_debug_callback_before_resize_chunk ) ( void * user_data, _tralloc_chunk * chunk );
typedef tralloc_error ( * _tralloc_debug_callback_after_resize_chunk )  ( void * user_data, _tralloc_chunk * chunk, size_t old_length );

typedef tralloc_error ( * _tralloc_debug_callback_before_move_chunk ) ( void * user_data, _tralloc_chunk * chunk );
typedef tralloc_error ( * _tralloc_debug_callback_after_move_chunk )  ( void * user_data, _tralloc_chunk * chunk, _tralloc_chunk * old_parent_chunk );

typedef tralloc_error ( * _tralloc_debug_callback_before_free_chunk ) ( void * user_data, _tralloc_chunk * chunk );
typedef tralloc_error ( * _tralloc_debug_callback_after_free_chunk )  ( void * user_data, _tralloc_chunk * chunk );

tralloc_error _tralloc_debug_set_user_data ( void * user_data );
tralloc_error _tralloc_debug_set_callbacks (
    _tralloc_debug_callback_before_add_chunk before_add_chunk,
    _tralloc_debug_callback_after_add_chunk  after_add_chunk,

    _tralloc_debug_callback_before_resize_chunk before_resize_chunk,
    _tralloc_debug_callback_after_resize_chunk  after_resize_chunk,

    _tralloc_debug_callback_before_move_chunk before_move_chunk,
    _tralloc_debug_callback_after_move_chunk  after_move_chunk,

    _tralloc_debug_callback_before_free_chunk before_free_chunk,
    _tralloc_debug_callback_after_free_chunk  after_free_chunk
);

tralloc_error _tralloc_debug_event_before_add_chunk ( _tralloc_chunk * parent_chunk, tralloc_extensions extensions, size_t chunk_length, size_t length );
tralloc_error _tralloc_debug_event_after_add_chunk  ( _tralloc_chunk * chunk, size_t chunk_length, size_t length );

tralloc_error _tralloc_debug_event_before_resize_chunk ( _tralloc_chunk * chunk );
tralloc_error _tralloc_debug_event_after_resize_chunk  ( _tralloc_chunk * chunk, size_t old_length, size_t length );

tralloc_error _tralloc_debug_event_before_move_chunk ( _tralloc_chunk * chunk );
tralloc_error _tralloc_debug_event_after_move_chunk  ( _tralloc_chunk * chunk, _tralloc_chunk * old_parent_chunk );

tralloc_error _tralloc_debug_event_before_free_chunk ( _tralloc_chunk * chunk );
tralloc_error _tralloc_debug_event_after_free_chunk  ( _tralloc_chunk * chunk );

tralloc_error _tralloc_debug_add_chunks_overhead_length      ( size_t length );
tralloc_error _tralloc_debug_subtract_chunks_overhead_length ( size_t length );

tralloc_error tralloc_debug_get_chunks_count           ( size_t * length );
tralloc_error tralloc_debug_get_chunks_overhead_length ( size_t * length );
tralloc_error tralloc_debug_get_chunks_length          ( size_t * length );


#endif
