// This file is part of talloc2. Copyright (C) 2013 Andrew Aladjev aladjev.andrew@gmail.com
// talloc2 is free software: you can redistribute it and/or modify it under the terms of the GNU General Lesser Public License as published by the Free Software Foundation, either version 3 of the License, or (at your option) any later version.
// talloc2 is distributed in the hope that it will be useful, but WITHOUT ANY WARRANTY; without even the implied warranty of MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU General Lesser Public License for more details.
// You should have received a copy of the GNU General Lesser Public License along with talloc2. If not, see <http://www.gnu.org/licenses/>.

#ifndef TALLOC_HELPERS_H
#define TALLOC_HELPERS_H

#include "tree.h"
#include <string.h>

// Function makes duplicate from length bytes of str.
// If parent_context is NULL function will set new chunk as root independent chunk.
// Otherwise it will obtain parent chunk from parent_context and attach new chunk to parent chunk.
// Function returns pointer to memory (with length + 1 size) or NULL if error occurred.
inline
char * talloc_strndup ( const talloc_context * parent_context, const char * str, size_t length )
{
    char * child_data = talloc ( parent_context, sizeof ( char ) * ( length + 1 ) );
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
char * talloc_strdup ( const talloc_context * parent_context, const char * str )
{
    return talloc_strndup ( parent_context, str, strlen ( str ) );
};

#endif
