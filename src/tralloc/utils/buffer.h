// This file is part of tralloc. Copyright (C) 2013 Andrew Aladjev aladjev.andrew@gmail.com
// tralloc is free software: you can redistribute it and/or modify it under the terms of the GNU General Lesser Public License as published by the Free Software Foundation, either version 3 of the License, or (at your option) any later version.
// tralloc is distributed in the hope that it will be useful, but WITHOUT ANY WARRANTY; without even the implied warranty of MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU General Lesser Public License for more details.
// You should have received a copy of the GNU General Lesser Public License along with tralloc. If not, see <http://www.gnu.org/licenses/>.

#ifndef TRALLOC_UTILS_BUFFER_H
#define TRALLOC_UTILS_BUFFER_H

#include "../macro.h"
#include "../types.h"

#undef INLINE
#ifdef _TRALLOC_UTILS_BUFFER_INCLUDED_FROM_OBJECT
#    define INLINE INLINE_IN_OBJECT
#else
#    define INLINE INLINE_IN_HEADER
#endif


typedef struct _tralloc_buffer_type {
    uint8_t * buf;
    size_t    data_offset;
    size_t    data_length;
    size_t    length;
} tralloc_buffer;

#if defined(TRALLOC_EXTENSIONS)

tralloc_error tralloc_buffer_with_extensions_new ( tralloc_context * ctx, tralloc_buffer ** buffer_ptr, tralloc_extensions extensions );

INLINE
tralloc_error tralloc_buffer_new ( tralloc_context * ctx, tralloc_buffer ** buffer_ptr )
{
    return tralloc_buffer_with_extensions_new ( ctx, buffer_ptr, 0 );
}

#else

tralloc_error tralloc_buffer_new ( tralloc_context * ctx, tralloc_buffer ** buffer_ptr );

#endif

INLINE
void tralloc_buffer_written ( tralloc_buffer * buffer, size_t length )
{
    buffer->data_length += length;
}

INLINE
tralloc_error tralloc_buffer_readed ( tralloc_buffer * buffer, size_t length )
{
    if ( buffer->data_length < length ) {
        return TRALLOC_ERROR_UTILS_BUFFER_OVERFLOW;
    }
    buffer->data_offset += length;
    buffer->data_length -= length;
    return 0;
}

INLINE
uint8_t * tralloc_buffer_get_read_point ( const tralloc_buffer * buffer )
{
    return buffer->buf + buffer->data_offset;
}

INLINE
uint8_t * tralloc_buffer_get_write_point ( const tralloc_buffer * buffer )
{
    return buffer->buf + buffer->data_offset + buffer->data_length;
}

INLINE
size_t tralloc_buffer_get_length ( const tralloc_buffer * buffer )
{
    return buffer->data_length;
}

tralloc_error tralloc_buffer_prepare ( tralloc_buffer * buffer, size_t length );
tralloc_error tralloc_buffer_trim    ( tralloc_buffer * buffer );


#endif
