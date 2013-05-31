// This file is part of talloc2. Copyright (C) 2013 Andrew Aladjev aladjev.andrew@gmail.com
// talloc2 is free software: you can redistribute it and/or modify it under the terms of the GNU General Lesser Public License as published by the Free Software Foundation, either version 3 of the License, or (at your option) any later version.
// talloc2 is distributed in the hope that it will be useful, but WITHOUT ANY WARRANTY; without even the implied warranty of MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU General Lesser Public License for more details.
// You should have received a copy of the GNU General Lesser Public License along with talloc2. If not, see <http://www.gnu.org/licenses/>.

#include "ext.h"

#ifdef TALLOC_EXT
extern inline
talloc_ext * talloc_ext_new ( uint8_t length );

extern inline
uint8_t talloc_ext_grow ( talloc_ext * ext, uint8_t length );

extern inline
uint8_t talloc_ext_set ( talloc_chunk * child, uint8_t mode, void * data );

extern inline
void * talloc_ext_get ( talloc_chunk * child, uint8_t mode );

extern inline
void talloc_ext_free ( talloc_chunk * child );
#endif
