// This file is part of tralloc. Copyright (C) 2013 Andrew Aladjev aladjev.andrew@gmail.com
// tralloc is free software: you can redistribute it and/or modify it under the terms of the GNU General Lesser Public License as published by the Free Software Foundation, either version 3 of the License, or (at your option) any later version.
// tralloc is distributed in the hope that it will be useful, but WITHOUT ANY WARRANTY; without even the implied warranty of MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU General Lesser Public License for more details.
// You should have received a copy of the GNU General Lesser Public License along with tralloc. If not, see <http://www.gnu.org/licenses/>.

#include "string.h"

extern inline char * tralloc_strndup   ( const tralloc_context * parent_context, const char * str, size_t length );
extern inline char * tralloc_strdup    ( const tralloc_context * parent_context, const char * str );
extern inline char * tralloc_vasprintf ( const tralloc_context * parent_context, const char * format, va_list arguments );
extern inline char * tralloc_asprintf  ( const tralloc_context * parent_context, const char * format, ... );