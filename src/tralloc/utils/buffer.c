// This file is part of tralloc. Copyright (C) 2013 Andrew Aladjev aladjev.andrew@gmail.com
// tralloc is free software: you can redistribute it and/or modify it under the terms of the GNU General Lesser Public License as published by the Free Software Foundation, either version 3 of the License, or (at your option) any later version.
// tralloc is distributed in the hope that it will be useful, but WITHOUT ANY WARRANTY; without even the implied warranty of MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU General Lesser Public License for more details.
// You should have received a copy of the GNU General Lesser Public License along with tralloc. If not, see <http://www.gnu.org/licenses/>.

#define _TRALLOC_UTILS_BUFFER_INCLUDED_FROM_OBJECT
#include <tralloc/utils/buffer.h>
#include <tralloc/tree/alloc.h>
#include <tralloc/tree/realloc.h>
#include <tralloc/tree/free.h>

#include <string.h>


tralloc_error tralloc_buffer_new_with_extensions ( tralloc_context * ctx, tralloc_buffer ** buffer_ptr, size_t capacity, tralloc_extensions extensions )
{
    tralloc_error result = tralloc_new_with_extensions ( ctx, ( tralloc_context ** ) buffer_ptr, extensions, sizeof ( tralloc_buffer ) );
    if ( result != 0 ) {
        return result;
    }

    tralloc_buffer * buffer = * buffer_ptr;
    result = tralloc_new ( buffer, ( tralloc_context ** ) &buffer->data, sizeof ( uint8_t ) * capacity );
    if ( result != 0 ) {
        tralloc_free ( buffer );
        return result;
    }

    buffer->offset   = 0;
    buffer->length   = 0;
    buffer->capacity = capacity;

    return 0;
}

static inline
void _tralloc_buffer_move_valid_data_to_begin ( tralloc_buffer * buffer )
{
    memmove ( buffer->data, buffer->data + buffer->offset, buffer->length );
    buffer->offset = 0;
}

tralloc_error tralloc_buffer_resize ( tralloc_buffer * buffer, size_t new_capacity )
{
    // Current valid data's "length" can't be less than new capacity.
    if ( buffer->length > new_capacity ) {
        return TRALLOC_ERROR_UTILS_BUFFER_OVERFLOW;
    }

    // If resize will affect left offset - valid data should be moved to the beginning of "buffer->data".
    if ( buffer->offset + buffer->length > new_capacity ) {
        if ( buffer->offset != 0 ) {
            _tralloc_buffer_move_valid_data_to_begin ( buffer );
        }
    }

    tralloc_error result = tralloc_realloc ( ( tralloc_context ** ) &buffer->data, sizeof ( uint8_t ) * new_capacity );
    if ( result != 0 ) {
        // Realloc failed, but valid data could be moved to the beginning of "buffer->data".
        // Buffer is still valid and it makes no sense to revert previous offset.
        return result;
    }
    buffer->capacity = new_capacity;

    return 0;
}

tralloc_error tralloc_buffer_left_trim ( tralloc_buffer * buffer )
{
    if ( buffer->offset == 0 ) {
        return 0;
    }
    size_t new_capacity = buffer->capacity - buffer->offset;

    _tralloc_buffer_move_valid_data_to_begin ( buffer );

    tralloc_error result = tralloc_realloc ( ( tralloc_context ** ) &buffer->data, sizeof ( uint8_t ) * new_capacity );
    if ( result != 0 ) {
        // Realloc failed, but valid data was moved to the beginning of "buffer->data".
        // Buffer is still valid and it makes no sense to revert previous offset.
        return result;
    }
    buffer->capacity = new_capacity;

    return 0;
}
