// This file is part of talloc. Copyright (C) 2013 Andrew Aladjev aladjev.andrew@gmail.com
// talloc is free software: you can redistribute it and/or modify it under the terms of the GNU General Lesser Public License as published by the Free Software Foundation, either version 3 of the License, or (at your option) any later version.
// talloc is distributed in the hope that it will be useful, but WITHOUT ANY WARRANTY; without even the implied warranty of MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU General Lesser Public License for more details.
// You should have received a copy of the GNU General Lesser Public License along with talloc. If not, see <http://www.gnu.org/licenses/>.

#ifndef TALLOC_EXT_H
#define TALLOC_EXT_H

#include <stdlib.h>

#include "tree.h"

#ifdef TALLOC_EXT_DESTRUCTOR
typedef void ( * talloc_destructor ) ( void * parent_data );
#endif

typedef struct talloc_ext_t {
#ifdef TALLOC_EXT_DESTRUCTOR
    talloc_destructor destructor;
#endif
} talloc_ext;

#ifdef TALLOC_EXT_DESTRUCTOR
uint8_t talloc_set_destructor ( const void * parent_data, talloc_destructor destructor );
#endif

inline
void talloc_free_ext ( talloc_chunk * parent ) {
    talloc_ext * ext = parent->ext;
    if ( ext != NULL ) {
#ifdef TALLOC_EXT_DESTRUCTOR
        talloc_destructor destructor = ext->destructor;
        if ( destructor != NULL ) {
            destructor ( talloc_data_from_chunk ( parent ) );
        }
#endif
        free ( ext );
    }
}

#endif

