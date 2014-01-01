// This file is part of tralloc. Copyright (C) 2013 Andrew Aladjev aladjev.andrew@gmail.com
// tralloc is free software: you can redistribute it and/or modify it under the terms of the GNU General Lesser Public License as published by the Free Software Foundation, either version 3 of the License, or (at your option) any later version.
// tralloc is distributed in the hope that it will be useful, but WITHOUT ANY WARRANTY; without even the implied warranty of MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU General Lesser Public License for more details.
// You should have received a copy of the GNU General Lesser Public License along with tralloc. If not, see <http://www.gnu.org/licenses/>.

#include "extensions.h"


extern inline char * tralloc_strndup_with_extensions ( tralloc_context * parent_context, uint8_t extensions, const char * str, size_t length );
extern inline char * tralloc_strndup                 ( tralloc_context * parent_context, const char * str, size_t length );

extern inline char * tralloc_strdup_with_extensions ( tralloc_context * parent_context, uint8_t extensions, const char * str );
extern inline char * tralloc_strdup                 ( tralloc_context * parent_context, const char * str );

extern inline char * tralloc_vasprintf_with_extensions ( tralloc_context * parent_context, uint8_t extensions, const char * format, va_list arguments );
extern inline char * tralloc_vasprintf                 ( tralloc_context * parent_context, const char * format, va_list arguments );
extern inline char * tralloc_asprintf_with_extensions  ( tralloc_context * parent_context, uint8_t extensions, const char * format, ... );
extern inline char * tralloc_asprintf                  ( tralloc_context * parent_context, const char * format, ... );
