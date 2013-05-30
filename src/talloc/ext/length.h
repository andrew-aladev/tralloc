// This file is part of talloc. Copyright (C) 2013 Andrew Aladjev aladjev.andrew@gmail.com
// talloc is free software: you can redistribute it and/or modify it under the terms of the GNU General Lesser Public License as published by the Free Software Foundation, either version 3 of the License, or (at your option) any later version.
// talloc is distributed in the hope that it will be useful, but WITHOUT ANY WARRANTY; without even the implied warranty of MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU General Lesser Public License for more details.
// You should have received a copy of the GNU General Lesser Public License along with talloc. If not, see <http://www.gnu.org/licenses/>.

#ifndef TALLOC_EXT_LENGTH_H
#define TALLOC_EXT_LENGTH_H

#include "../tree.h"
#include "../ext.h"

#ifdef TALLOC_EXT

inline
uint8_t talloc_add_length ( talloc_chunk * child, size_t user_length, uint8_t ext_mode )
{
    if ( ext_mode & TALLOC_MODE_LENGTH ) {
        size_t * length = malloc ( sizeof ( size_t ) );
        if ( length == NULL ) {
            return 1;
        }
        if ( talloc_ext_set ( child, TALLOC_EXT_LENGTH, length ) != 0 ) {
            return 2;
        }
        * length = user_length;
    }
    return 0;
}

inline
uint8_t talloc_set_length ( talloc_chunk * child, size_t user_length )
{
    if ( child->ext != NULL && child->ext->mode & TALLOC_MODE_LENGTH ) {
        size_t * length = talloc_ext_get ( child, TALLOC_EXT_LENGTH );
        if ( length == NULL ) {
            return 1;
        }
        * length = user_length;
    }
    return 0;
}

inline
uint8_t talloc_get_length ( const void * child_data, size_t * result_length )
{
    talloc_chunk * child = talloc_chunk_from_data ( child_data );
    if ( child == NULL ) {
        return 1;
    }
    size_t * length = talloc_ext_get ( child, TALLOC_EXT_LENGTH );
    if ( length == NULL ) {
        return 2;
    }
    * result_length = * length;
    return 0;
}

#endif

#endif

