// This file is part of tralloc. Copyright (C) 2013 Andrew Aladjev aladjev.andrew@gmail.com
// tralloc is free software: you can redistribute it and/or modify it under the terms of the GNU General Lesser Public License as published by the Free Software Foundation, either version 3 of the License, or (at your option) any later version.
// tralloc is distributed in the hope that it will be useful, but WITHOUT ANY WARRANTY; without even the implied warranty of MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU General Lesser Public License for more details.
// You should have received a copy of the GNU General Lesser Public License along with tralloc. If not, see <http://www.gnu.org/licenses/>.

#define _TRALLOC_UTILS_BUFFER_INCLUDED_FROM_OBJECT
#include "buffer.h"
#include "../tree/alloc.h"
#include "../tree/realloc.h"
#include "../tree/free.h"

#include <string.h>


#if defined(TRALLOC_EXTENSIONS)

tralloc_error tralloc_buffer_with_extensions_new ( tralloc_context * ctx, tralloc_buffer ** buffer_ptr, tralloc_extensions extensions )
{
    if ( buffer_ptr == NULL ) {
        return TRALLOC_ERROR_REQUIRED_ARGUMENT_IS_NULL;
    }
    tralloc_error result = tralloc_with_extensions ( ctx, ( tralloc_context ** ) buffer_ptr, extensions, sizeof ( tralloc_buffer ) );
    if ( result != 0 ) {
        return result;
    }
    tralloc_buffer * buffer = * buffer_ptr;

    buffer->buf         = NULL;
    buffer->data_offset = 0;
    buffer->data_length = 0;
    buffer->length      = 0;
    return 0;
}

#else

tralloc_error tralloc_buffer_new ( tralloc_context * ctx, tralloc_buffer ** buffer_ptr )
{
    if ( buffer_ptr == NULL ) {
        return TRALLOC_ERROR_REQUIRED_ARGUMENT_IS_NULL;
    }
    tralloc_error result = tralloc ( ctx, ( tralloc_context ** ) buffer_ptr, sizeof ( tralloc_buffer ) );
    if ( result != 0 ) {
        return result;
    }
    tralloc_buffer * buffer = * buffer_ptr;

    buffer->buf         = NULL;
    buffer->data_offset = 0;
    buffer->data_length = 0;
    buffer->length      = 0;
    return 0;
}

#endif

tralloc_error tralloc_buffer_prepare ( tralloc_buffer * buffer, size_t length )
{
    uint8_t * buf = buffer->buf;
    uint8_t * new_buf;
    tralloc_error result;

    if ( buf == NULL ) {
        result = tralloc ( buffer, ( tralloc_context ** ) &new_buf, sizeof ( uint8_t ) * length );
        if ( result != 0 ) {
            return result;
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
    new_buf = buf;
    result  = tralloc_realloc ( ( tralloc_context ** ) &new_buf, sizeof ( uint8_t ) * new_length );
    if ( result != 0 ) {
        return result;
    }
    if ( new_buf != buf ) {
        buffer->buf = new_buf;
    }
    buffer->length = new_length;
    return 0;
}

tralloc_error tralloc_buffer_trim ( tralloc_buffer * buffer )
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
        tralloc_error result = tralloc_free ( buf );
        if ( result != 0 ) {
            return result;
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
