// This file is part of tralloc. Copyright (C) 2013 Andrew Aladjev aladjev.andrew@gmail.com
// tralloc is free software: you can redistribute it and/or modify it under the terms of the GNU General Lesser Public License as published by the Free Software Foundation, either version 3 of the License, or (at your option) any later version.
// tralloc is distributed in the hope that it will be useful, but WITHOUT ANY WARRANTY; without even the implied warranty of MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU General Lesser Public License for more details.
// You should have received a copy of the GNU General Lesser Public License along with tralloc. If not, see <http://www.gnu.org/licenses/>.

#ifndef TRALLOC_EVENTS_H
#define TRALLOC_EVENTS_H

#include "types.h"

typedef uint8_t ( * tralloc_callback_on_add )    ( void * user_data, tralloc_chunk * chunk );
typedef uint8_t ( * tralloc_callback_on_resize ) ( void * user_data, tralloc_chunk * chunk, size_t old_length );
typedef uint8_t ( * tralloc_callback_on_move )   ( void * user_data, tralloc_chunk * chunk, tralloc_chunk * old_parent_chunk );
typedef uint8_t ( * tralloc_callback_on_free )   ( void * user_data, tralloc_chunk * chunk );

// Function sets global user_data to user_data.
// It will be passed as a first argument in on_add, on_resize, on_move and on_free functions.
void tralloc_set_user_data ( void * user_data );

// Function sets global on_add, on_resize, on_move and on_free functions.
void tralloc_set_callback ( tralloc_callback_on_add on_add, tralloc_callback_on_resize on_resize, tralloc_callback_on_move on_move, tralloc_callback_on_free on_free );

// Function calls after some chunk was created.
uint8_t tralloc_on_add ( tralloc_chunk * chunk );

// Function calls after some chunk was resizes.
uint8_t tralloc_on_resize ( tralloc_chunk * chunk, size_t old_length );

// Function calls after some chunk was moved.
uint8_t tralloc_on_move ( tralloc_chunk * chunk, tralloc_chunk * old_parent_chunk );

// Function calls before some chunk was freed.
uint8_t tralloc_on_free ( tralloc_chunk * chunk );

// Function returns global count of chunks.
size_t tralloc_get_chunks_count ();

// Function returns global length used by chunk's controll structures.
size_t tralloc_get_chunks_overhead_length ();

// Function returns global usable for user length of chunks.
size_t tralloc_get_chunks_length ();

#endif
