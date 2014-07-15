// This file is part of tralloc. Copyright (C) 2013 Andrew Aladjev aladjev.andrew@gmail.com
// tralloc is free software: you can redistribute it and/or modify it under the terms of the GNU General Lesser Public License as published by the Free Software Foundation, either version 3 of the License, or (at your option) any later version.
// tralloc is distributed in the hope that it will be useful, but WITHOUT ANY WARRANTY; without even the implied warranty of MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU General Lesser Public License for more details.
// You should have received a copy of the GNU General Lesser Public License along with tralloc. If not, see <http://www.gnu.org/licenses/>.

#define _TRALLOC_INCLUDED_FROM_POOL_FRAGMENT_C
#include <tralloc/pool/fragment.h>

#include <string.h>


void _tralloc_attach_pool_fragment ( _tralloc_pool * pool, _tralloc_pool_fragment * current, _tralloc_pool_fragment * prev, _tralloc_pool_fragment * next )
{
    current->prev = prev;
    current->next = next;

    if ( prev != NULL ) {
        prev->next = current;
    } else {
        pool->max_fragment = current;
    }
    if ( next != NULL ) {
        next->prev = current;
    }
}

void _tralloc_detach_pool_fragment ( _tralloc_pool * pool, _tralloc_pool_fragment * current )
{
    _tralloc_pool_fragment * prev = current->prev;
    _tralloc_pool_fragment * next = current->next;

    if ( prev != NULL ) {
        prev->next = next;
    } else {
        pool->max_fragment = next;
    }
    if ( next != NULL ) {
        next->prev = prev;
    }
}

static inline
void _tralloc_update_pool_fragment ( _tralloc_pool * pool, _tralloc_pool_fragment * current )
{
    _tralloc_pool_fragment * prev = current->prev;
    _tralloc_pool_fragment * next = current->next;

    if ( prev != NULL ) {
        prev->next = current;
    } else {
        pool->max_fragment = current;
    }
    if ( next != NULL ) {
        next->prev = current;
    }
}

static inline
// Function inserts "fragment" to the fragment's list if it has been increased.
void _tralloc_pool_fragment_increased ( _tralloc_pool * pool, _tralloc_pool_fragment * current )
{
    _tralloc_pool_fragment * prev = current->prev;
    _tralloc_pool_fragment * next = current->next;

    while ( prev != NULL && current->length > prev->length ) {
        next = prev;
        prev = prev->prev;
    }

    if ( prev != current->prev ) {
        _tralloc_detach_pool_fragment ( pool, current );
        _tralloc_attach_pool_fragment ( pool, current, prev, next );
    }
}

static inline
// Function inserts "fragment" to the fragment's list if it has been decreased.
void _tralloc_pool_fragment_decreased ( _tralloc_pool * pool, _tralloc_pool_fragment * current )
{
    _tralloc_pool_fragment * prev = current->prev;
    _tralloc_pool_fragment * next = current->next;

    while ( next != NULL && current->length < next->length ) {
        prev = next;
        next = next->next;
    }

    if ( prev != current->prev ) {
        _tralloc_detach_pool_fragment ( pool, current );
        _tralloc_attach_pool_fragment ( pool, current, prev, next );
    }
}

_tralloc_pool_child * _tralloc_alloc_from_pool_fragment ( _tralloc_pool * pool, _tralloc_pool_fragment * fragment, size_t length )
{
    size_t new_fragment_length = fragment->length - length;

    // Free space will be [ "fragment", "next_child" ].
    // "fragment" may have 0 length.
    _tralloc_pool_child * next_child = ( _tralloc_pool_child * ) ( ( uintptr_t ) fragment + new_fragment_length );

    if ( new_fragment_length >= sizeof ( _tralloc_pool_fragment ) ) {
        fragment->next_child = next_child;
        fragment->length     = new_fragment_length;
        _tralloc_pool_fragment_decreased ( pool, fragment );
    } else {
        _tralloc_detach_pool_fragment ( pool, fragment );
    }

    return next_child;
}


