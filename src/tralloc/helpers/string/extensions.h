// This file is part of tralloc. Copyright (C) 2013 Andrew Aladjev aladjev.andrew@gmail.com
// tralloc is free software: you can redistribute it and/or modify it under the terms of the GNU General Lesser Public License as published by the Free Software Foundation, either version 3 of the License, or (at your option) any later version.
// tralloc is distributed in the hope that it will be useful, but WITHOUT ANY WARRANTY; without even the implied warranty of MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU General Lesser Public License for more details.
// You should have received a copy of the GNU General Lesser Public License along with tralloc. If not, see <http://www.gnu.org/licenses/>.

#ifndef TRALLOC_HELPERS_STRING_EXTENSIONS_H
#define TRALLOC_HELPERS_STRING_EXTENSIONS_H

#include "../../tree.h"
#include <string.h>
#include <stdio.h>
#include <stdarg.h>


inline
char * tralloc_strndup_with_extensions ( tralloc_context * parent_context, uint8_t extensions, const char * str, size_t length )
{
    char * child_data = tralloc_with_extensions ( parent_context, extensions, sizeof ( char ) * ( length + 1 ) );
    if ( child_data == NULL ) {
        return NULL;
    }
    memmove ( child_data, str, length );
    child_data[length] = '\0';
    return child_data;
}

inline
char * tralloc_strndup ( tralloc_context * parent_context, const char * str, size_t length )
{
    return tralloc_strndup_with_extensions ( parent_context, 0, str, length );
}

inline
char * tralloc_strdup_with_extensions ( tralloc_context * parent_context, uint8_t extensions, const char * str )
{
    return tralloc_strndup_with_extensions ( parent_context, extensions, str, strlen ( str ) );
}

inline
char * tralloc_strdup ( tralloc_context * parent_context, const char * str )
{
    return tralloc_strdup_with_extensions ( parent_context, 0, str );
}


inline
char * tralloc_vasprintf_with_extensions ( tralloc_context * parent_context, uint8_t extensions, const char * format, va_list arguments )
{
    va_list arguments_copy;

    va_copy ( arguments_copy, arguments );
    int predicted_length = vsnprintf ( NULL, 0, format, arguments_copy );
    va_end ( arguments_copy );
    if ( predicted_length <= 0 ) {
        return NULL;
    }
    predicted_length++;

    char * result = tralloc_with_extensions ( parent_context, extensions, sizeof ( char ) * predicted_length );
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

inline
char * tralloc_vasprintf ( tralloc_context * parent_context, const char * format, va_list arguments )
{
    return tralloc_vasprintf_with_extensions ( parent_context, 0, format, arguments );
}

inline
char * tralloc_asprintf_with_extensions ( tralloc_context * parent_context, uint8_t extensions, const char * format, ... )
{
    va_list arguments;
    va_start ( arguments, format );
    char * result = tralloc_vasprintf_with_extensions ( parent_context, extensions, format, arguments );
    va_end ( arguments );
    return result;
}

inline
char * tralloc_asprintf ( tralloc_context * parent_context, const char * format, ... )
{
    va_list arguments;
    va_start ( arguments, format );
    char * result = tralloc_vasprintf ( parent_context, format, arguments );
    va_end ( arguments );
    return result;
}


#endif
