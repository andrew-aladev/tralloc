// This file is part of talloc2. Copyright (C) 2013 Andrew Aladjev aladjev.andrew@gmail.com
// talloc2 is free software: you can redistribute it and/or modify it under the terms of the GNU General Lesser Public License as published by the Free Software Foundation, either version 3 of the License, or (at your option) any later version.
// talloc2 is distributed in the hope that it will be useful, but WITHOUT ANY WARRANTY; without even the implied warranty of MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU General Lesser Public License for more details.
// You should have received a copy of the GNU General Lesser Public License along with talloc2. If not, see <http://www.gnu.org/licenses/>.

#ifndef TALLOC_EVENTS_H
#define TALLOC_EVENTS_H

#include <stdint.h>
#include <stddef.h>

#include "types.h"

#ifdef TALLOC_EXT
#include "ext/core.h"
#endif

#ifdef TALLOC_EXT_DESTRUCTOR
#include "ext/destructor.h"
#endif

#ifdef TALLOC_EXT_LENGTH
#include "ext/length.h"
#endif

#ifdef TALLOC_DEBUG
// Full global history of operations
typedef uint8_t ( * talloc_callback ) ( talloc_chunk * chunk );

talloc_callback talloc_debug_on_add;
talloc_callback talloc_debug_on_update;
talloc_callback talloc_debug_on_move;
talloc_callback talloc_debug_on_del;

inline
void talloc_set_callback ( talloc_callback on_add, talloc_callback on_update, talloc_callback on_move, talloc_callback on_del )
{
    talloc_debug_on_add    = on_add;
    talloc_debug_on_update = on_update;
    talloc_debug_on_move   = on_move;
    talloc_debug_on_del    = on_del;
}
#endif

inline
uint8_t talloc_on_add ( talloc_chunk * child, size_t length, uint8_t ext_mode )
{
#ifdef TALLOC_EXT
    child->ext = NULL;

#ifdef TALLOC_EXT_LENGTH
    if ( talloc_add_length ( child, length, ext_mode ) != 0 ) {
        return 1;
    }
#endif

    if ( child->ext != NULL ) {
        child->ext->mode = ext_mode;
    }
#endif

#ifdef TALLOC_DEBUG
    if ( talloc_debug_on_add != NULL ) {
        if ( talloc_debug_on_add ( child ) != 0 ) {
            return 2;
        }
    }
#endif

    return 0;
}

inline
uint8_t talloc_on_update ( talloc_chunk * child, size_t length )
{
#ifdef TALLOC_EXT_LENGTH
    if ( talloc_set_length ( child, length ) != 0 ) {
        return 1;
    }
#endif

#ifdef TALLOC_DEBUG
    if ( talloc_debug_on_update != NULL ) {
        if ( talloc_debug_on_update ( child ) != 0 ) {
            return 2;
        }
    }
#endif

    return 0;
}

inline
uint8_t talloc_on_move ( talloc_chunk * child )
{
#ifdef TALLOC_DEBUG
    if ( talloc_debug_on_move != NULL ) {
        if ( talloc_debug_on_move ( child ) != 0 ) {
            return 2;
        }
    }
#endif

    return 0;
}

inline
uint8_t talloc_on_del ( talloc_chunk * child )
{
#ifdef TALLOC_EXT_DESTRUCTOR
    if ( talloc_destructor_on_del ( child ) != 0 ) {
        return 1;
    }
#endif

#ifdef TALLOC_EXT_LENGTH
    if ( talloc_length_on_del ( child ) != 0 ) {
        return 1;
    }
#endif

#ifdef TALLOC_DEBUG
    if ( talloc_debug_on_del != NULL ) {
        if ( talloc_debug_on_del ( child ) != 0 ) {
            return 2;
        }
    }
#endif

#ifdef TALLOC_EXT
    talloc_ext_free ( child );
#endif

    return 0;
}

#endif
