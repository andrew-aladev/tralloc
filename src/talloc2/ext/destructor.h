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
void talloc_destructor_items_free ( talloc_destructor_items * items )
{
    if ( items == NULL ) {
        return;
    }

    if ( items->length == 1 ) {
        free ( items->data );
    } else {
        uint8_t index;
        for ( index = 0; index < items->length; index ++ ) {
            free ( items->data[index] );
        }
        free ( items->data );
    }

    free ( items );
}

inline
uint8_t talloc_destructor_run ( void * child_data, talloc_destructor_item * item )
{
    talloc_destructor destructor = item->destructor;
    if ( destructor == NULL ) {
        return 1;
    }
    if ( destructor ( child_data, item->user_data ) != 0 ) {
        return 2;
    }
    return 0;
}

inline
uint8_t talloc_destructor_on_del ( talloc_chunk * child )
{
    if ( child->ext != NULL && ( child->ext->mode & TALLOC_MODE_DESTRUCTOR ) != 0 ) {
        talloc_destructor_items * items = talloc_ext_get ( child, TALLOC_EXT_INDEX_DESTRUCTOR );
        if ( items == NULL ) {
            return 1;
        }

        void * child_data = talloc_data_from_chunk ( child );
        if ( items->length == 1 ) {
            if ( talloc_destructor_run ( child_data, ( talloc_destructor_item * ) items->data ) != 0 ) {
                talloc_destructor_items_free ( items );
                return 2;
            }
        } else {
            uint8_t index;
            for ( index = 0; index < items->length; index ++ ) {
                if ( talloc_destructor_run ( child_data, ( talloc_destructor_item * ) items->data[index] ) != 0 ) {
                    talloc_destructor_items_free ( items );
                    return 3;
                }
            }
        }

        talloc_destructor_items_free ( items );
    }
    return 0;
}

uint8_t talloc_add_destructor ( const void * child_data, talloc_destructor destructor, void * user_data );

#endif


