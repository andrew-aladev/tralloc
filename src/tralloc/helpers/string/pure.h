// This file is part of tralloc. Copyright (C) 2013 Andrew Aladjev aladjev.andrew@gmail.com
// tralloc is free software: you can redistribute it and/or modify it under the terms of the GNU General Lesser Public License as published by the Free Software Foundation, either version 3 of the License, or (at your option) any later version.
// tralloc is distributed in the hope that it will be useful, but WITHOUT ANY WARRANTY; without even the implied warranty of MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU General Lesser Public License for more details.
// You should have received a copy of the GNU General Lesser Public License along with tralloc. If not, see <http://www.gnu.org/licenses/>.

#ifndef TRALLOC_HELPERS_STRING_PURE_H
#define TRALLOC_HELPERS_STRING_PURE_H

#include "../../macro.h"
#include "../../types.h"
#include <stdarg.h>
#include <string.h>

#undef _TRALLOC_INLINE
#ifdef _TRALLOC_HELPERS_STRING_PURE_INCLUDED_FROM_OBJECT
#    define _TRALLOC_INLINE _TRALLOC_INLINE_IN_OBJECT
#else
#    define _TRALLOC_INLINE _TRALLOC_INLINE_IN_HEADER
#endif


tralloc_error _tralloc_strndup ( tralloc_context * parent_context, char ** child_context, const char * str, size_t length );

_TRALLOC_INLINE
tralloc_error tralloc_strndup ( tralloc_context * parent_context, char ** child_context, const char * str, size_t length )
{
    if ( str == NULL ) {
        return TRALLOC_ERROR_REQUIRED_ARGUMENT_IS_NULL;
    }
    return _tralloc_strndup ( parent_context, child_context, str, length );
}

_TRALLOC_INLINE
tralloc_error tralloc_strdup ( tralloc_context * parent_context, char ** child_context, const char * str )
{
    if ( str == NULL ) {
        return TRALLOC_ERROR_REQUIRED_ARGUMENT_IS_NULL;
    }
    return _tralloc_strndup ( parent_context, child_context, str, strlen ( str ) );
}


tralloc_error tralloc_vasprintf ( tralloc_context * parent_context, char ** child_context, const char * format, va_list arguments );
tralloc_error tralloc_asprintf  ( tralloc_context * parent_context, char ** child_context, const char * format, ... );


#endif
