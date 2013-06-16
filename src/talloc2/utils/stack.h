// This file is part of talloc2. Copyright (C) 2013 Andrew Aladjev aladjev.andrew@gmail.com
// talloc2 is free software: you can redistribute it and/or modify it under the terms of the GNU General Lesser Public License as published by the Free Software Foundation, either version 3 of the License, or (at your option) any later version.
// talloc2 is distributed in the hope that it will be useful, but WITHOUT ANY WARRANTY; without even the implied warranty of MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU General Lesser Public License for more details.
// You should have received a copy of the GNU General Lesser Public License along with talloc2. If not, see <http://www.gnu.org/licenses/>.

#ifndef TALLOC_UTILS_STACK_H
#define TALLOC_UTILS_STACK_H

#include "../tree.h"
#include "../ext/destructor.h"

#include <stdbool.h>

typedef struct talloc_stack_item_t {
    struct talloc_stack_item_t * prev;
    void * data;
} talloc_stack_item;

typedef struct talloc_stack_t {
    struct talloc_stack_item_t * last_item;
    size_t length;
} talloc_stack;

inline
uint8_t talloc_stack_free ( void * current_stack )
{
    talloc_stack * stack      = current_stack;
    talloc_stack_item * item = stack->last_item;
    talloc_stack_item * prev_item;
    while ( item != NULL ) {
        prev_item = item->prev;
        free ( item );
        item = prev_item;
    }
    return 0;
}

inline
talloc_stack * talloc_stack_new ( void * ctx )
{
    talloc_stack * stack = talloc ( ctx, sizeof ( talloc_stack ) );
    if ( stack == NULL ) {
        return NULL;
    }
    talloc_set_destructor ( stack, talloc_stack_free );

    stack->last_item = NULL;
    stack->length    = 0;
    return stack;
}

inline
uint8_t talloc_stack_push ( talloc_stack * stack, void * data )
{
    talloc_stack_item * item = malloc ( sizeof ( talloc_stack_item ) );
    if ( item == NULL ) {
        return 1;
    }
    item->prev = stack->last_item;
    item->data = data;

    stack->last_item = item;
    stack->length++;

    return 0;
}

inline
void talloc_stack_pop ( talloc_stack * stack )
{
    talloc_stack_item * item = stack->last_item;
    if ( item == NULL ) {
        return;
    }
    stack->last_item = item->prev;
    stack->length--;
    free ( item );
}

inline
size_t talloc_stack_get_length ( talloc_stack * stack )
{
    return stack->length;
}

#endif
