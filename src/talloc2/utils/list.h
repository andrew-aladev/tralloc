// This file is part of talloc2. Copyright (C) 2013 Andrew Aladjev aladjev.andrew@gmail.com
// talloc2 is free software: you can redistribute it and/or modify it under the terms of the GNU General Lesser Public License as published by the Free Software Foundation, either version 3 of the License, or (at your option) any later version.
// talloc2 is distributed in the hope that it will be useful, but WITHOUT ANY WARRANTY; without even the implied warranty of MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU General Lesser Public License for more details.
// You should have received a copy of the GNU General Lesser Public License along with talloc2. If not, see <http://www.gnu.org/licenses/>.

#ifndef TALLOC_UTILS_LIST_H
#define TALLOC_UTILS_LIST_H

#include "../tree.h"
#include "../ext/destructor.h"

#include <stdbool.h>

typedef struct talloc_list_item_t {
    struct talloc_list_item_t * prev;
    void * data;
} talloc_list_item;

typedef struct talloc_list_t {
    struct talloc_list_item_t * last_item;
    size_t length;
} talloc_list;

inline
uint8_t talloc_list_free ( void * current_list )
{
    talloc_list * list      = current_list;
    talloc_list_item * item = list->last_item;
    talloc_list_item * prev_item;
    while ( item != NULL ) {
        prev_item = item->prev;
        free ( item );
        item = prev_item;
    }
    return 0;
}

inline
talloc_list * talloc_list_new ( void * ctx )
{
    talloc_list * list = talloc ( ctx, sizeof ( talloc_list ) );
    if ( list == NULL ) {
        return NULL;
    }
    talloc_set_destructor ( list, talloc_list_free );

    list->last_item = NULL;
    list->length    = 0;
    return list;
}

inline
uint8_t talloc_list_append ( talloc_list * list, void * data )
{
    talloc_list_item * item = malloc ( sizeof ( talloc_list_item ) );
    if ( item == NULL ) {
        return 1;
    }
    item->prev = list->last_item;
    item->data = data;

    list->last_item = item;
    list->length++;

    return 0;
}

inline
void talloc_list_pop ( talloc_list * list )
{
    talloc_list_item * item = list->last_item;
    if ( item == NULL ) {
        return;
    }
    list->last_item = item->prev;
    list->length--;
    free ( item );
}

inline
size_t talloc_list_get_length ( talloc_list * list )
{
    return list->length;
}

#endif
