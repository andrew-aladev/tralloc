// This file is part of tralloc. Copyright (C) 2013 Andrew Aladjev aladjev.andrew@gmail.com
// tralloc is free software: you can redistribute it and/or modify it under the terms of the GNU General Lesser Public License as published by the Free Software Foundation, either version 3 of the License, or (at your option) any later version.
// tralloc is distributed in the hope that it will be useful, but WITHOUT ANY WARRANTY; without even the implied warranty of MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU General Lesser Public License for more details.
// You should have received a copy of the GNU General Lesser Public License along with tralloc. If not, see <http://www.gnu.org/licenses/>.

#ifndef TRALLOC_HELPERS_STRING_H
#define TRALLOC_HELPERS_STRING_H

#include "../tree.h"
#include <string.h>
#include <stdio.h>
#include <stdarg.h>

// Function makes duplicate from length bytes of str.
// If parent_context is NULL function will set new chunk as root independent chunk.
// Otherwise it will obtain parent chunk from parent_context and attach new chunk to parent chunk.
// Function returns pointer to memory (with length + 1 size) or NULL if error occurred.
inline
char * tralloc_strndup ( const tralloc_context * parent_context, const char * str, size_t length )
{
    char * child_data = tralloc ( parent_context, sizeof ( char ) * ( length + 1 ) );
    if ( child_data == NULL ) {
        return NULL;
    }
    memmove ( child_data, str, length );
    child_data[length] = '\0';
    return child_data;
};

// Function makes duplicate of str.
// If parent_context is NULL function will set new chunk as root independent chunk.
// Otherwise it will obtain parent chunk from parent_context and attach new chunk to parent chunk.
// Function returns pointer to memory (with strlen ( str ) + 1 size) or NULL if error occurred.
inline
char * tralloc_strdup ( const tralloc_context * parent_context, const char * str )
{
    return tralloc_strndup ( parent_context, str, strlen ( str ) );
};

// Function creates string from provided format.
// Function returns pointer to string or NULL if error occurred.
// Do not use this function if you care about performance.
// It runs formating two times, because there is no ability to pass custom allocator to asprintf.
inline
char * tralloc_vasprintf ( const tralloc_context * parent_context, const char * format, va_list arguments )
{
    va_list arguments_copy;

    va_copy ( arguments_copy, arguments );
    int predicted_length = vsnprintf ( NULL, 0, format, arguments_copy );
    va_end ( arguments_copy );
    if ( predicted_length <= 0 ) {
        return NULL;
    }
    predicted_length++;

    char * result = tralloc ( parent_context, sizeof ( char ) * predicted_length );
    if ( result == NULL ) {
        return NULL;
    }
    va_copy ( arguments_copy, arguments );
    int length = vsnprintf ( result, predicted_length, format, arguments_copy );
    va_end ( arguments_copy );
    if ( length + 1 != predicted_length ) {
        tralloc_free ( result );
        return NULL;
    }
    return result;
}

// Function works the same as "tralloc_vasprintf", but takes variable arguments instead of va_list.
inline
char * tralloc_asprintf ( const tralloc_context * parent_context, const char * format, ... )
{
    va_list arguments;
    va_start ( arguments, format );
    char * result = tralloc_vasprintf ( parent_context, format, arguments );
    va_end ( arguments );
    return result;
}

#endif
