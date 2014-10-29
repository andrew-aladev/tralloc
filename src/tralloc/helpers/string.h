// This file is part of tralloc. Copyright (C) 2013 Andrew Aladjev aladjev.andrew@gmail.com
// tralloc is free software: you can redistribute it and/or modify it under the terms of the GNU General Lesser Public License as published by the Free Software Foundation, either version 3 of the License, or (at your option) any later version.
// tralloc is distributed in the hope that it will be useful, but WITHOUT ANY WARRANTY; without even the implied warranty of MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU General Lesser Public License for more details.
// You should have received a copy of the GNU General Lesser Public License along with tralloc. If not, see <http://www.gnu.org/licenses/>.

#if !defined ( TRALLOC_HELPERS_STRING_H )
#define TRALLOC_HELPERS_STRING_H

#include "../tree/alloc.h"
#include <string.h>
#include <stdarg.h>
#include <stdio.h>

#undef _TRALLOC_INLINE
#if defined ( _TRALLOC_INCLUDED_FROM_HELPERS_STRING_C )
#    define _TRALLOC_INLINE _TRALLOC_INLINE_IN_OBJECT
#else
#    define _TRALLOC_INLINE _TRALLOC_INLINE_IN_HEADER
#endif


tralloc_error _tralloc_strndup   ( _tralloc_alloc_options * options, char ** child_context, const char * str, size_t str_length );
tralloc_error _tralloc_vasprintf ( _tralloc_alloc_options * options, char ** child_context, const char * format, va_list arguments );

#if defined ( TRALLOC_DEBUG_LOG )
_TRALLOC_INLINE
tralloc_error _tralloc_debug_log_forward_to_strndup ( const char * file, size_t line, tralloc_context * parent_context, char ** child_context, const char * str, size_t str_length )
{
    _tralloc_alloc_options options;
    options.file           = file;
    options.line           = line;
    options.parent_context = parent_context;
    options.zero           = false;

#   if defined ( TRALLOC_EXTENSIONS )
    options.extensions = 0;
#   endif

    return _tralloc_strndup ( &options, child_context, str, str_length );
}
_TRALLOC_INLINE
tralloc_error _tralloc_debug_log_strndup ( const char * file, size_t line, tralloc_context * parent_context, char ** child_context, const char * str, size_t str_length )
{
    if ( str == NULL ) {
        return TRALLOC_ERROR_REQUIRED_ARGUMENT_IS_NULL;
    }
    return _tralloc_debug_log_forward_to_strndup ( file, line, parent_context, child_context, str, str_length );
}
_TRALLOC_INLINE
tralloc_error _tralloc_debug_log_strdup ( const char * file, size_t line, tralloc_context * parent_context, char ** child_context, const char * str )
{
    if ( str == NULL ) {
        return TRALLOC_ERROR_REQUIRED_ARGUMENT_IS_NULL;
    }
    return _tralloc_debug_log_forward_to_strndup ( file, line, parent_context, child_context, str, strlen ( str ) );
}

_TRALLOC_INLINE
tralloc_error _tralloc_debug_log_vasprintf ( const char * file, size_t line, tralloc_context * parent_context, char ** child_context, const char * format, va_list arguments )
{
    _tralloc_alloc_options options;
    options.file           = file;
    options.line           = line;
    options.parent_context = parent_context;
    options.zero           = false;

#   if defined ( TRALLOC_EXTENSIONS )
    options.extensions = 0;
#   endif

    return _tralloc_vasprintf ( &options, child_context, format, arguments );
}
_TRALLOC_INLINE
tralloc_error _tralloc_debug_log_asprintf ( const char * file, size_t line, tralloc_context * parent_context, char ** child_context, const char * format, ... )
{
    va_list arguments;
    va_start ( arguments, format );
    tralloc_error result = _tralloc_debug_log_vasprintf ( file, line, parent_context, child_context, format, arguments );
    va_end ( arguments );
    return result;
}

