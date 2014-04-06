// This file is part of tralloc. Copyright (C) 2013 Andrew Aladjev aladjev.andrew@gmail.com
// tralloc is free software: you can redistribute it and/or modify it under the terms of the GNU General Lesser Public License as published by the Free Software Foundation, either version 3 of the License, or (at your option) any later version.
// tralloc is distributed in the hope that it will be useful, but WITHOUT ANY WARRANTY; without even the implied warranty of MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU General Lesser Public License for more details.
// You should have received a copy of the GNU General Lesser Public License along with tralloc. If not, see <http://www.gnu.org/licenses/>.

#ifndef TRALLOC_DEBUG_EVENTS_H
#define TRALLOC_DEBUG_EVENTS_H

#include "../types.h"


typedef tralloc_error ( * _tralloc_debug_callback_on_add )    ( void * user_data, _tralloc_chunk * chunk );
typedef tralloc_error ( * _tralloc_debug_callback_on_resize ) ( void * user_data, _tralloc_chunk * chunk, size_t old_length );
typedef tralloc_error ( * _tralloc_debug_callback_on_move )   ( void * user_data, _tralloc_chunk * chunk, _tralloc_chunk * old_parent_chunk );
typedef tralloc_error ( * _tralloc_debug_callback_on_free )   ( void * user_data, _tralloc_chunk * chunk );

tralloc_error _tralloc_debug_set_user_data ( void * user_data );
tralloc_error _tralloc_debug_set_callbacks (
    _tralloc_debug_callback_on_add on_add,
    _tralloc_debug_callback_on_resize on_resize,
    _tralloc_debug_callback_on_move on_move,
    _tralloc_debug_callback_on_free on_free
);

tralloc_error _tralloc_debug_add_event    ( _tralloc_chunk * chunk, size_t chunk_length, size_t length );
tralloc_error _tralloc_debug_resize_event ( _tralloc_chunk * chunk, size_t old_length, size_t length );
tralloc_error _tralloc_debug_move_event   ( _tralloc_chunk * chunk, _tralloc_chunk * old_parent_chunk );
tralloc_error _tralloc_debug_free_event   ( _tralloc_chunk * chunk );

tralloc_error _tralloc_debug_add_chunks_overhead_length      ( size_t length );
tralloc_error _tralloc_debug_subtract_chunks_overhead_length ( size_t length );

tralloc_error tralloc_debug_get_chunks_count           ( size_t * length );
tralloc_error tralloc_debug_get_chunks_overhead_length ( size_t * length );
tralloc_error tralloc_debug_get_chunks_length          ( size_t * length );


#endif
