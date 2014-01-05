// This file is part of tralloc. Copyright (C) 2013 Andrew Aladjev aladjev.andrew@gmail.com
// tralloc is free software: you can redistribute it and/or modify it under the terms of the GNU General Lesser Public License as published by the Free Software Foundation, either version 3 of the License, or (at your option) any later version.
// tralloc is distributed in the hope that it will be useful, but WITHOUT ANY WARRANTY; without even the implied warranty of MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU General Lesser Public License for more details.
// You should have received a copy of the GNU General Lesser Public License along with tralloc. If not, see <http://www.gnu.org/licenses/>.

#ifndef TRALLOC_UTILS_BUFFER_H
#define TRALLOC_UTILS_BUFFER_H

#include "../tree.h"


typedef struct tralloc_buffer_t {
    uint8_t * buf;
    size_t    data_offset;
    size_t    data_length;
    size_t    length;
} tralloc_buffer;

#if defined(TRALLOC_EXTENSIONS)

inline
tralloc_buffer * tralloc_buffer_with_extensions_new ( tralloc_context * ctx, uint8_t extensions )
{
    tralloc_buffer * buffer;
    if ( tralloc_with_extensions ( ctx, ( tralloc_context ** ) &buffer, extensions, sizeof ( tralloc_buffer ) ) != 0 ) {
        return NULL;
    }
    buffer->buf         = NULL;
    buffer->data_offset = 0;
    buffer->data_length = 0;
    buffer->length      = 0;
    return buffer;
}

inline
tralloc_buffer * tralloc_buffer_new ( tralloc_context * ctx )
{
    return tralloc_buffer_with_extensions_new ( ctx, 0 );
}

#else

inline
tralloc_buffer * tralloc_buffer_new ( tralloc_context * ctx )
{
    tralloc_buffer * buffer;
    if ( tralloc ( ctx, ( tralloc_context ** ) &buffer, sizeof ( tralloc_buffer ) ) != 0 ) {
        return NULL;
    }
    buffer->buf         = NULL;
    buffer->data_offset = 0;
    buffer->data_length = 0;
    buffer->length      = 0;
    return buffer;
}

#endif

inline
void tralloc_buffer_written ( tralloc_buffer * buffer, size_t length )
{
    buffer->data_length += length;
}

inline
uint8_t tralloc_buffer_readed ( tralloc_buffer * buffer, size_t length )
{
    if ( buffer->data_length < length ) {
        return 1;
    }
    buffer->data_offset += length;
    buffer->data_length -= length;
    return 0;
}

inline
uint8_t * tralloc_buffer_get_read_point ( const tralloc_buffer * buffer )
{
    return buffer->buf + buffer->data_offset;
}

inline
uint8_t * tralloc_buffer_get_write_point ( const tralloc_buffer * buffer )
{
    return buffer->buf + buffer->data_offset + buffer->data_length;
}

inline
size_t tralloc_buffer_get_length ( const tralloc_buffer * buffer )
{
    return buffer->data_length;
}

uint8_t tralloc_buffer_prepare ( tralloc_buffer * buffer, size_t length );

uint8_t tralloc_buffer_trim ( tralloc_buffer * buffer );


#endif
