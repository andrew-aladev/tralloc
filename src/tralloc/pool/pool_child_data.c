// This file is part of tralloc. Copyright (C) 2013 Andrew Aladjev aladjev.andrew@gmail.com
// tralloc is free software: you can redistribute it and/or modify it under the terms of the GNU General Lesser Public License as published by the Free Software Foundation, either version 3 of the License, or (at your option) any later version.
// tralloc is distributed in the hope that it will be useful, but WITHOUT ANY WARRANTY; without even the implied warranty of MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU General Lesser Public License for more details.
// You should have received a copy of the GNU General Lesser Public License along with tralloc. If not, see <http://www.gnu.org/licenses/>.

#define _TRALLOC_INCLUDED_FROM_POOL_POOL_CHILD_DATA_C
#include <tralloc/pool/pool_child_data.h>
#include <tralloc/pool/pool_data.h>
#include <tralloc/pool/fragment.h>
#include <tralloc/tree/free.h>

#include <string.h>


// Function returns size of empty space ( >= 0 ).
// There is no warranty, that this size will be enough for "_tralloc_pool_fragment".
static
size_t _tralloc_pool_child_get_prev_fragment_length ( _tralloc_pool_child * pool_child )
{
    _tralloc_pool_child * prev = pool_child->prev;
    if ( prev == NULL ) {
        // Returning size of empty space between pool memory's start and first "pool_child".
        return ( uintptr_t ) pool_child - ( uintptr_t ) pool_child->pool->memory;
    } else {
        // Returning size of empty space between prev and current pool_childs.
        return ( uintptr_t ) pool_child - ( uintptr_t ) prev - prev->length;
    }
}

// Function returns size of empty space ( >= 0 ).
// There is no warranty, that this size will be enough for "_tralloc_pool_fragment".
static
size_t _tralloc_pool_child_get_next_fragment_length ( _tralloc_pool_child * pool_child )
{
    _tralloc_pool_child * next = pool_child->next;
    if ( next == NULL ) {
        // Returning size of empty space between last "pool_child" and pool memory's end.
        return ( uintptr_t ) pool_child->pool->memory + pool_child->pool->length - ( uintptr_t ) pool_child - pool_child->length;
    } else {
        // Returning size of empty space between current and next "pool_child"s.
        return ( uintptr_t ) next - ( uintptr_t ) pool_child - pool_child->length;
    }
}

static
void _tralloc_pool_child_attach ( _tralloc_pool_child * current, _tralloc_pool_child * prev, _tralloc_pool_child * next )
{
    current->prev = prev;
    current->next = next;

    if ( prev != NULL ) {
        prev->next = current;
    } else {
        current->pool->first_child = current;
    }
    if ( next != NULL ) {
        next->prev = current;
    }
}

static
void _tralloc_pool_child_detach ( _tralloc_pool_child * current )
{
    _tralloc_pool_child * prev = current->prev;
    _tralloc_pool_child * next = current->next;

    if ( prev != NULL ) {
        prev->next = next;
    } else {
        current->pool->first_child = next;
    }
    if ( next != NULL ) {
        next->prev = prev;
    }
}

static
void _tralloc_pool_child_update ( _tralloc_pool_child * current )
{
    _tralloc_pool_child * prev = current->prev;
    _tralloc_pool_child * next = current->next;

    if ( prev != NULL ) {
        prev->next = current;
    } else {
        current->pool->first_child = current;
    }
    if ( next != NULL ) {
        next->prev = current;
    }
}

void _tralloc_pool_child_new ( _tralloc_pool_child * pool_child, _tralloc_pool * pool, size_t length, _tralloc_pool_child * prev, _tralloc_pool_child * next )
{
    pool_child->pool   = pool;
    pool_child->length = length;
    _tralloc_pool_child_attach ( pool_child, prev, next );
}

static
void _tralloc_pool_child_free_data ( _tralloc_pool_child * pool_child )
{
    _tralloc_pool_child_detach ( pool_child );

    _tralloc_pool_child_merge_fragments_around (
        pool_child,
        _tralloc_pool_child_get_prev_fragment_length ( pool_child ),
        _tralloc_pool_child_get_next_fragment_length ( pool_child )
    );
}

