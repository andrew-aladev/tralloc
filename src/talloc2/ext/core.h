// This file is part of talloc2. Copyright (C) 2013 Andrew Aladjev aladjev.andrew@gmail.com
// talloc2 is free software: you can redistribute it and/or modify it under the terms of the GNU General Lesser Public License as published by the Free Software Foundation, either version 3 of the License, or (at your option) any later version.
// talloc2 is distributed in the hope that it will be useful, but WITHOUT ANY WARRANTY; without even the implied warranty of MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU General Lesser Public License for more details.
// You should have received a copy of the GNU General Lesser Public License along with talloc2. If not, see <http://www.gnu.org/licenses/>.

#ifndef TALLOC_EXT_H
#define TALLOC_EXT_H

#include <stdlib.h>

#include "../types.h"

inline
talloc_ext * talloc_ext_new ( uint8_t length )
{
    talloc_ext * ext = malloc ( sizeof ( talloc_ext ) );
    if ( ext == NULL ) {
        return NULL;
    }

    if ( length != 1 ) {
        void ** data = malloc ( sizeof ( uintptr_t ) * length );
        if ( data == NULL ) {
            free ( ext );
            return NULL;
        }
        uint8_t index;
        for ( index = 0; index < length; index ++ ) {
            data[index] = NULL;
        }
        ext->data = data;
    }

    ext->length = length;
    return ext;
}

inline
uint8_t talloc_ext_grow ( talloc_ext * ext, uint8_t length )
{
    uint8_t current_length = ext->length;
    if ( current_length >= length ) {
        return 0;
    }
    void ** data;
    if ( current_length == 1 ) {
        data = malloc ( sizeof ( uintptr_t ) * length );
        if ( data == NULL ) {
            return 1;
        }
        uint8_t index;
        for ( index = 1; index < length; index ++ ) {
            data[index] = NULL;
        }
        data[0] = ext->data;
    } else {
        data = realloc ( ext->data, sizeof ( uintptr_t ) * length );
        if ( data == NULL ) {
            return 1;
        }
        uint8_t index;
        for ( index = current_length; index < length; index ++ ) {
            data[index] = NULL;
        }
    }

    ext->data   = data;
    ext->length = length;
    return 0;
}

inline
uint8_t talloc_ext_set ( talloc_chunk * child, uint8_t mode_index, void * data )
{
    uint8_t length   = mode_index + 1;
    talloc_ext * ext = child->ext;

    if ( ext == NULL ) {
        ext = talloc_ext_new ( length );
        if ( ext == NULL ) {
            return 1;
        }
        child->ext = ext;
    } else {
        if ( talloc_ext_grow ( ext, length ) != 0 ) {
            return 2;
        }
    }

    if ( ext->length == 1 ) {
        ext->data = data;
    } else {
        ext->data[mode_index] = data;
    }

    return 0;
}

inline
void * talloc_ext_get ( talloc_chunk * child, uint8_t mode_index )
{
    talloc_ext * ext = child->ext;
    if ( ext == NULL ) {
        return NULL;
    }

    uint8_t length         = mode_index + 1;
    uint8_t current_length = ext->length;
    if ( current_length < length ) {
        return NULL;
    }

    if ( current_length == 1 ) {
        return ext->data;
    } else {
        return ext->data[mode_index];
    }
}

inline
void talloc_ext_free ( talloc_chunk * child )
{
    talloc_ext * ext = child->ext;
    if ( ext == NULL ) {
        return;
    }
    if ( ext->length > 1 ) {
        free ( ext->data );
    }
    free ( ext );
}

#endif