#define tralloc_strndup(...)   _tralloc_debug_log_strndup   (__FILE__, __LINE__, __VA_ARGS__)
#define tralloc_strdup(...)    _tralloc_debug_log_strdup    (__FILE__, __LINE__, __VA_ARGS__)
#define tralloc_vasprintf(...) _tralloc_debug_log_vasprintf (__FILE__, __LINE__, __VA_ARGS__)
#define tralloc_asprintf(...)  _tralloc_debug_log_asprintf  (__FILE__, __LINE__, __VA_ARGS__)

#if defined ( TRALLOC_EXTENSIONS )
_TRALLOC_INLINE
tralloc_error _tralloc_debug_log_forward_to_strndup_with_extensions ( const char * file, size_t line, tralloc_context * parent_context, char ** child_context, tralloc_extensions extensions, const char * str, size_t str_length )
{
    _tralloc_alloc_options options;
    options.file           = file;
    options.line           = line;
    options.parent_context = parent_context;
    options.zero           = false;
    options.extensions     = extensions;
    return _tralloc_strndup ( &options, child_context, str, str_length );
}
_TRALLOC_INLINE
tralloc_error _tralloc_debug_log_strndup_with_extensions ( const char * file, size_t line, tralloc_context * parent_context, char ** child_context, tralloc_extensions extensions, const char * str, size_t str_length )
{
    if ( str == NULL ) {
        return TRALLOC_ERROR_REQUIRED_ARGUMENT_IS_NULL;
    }
    return _tralloc_debug_log_forward_to_strndup_with_extensions ( file, line, parent_context, child_context, extensions, str, str_length );
}
_TRALLOC_INLINE
tralloc_error _tralloc_debug_log_strdup_with_extensions ( const char * file, size_t line, tralloc_context * parent_context, char ** child_context, tralloc_extensions extensions, const char * str )
{
    if ( str == NULL ) {
        return TRALLOC_ERROR_REQUIRED_ARGUMENT_IS_NULL;
    }
    return _tralloc_debug_log_forward_to_strndup_with_extensions ( file, line, parent_context, child_context, extensions, str, strlen ( str ) );
}

_TRALLOC_INLINE
tralloc_error _tralloc_debug_log_vasprintf_with_extensions ( const char * file, size_t line, tralloc_context * parent_context, char ** child_context, tralloc_extensions extensions, const char * format, va_list arguments )
{
    _tralloc_alloc_options options;
    options.file           = file;
    options.line           = line;
    options.parent_context = parent_context;
    options.zero           = false;
    options.extensions     = extensions;
    return _tralloc_vasprintf ( &options, child_context, format, arguments );
}
_TRALLOC_INLINE
tralloc_error _tralloc_debug_log_asprintf_with_extensions ( const char * file, size_t line, tralloc_context * parent_context, char ** child_context, tralloc_extensions extensions, const char * format, ... )
{
    va_list arguments;
    va_start ( arguments, format );
    tralloc_error result = _tralloc_debug_log_vasprintf_with_extensions ( file, line, parent_context, child_context, extensions, format, arguments );
    va_end ( arguments );
    return result;
}

#define tralloc_strndup_with_extensions(...)   _tralloc_debug_log_strndup_with_extensions   (__FILE__, __LINE__, __VA_ARGS__)
#define tralloc_strdup_with_extensions(...)    _tralloc_debug_log_strdup_with_extensions    (__FILE__, __LINE__, __VA_ARGS__)
#define tralloc_vasprintf_with_extensions(...) _tralloc_debug_log_vasprintf_with_extensions (__FILE__, __LINE__, __VA_ARGS__)
#define tralloc_asprintf_with_extensions(...)  _tralloc_debug_log_asprintf_with_extensions  (__FILE__, __LINE__, __VA_ARGS__)

#endif

#else