_tralloc_pool_child * _tralloc_pool_child_resize ( _tralloc_pool_child * pool_child, size_t target_length )
{
    size_t next_fragment_length = _tralloc_pool_child_get_next_fragment_length ( pool_child );

    // "length" means how much space can be occupied at the expense of current "pool_child"'s length and next fragment.
    size_t length = pool_child->length + next_fragment_length;
    if ( length >= target_length ) {
        // "pool_child"'s pointer will not be changed.

        // Resizing pool child by next fragment.
        _tralloc_pool_child_resize_by_next_fragment ( pool_child, target_length, next_fragment_length );

        pool_child->length = target_length;
        return pool_child;
    }

    size_t prev_fragment_length = _tralloc_pool_child_get_prev_fragment_length ( pool_child );

    // "length" means how much space can be occupied at the expense of current "pool_child"'s length, prev and next fragments.
    length += prev_fragment_length;
    if ( length >= target_length ) {
        // "pool_child"'s pointer will be changed.
        // Prev fragment will be lost.

        _tralloc_pool_fragment * prev_fragment = ( _tralloc_pool_fragment * ) ( ( uintptr_t ) pool_child - prev_fragment_length );
        if ( prev_fragment_length >= sizeof ( _tralloc_pool_fragment ) ) {
            // If prev fragment is represented by existing object it should be detached.
            _tralloc_pool_detach_fragment ( pool_child->pool, prev_fragment );
        }

        // Moving "pool_child" to the left.
        _tralloc_pool_child * new_pool_child = ( _tralloc_pool_child * ) prev_fragment;
        memmove ( new_pool_child, pool_child, pool_child->length );
        new_pool_child->length += prev_fragment_length;
        _tralloc_pool_child_update ( new_pool_child );

        // "next_fragment" should be notified about "new_pool_child".
        _tralloc_pool_fragment * next_fragment = ( _tralloc_pool_fragment * ) ( ( uintptr_t ) pool_child + pool_child->length );
        if ( next_fragment_length >= sizeof ( _tralloc_pool_fragment ) ) {
            next_fragment->prev_child = new_pool_child;
        }

        // Resizing pool child by next fragment.
        _tralloc_pool_child_resize_by_next_fragment ( new_pool_child, target_length, next_fragment_length );

        new_pool_child->length = target_length;
        return new_pool_child;
    }

    // Amount of empty space around "pool_child" is not enough to meet "target_length" requirement.
    // Checking whether pool have "target_length" free space.

    _tralloc_pool * pool = pool_child->pool;
    if ( _tralloc_pool_can_alloc ( pool, target_length ) ) {
        // "pool_child"'s pointer will be changed.

        _tralloc_pool_child * new_pool_child;
        _tralloc_pool_child * new_prev_pool_child, * new_next_pool_child;
        _tralloc_pool_alloc ( pool, ( void ** ) &new_pool_child, target_length, TRALLOC_FALSE, &new_prev_pool_child, &new_next_pool_child );

        // Moving "pool_child" to the new position.
        memmove ( new_pool_child, pool_child, pool_child->length );

        // If "new_prev_pool_child" or "new_next_pool_child" equals current "pool_child" -
        // the free space with length >= "target_length" should act as prev or next fragment to current "pool_child".
        // But this is impossible, because function have checked that "prev_fragment_length" + "pool_child->length" + "next_fragment_length" < target_length.
        // So "pool_child" can be freed without risk to broke "new_prev_pool_child" or "new_next_pool_child".
        _tralloc_pool_child_free_data ( pool_child );

        new_pool_child->length = target_length;
        _tralloc_pool_child_attach ( new_pool_child, new_prev_pool_child, new_next_pool_child );

        return new_pool_child;
    }

    return NULL;
}

tralloc_error _tralloc_pool_child_free ( _tralloc_pool_child * pool_child )
{
    _tralloc_pool       * pool            = pool_child->pool;
    _tralloc_pool_child * prev_pool_child = pool_child->prev;
    _tralloc_pool_child * next_pool_child = pool_child->next;

    _tralloc_pool_child_free_data ( pool_child );

    if ( prev_pool_child == NULL && next_pool_child == NULL ) {
        if ( pool->autofree ) {
            // "pool->autofree == true" is a signal, that current pool is not needed.
            // So if current "pool_child" was the last - it should be freed.
            // This call will actually free only one chunk, because pool will have no children.
            return _tralloc_free_subtree ( _tralloc_pool_get_chunk ( pool ) );
        }
    }

    return 0;
}