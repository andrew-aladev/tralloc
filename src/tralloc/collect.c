// This file is part of tralloc. Copyright (C) 2013 Andrew Aladjev aladjev.andrew@gmail.com
// tralloc is free software: you can redistribute it and/or modify it under the terms of the GNU General Lesser Public License as published by the Free Software Foundation, either version 3 of the License, or (at your option) any later version.
// tralloc is distributed in the hope that it will be useful, but WITHOUT ANY WARRANTY; without even the implied warranty of MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU General Lesser Public License for more details.
// You should have received a copy of the GNU General Lesser Public License along with tralloc. If not, see <http://www.gnu.org/licenses/>.

#include <tralloc/config.h>

// for common.c
#define _TRALLOC_COMMON_INCLUDED_FROM_OBJECT

// for tree/alloc.c
#define _TRALLOC_TREE_ALLOC_INCLUDED_FROM_OBJECT

// for tree/free.c
#define _TRALLOC_TREE_FREE_INCLUDED_FROM_OBJECT

// for helpers/string.c
#define _TRALLOC_HELPERS_STRING_INCLUDED_FROM_OBJECT

#if defined(TRALLOC_FILE)

    // for helpers/file.c
#   define _TRALLOC_HELPERS_FILE_INCLUDED_FROM_OBJECT

#endif

#if defined(TRALLOC_THREADS)

    // for threads/mutex.c
#   define _TRALLOC_THREADS_MUTEX_INCLUDED_FROM_OBJECT

    // for threads/spinlock.c
#   define _TRALLOC_THREADS_SPINLOCK_INCLUDED_FROM_OBJECT

#endif

#if defined(TRALLOC_LENGTH)

    // for length/chunk.c
#   define _TRALLOC_LENGTH_CHUNK_INCLUDED_FROM_OBJECT

#endif

#if defined(TRALLOC_DESTRUCTOR)

    // for destructor/chunk.c
#   define _TRALLOC_DESTRUCTOR_CHUNK_INCLUDED_FROM_OBJECT

    // for destructor/delete.c
#   define _TRALLOC_DESTRUCTOR_DELETE_INCLUDED_FROM_OBJECT

#endif

#if defined(TRALLOC_REFERENCE)

    // for reference/chunk.c
#   define _TRALLOC_REFERENCE_CHUNK_INCLUDED_FROM_OBJECT

    // for reference/common.c
#   define _TRALLOC_REFERENCE_COMMON_INCLUDED_FROM_OBJECT

    // for reference/head_chunk.c
#   define _TRALLOC_REFERENCE_HEAD_CHUNK_INCLUDED_FROM_OBJECT

    // for threads/chunk.c
#   define _TRALLOC_THREADS_CHUNK_INCLUDED_FROM_OBJECT

#endif

#if defined(TRALLOC_POOL)

    // for pool/chunk.c
#   define _TRALLOC_POOL_CHUNK_INCLUDED_FROM_OBJECT

    // for pool/common.c
#   define _TRALLOC_POOL_COMMON_INCLUDED_FROM_OBJECT

    // for pool/fragment.c
#   define _TRALLOC_POOL_FRAGMENT_INCLUDED_FROM_OBJECT

    // for pool/head_chunk.c
#   define _TRALLOC_POOL_HEAD_CHUNK_INCLUDED_FROM_OBJECT

#endif

#if defined(TRALLOC_UTILS_BUFFER)

    // for utils/buffer.c
#   define _TRALLOC_UTILS_BUFFER_INCLUDED_FROM_OBJECT

#endif

// all .c files can be appended after this line
