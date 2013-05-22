// This file is part of talloc. Copyright (C) 2013 Andrew Aladjev aladjev.andrew@gmail.com
// talloc is free software: you can redistribute it and/or modify it under the terms of the GNU General Lesser Public License as published by the Free Software Foundation, either version 3 of the License, or (at your option) any later version.
// talloc is distributed in the hope that it will be useful, but WITHOUT ANY WARRANTY; without even the implied warranty of MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU General Lesser Public License for more details.
// You should have received a copy of the GNU General Lesser Public License along with talloc. If not, see <http://www.gnu.org/licenses/>.

#ifndef TALLOC_EVENTS_H
#define TALLOC_EVENTS_H

#include "ext.h"
#include "ext/destructor.h"

#ifdef TALLOC_EVENTS

#ifdef TALLOC_DEBUG
// Full global history of operations
typedef void ( * talloc_callback ) ( talloc_chunk * chunk );

talloc_callback talloc_debug_on_add;
talloc_callback talloc_debug_on_update;
talloc_callback talloc_debug_on_del;

inline
void talloc_set_callback ( talloc_callback on_add, talloc_callback on_update, talloc_callback on_del ) {
    talloc_debug_on_add    = on_add;
    talloc_debug_on_update = on_update;
    talloc_debug_on_del    = on_del;
}
#endif

inline
void talloc_on_add ( talloc_chunk * child ) {
#ifdef TALLOC_DEBUG
    if ( talloc_debug_on_add != NULL ) {
        talloc_debug_on_add ( child );
    }
#endif

#ifdef TALLOC_EXT
    child->ext = NULL;
#endif
}

inline
void talloc_on_update ( talloc_chunk * child ) {
#ifdef TALLOC_DEBUG
    if ( talloc_debug_on_update != NULL ) {
        talloc_debug_on_update ( child );
    }
#endif
}

#include <stdio.h>

inline
void talloc_on_del ( talloc_chunk * child ) {
#ifdef TALLOC_DEBUG
    if ( talloc_debug_on_del != NULL ) {
        talloc_debug_on_del ( child );
    }
#endif

#ifdef TALLOC_EXT
    talloc_destructor_on_del ( child );
    talloc_ext_free ( child );
#endif
}

#endif

#endif
