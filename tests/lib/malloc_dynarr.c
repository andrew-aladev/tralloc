// This file is part of talloc2. Copyright (C) 2013 Andrew Aladjev aladjev.andrew@gmail.com
// talloc2 is free software: you can redistribute it and/or modify it under the terms of the GNU General Public License as published by the Free Software Foundation, either version 3 of the License, or (at your option) any later version.
// talloc2 is distributed in the hope that it will be useful, but WITHOUT ANY WARRANTY; without even the implied warranty of MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU General Public License for more details.
// You should have received a copy of the GNU General Public License along with talloc2. If not, see <http://www.gnu.org/licenses/>.

#include "malloc_dynarr.h"

extern inline
malloc_dynarr * malloc_dynarr_new ( size_t capacity );

extern inline
uint8_t malloc_dynarr_grow ( malloc_dynarr * arr );

extern inline
uint8_t malloc_dynarr_append ( malloc_dynarr * arr, void * pointer );

extern inline
void malloc_dynarr_set ( malloc_dynarr * arr, size_t position, void * pointer );

extern inline
void * malloc_dynarr_get ( malloc_dynarr * arr, size_t position );

extern inline
size_t malloc_dynarr_get_length ( malloc_dynarr * arr );

extern inline
void malloc_dynarr_free ( malloc_dynarr * arr );