// This file is part of talloc2. Copyright (C) 2013 Andrew Aladjev aladjev.andrew@gmail.com
// talloc2 is free software: you can redistribute it and/or modify it under the terms of the GNU General Lesser Public License as published by the Free Software Foundation, either version 3 of the License, or (at your option) any later version.
// talloc2 is distributed in the hope that it will be useful, but WITHOUT ANY WARRANTY; without even the implied warranty of MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU General Lesser Public License for more details.
// You should have received a copy of the GNU General Lesser Public License along with talloc2. If not, see <http://www.gnu.org/licenses/>.

#ifndef TALLOC_EXT_DESTRUCTOR_H
#define TALLOC_EXT_DESTRUCTOR_H

#include "../tree.h"
#include "core.h"

#include <stdio.h>

inline
uint8_t talloc_destructor_on_del ( talloc_chunk * child )
{
    if ( child->ext != NULL && ( child->ext->mode & TALLOC_MODE_DESTRUCTOR ) != 0 ) {
        talloc_destructor_item * item = talloc_ext_get ( child, TALLOC_EXT_INDEX_DESTRUCTOR );
        if ( item == NULL ) {
            return 1;
        }
        talloc_destructor destructor = item->destructor;
        if ( destructor == NULL ) {
            free ( item );
            return 2;
        }
        if ( item->destructor ( talloc_data_from_chunk ( child ), item->user_data ) != 0 ) {
            free ( item );
            return 3;
        }
        free ( item );
    }
    return 0;
}

inline
uint8_t talloc_set_destructor ( const void * child_data, talloc_destructor destructor, void * user_data )
{
    if ( child_data == NULL ) {
        return 1;
    }
    talloc_chunk * child = talloc_chunk_from_data ( child_data );

    talloc_destructor_item * item = talloc_ext_get ( child, TALLOC_EXT_INDEX_DESTRUCTOR );
    if ( item != NULL ) {
        free ( item );
    }
    item = malloc ( sizeof ( talloc_destructor_item ) );
    if ( item == NULL ) {
        return 2;
    }
    item->destructor = destructor;
    item->user_data  = user_data;

    if ( talloc_ext_set ( child, TALLOC_EXT_INDEX_DESTRUCTOR, item ) != 0 ) {
        free ( item );
        return 3;
    }
    child->ext->mode |= TALLOC_MODE_DESTRUCTOR;
    return 0;
}

#endif
