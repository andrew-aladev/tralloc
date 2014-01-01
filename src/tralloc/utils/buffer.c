// This file is part of tralloc. Copyright (C) 2013 Andrew Aladjev aladjev.andrew@gmail.com
// tralloc is free software: you can redistribute it and/or modify it under the terms of the GNU General Lesser Public License as published by the Free Software Foundation, either version 3 of the License, or (at your option) any later version.
// tralloc is distributed in the hope that it will be useful, but WITHOUT ANY WARRANTY; without even the implied warranty of MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU General Lesser Public License for more details.
// You should have received a copy of the GNU General Lesser Public License along with tralloc. If not, see <http://www.gnu.org/licenses/>.

#include "buffer.h"
#include <string.h>


#if defined(TRALLOC_EXTENSIONS)

extern inline tralloc_buffer * tralloc_buffer_with_extensions_new ( tralloc_context * ctx, uint8_t extensions );
extern inline tralloc_buffer * tralloc_buffer_new                 ( tralloc_context * ctx );

#else

extern inline tralloc_buffer * tralloc_buffer_new ( tralloc_context * ctx );

#endif


uint8_t tralloc_buffer_prepare ( tralloc_buffer * buffer, size_t length )
{
    uint8_t * buf = buffer->buf;
    uint8_t * new_buf;

    if ( buf == NULL ) {
        new_buf = tralloc ( buffer, sizeof ( uint8_t ) * length );
        if ( new_buf == NULL ) {
            return 1;
        }
        buffer->buf    = new_buf;
        buffer->length = length;
        return 0;
    }

    size_t data_length = buffer->data_offset + buffer->data_length;
    size_t tail        = buffer->length - data_length;
    if ( tail >= length ) {
        return 0;
    }

    size_t new_length = data_length + length;
    new_buf = tralloc_realloc ( buf, sizeof ( uint8_t ) * new_length );
    if ( new_buf == NULL ) {
        return 2;
    }
    if ( new_buf != buf ) {
        buffer->buf = new_buf;
    }
    buffer->length = new_length;
    return 0;
}

uint8_t tralloc_buffer_trim ( tralloc_buffer * buffer )
{
    uint8_t * buf = buffer->buf;
    if ( buf == NULL ) {
        return 0;
    }

    size_t data_offset = buffer->data_offset;
    if ( data_offset == 0 ) {
        return 0;
    }

    size_t length = buffer->length;
    length        -= data_offset;
    if ( length == 0 ) {
        if ( tralloc_free ( buf ) != 0 ) {
            return 1;
        }
        buffer->buf         = NULL;
        buffer->data_offset = 0;
        buffer->length      = 0;
        return 0;
    }

    size_t data_length = buffer->data_length;
    memmove ( buf, buf + data_offset, data_length );
    buffer->data_offset = 0;
    buffer->length      = length;

    return 0;
}

extern inline void             tralloc_buffer_written         ( tralloc_buffer * buffer, size_t length );
extern inline uint8_t          tralloc_buffer_readed          ( tralloc_buffer * buffer, size_t length );
extern inline uint8_t *        tralloc_buffer_get_read_point  ( const tralloc_buffer * buffer );
extern inline uint8_t *        tralloc_buffer_get_write_point ( const tralloc_buffer * buffer );
extern inline size_t           tralloc_buffer_get_length      ( const tralloc_buffer * buffer );
