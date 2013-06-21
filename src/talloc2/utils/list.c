// This file is part of talloc2. Copyright (C) 2013 Andrew Aladjev aladjev.andrew@gmail.com
// talloc2 is free software: you can redistribute it and/or modify it under the terms of the GNU General Lesser Public License as published by the Free Software Foundation, either version 3 of the License, or (at your option) any later version.
// talloc2 is distributed in the hope that it will be useful, but WITHOUT ANY WARRANTY; without even the implied warranty of MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU General Lesser Public License for more details.
// You should have received a copy of the GNU General Lesser Public License along with talloc2. If not, see <http://www.gnu.org/licenses/>.

#include "list.h"
#include "../ext/destructor.h"

#include <stdlib.h>

static
uint8_t delete_on_free ( void * child_data, void * user_data )
{
    talloc_list_item * item = user_data;
    if ( item == NULL ) {
        return 1;
    }
    talloc_list *          items = item->parent;
    talloc_list_item * next_item = item->next;
    talloc_list_item * prev_item = item->prev;
    if ( next_item == NULL ) {
        items->last_item = prev_item;
    } else {
        next_item->prev = prev_item;
    }
    if ( prev_item == NULL ) {
        items->first_item = next_item;
    } else {
        prev_item->next = next_item;
    }
    items->length--;
    free ( item );
    return 0;
}

static inline
uint8_t list_free ( void * child_data, void * user_data )
{
    uint8_t result = 0;
    talloc_list * list      = child_data;
    talloc_list_item * item = list->first_item;
    talloc_list_item * next_item;
    while ( item != NULL ) {
        next_item = item->next;
        if ( talloc_del_destructor ( item->data, delete_on_free, item ) != 0 ) {
            result = 1;
        }
        free ( item );
        item = next_item;
    }

    return result;
}

talloc_list * talloc_list_new ( void * ctx )
{
    talloc_list * list = talloc ( ctx, sizeof ( talloc_list ) );
    if ( list == NULL ) {
        return NULL;
    }
    list->first_item = NULL;
    list->last_item  = NULL;
    list->length     = 0;

    if ( talloc_add_destructor ( list, list_free, NULL ) != 0 ) {
        talloc_free ( list );
        return NULL;
    }

    return list;
}

uint8_t talloc_list_push ( talloc_list * list, void * data )
{
    talloc_list_item * item = malloc ( sizeof ( talloc_list_item ) );
    if ( item == NULL ) {
        return 1;
    }
    item->parent = list;
    item->next   = NULL;
    item->data   = data;

    talloc_list_item * last_item = list->last_item;
    if ( last_item == NULL ) {
        item->prev = NULL;
    } else {
        last_item->next = item;
        item->prev      = last_item;
    }

    list->last_item = item;
    if ( list->first_item == NULL ) {
        list->first_item = item;
    }
    list->length++;

    if ( talloc_add_destructor ( data, delete_on_free, item ) != 0 ) {
        return 2;
    }

    return 0;
}

uint8_t talloc_list_unshift ( talloc_list * list, void * data )
{
    talloc_list_item * item = malloc ( sizeof ( talloc_list_item ) );
    if ( item == NULL ) {
        return 1;
    }
    item->parent = list;
    item->prev   = NULL;
    item->data   = data;

    talloc_list_item * first_item = list->first_item;
    if ( first_item == NULL ) {
        item->next = NULL;
    } else {
        first_item->prev = item;
        item->next       = first_item;
    }

    list->first_item = item;
    if ( list->last_item == NULL ) {
        list->last_item = item;
    }
    list->length++;

    if ( talloc_add_destructor ( data, delete_on_free, item ) != 0 ) {
        return 2;
    }

    return 0;
}

extern inline
size_t talloc_list_get_length ( talloc_list * list );