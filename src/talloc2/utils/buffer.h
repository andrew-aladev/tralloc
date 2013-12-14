// This file is part of talloc2. Copyright (C) 2013 Andrew Aladjev aladjev.andrew@gmail.com
// talloc2 is free software: you can redistribute it and/or modify it under the terms of the GNU General Lesser Public License as published by the Free Software Foundation, either version 3 of the License, or (at your option) any later version.
// talloc2 is distributed in the hope that it will be useful, but WITHOUT ANY WARRANTY; without even the implied warranty of MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU General Lesser Public License for more details.
// You should have received a copy of the GNU General Lesser Public License along with talloc2. If not, see <http://www.gnu.org/licenses/>.

#ifndef TALLOC_UTILS_BUFFER_H
#define TALLOC_UTILS_BUFFER_H

#include "../tree.h"

typedef struct talloc_buffer_t {
    uint8_t * buf;
    size_t    data_offset;
    size_t    data_length;
    size_t    length;
} talloc_buffer;

// Function creates new buffer and attaches it to ctx.
// Function returns pointer to talloc_buffer or NULL if error occurred.
inline
talloc_buffer * talloc_buffer_new ( const talloc_context * ctx )
{
    talloc_buffer * buffer = talloc ( ctx, sizeof ( talloc_buffer ) );
    if ( buffer == NULL ) {
        return NULL;
    }
    buffer->buf         = NULL;
    buffer->data_offset = 0;
    buffer->data_length = 0;
    buffer->length      = 0;
    return buffer;
}

// Function adds length written to buffer.
inline
void talloc_buffer_written ( talloc_buffer * buffer, size_t length )
{
    buffer->data_length += length;
}

// Function deletes length readed from buffer.
// Function returns zero or non-zero value if error occurred.
inline
uint8_t talloc_buffer_readed ( talloc_buffer * buffer, size_t length )
{
    if ( buffer->data_length < length ) {
        return 1;
    }
    buffer->data_offset += length;
    buffer->data_length -= length;
    return 0;
}

// Function returns read point of buffer.
inline
uint8_t * talloc_buffer_get_read_point ( const talloc_buffer * buffer )
{
    return buffer->buf + buffer->data_offset;
}

// Function returns write point of buffer.
inline
uint8_t * talloc_buffer_get_write_point ( const talloc_buffer * buffer )
{
    return buffer->buf + buffer->data_offset + buffer->data_length;
}

// Function returns length of buffer.
inline
size_t talloc_buffer_get_length ( const talloc_buffer * buffer )
{
    return buffer->data_length;
}

// Function prepairs buffer for writing length bytes.
// Function returns zero or non-zero value if error occurred.
uint8_t talloc_buffer_prepare ( talloc_buffer * buffer, size_t length );

// Function trims readed data and empty tail from buffer.
// Function returns zero or non-zero value if error occurred.
uint8_t talloc_buffer_trim ( talloc_buffer * buffer );

#endif
