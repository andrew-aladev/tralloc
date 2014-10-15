// This file is part of tralloc. Copyright (C) 2013 Andrew Aladjev aladjev.andrew@gmail.com
// tralloc is free software: you can redistribute it and/or modify it under the terms of the GNU General Lesser Public License as published by the Free Software Foundation, either version 3 of the License, or (at your option) any later version.
// tralloc is distributed in the hope that it will be useful, but WITHOUT ANY WARRANTY; without even the implied warranty of MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU General Lesser Public License for more details.
// You should have received a copy of the GNU General Lesser Public License along with tralloc. If not, see <http://www.gnu.org/licenses/>.

#define _TRALLOC_INCLUDED_FROM_HELPERS_STRING_C
#include <tralloc/helpers/string.h>
#include <tralloc/tree/alloc.h>
#include <tralloc/tree/free.h>


typedef struct _tralloc_str_chunk_environment_type {
    tralloc_context * parent_context;

#   if defined ( TRALLOC_EXTENSIONS )
    tralloc_extensions extensions;
#   endif

} _tralloc_str_chunk_environment;

typedef tralloc_error ( * _tralloc_str_constructor_function ) ( _tralloc_str_chunk_environment * chunk_environment, char ** child_context, size_t length );

#if defined ( TRALLOC_EXTENSIONS )
static
tralloc_error _tralloc_str_constructor_with_extensions ( _tralloc_str_chunk_environment * chunk_environment, char ** child_context, size_t length )
{
    return tralloc_new_with_extensions ( chunk_environment->parent_context, ( tralloc_context ** ) child_context, length, chunk_environment->extensions );
}
#endif

static
tralloc_error _tralloc_str_constructor ( _tralloc_str_chunk_environment * chunk_environment, char ** child_context, size_t length )
{
    return tralloc_new ( chunk_environment->parent_context, ( tralloc_context ** ) child_context, length );
}


static
tralloc_error _tralloc_strndup_internal ( _tralloc_str_constructor_function constructor, _tralloc_str_chunk_environment * chunk_environment, char ** child_context, const char * str, size_t str_length )
{
    tralloc_error result = constructor ( chunk_environment, child_context, sizeof ( char ) * ( str_length + 1 ) );
    if ( result != 0 ) {
        return result;
    }
    memmove ( * child_context, str, str_length );
    ( * child_context ) [str_length] = '\0';
    return 0;
}

#if defined ( TRALLOC_EXTENSIONS )
tralloc_error _tralloc_strndup_with_extensions ( tralloc_context * parent_context, char ** child_context, tralloc_extensions extensions, const char * str, size_t str_length )
{
    _tralloc_str_chunk_environment chunk_environment;
    chunk_environment.parent_context = parent_context;
    chunk_environment.extensions     = extensions;
    return _tralloc_strndup_internal ( _tralloc_str_constructor_with_extensions, &chunk_environment, child_context, str, str_length );
}
#endif

tralloc_error _tralloc_strndup ( tralloc_context * parent_context, char ** child_context, const char * str, size_t str_length )
{
    _tralloc_str_chunk_environment chunk_environment;
    chunk_environment.parent_context = parent_context;
    return _tralloc_strndup_internal ( _tralloc_str_constructor, &chunk_environment, child_context, str, str_length );
}


static
tralloc_error _tralloc_vasprintf_internal ( _tralloc_str_constructor_function constructor, _tralloc_str_chunk_environment * chunk_environment, char ** child_context, const char * format, va_list arguments )
{
    va_list arguments_copy;
    va_copy ( arguments_copy, arguments );
    int predicted_str_length = vsnprintf ( NULL, 0, format, arguments_copy );
    va_end ( arguments_copy );
    if ( predicted_str_length <= 0 ) {
        return TRALLOC_ERROR_PRINTF_FAILED;
    }

    tralloc_error result = constructor ( chunk_environment, child_context, sizeof ( char ) * ( predicted_str_length + 1 ) );
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

#if defined ( TRALLOC_EXTENSIONS )
tralloc_error _tralloc_vasprintf_with_extensions ( tralloc_context * parent_context, char ** child_context, tralloc_extensions extensions, const char * format, va_list arguments )
{
    _tralloc_str_chunk_environment chunk_environment;
    chunk_environment.parent_context = parent_context;
    chunk_environment.extensions     = extensions;
    return _tralloc_vasprintf_internal ( _tralloc_str_constructor_with_extensions, &chunk_environment, child_context, format, arguments );
}
#endif

tralloc_error _tralloc_vasprintf ( tralloc_context * parent_context, char ** child_context, const char * format, va_list arguments )
{
    _tralloc_str_chunk_environment chunk_environment;
    chunk_environment.parent_context = parent_context;
    return _tralloc_vasprintf_internal ( _tralloc_str_constructor, &chunk_environment, child_context, format, arguments );
}
