// This file is part of tralloc. Copyright (C) 2013 Andrew Aladjev aladjev.andrew@gmail.com
// tralloc is free software: you can redistribute it and/or modify it under the terms of the GNU General Lesser Public License as published by the Free Software Foundation, either version 3 of the License, or (at your option) any later version.
// tralloc is distributed in the hope that it will be useful, but WITHOUT ANY WARRANTY; without even the implied warranty of MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU General Lesser Public License for more details.
// You should have received a copy of the GNU General Lesser Public License along with tralloc. If not, see <http://www.gnu.org/licenses/>.

#include "head_chunk.h"


extern inline tralloc_error _tralloc_pool_new_chunk                 ( _tralloc_chunk * chunk, void * memory, size_t length );
extern inline bool          _tralloc_pool_can_alloc                 ( _tralloc_pool * pool, size_t length );
extern inline void          _tralloc_pool_attach_fragment           ( _tralloc_pool * pool, _tralloc_pool_fragment * fragment, _tralloc_pool_fragment * prev_fragment, _tralloc_pool_fragment * next_fragment );
extern inline void          _tralloc_pool_detach_fragment           ( _tralloc_pool * pool, _tralloc_pool_fragment * fragment );
extern inline void          _tralloc_pool_new_fragment_insert_after ( _tralloc_pool * pool, _tralloc_pool_fragment * new_fragment, _tralloc_pool_fragment * prev_fragment, _tralloc_pool_fragment * next_fragment );
extern inline void          _tralloc_pool_alloc                     ( _tralloc_pool * pool, void ** memory, size_t length, bool zero, _tralloc_pool_child ** prev_pool_child, _tralloc_pool_child ** next_pool_child );
extern inline bool          _tralloc_pool_try_free_chunk            ( _tralloc_chunk * chunk );
