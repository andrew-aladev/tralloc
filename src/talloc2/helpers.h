// This file is part of talloc2. Copyright (C) 2013 Andrew Aladjev aladjev.andrew@gmail.com
// talloc2 is free software: you can redistribute it and/or modify it under the terms of the GNU General Lesser Public License as published by the Free Software Foundation, either version 3 of the License, or (at your option) any later version.
// talloc2 is distributed in the hope that it will be useful, but WITHOUT ANY WARRANTY; without even the implied warranty of MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU General Lesser Public License for more details.
// You should have received a copy of the GNU General Lesser Public License along with talloc2. If not, see <http://www.gnu.org/licenses/>.

#ifndef TALLOC_HELPERS_H
#define TALLOC_HELPERS_H

#include <string.h>

#include "tree.h"

inline
char * talloc_strndup ( const void * parent_data, const char * str, size_t length )
{
    char * child_data = talloc ( parent_data, sizeof ( char ) * ( length + 1 ) );
    if ( child_data == NULL ) {
        return NULL;
    }
    memmove ( child_data, str, length );
    child_data[length] = '\0';
    return child_data;
};

inline
char * talloc_strdup ( const void * parent_data, const char * str )
{
    return talloc_strndup ( parent_data, str, strlen ( str ) );
};

#endif
