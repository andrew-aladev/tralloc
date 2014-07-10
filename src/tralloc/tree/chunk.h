// This file is part of tralloc. Copyright (C) 2013 Andrew Aladjev aladjev.andrew@gmail.com
// tralloc is free software: you can redistribute it and/or modify it under the terms of the GNU General Lesser Public License as published by the Free Software Foundation, either version 3 of the License, or (at your option) any later version.
// tralloc is distributed in the hope that it will be useful, but WITHOUT ANY WARRANTY; without even the implied warranty of MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU General Lesser Public License for more details.
// You should have received a copy of the GNU General Lesser Public License along with tralloc. If not, see <http://www.gnu.org/licenses/>.

#if !defined ( TRALLOC_TREE_CHUNK_H )
#define TRALLOC_TREE_CHUNK_H

#include "../types.h"


void _tralloc_update_chunk ( _tralloc_chunk * chunk );

// "chunk" can't be NULL, "new_parent_chunk" can be NULL.
void _tralloc_attach_chunk ( _tralloc_chunk * chunk, _tralloc_chunk * new_parent_chunk );


#endif
