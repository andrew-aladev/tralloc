// This file is part of talloc2. Copyright (C) 2013 Andrew Aladjev aladjev.andrew@gmail.com
// talloc2 is free software: you can redistribute it and/or modify it under the terms of the GNU General Lesser Public License as published by the Free Software Foundation, either version 3 of the License, or (at your option) any later version.
// talloc2 is distributed in the hope that it will be useful, but WITHOUT ANY WARRANTY; without even the implied warranty of MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU General Lesser Public License for more details.
// You should have received a copy of the GNU General Lesser Public License along with talloc2. If not, see <http://www.gnu.org/licenses/>.

#ifndef TALLOC_UTILS_BUFFER_H
#define TALLOC_UTILS_BUFFER_H

#include "../tree.h"

#include <string.h>

typedef struct talloc_buffer_t {
    char * buf;
    size_t length;
} talloc_buffer;

inline
talloc_buffer * talloc_buffer_new ( void * ctx )
{
    talloc_buffer * buffer = talloc ( ctx, sizeof ( talloc_buffer ) );
    if ( buffer == NULL ) {
        return NULL;
    }
    buffer->buf    = NULL;
    buffer->length = 0;
    return buffer;
}

char *  talloc_buffer_get ( talloc_buffer * buffer, size_t length );
uint8_t talloc_buffer_cut ( talloc_buffer * buffer, size_t diff );

#endif