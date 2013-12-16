// This file is part of tralloc. Copyright (C) 2013 Andrew Aladjev aladjev.andrew@gmail.com
// tralloc is free software: you can redistribute it and/or modify it under the terms of the GNU General Lesser Public License as published by the Free Software Foundation, either version 3 of the License, or (at your option) any later version.
// tralloc is distributed in the hope that it will be useful, but WITHOUT ANY WARRANTY; without even the implied warranty of MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU General Lesser Public License for more details.
// You should have received a copy of the GNU General Lesser Public License along with tralloc. If not, see <http://www.gnu.org/licenses/>.

#include "events.h"

static void *                     tralloc_user_data;
static tralloc_callback_on_add    tralloc_debug_on_add;
static tralloc_callback_on_resize tralloc_debug_on_resize;
static tralloc_callback_on_move   tralloc_debug_on_move;
static tralloc_callback_on_free   tralloc_debug_on_free;

void tralloc_set_user_data ( void * user_data )
{
    tralloc_user_data = user_data;
}

void tralloc_set_callback ( tralloc_callback_on_add on_add, tralloc_callback_on_resize on_resize, tralloc_callback_on_move on_move, tralloc_callback_on_free on_free )
{
    tralloc_debug_on_add    = on_add;
    tralloc_debug_on_resize = on_resize;
    tralloc_debug_on_move   = on_move;
    tralloc_debug_on_free   = on_free;
}

static size_t tralloc_chunks_count           = 0;
static size_t tralloc_chunks_overhead_length = 0;
static size_t tralloc_chunks_length          = 0;

uint8_t tralloc_on_add ( tralloc_chunk * chunk )
{
    tralloc_chunks_count++;
    tralloc_chunks_overhead_length += chunk->chunk_length;
    tralloc_chunks_length          += chunk->length;

    if ( tralloc_debug_on_add != NULL ) {
        return tralloc_debug_on_add ( tralloc_user_data, chunk );
    }

    return 0;
}

uint8_t tralloc_on_resize ( tralloc_chunk * chunk, size_t old_length )
{
    tralloc_chunks_length += chunk->length - old_length;

    if ( tralloc_debug_on_resize != NULL ) {
        return tralloc_debug_on_resize ( tralloc_user_data, chunk, old_length );
    }

    return 0;
}

uint8_t tralloc_on_move ( tralloc_chunk * chunk, tralloc_chunk * old_parent_chunk )
{
    if ( tralloc_debug_on_move != NULL ) {
        return tralloc_debug_on_move ( tralloc_user_data, chunk, old_parent_chunk );
    }

    return 0;
}

uint8_t tralloc_on_free ( tralloc_chunk * chunk )
{
    tralloc_chunks_count--;
    tralloc_chunks_overhead_length -= chunk->chunk_length;
    tralloc_chunks_length          -= chunk->length;

    if ( tralloc_debug_on_free != NULL ) {
        return tralloc_debug_on_free ( tralloc_user_data, chunk );
    }

    return 0;
}

size_t tralloc_get_chunks_count ()
{
    return tralloc_chunks_count;
}

size_t tralloc_get_chunks_overhead_length ()
{
    return tralloc_chunks_overhead_length;
}

size_t tralloc_get_chunks_length ()
{
    return tralloc_chunks_length;
}