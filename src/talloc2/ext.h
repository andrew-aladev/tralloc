// This file is part of talloc2. Copyright (C) 2013 Andrew Aladjev aladjev.andrew@gmail.com
// talloc2 is free software: you can redistribute it and/or modify it under the terms of the GNU General Lesser Public License as published by the Free Software Foundation, either version 3 of the License, or (at your option) any later version.
// talloc2 is distributed in the hope that it will be useful, but WITHOUT ANY WARRANTY; without even the implied warranty of MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU General Lesser Public License for more details.
// You should have received a copy of the GNU General Lesser Public License along with talloc2. If not, see <http://www.gnu.org/licenses/>.

#ifndef TALLOC_EXT_H
#define TALLOC_EXT_H

#include <stdlib.h>

#include "types.h"

#ifdef TALLOC_EXT

inline
talloc_ext * talloc_ext_new ( uint8_t length )
{
    talloc_ext * ext = malloc ( sizeof ( talloc_ext ) );
    if ( ext == NULL ) {
        return NULL;
    }
    void ** data = malloc ( sizeof ( uintptr_t ) * length );
    if ( data == NULL ) {
        free ( ext );
        return NULL;
    }
    ext->data   = data;
    ext->length = length;
    return ext;
}

inline
uint8_t talloc_ext_grow ( talloc_ext * ext, uint8_t length )
{
    if ( ext->length < length ) {
        void ** data = realloc ( ext->data, sizeof ( uintptr_t ) * length );
        if ( data == NULL ) {
            return 1;
        }
        ext->data   = data;
        ext->length = length;
    }
    return 0;
}

inline
uint8_t talloc_ext_set ( talloc_chunk * child, uint8_t mode, void * data )
{
    uint8_t length   = mode + 1;
    talloc_ext * ext = child->ext;

    if ( ext == NULL ) {
        ext = talloc_ext_new ( length );
        if ( ext == NULL ) {
            return 1;
        }
        child->ext = ext;
    } else {
        if ( talloc_ext_grow ( ext, length ) ) {
            return 2;
        }
    }
    ext->data[mode] = data;

    return 0;
}

inline
void * talloc_ext_get ( talloc_chunk * child, uint8_t mode )
{
    talloc_ext * ext = child->ext;
    if ( ext == NULL ) {
        return NULL;
    }

    uint8_t length = mode + 1;
    if ( ext->length < length ) {
        return NULL;
    }
    return ext->data[mode];
}

inline
void talloc_ext_free ( talloc_chunk * child )
{
    talloc_ext * ext = child->ext;
    if ( ext != NULL ) {
        void ** data = ext->data;
        if ( data != NULL ) {
            free ( data );
        }
        free ( ext );
    }
}

#endif

#endif

