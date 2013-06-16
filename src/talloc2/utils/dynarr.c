// This file is part of talloc2. Copyright (C) 2013 Andrew Aladjev aladjev.andrew@gmail.com
// talloc2 is free software: you can redistribute it and/or modify it under the terms of the GNU General Lesser Public License as published by the Free Software Foundation, either version 3 of the License, or (at your option) any later version.
// talloc2 is distributed in the hope that it will be useful, but WITHOUT ANY WARRANTY; without even the implied warranty of MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU General Lesser Public License for more details.
// You should have received a copy of the GNU General Lesser Public License along with talloc2. If not, see <http://www.gnu.org/licenses/>.

#include "dynarr.h"

extern inline
talloc_dynarr * talloc_dynarr_new ( void * ctx, size_t capacity );

extern inline
uint8_t talloc_dynarr_grow ( talloc_dynarr * arr );

extern inline
uint8_t talloc_dynarr_append ( talloc_dynarr * arr, void * pointer );

extern inline
void talloc_dynarr_set ( talloc_dynarr * arr, size_t position, void * pointer );

extern inline
void * talloc_dynarr_get ( talloc_dynarr * arr, size_t position );

extern inline
size_t talloc_dynarr_get_length ( talloc_dynarr * arr );
