// This file is part of tralloc. Copyright (C) 2013 Andrew Aladjev aladjev.andrew@gmail.com
// tralloc is free software: you can redistribute it and/or modify it under the terms of the GNU General Lesser Public License as published by the Free Software Foundation, either version 3 of the License, or (at your option) any later version.
// tralloc is distributed in the hope that it will be useful, but WITHOUT ANY WARRANTY; without even the implied warranty of MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU General Lesser Public License for more details.
// You should have received a copy of the GNU General Lesser Public License along with tralloc. If not, see <http://www.gnu.org/licenses/>.

#include "events.h"

static void *                      _user_data;
static _tralloc_callback_on_add    _debug_on_add;
static _tralloc_callback_on_resize _debug_on_resize;
static _tralloc_callback_on_move   _debug_on_move;
static _tralloc_callback_on_free   _debug_on_free;

void _tralloc_set_user_data ( void * user_data )
{
    _user_data = user_data;
}

void _tralloc_set_callback ( _tralloc_callback_on_add on_add, _tralloc_callback_on_resize on_resize, _tralloc_callback_on_move on_move, _tralloc_callback_on_free on_free )
{
    _debug_on_add    = on_add;
    _debug_on_resize = on_resize;
    _debug_on_move   = on_move;
    _debug_on_free   = on_free;
}

static size_t _chunks_count           = 0;
static size_t _chunks_overhead_length = 0;
static size_t _chunks_length          = 0;

uint8_t _tralloc_on_add ( _tralloc_chunk * chunk )
{
    _chunks_count++;
    _chunks_overhead_length += chunk->chunk_length;
    _chunks_length          += chunk->length;

    if ( _debug_on_add != NULL ) {
        return _debug_on_add ( _user_data, chunk );
    }

    return 0;
}

uint8_t _tralloc_on_resize ( _tralloc_chunk * chunk, size_t old_length )
{
    _chunks_length += chunk->length - old_length;

    if ( _debug_on_resize != NULL ) {
        return _debug_on_resize ( _user_data, chunk, old_length );
    }

    return 0;
}

uint8_t _tralloc_on_move ( _tralloc_chunk * chunk, _tralloc_chunk * old_parent_chunk )
{
    if ( _debug_on_move != NULL ) {
        return _debug_on_move ( _user_data, chunk, old_parent_chunk );
    }

    return 0;
}

uint8_t _tralloc_on_free ( _tralloc_chunk * chunk )
{
    _chunks_count--;
    _chunks_overhead_length -= chunk->chunk_length;
    _chunks_length          -= chunk->length;

    if ( _debug_on_free != NULL ) {
        return _debug_on_free ( _user_data, chunk );
    }

    return 0;
}

size_t tralloc_get_chunks_count ()
{
    return _chunks_count;
}

size_t tralloc_get_chunks_overhead_length ()
{
    return _chunks_overhead_length;
}

size_t tralloc_get_chunks_length ()
{
    return _chunks_length;
}