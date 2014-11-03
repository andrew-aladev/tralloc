// This file is part of tralloc. Copyright (C) 2013 Andrew Aladjev aladjev.andrew@gmail.com
// tralloc is free software: you can redistribute it and/or modify it under the terms of the GNU General Lesser Public License as published by the Free Software Foundation, either version 3 of the License, or (at your option) any later version.
// tralloc is distributed in the hope that it will be useful, but WITHOUT ANY WARRANTY; without even the implied warranty of MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU General Lesser Public License for more details.
// You should have received a copy of the GNU General Lesser Public License along with tralloc. If not, see <http://www.gnu.org/licenses/>.

#define _TRALLOC_INCLUDED_FROM_HELPERS_STRING_C
#include <tralloc/helpers/string.h>
#include <tralloc/tree/free.h>


tralloc_error _tralloc_strndup ( _tralloc_alloc_options * options, char ** child_context, const char * str, size_t str_length )
{
    options->zero   = TRALLOC_FALSE;
    options->length = sizeof ( char ) * ( str_length + 1 );
    tralloc_error result = _tralloc_alloc ( options, ( tralloc_context ** ) child_context );
    if ( result != 0 ) {
        return result;
    }
    memmove ( * child_context, str, str_length );
    ( * child_context ) [str_length] = '\0';
    return 0;
}

tralloc_error _tralloc_vasprintf ( _tralloc_alloc_options * options, char ** child_context, const char * format, va_list arguments )
{
    if ( format == NULL ) {
        return TRALLOC_ERROR_REQUIRED_ARGUMENT_IS_UNDEFINED;
    }

    va_list arguments_copy;
    va_copy ( arguments_copy, arguments );
    int predicted_str_length = vsnprintf ( NULL, 0, format, arguments_copy );
    va_end ( arguments_copy );
    if ( predicted_str_length <= 0 ) {
        return TRALLOC_ERROR_PRINTF_FAILED;
    }

    options->zero   = TRALLOC_FALSE;
    options->length = sizeof ( char ) * ( predicted_str_length + 1 );
    tralloc_error result = _tralloc_alloc ( options, ( tralloc_context ** ) child_context );
    if ( result != 0 ) {
        return result;
    }

    va_copy ( arguments_copy, arguments );
    int str_length = vsnprintf ( * child_context, predicted_str_length + 1, format, arguments_copy );
    va_end ( arguments_copy );
    if ( str_length != predicted_str_length ) {
        tralloc_free ( * child_context );
        return TRALLOC_ERROR_PRINTF_FAILED;
    }
    return 0;
}
