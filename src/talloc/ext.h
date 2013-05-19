// This file is part of talloc. Copyright (C) 2013 Andrew Aladjev aladjev.andrew@gmail.com
// talloc is free software: you can redistribute it and/or modify it under the terms of the GNU General Lesser Public License as published by the Free Software Foundation, either version 3 of the License, or (at your option) any later version.
// talloc is distributed in the hope that it will be useful, but WITHOUT ANY WARRANTY; without even the implied warranty of MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU General Lesser Public License for more details.
// You should have received a copy of the GNU General Lesser Public License along with talloc. If not, see <http://www.gnu.org/licenses/>.

#ifndef TALLOC_EXT_H
#define TALLOC_EXT_H

#include <stdlib.h>

#include "types.h"

inline
talloc_ext * get_ext ( talloc_chunk * child ) {
    talloc_ext * ext = child->ext;
    if ( ext == NULL ) {
        ext = child->ext = malloc ( sizeof ( talloc_ext ) );
#ifdef TALLOC_EXT_DESTRUCTOR
        ext->destructor = NULL;
#endif
    }
    return ext;
}

#ifdef TALLOC_EXT_DESTRUCTOR
#include "ext/destructor.h"
#endif

inline
void talloc_ext_on_del ( talloc_chunk * child ) {
    talloc_ext * ext = child->ext;
    if ( ext != NULL ) {
#ifdef TALLOC_EXT_DESTRUCTOR
        talloc_destructor_on_del ( child );
#endif
        free ( ext );
    }
}

#endif

