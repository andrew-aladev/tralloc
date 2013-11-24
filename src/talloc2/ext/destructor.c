// This file is part of talloc2. Copyright (C) 2013 Andrew Aladjev aladjev.andrew@gmail.com
// talloc2 is free software: you can redistribute it and/or modify it under the terms of the GNU General Lesser Public License as published by the Free Software Foundation, either version 3 of the License, or (at your option) any later version.
// talloc2 is distributed in the hope that it will be useful, but WITHOUT ANY WARRANTY; without even the implied warranty of MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU General Lesser Public License for more details.
// You should have received a copy of the GNU General Lesser Public License along with talloc2. If not, see <http://www.gnu.org/licenses/>.

#include "destructor.h"

#include <stdbool.h>

static inline
bool talloc_destructor_append ( talloc_chunk * child, talloc_destructor destructor, void * user_data )
{
    talloc_destructor_item * new_item = malloc ( sizeof ( talloc_destructor_item ) );
    if ( new_item == NULL ) {
        return false;
    }
    new_item->destructor = destructor;
    new_item->user_data  = user_data;
    new_item->next       = NULL;

    talloc_destructor_item * first_item = child->first_destructor_item;
    child->first_destructor_item        = new_item;
    new_item->next                      = first_item;

    return true;
}

uint8_t talloc_add_destructor ( const void * child_data, talloc_destructor destructor, void * user_data )
{
    if ( child_data == NULL ) {
        return 1;
    }
    talloc_chunk * child = talloc_chunk_from_data ( child_data );

    if ( !talloc_destructor_append ( child, destructor, user_data ) ) {
        return 2;
    }

    return 0;
}

static inline
bool destructor_comparator_by_function ( talloc_destructor_item * item, talloc_destructor destructor, void * user_data )
{
    return item->destructor == destructor;
}

static inline
bool destructor_comparator_by_data ( talloc_destructor_item * item, talloc_destructor destructor, void * user_data )
{
    return item->user_data == user_data;
}

static inline
bool destructor_comparator_strict ( talloc_destructor_item * item, talloc_destructor destructor, void * user_data )
{
    return item->destructor == destructor && item->user_data == user_data;
}

typedef bool ( * destructor_comparator ) ( talloc_destructor_item * item, talloc_destructor destructor, void * user_data );

static inline
void delete_destructors_by_comparator ( talloc_chunk * child, destructor_comparator comparator, talloc_destructor destructor, void * user_data )
{
    talloc_destructor_item * item = child->first_destructor_item;
    talloc_destructor_item * next;
    talloc_destructor_item * prev = NULL;

    while ( item != NULL ) {
        next = item->next;
        if ( comparator ( item, destructor, user_data ) ) {
            free ( item );
            if ( prev == NULL ) {
                child->first_destructor_item = next;
            } else {
                prev->next = next;
            }
        } else {
            prev = item;
        }
        item = next;
    }

    return true;
}

static inline
uint8_t delete_destructors ( const void * child_data, destructor_comparator comparator, talloc_destructor destructor, void * user_data )
{
    if ( child_data == NULL ) {
        return 1;
    }
    talloc_chunk * child = talloc_chunk_from_data ( child_data );

    delete_destructors_by_comparator ( child, comparator, destructor, user_data );
    return 0;
}

uint8_t talloc_del_destructor ( const void * child_data, talloc_destructor destructor, void * user_data )
{
    return delete_destructors ( child_data, destructor_comparator_strict, destructor, user_data );
}

uint8_t talloc_del_destructor_by_function ( const void * child_data, talloc_destructor destructor )
{
    return delete_destructors ( child_data, destructor_comparator_by_function, destructor, NULL );
}

uint8_t talloc_del_destructor_by_data ( const void * child_data, void * user_data )
{
    return delete_destructors ( child_data, destructor_comparator_by_data, NULL, user_data );
}

uint8_t talloc_clear_destructors ( const void * child_data )
{
    if ( child_data == NULL ) {
        return 1;
    }
    talloc_chunk * child = talloc_chunk_from_data ( child_data );

    talloc_destructor_free ( child->first_destructor_item );
    child->first_destructor_item = NULL;
    return 0;
}

extern inline
void talloc_destructor_free ( talloc_destructor_item * item );

extern inline
uint8_t talloc_destructor_run ( void * child_data, talloc_destructor_item * item );

extern inline
void talloc_destructor_on_add ( talloc_chunk * child );

extern inline
uint8_t talloc_destructor_on_del ( talloc_chunk * child );