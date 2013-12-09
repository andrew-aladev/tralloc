// This file is part of talloc2. Copyright (C) 2013 Andrew Aladjev aladjev.andrew@gmail.com
// talloc2 is free software: you can redistribute it and/or modify it under the terms of the GNU General Lesser Public License as published by the Free Software Foundation, either version 3 of the License, or (at your option) any later version.
// talloc2 is distributed in the hope that it will be useful, but WITHOUT ANY WARRANTY; without even the implied warranty of MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU General Lesser Public License for more details.
// You should have received a copy of the GNU General Lesser Public License along with talloc2. If not, see <http://www.gnu.org/licenses/>.

#include "events.h"

static void *                    talloc_user_data;
static talloc_callback_on_add    talloc_debug_on_add;
static talloc_callback_on_resize talloc_debug_on_resize;
static talloc_callback_on_move   talloc_debug_on_move;
static talloc_callback_on_free   talloc_debug_on_free;

void talloc_set_user_data ( void * user_data )
{
    talloc_user_data = user_data;
}

void talloc_set_callback ( talloc_callback_on_add on_add, talloc_callback_on_resize on_resize, talloc_callback_on_move on_move, talloc_callback_on_free on_free )
{
    talloc_debug_on_add    = on_add;
    talloc_debug_on_resize = on_resize;
    talloc_debug_on_move   = on_move;
    talloc_debug_on_free   = on_free;
}

static size_t talloc_chunks_count         = 0;
static size_t talloc_chunks_overhead_length = 0;
static size_t talloc_chunks_length        = 0;

uint8_t talloc_on_add ( talloc_chunk * chunk )
{
    talloc_chunks_count++;
    talloc_chunks_overhead_length += chunk->chunk_length;
    talloc_chunks_length        += chunk->length;

    if ( talloc_debug_on_add != NULL ) {
        return talloc_debug_on_add ( talloc_user_data, chunk );
    }

    return 0;
}

uint8_t talloc_on_resize ( talloc_chunk * chunk, size_t old_length )
{
    talloc_chunks_length += chunk->length - old_length;

    if ( talloc_debug_on_resize != NULL ) {
        return talloc_debug_on_resize ( talloc_user_data, chunk, old_length );
    }

    return 0;
}

uint8_t talloc_on_move ( talloc_chunk * chunk, talloc_chunk * old_parent_chunk )
{
    if ( talloc_debug_on_move != NULL ) {
        return talloc_debug_on_move ( talloc_user_data, chunk, old_parent_chunk );
    }

    return 0;
}

uint8_t talloc_on_free ( talloc_chunk * chunk )
{
    talloc_chunks_count--;
    talloc_chunks_overhead_length -= chunk->chunk_length;
    talloc_chunks_length          -= chunk->length;

    if ( talloc_debug_on_free != NULL ) {
        return talloc_debug_on_free ( talloc_user_data, chunk );
    }

    return 0;
}

size_t talloc_get_chunks_count ()
{
    return talloc_chunks_count;
}

size_t talloc_get_chunks_overhead_length ()
{
    return talloc_chunks_overhead_length;
}

size_t talloc_get_chunks_length ()
{
    return talloc_chunks_length;
}