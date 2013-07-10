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

inline
talloc_buffer * talloc_buffer_new ( void * ctx )
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

uint8_t * talloc_buffer_prepare  ( talloc_buffer * buffer, size_t length );
uint8_t   talloc_buffer_trim ( talloc_buffer * buffer );

inline
void talloc_buffer_written ( talloc_buffer * buffer, size_t length )
{
    buffer->data_length += length;
}

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

inline
uint8_t * talloc_buffer_get ( talloc_buffer * buffer )
{
    return buffer->buf + buffer->data_offset;
}

inline
size_t talloc_buffer_get_length ( talloc_buffer * buffer )
{
    return buffer->data_length;
}

#endif
