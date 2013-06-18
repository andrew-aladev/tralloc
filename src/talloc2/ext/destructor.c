// This file is part of talloc2. Copyright (C) 2013 Andrew Aladjev aladjev.andrew@gmail.com
// talloc2 is free software: you can redistribute it and/or modify it under the terms of the GNU General Lesser Public License as published by the Free Software Foundation, either version 3 of the License, or (at your option) any later version.
// talloc2 is distributed in the hope that it will be useful, but WITHOUT ANY WARRANTY; without even the implied warranty of MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU General Lesser Public License for more details.
// You should have received a copy of the GNU General Lesser Public License along with talloc2. If not, see <http://www.gnu.org/licenses/>.

#include "destructor.h"

extern inline
void talloc_destructor_items_free ( talloc_destructor_items * items );

extern inline
uint8_t talloc_destructor_run ( void * child_data, talloc_destructor_item * item );

extern inline
uint8_t talloc_destructor_on_del ( talloc_chunk * child );

static inline
talloc_destructor_items * talloc_destructor_items_new ( talloc_destructor destructor, void * user_data )
{
    talloc_destructor_items * items = malloc ( sizeof ( talloc_destructor_items ) );
    if ( items == NULL ) {
        return NULL;
    }
    talloc_destructor_item * item = malloc ( sizeof ( talloc_destructor_item ) );
    if ( item == NULL ) {
        free ( items );
        return NULL;
    }
    item->destructor = destructor;
    item->user_data  = user_data;

    items->data   = ( void * ) item;
    items->length = 1;
    return items;
}

static inline
uint8_t talloc_destructor_append ( talloc_destructor_items * items, talloc_destructor destructor, void * user_data )
{
    talloc_destructor_item * new_item = malloc ( sizeof ( talloc_destructor_item ) );
    if ( new_item == NULL ) {
        return 1;
    }
    new_item->destructor = destructor;
    new_item->user_data  = user_data;

    uint8_t length = items->length;
    if ( length == 1 ) {
        void ** data = malloc ( sizeof ( uintptr_t ) * 2 );
        if ( data == NULL ) {
            return 2;
        }
        talloc_destructor_item * old_item = ( talloc_destructor_item * ) items->data;
        data[0] = old_item;
        data[1] = new_item;

        items->data   = data;
        items->length = 2;
    } else {
        length ++;
        void ** data = realloc ( items->data, sizeof ( uintptr_t ) * length );
        if ( data == NULL ) {
            return 3;
        }
        data[length - 1] = new_item;

        items->data   = data;
        items->length = length;
    }

    return 0;
}

uint8_t talloc_add_destructor ( const void * child_data, talloc_destructor destructor, void * user_data )
{
    if ( child_data == NULL ) {
        return 1;
    }
    talloc_chunk * child = talloc_chunk_from_data ( child_data );

    talloc_destructor_items * items = talloc_ext_get ( child, TALLOC_EXT_INDEX_DESTRUCTOR );
    if ( items == NULL ) {
        items = talloc_destructor_items_new ( destructor, user_data );
        if ( items == NULL ) {
            return 2;
        }
    } else {
        if ( talloc_destructor_append ( items, destructor, user_data ) != 0 ) {
            return 3;
        }
    }

    if ( talloc_ext_set ( child, TALLOC_EXT_INDEX_DESTRUCTOR, items ) != 0 ) {
        talloc_destructor_items_free ( items );
        child->ext->mode ^= TALLOC_MODE_DESTRUCTOR;
        return 4;
    }

    child->ext->mode |= TALLOC_MODE_DESTRUCTOR;
    return 0;
}