// There are [ "pool_child", "fragment" ] in memory.
// "pool_child" reports, that it wants to obtain "pool_child_target_length" size.
// Function expands, reduces or frees "fragment" to fit the remaining free space.
// Fragment is aligned to the right of memory block.
void _tralloc_resize_pool_fragment_by_prev_child ( _tralloc_pool_child * pool_child, size_t pool_child_target_length, size_t fragment_length )
{
    _tralloc_pool       * pool            = pool_child->pool;
    _tralloc_pool_child * next_pool_child = pool_child->next;

    _tralloc_pool_fragment * fragment     = ( _tralloc_pool_fragment * ) ( ( uintptr_t ) pool_child + pool_child->length );
    _tralloc_pool_fragment * new_fragment = ( _tralloc_pool_fragment * ) ( ( uintptr_t ) pool_child + pool_child_target_length );

    size_t new_fragment_length = pool_child->length + fragment_length - pool_child_target_length;

    if ( fragment_length < sizeof ( _tralloc_pool_fragment ) ) {
        // There is free space with size "fragment_length".
        // But it is not registered as a fragment, because it is too small.

        if ( new_fragment_length < sizeof ( _tralloc_pool_fragment ) ) {
            // Anyway the free space is too small to act as registered fragment.
            return;
        } else {
            // Creating new registered fragment.

            new_fragment->length     = new_fragment_length;
            new_fragment->prev_child = pool_child;
            new_fragment->next_child = next_pool_child;

            // Let "new_fragment" is the longest fragment in fragment's list.
            _tralloc_attach_pool_fragment ( pool, new_fragment, NULL, pool->max_fragment );

            // Fixing the guess.
            _tralloc_pool_fragment_decreased ( pool, new_fragment );
        }
    } else {
        // There is a registered "fragment".

        if ( new_fragment_length < sizeof ( _tralloc_pool_fragment ) ) {
            // The free space is too small to act as registered fragment.
            _tralloc_detach_pool_fragment ( pool, fragment );
            return;
        } else {
            // Copying existing registered fragment to new location.

            memmove ( new_fragment, fragment, sizeof ( _tralloc_pool_fragment ) );
            _tralloc_update_pool_fragment ( pool, new_fragment );

            new_fragment->length = new_fragment_length;

            if ( new_fragment_length > fragment_length ) {
                _tralloc_pool_fragment_increased ( pool, new_fragment );
            } else {
                _tralloc_pool_fragment_decreased ( pool, new_fragment );
            }
        }
    }
}

// There are [ "prev_fragment", "pool_child", "next_fragment" ] in memory.
// "pool_child" reports, that it will be freed.
// Function expands "prev_fragment" to occupy all memory block.
void _tralloc_merge_pool_fragments_around_pool_child ( _tralloc_pool_child * pool_child, size_t prev_fragment_length, size_t next_fragment_length )
{
    size_t length = prev_fragment_length + pool_child->length + next_fragment_length;

    _tralloc_pool_fragment * prev_fragment = ( _tralloc_pool_fragment * ) ( ( uintptr_t ) pool_child - prev_fragment_length );
    _tralloc_pool_fragment * next_fragment = ( _tralloc_pool_fragment * ) ( ( uintptr_t ) pool_child + pool_child->length );

    _tralloc_pool       * pool       = pool_child->pool;
    _tralloc_pool_child * prev_child = pool_child->prev;
    _tralloc_pool_child * next_child = pool_child->next;

    if ( prev_fragment_length < sizeof ( _tralloc_pool_fragment ) ) {
        // There is free space with size "prev_fragment_length".
        // But it is not registered as a fragment, because it is too small.

        if ( next_fragment_length < sizeof ( _tralloc_pool_fragment ) ) {
            // There is no registered "prev_fragment" and "next_fragment".

            if ( length < sizeof ( _tralloc_pool_fragment ) ) {
                // There is not enough space to create new registered fragment.
            } else {
                // Creating new registered fragment.
                prev_fragment->prev_child = prev_child;
                prev_fragment->next_child = next_child;
                prev_fragment->length     = length;

                // Let "prev_fragment" is the longest fragment in fragment's list.
                _tralloc_attach_pool_fragment ( pool, prev_fragment, NULL, pool->max_fragment );

                // Fixing the guess.
                _tralloc_pool_fragment_decreased ( pool, prev_fragment );
            }
        } else {
            // There is a registered "next_fragment".
            // Moving "next_fragment" to "prev_fragment" location.

            memmove ( prev_fragment, next_fragment, sizeof ( _tralloc_pool_fragment ) );
            prev_fragment->prev_child = prev_child;
            prev_fragment->length     = length;

            _tralloc_update_pool_fragment ( pool, prev_fragment );

            // Increasing length of "prev_fragment".
            _tralloc_pool_fragment_increased ( pool, prev_fragment );
        }
    } else {
        // There is a registered "prev_fragment".

        if ( next_fragment_length < sizeof ( _tralloc_pool_fragment ) ) {
            // There is no registered "next_fragment".
            // Increasing length of "prev_fragment".

            prev_fragment->length     = length;
            prev_fragment->next_child = next_child;

            _tralloc_pool_fragment_increased ( pool, prev_fragment );
        } else {
            // There are both registered "prev_fragment" and "next_fragment".
            // Detaching "next_fragment".
            // Increasing length of prev fragment.

            _tralloc_detach_pool_fragment ( pool, next_fragment );

            prev_fragment->length     = length;
            prev_fragment->next_child = next_child;

            _tralloc_pool_fragment_increased ( pool, prev_fragment );
        }
    }
}
