// This file is part of talloc2. Copyright (C) 2013 Andrew Aladjev aladjev.andrew@gmail.com
// talloc2 is free software: you can redistribute it and/or modify it under the terms of the GNU General Lesser Public License as published by the Free Software Foundation, either version 3 of the License, or (at your option) any later version.
// talloc2 is distributed in the hope that it will be useful, but WITHOUT ANY WARRANTY; without even the implied warranty of MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU General Lesser Public License for more details.
// You should have received a copy of the GNU General Lesser Public License along with talloc2. If not, see <http://www.gnu.org/licenses/>.

#include "buffer.h"

#include <string.h>

uint8_t * talloc_buffer_prepare ( talloc_buffer * buffer, size_t length )
{
    uint8_t * buf = buffer->buf;
    uint8_t * new_buf;

    if ( buf == NULL ) {
        new_buf = talloc ( buffer, sizeof ( uint8_t ) * length );
        if ( new_buf == NULL ) {
            return NULL;
        }
        buffer->buf    = new_buf;
        buffer->length = length;
        return new_buf;
    }

    size_t data_length = buffer->data_offset + buffer->data_length;
    size_t tail        = buffer->length - data_length;
    if ( tail >= length ) {
        return buf + data_length;
    }

    size_t new_length = data_length + length;
    new_buf = talloc_realloc ( buf, sizeof ( uint8_t ) * new_length );
    if ( new_buf == NULL ) {
        return NULL;
    }
    if ( new_buf != buf ) {
        buffer->buf = new_buf;
    }
    buffer->length = new_length;
    return new_buf + data_length;
}

uint8_t talloc_buffer_trim ( talloc_buffer * buffer )
{
    uint8_t * buf = buffer->buf;
    if ( buf == NULL ) {
        return 0;
    }

    size_t data_length = buffer->data_length;
    if ( data_length == 0 ) {
        if ( talloc_free ( buf ) != 0 ) {
            return 1;
        }
        buffer->buf         = NULL;
        buffer->data_offset = 0;
        buffer->length      = 0;
        return 0;
    }

    size_t data_offset = buffer->data_offset;
    if ( data_offset != 0 ) {
        memmove ( buf, buf + data_offset, data_length );
        buffer->data_offset = 0;
    }

    uint8_t * new_buf = talloc_realloc ( buf, sizeof ( uint8_t ) * data_length );
    if ( new_buf == NULL ) {
        return 2;
    }
    if ( new_buf != buf ) {
        buffer->buf = new_buf;
    }
    buffer->length = data_length;
    return 0;
}

extern inline talloc_buffer * talloc_buffer_new        ( void * ctx );
extern inline void            talloc_buffer_written    ( talloc_buffer * buffer, size_t length );
extern inline uint8_t         talloc_buffer_readed     ( talloc_buffer * buffer, size_t length );
extern inline uint8_t *       talloc_buffer_get        ( talloc_buffer * buffer );
extern inline size_t          talloc_buffer_get_length ( talloc_buffer * buffer );