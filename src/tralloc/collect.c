// This file is part of tralloc. Copyright (C) 2013 Andrew Aladjev aladjev.andrew@gmail.com
// tralloc is free software: you can redistribute it and/or modify it under the terms of the GNU General Lesser Public License as published by the Free Software Foundation, either version 3 of the License, or (at your option) any later version.
// tralloc is distributed in the hope that it will be useful, but WITHOUT ANY WARRANTY; without even the implied warranty of MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU General Lesser Public License for more details.
// You should have received a copy of the GNU General Lesser Public License along with tralloc. If not, see <http://www.gnu.org/licenses/>.

#include <tralloc/config.h>

#define _TRALLOC_INCLUDED_FROM_COMMON_C
#define _TRALLOC_INCLUDED_FROM_TREE_ALLOC_C
#define _TRALLOC_INCLUDED_FROM_TREE_FREE_C

#define _TRALLOC_INCLUDED_FROM_HELPERS_STRING_C

#if defined ( TRALLOC_FILE )
#   define _TRALLOC_INCLUDED_FROM_HELPERS_FILE_C
#endif

#if defined ( TRALLOC_THREADS )
#   define _TRALLOC_INCLUDED_FROM_THREADS_CHUNK_C
#   define _TRALLOC_INCLUDED_FROM_THREADS_LOCK_C
#endif

#if defined ( TRALLOC_LENGTH )
#   define _TRALLOC_INCLUDED_FROM_LENGTH_CHUNK_C
#endif

#if defined ( TRALLOC_DESTRUCTOR )
#   define _TRALLOC_INCLUDED_FROM_DESTRUCTOR_CHUNK_C
#   define _TRALLOC_INCLUDED_FROM_DESTRUCTOR_DELETE_C
#   define _TRALLOC_INCLUDED_FROM_DESTRUCTOR_DESTRUCTORS_C
#endif

#if defined ( TRALLOC_REFERENCE )
#   define _TRALLOC_INCLUDED_FROM_REFERENCE_CHUNK_C
#   define _TRALLOC_INCLUDED_FROM_REFERENCE_COMMON_C
#   define _TRALLOC_INCLUDED_FROM_REFERENCE_HEAD_CHUNK_C
#   define _TRALLOC_INCLUDED_FROM_THREADS_CHUNK_C
#endif

#if defined ( TRALLOC_POOL )
#   define _TRALLOC_INCLUDED_FROM_POOL_CHUNK_C
#   define _TRALLOC_INCLUDED_FROM_POOL_FRAGMENT_C
#   define _TRALLOC_INCLUDED_FROM_POOL_POOL_C
#endif

#if defined ( TRALLOC_UTILS_BUFFER )
#   define _TRALLOC_INCLUDED_FROM_UTILS_BUFFER_C
#endif

// all .c files can be appended after this line
