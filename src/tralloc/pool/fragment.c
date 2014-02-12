// This file is part of tralloc. Copyright (C) 2013 Andrew Aladjev aladjev.andrew@gmail.com
// tralloc is free software: you can redistribute it and/or modify it under the terms of the GNU General Lesser Public License as published by the Free Software Foundation, either version 3 of the License, or (at your option) any later version.
// tralloc is distributed in the hope that it will be useful, but WITHOUT ANY WARRANTY; without even the implied warranty of MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU General Lesser Public License for more details.
// You should have received a copy of the GNU General Lesser Public License along with tralloc. If not, see <http://www.gnu.org/licenses/>.

#include "fragment.h"


extern inline void _tralloc_pool_fragment_attach                  ( _tralloc_pool * pool, _tralloc_pool_fragment * fragment, _tralloc_pool_fragment * prev, _tralloc_pool_fragment * next );
extern inline void _tralloc_pool_fragment_detach                  ( _tralloc_pool * pool, _tralloc_pool_fragment * fragment );
extern inline void _tralloc_pool_fragment_increased               ( _tralloc_pool * pool, _tralloc_pool_fragment * fragment );
extern inline void _tralloc_pool_fragment_decreased               ( _tralloc_pool * pool, _tralloc_pool_fragment * fragment );
extern inline void _tralloc_pool_fragment_resize_with_right_align ( _tralloc_pool * pool, _tralloc_pool_fragment * fragment, size_t new_fragment_length );
