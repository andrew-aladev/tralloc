// This file is part of talloc2. Copyright (C) 2013 Andrew Aladjev aladjev.andrew@gmail.com
// talloc2 is free software: you can redistribute it and/or modify it under the terms of the GNU General Lesser Public License as published by the Free Software Foundation, either version 3 of the License, or (at your option) any later version.
// talloc2 is distributed in the hope that it will be useful, but WITHOUT ANY WARRANTY; without even the implied warranty of MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU General Lesser Public License for more details.
// You should have received a copy of the GNU General Lesser Public License along with talloc2. If not, see <http://www.gnu.org/licenses/>.

#ifndef TALLOC_EXT_DESTRUCTOR_H
#define TALLOC_EXT_DESTRUCTOR_H

#include "../tree.h"
#include "core.h"

#include <stdio.h>

inline
uint8_t talloc_destructor_on_del ( talloc_chunk * child )
{
    if ( child->ext != NULL && ( child->ext->mode & TALLOC_MODE_DESTRUCTOR ) != 0 ) {
        talloc_destructor destructor = talloc_ext_get ( child, TALLOC_EXT_INDEX_DESTRUCTOR );
        if ( destructor == NULL ) {
            return 1;
        }
        if ( destructor ( talloc_data_from_chunk ( child ) ) != 0 ) {
            return 2;
        }
    }
    return 0;
}

inline
uint8_t talloc_set_destructor ( const void * child_data, talloc_destructor destructor )
{
    if ( child_data == NULL ) {
        return 1;
    }
    talloc_chunk * child = talloc_chunk_from_data ( child_data );
    if ( talloc_ext_set ( child, TALLOC_EXT_INDEX_DESTRUCTOR, destructor ) ) {
        return 2;
    }
    child->ext->mode |= TALLOC_MODE_DESTRUCTOR;
    return 0;
}

#endif
