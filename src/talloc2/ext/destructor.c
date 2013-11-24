// This file is part of talloc2. Copyright (C) 2013 Andrew Aladjev aladjev.andrew@gmail.com
// talloc2 is free software: you can redistribute it and/or modify it under the terms of the GNU General Lesser Public License as published by the Free Software Foundation, either version 3 of the License, or (at your option) any later version.
// talloc2 is distributed in the hope that it will be useful, but WITHOUT ANY WARRANTY; without even the implied warranty of MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU General Lesser Public License for more details.
// You should have received a copy of the GNU General Lesser Public License along with talloc2. If not, see <http://www.gnu.org/licenses/>.

#include "chunk.h"
#include "destructor.h"

static inline
bool talloc_destructor_append ( talloc_ext * ext, talloc_destructor destructor, void * user_data )
{
    talloc_destructor_item * new_item = malloc ( sizeof ( talloc_destructor_item ) );
    if ( new_item == NULL ) {
        return false;
    }
    new_item->destructor = destructor;
    new_item->user_data  = user_data;
    new_item->next       = NULL;

    talloc_destructor_item * first_item = ext->first_destructor_item;
    ext->first_destructor_item          = new_item;
    new_item->next                      = first_item;

    return true;
}

uint8_t talloc_add_destructor ( const void * chunk_data, talloc_destructor destructor, void * user_data )
{
    if ( chunk_data == NULL ) {
        return 1;
    }
    talloc_ext * ext = talloc_memory_from_ext_chunk ( talloc_chunk_from_data ( chunk_data ) );

    if ( !talloc_destructor_append ( ext, destructor, user_data ) ) {
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
void delete_destructors_by_comparator ( talloc_ext * ext, destructor_comparator comparator, talloc_destructor destructor, void * user_data )
{
    talloc_destructor_item * item = ext->first_destructor_item;
    talloc_destructor_item * next;
    talloc_destructor_item * prev = NULL;

    while ( item != NULL ) {
        next = item->next;
        if ( comparator ( item, destructor, user_data ) ) {
            free ( item );
            if ( prev == NULL ) {
                ext->first_destructor_item = next;
            } else {
                prev->next = next;
            }
        } else {
            prev = item;
        }
        item = next;
    }
}

static inline
uint8_t delete_destructors ( const void * chunk_data, destructor_comparator comparator, talloc_destructor destructor, void * user_data )
{
    if ( chunk_data == NULL ) {
        return 1;
    }
    talloc_ext * ext = talloc_memory_from_ext_chunk ( talloc_chunk_from_data ( chunk_data ) );

    delete_destructors_by_comparator ( ext, comparator, destructor, user_data );
    return 0;
}

uint8_t talloc_del_destructor ( const void * chunk_data, talloc_destructor destructor, void * user_data )
{
    return delete_destructors ( chunk_data, destructor_comparator_strict, destructor, user_data );
}

uint8_t talloc_del_destructor_by_function ( const void * chunk_data, talloc_destructor destructor )
{
    return delete_destructors ( chunk_data, destructor_comparator_by_function, destructor, NULL );
}

uint8_t talloc_del_destructor_by_data ( const void * chunk_data, void * user_data )
{
    return delete_destructors ( chunk_data, destructor_comparator_by_data, NULL, user_data );
}

uint8_t talloc_clear_destructors ( const void * chunk_data )
{
    if ( chunk_data == NULL ) {
        return 1;
    }
    talloc_ext * ext = talloc_memory_from_ext_chunk ( talloc_chunk_from_data ( chunk_data ) );

    talloc_destructor_free_silent ( ext->first_destructor_item );
    ext->first_destructor_item = NULL;
    return 0;
}

static inline
uint8_t _destructor_run ( void * chunk_data, talloc_destructor_item * item )
{
    talloc_destructor destructor = item->destructor;
    if ( destructor == NULL ) {
        return 1;
    }
    if ( destructor ( chunk_data, item->user_data ) != 0 ) {
        return 2;
    }
    return 0;
}

bool talloc_destructor_free ( talloc_chunk * chunk, talloc_destructor_item * current )
{
    void * chunk_data = talloc_data_from_chunk ( chunk );
    bool result       = true;

    talloc_destructor_item * next;
    while ( current != NULL ) {
        if ( _destructor_run ( chunk_data, current ) ) {
            result = false;
        }

        next = current->next;
        free ( current );
        current = next;
    }
    return result;
}

void talloc_destructor_free_silent ( talloc_destructor_item * current )
{
    talloc_destructor_item * next;
    while ( current != NULL ) {
        next = current->next;
        free ( current );
        current = next;
    }
}