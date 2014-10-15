// This file is part of tralloc. Copyright (C) 2013 Andrew Aladjev aladjev.andrew@gmail.com
// tralloc is free software: you can redistribute it and/or modify it under the terms of the GNU General Lesser Public License as published by the Free Software Foundation, either version 3 of the License, or (at your option) any later version.
// tralloc is distributed in the hope that it will be useful, but WITHOUT ANY WARRANTY; without even the implied warranty of MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU General Lesser Public License for more details.
// You should have received a copy of the GNU General Lesser Public License along with tralloc. If not, see <http://www.gnu.org/licenses/>.

#if !defined ( TRALLOC_HELPERS_STRING_H )
#define TRALLOC_HELPERS_STRING_H

#include "../macro.h"
#include "../types.h"
#include <string.h>
#include <stdarg.h>
#include <stdio.h>

#undef _TRALLOC_INLINE
#if defined ( _TRALLOC_INCLUDED_FROM_HELPERS_STRING_C )
#    define _TRALLOC_INLINE _TRALLOC_INLINE_IN_OBJECT
#else
#    define _TRALLOC_INLINE _TRALLOC_INLINE_IN_HEADER
#endif


#if defined ( TRALLOC_EXTENSIONS )
tralloc_error _tralloc_strndup_with_extensions ( tralloc_context * parent_context, char ** child_context, tralloc_extensions extensions, const char * str, size_t str_length );

_TRALLOC_INLINE
tralloc_error tralloc_strndup_with_extensions ( tralloc_context * parent_context, char ** child_context, tralloc_extensions extensions, const char * str, size_t str_length )
{
    if ( str == NULL ) {
        return TRALLOC_ERROR_REQUIRED_ARGUMENT_IS_NULL;
    }
    return _tralloc_strndup_with_extensions ( parent_context, child_context, extensions, str, str_length );
}

_TRALLOC_INLINE
tralloc_error tralloc_strdup_with_extensions ( tralloc_context * parent_context, char ** child_context, tralloc_extensions extensions, const char * str )
{
    if ( str == NULL ) {
        return TRALLOC_ERROR_REQUIRED_ARGUMENT_IS_NULL;
    }
    return _tralloc_strndup_with_extensions ( parent_context, child_context, extensions, str, strlen ( str ) );
}
#endif

tralloc_error _tralloc_strndup ( tralloc_context * parent_context, char ** child_context, const char * str, size_t str_length );

_TRALLOC_INLINE
tralloc_error tralloc_strndup ( tralloc_context * parent_context, char ** child_context, const char * str, size_t str_length )
{
    if ( str == NULL ) {
        return TRALLOC_ERROR_REQUIRED_ARGUMENT_IS_NULL;
    }
    return _tralloc_strndup ( parent_context, child_context, str, str_length );
}

_TRALLOC_INLINE
tralloc_error tralloc_strdup ( tralloc_context * parent_context, char ** child_context, const char * str )
{
    if ( str == NULL ) {
        return TRALLOC_ERROR_REQUIRED_ARGUMENT_IS_NULL;
    }
    return _tralloc_strndup ( parent_context, child_context, str, strlen ( str ) );
}


#if defined ( TRALLOC_EXTENSIONS )
tralloc_error _tralloc_vasprintf_with_extensions ( tralloc_context * parent_context, char ** child_context, tralloc_extensions extensions, const char * format, va_list arguments );

_TRALLOC_INLINE
tralloc_error tralloc_vasprintf_with_extensions ( tralloc_context * parent_context, char ** child_context, tralloc_extensions extensions, const char * format, va_list arguments )
{
    if ( format == NULL ) {
        return TRALLOC_ERROR_REQUIRED_ARGUMENT_IS_NULL;
    }
    return _tralloc_vasprintf_with_extensions ( parent_context, child_context, extensions, format, arguments );
}

_TRALLOC_INLINE
tralloc_error tralloc_asprintf_with_extensions ( tralloc_context * parent_context, char ** child_context, tralloc_extensions extensions, const char * format, ... )
{
    if ( format == NULL ) {
        return TRALLOC_ERROR_REQUIRED_ARGUMENT_IS_NULL;
    }
    va_list arguments;
    va_start ( arguments, format );
    tralloc_error result = _tralloc_vasprintf_with_extensions ( parent_context, child_context, extensions, format, arguments );
    va_end ( arguments );
    return result;
}
#endif

tralloc_error _tralloc_vasprintf ( tralloc_context * parent_context, char ** child_context, const char * format, va_list arguments );

_TRALLOC_INLINE
tralloc_error tralloc_vasprintf ( tralloc_context * parent_context, char ** child_context, const char * format, va_list arguments )
{
    if ( format == NULL ) {
        return TRALLOC_ERROR_REQUIRED_ARGUMENT_IS_NULL;
    }
    return _tralloc_vasprintf ( parent_context, child_context, format, arguments );
}

_TRALLOC_INLINE
tralloc_error tralloc_asprintf  ( tralloc_context * parent_context, char ** child_context, const char * format, ... )
{
    if ( format == NULL ) {
        return TRALLOC_ERROR_REQUIRED_ARGUMENT_IS_NULL;
    }
    va_list arguments;
    va_start ( arguments, format );
    tralloc_error result = _tralloc_vasprintf ( parent_context, child_context, format, arguments );
    va_end ( arguments );
    return result;
}


#endif
