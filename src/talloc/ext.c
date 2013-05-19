// This file is part of talloc. Copyright (C) 2013 Andrew Aladjev aladjev.andrew@gmail.com
// talloc is free software: you can redistribute it and/or modify it under the terms of the GNU General Lesser Public License as published by the Free Software Foundation, either version 3 of the License, or (at your option) any later version.
// talloc is distributed in the hope that it will be useful, but WITHOUT ANY WARRANTY; without even the implied warranty of MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU General Lesser Public License for more details.
// You should have received a copy of the GNU General Lesser Public License along with talloc. If not, see <http://www.gnu.org/licenses/>.

#include "ext.h"

#ifdef TALLOC_EXT_DESTRUCTOR
uint8_t talloc_set_destructor ( const void * parent_data, talloc_destructor destructor ) {
    talloc_chunk * parent = talloc_chunk_from_data ( parent_data );
    if ( parent == NULL ) {
        return 1;
    }
    if ( parent->ext == NULL ) {
        parent->ext = malloc ( sizeof ( talloc_ext ) );
    }
    parent->ext->destructor = destructor;
    return 0;
}
#endif