_TRALLOC_INLINE
tralloc_error _tralloc_forward_to_strndup ( tralloc_context * parent_context, char ** child_context, const char * str, size_t str_length )
{
    _tralloc_alloc_options options;
    options.parent_context = parent_context;
    options.zero           = false;

#   if defined ( TRALLOC_EXTENSIONS )
    options.extensions = 0;
#   endif

    return _tralloc_strndup ( &options, child_context, str, str_length );
}
_TRALLOC_INLINE
tralloc_error tralloc_strndup ( tralloc_context * parent_context, char ** child_context, const char * str, size_t str_length )
{
    if ( str == NULL ) {
        return TRALLOC_ERROR_REQUIRED_ARGUMENT_IS_NULL;
    }
    return _tralloc_forward_to_strndup ( parent_context, child_context, str, str_length );
}
_TRALLOC_INLINE
tralloc_error tralloc_strdup ( const char * file, size_t line, tralloc_context * parent_context, char ** child_context, const char * str )
{
    if ( str == NULL ) {
        return TRALLOC_ERROR_REQUIRED_ARGUMENT_IS_NULL;
    }
    return _tralloc_forward_to_strndup ( parent_context, child_context, str, strlen ( str ) );
}

_TRALLOC_INLINE
tralloc_error tralloc_vasprintf ( const char * file, size_t line, tralloc_context * parent_context, char ** child_context, const char * format, va_list arguments )
{
    _tralloc_alloc_options options;
    options.parent_context = parent_context;
    options.zero           = false;

#   if defined ( TRALLOC_EXTENSIONS )
    options.extensions = 0;
#   endif

    return _tralloc_vasprintf ( &options, child_context, format, arguments );
}
_TRALLOC_INLINE
tralloc_error tralloc_asprintf ( const char * file, size_t line, tralloc_context * parent_context, char ** child_context, const char * format, ... )
{
    va_list arguments;
    va_start ( arguments, format );
    tralloc_error result = tralloc_vasprintf ( file, line, parent_context, child_context, format, arguments );
    va_end ( arguments );
    return result;
}

#if defined ( TRALLOC_EXTENSIONS )
_TRALLOC_INLINE
tralloc_error _tralloc_forward_to_strndup_with_extensions ( tralloc_context * parent_context, char ** child_context, tralloc_extensions extensions, const char * str, size_t str_length )
{
    _tralloc_alloc_options options;
    options.parent_context = parent_context;
    options.zero           = false;
    options.extensions     = extensions;
    return _tralloc_strndup ( &options, child_context, str, str_length );
}
_TRALLOC_INLINE
tralloc_error tralloc_strndup_with_extensions ( tralloc_context * parent_context, char ** child_context, tralloc_extensions extensions, const char * str, size_t str_length )
{
    if ( str == NULL ) {
        return TRALLOC_ERROR_REQUIRED_ARGUMENT_IS_NULL;
    }
    return _tralloc_forward_to_strndup_with_extensions ( parent_context, child_context, extensions, str, str_length );
}
_TRALLOC_INLINE
tralloc_error tralloc_strdup_with_extensions ( tralloc_context * parent_context, char ** child_context, tralloc_extensions extensions, const char * str )
{
    if ( str == NULL ) {
        return TRALLOC_ERROR_REQUIRED_ARGUMENT_IS_NULL;
    }
    return _tralloc_forward_to_strndup_with_extensions ( parent_context, child_context, extensions, str, strlen ( str ) );
}

_TRALLOC_INLINE
tralloc_error tralloc_vasprintf_with_extensions ( tralloc_context * parent_context, char ** child_context, const char * format, tralloc_extensions extensions, va_list arguments )
{
    _tralloc_alloc_options options;
    options.parent_context = parent_context;
    options.zero           = false;
    options.extensions     = extensions;
    return _tralloc_vasprintf ( &options, child_context, format, arguments );
}
_TRALLOC_INLINE
tralloc_error tralloc_asprintf_with_extensions ( const char * file, size_t line, tralloc_context * parent_context, char ** child_context, tralloc_extensions extensions, const char * format, ... )
{
    va_list arguments;
    va_start ( arguments, format );
    tralloc_error result = tralloc_vasprintf_with_extensions ( file, line, parent_context, child_context, extensions, format, arguments );
    va_end ( arguments );
    return result;
}
#endif

#endif


#endif
