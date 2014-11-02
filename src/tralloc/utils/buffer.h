// This file is part of tralloc. Copyright (C) 2013 Andrew Aladjev aladjev.andrew@gmail.com
// tralloc is free software: you can redistribute it and/or modify it under the terms of the GNU General Lesser Public License as published by the Free Software Foundation, either version 3 of the License, or (at your option) any later version.
// tralloc is distributed in the hope that it will be useful, but WITHOUT ANY WARRANTY; without even the implied warranty of MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU General Lesser Public License for more details.
// You should have received a copy of the GNU General Lesser Public License along with tralloc. If not, see <http://www.gnu.org/licenses/>.

#if !defined ( TRALLOC_UTILS_BUFFER_H )
#define TRALLOC_UTILS_BUFFER_H

#include "../tree/alloc.h"

#undef _TRALLOC_INLINE
#if defined ( _TRALLOC_INCLUDED_FROM_UTILS_BUFFER_C )
#    define _TRALLOC_INLINE _TRALLOC_INLINE_IN_OBJECT
#else
#    define _TRALLOC_INLINE _TRALLOC_INLINE_IN_HEADER
#endif


// User have some stream of data.
// He wants to write portions of data to the stream.
// He wants to read fixed portions of data from stream.

// "capacity" is buffer capacity, "offset" is a offset to the valid data and "length" is the length of valid data.
// User can write data to the empty bytes after "length" part and read data from "length" part.
// User can prepare buffer for the portion of data.
// User can trim buffer from the left, from the right and from both sides at once.
// User can resize buffer to the size >= "length".

// <-- empty bytes --> <-- valid_data  --> <-- empty bytes -->
// <----------------------- capacity ------------------------>
// <---- offset -----> <---- length -----> -------------------

typedef struct _tralloc_buffer_type {
    uint8_t * data;
    size_t    offset;
    size_t    length;
    size_t    capacity;
} tralloc_buffer;


tralloc_error _tralloc_new_buffer ( _tralloc_alloc_options * options, tralloc_buffer ** buffer_ptr, size_t capacity );

#if defined ( TRALLOC_DEBUG_LOG )
_TRALLOC_INLINE
tralloc_error _tralloc_debug_log_new_buffer ( const char * file, size_t line, tralloc_context * parent_context, tralloc_buffer ** buffer_ptr, size_t capacity )
{
    _tralloc_alloc_options options;
    options.file           = file;
    options.line           = line;
    options.parent_context = parent_context;

#   if defined ( TRALLOC_EXTENSIONS )
    options.extensions = 0;
#   endif

    return _tralloc_new_buffer ( &options, buffer_ptr, capacity );
}

#define tralloc_new_buffer(...) _tralloc_debug_log_new_buffer (__FILE__, __LINE__, __VA_ARGS__)

#if defined ( TRALLOC_EXTENSIONS )

_TRALLOC_INLINE
tralloc_error _tralloc_debug_log_new_buffer_with_extensions ( const char * file, size_t line, tralloc_context * parent_context, tralloc_buffer ** buffer_ptr, tralloc_extensions extensions, size_t capacity )
{
    _tralloc_alloc_options options;
    options.file           = file;
    options.line           = line;
    options.parent_context = parent_context;
    options.extensions     = extensions;
    return _tralloc_new_buffer ( &options, buffer_ptr, capacity );
}

#define tralloc_new_buffer_with_extensions(...) _tralloc_debug_log_new_buffer_with_extensions (__FILE__, __LINE__, __VA_ARGS__)

#endif

#else

_TRALLOC_INLINE
tralloc_error tralloc_new_buffer ( tralloc_context * parent_context, tralloc_buffer ** buffer_ptr, size_t capacity )
{
    _tralloc_alloc_options options;
    options.parent_context = parent_context;

#   if defined ( TRALLOC_EXTENSIONS )
    options.extensions = 0;
#   endif

    return _tralloc_new_buffer ( &options, buffer_ptr, capacity );
}

#if defined ( TRALLOC_EXTENSIONS )
_TRALLOC_INLINE
tralloc_error tralloc_new_buffer_with_extensions ( tralloc_context * parent_context, tralloc_buffer ** buffer_ptr, tralloc_extensions extensions, size_t capacity )
{
    _tralloc_alloc_options options;
    options.parent_context = parent_context;
    options.extensions     = extensions;
    return _tralloc_new_buffer ( &options, buffer_ptr, capacity );
}
#endif

#endif


_TRALLOC_INLINE
size_t tralloc_buffer_get_read_length ( const tralloc_buffer * buffer )
{
    return buffer->length;
}

_TRALLOC_INLINE
tralloc_error tralloc_buffer_add_read_length ( tralloc_buffer * buffer, size_t read_length )
{
    if ( tralloc_buffer_get_read_length ( buffer ) < read_length ) {
        return TRALLOC_ERROR_UTILS_BUFFER_OVERFLOW;
    }
    buffer->offset += read_length;
    buffer->length -= read_length;
    return 0;
}

_TRALLOC_INLINE
uint8_t * tralloc_buffer_get_read_pointer ( const tralloc_buffer * buffer )
{
    return buffer->data + buffer->offset;
}

_TRALLOC_INLINE
size_t tralloc_buffer_get_write_length ( const tralloc_buffer * buffer )
{
    return buffer->capacity - buffer->offset - buffer->length;
}

_TRALLOC_INLINE
tralloc_error tralloc_buffer_add_write_length ( tralloc_buffer * buffer, size_t write_length )
{
    if ( tralloc_buffer_get_write_length ( buffer ) < write_length ) {
        return TRALLOC_ERROR_UTILS_BUFFER_OVERFLOW;
    }
    buffer->length += write_length;
    return 0;
}

_TRALLOC_INLINE
uint8_t * tralloc_buffer_get_write_pointer ( const tralloc_buffer * buffer )
{
    return buffer->data + buffer->offset + buffer->length;
}

tralloc_error _tralloc_buffer_resize ( tralloc_buffer * buffer, size_t new_capacity );

_TRALLOC_INLINE
tralloc_error tralloc_buffer_resize ( tralloc_buffer * buffer, size_t new_capacity )
{
    // Current valid data's "length" can't be less than new capacity.
    if ( buffer->length > new_capacity ) {
        return TRALLOC_ERROR_UTILS_BUFFER_OVERFLOW;
    }
    return _tralloc_buffer_resize ( buffer, new_capacity );
}

_TRALLOC_INLINE
tralloc_error tralloc_buffer_prepare_write_length ( tralloc_buffer * buffer, size_t new_write_length )
{
    size_t write_length = tralloc_buffer_get_write_length ( buffer );
    if ( write_length < new_write_length ) {
        return tralloc_buffer_resize ( buffer, buffer->capacity + new_write_length - write_length );
    } else {
        return 0;
    }
}

tralloc_error tralloc_buffer_left_trim ( tralloc_buffer * buffer );

_TRALLOC_INLINE
tralloc_error tralloc_buffer_right_trim ( tralloc_buffer * buffer )
{
    return _tralloc_buffer_resize ( buffer, buffer->offset + buffer->length );
}

_TRALLOC_INLINE
tralloc_error tralloc_buffer_trim ( tralloc_buffer * buffer )
{
    return _tralloc_buffer_resize ( buffer, buffer->length );
}


#endif
