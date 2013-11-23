// This file is part of talloc2. Copyright (C) 2013 Andrew Aladjev aladjev.andrew@gmail.com
// talloc2 is free software: you can redistribute it and/or modify it under the terms of the GNU General Lesser Public License as published by the Free Software Foundation, either version 3 of the License, or (at your option) any later version.
// talloc2 is distributed in the hope that it will be useful, but WITHOUT ANY WARRANTY; without even the implied warranty of MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU General Lesser Public License for more details.
// You should have received a copy of the GNU General Lesser Public License along with talloc2. If not, see <http://www.gnu.org/licenses/>.

#ifndef TALLOC_DESTRUCTOR_H
#define TALLOC_DESTRUCTOR_H

#include "tree.h"

#include <stdlib.h>
#include <stdio.h>

inline
void talloc_destructor_on_add ( talloc_chunk * child )
{
    child->first_destructor_item = NULL;
}

inline
void talloc_destructor_free ( talloc_destructor_item * current )
{
    talloc_destructor_item * next;
    while ( current != NULL ) {
        next = current->next;
        free ( current );
        current = next;
    }
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
    void * child_data = talloc_data_from_chunk ( child );
    talloc_destructor_item * first_item = child->first_destructor_item;
    talloc_destructor_item * item       = first_item;

    while ( item != NULL ) {
        if ( talloc_destructor_run ( child_data, item ) != 0 ) {
            talloc_destructor_free ( first_item );
            return 1;
        }
        item = item->next;
    }

    talloc_destructor_free ( first_item );
    return 0;
}

uint8_t talloc_add_destructor ( const void * child_data, talloc_destructor destructor, void * user_data );
uint8_t talloc_del_destructor ( const void * child_data, talloc_destructor destructor, void * user_data );
uint8_t talloc_del_destructor_by_function ( const void * child_data, talloc_destructor destructor );
uint8_t talloc_del_destructor_by_data ( const void * child_data, void * user_data );
uint8_t talloc_clear_destructors ( const void * child_data );

#endif
