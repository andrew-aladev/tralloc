// This file is part of tralloc. Copyright (C) 2013 Andrew Aladjev aladjev.andrew@gmail.com
// tralloc is free software: you can redistribute it and/or modify it under the terms of the GNU General Lesser Public License as published by the Free Software Foundation, either version 3 of the License, or (at your option) any later version.
// tralloc is distributed in the hope that it will be useful, but WITHOUT ANY WARRANTY; without even the implied warranty of MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU General Lesser Public License for more details.
// You should have received a copy of the GNU General Lesser Public License along with tralloc. If not, see <http://www.gnu.org/licenses/>.

#ifndef TRALLOC_HELPERS_H
#define TRALLOC_HELPERS_H

#include "tree.h"
#include <string.h>

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

#endif
