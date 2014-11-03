// This file is part of tralloc. Copyright (C) 2013 Andrew Aladjev aladjev.andrew@gmail.com
// tralloc is free software: you can redistribute it and/or modify it under the terms of the GNU General Lesser Public License as published by the Free Software Foundation, either version 3 of the License, or (at your option) any later version.
// tralloc is distributed in the hope that it will be useful, but WITHOUT ANY WARRANTY; without even the implied warranty of MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU General Lesser Public License for more details.
// You should have received a copy of the GNU General Lesser Public License along with tralloc. If not, see <http://www.gnu.org/licenses/>.

#if !defined ( TRALLOC_POOL_POOL_CHILD_H )
#define TRALLOC_POOL_POOL_CHILD_H

#include "../types.h"


void                  _tralloc_pool_child_new    ( _tralloc_pool_child * pool_child, _tralloc_pool * pool, size_t length, _tralloc_pool_child * prev, _tralloc_pool_child * next );
_tralloc_pool_child * _tralloc_pool_child_resize ( _tralloc_pool_child * pool_child, size_t target_length );
tralloc_error         _tralloc_pool_child_free   ( _tralloc_pool_child * pool_child );


#endif
