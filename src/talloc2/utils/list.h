// This file is part of talloc2. Copyright (C) 2013 Andrew Aladjev aladjev.andrew@gmail.com
// talloc2 is free software: you can redistribute it and/or modify it under the terms of the GNU General Lesser Public License as published by the Free Software Foundation, either version 3 of the License, or (at your option) any later version.
// talloc2 is distributed in the hope that it will be useful, but WITHOUT ANY WARRANTY; without even the implied warranty of MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU General Lesser Public License for more details.
// You should have received a copy of the GNU General Lesser Public License along with talloc2. If not, see <http://www.gnu.org/licenses/>.

#ifndef TALLOC_UTILS_LIST_H
#define TALLOC_UTILS_LIST_H

#include "../tree.h"

#include <stdbool.h>

typedef struct talloc_list_t {
    struct talloc_list_item_t * first_item, * last_item;
    size_t length;
} talloc_list;

typedef struct talloc_list_item_t {
    struct talloc_list_t * parent;
    struct talloc_list_item_t * prev, * next;
    void * data;
} talloc_list_item;

talloc_list * talloc_list_new     ( void * ctx );
uint8_t       talloc_list_push    ( talloc_list * list, void * data, bool auto_delete_on_free );
uint8_t       talloc_list_unshift ( talloc_list * list, void * data, bool auto_delete_on_free );

inline
size_t talloc_list_get_length ( talloc_list * list )
{
    return list->length;
}

#endif
