// This file is part of tralloc. Copyright (C) 2013 Andrew Aladjev aladjev.andrew@gmail.com
// tralloc is free software: you can redistribute it and/or modify it under the terms of the GNU General Lesser Public License as published by the Free Software Foundation, either version 3 of the License, or (at your option) any later version.
// tralloc is distributed in the hope that it will be useful, but WITHOUT ANY WARRANTY; without even the implied warranty of MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU General Lesser Public License for more details.
// You should have received a copy of the GNU General Lesser Public License along with tralloc. If not, see <http://www.gnu.org/licenses/>.

#include "chunk.h"

extern inline tralloc_chunk * tralloc_usual_process_new_chunk ( tralloc_chunk * chunk, const tralloc_context * parent_context, size_t length );
extern inline tralloc_chunk * tralloc_usual_malloc_chunk      ( const tralloc_context * parent_context, size_t length );
extern inline tralloc_chunk * tralloc_usual_calloc_chunk      ( const tralloc_context * parent_context, size_t length );
extern inline tralloc_chunk * tralloc_usual_realloc_chunk     ( tralloc_chunk * chunk, size_t length );
extern inline uint8_t         tralloc_usual_free_chunk        ( tralloc_chunk * chunk );