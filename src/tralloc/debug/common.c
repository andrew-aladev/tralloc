// This file is part of tralloc. Copyright (C) 2013 Andrew Aladjev aladjev.andrew@gmail.com
// tralloc is free software: you can redistribute it and/or modify it under the terms of the GNU General Lesser Public License as published by the Free Software Foundation, either version 3 of the License, or (at your option) any later version.
// tralloc is distributed in the hope that it will be useful, but WITHOUT ANY WARRANTY; without even the implied warranty of MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU General Lesser Public License for more details.
// You should have received a copy of the GNU General Lesser Public License along with tralloc. If not, see <http://www.gnu.org/licenses/>.

#include "common.h"


#if defined(TRALLOC_DEBUG_LOG)

static const char _TRALLOC_ERROR_REQUIRED_ARGUMENT_IS_NULL [] = "required argument is NULL";
static const char _TRALLOC_ERROR_MALLOC_FAILED             [] = "malloc failed";
static const char _TRALLOC_ERROR_CALLOC_FAILED             [] = "calloc failed";
static const char _TRALLOC_ERROR_REALLOC_FAILED            [] = "realloc failed";
static const char _TRALLOC_ERROR_OPEN_DESCRIPTOR_FAILED    [] = "open descriptor failed";
static const char _TRALLOC_ERROR_CLOSE_DESCRIPTOR_FAILED   [] = "close descriptor failed";
static const char _TRALLOC_ERROR_PRINTF_FAILED             [] = "printf failed";

static const char _TRALLOC_ERROR_CHILD_EQUALS_PARENT   [] = "child equals parent";
static const char _TRALLOC_ERROR_CHILD_HAS_SAME_PARENT [] = "child has same parent";

#if defined(TRALLOC_EXTENSIONS)
static const char _TRALLOC_ERROR_NO_SUCH_EXTENSION [] = "no such extension";
#endif

#if defined(TRALLOC_POOL)
static const char _TRALLOC_ERROR_POOL_CANT_BE_REALLOCATED [] = "pool cant be reallocated";
#endif

#if defined(TRALLOC_UTILS_BUFFER)
static const char _TRALLOC_ERROR_UTILS_BUFFER_OVERFLOW [] = "overflow of buffer util";
#endif

#if defined(TRALLOC_THREADS)
static const char _TRALLOC_ERROR_MUTEX_FAILED    [] = "mutex failed";
static const char _TRALLOC_ERROR_SPINLOCK_FAILED [] = "spinlock failed";

static const char _TRALLOC_ERROR_NO_SUBTREE_LOCK  [] = "chunk should have TRALLOC_EXTENSION_LOCK_SUBTREE";
static const char _TRALLOC_ERROR_NO_CHILDREN_LOCK [] = "chunk should have TRALLOC_EXTENSION_LOCK_CHILDREN";
#endif

const char * tralloc_debug_get_string_for_error ( tralloc_error error )
{
    switch ( error ) {
    case TRALLOC_ERROR_REQUIRED_ARGUMENT_IS_NULL:
        return _TRALLOC_ERROR_REQUIRED_ARGUMENT_IS_NULL;
    case TRALLOC_ERROR_MALLOC_FAILED:
        return _TRALLOC_ERROR_MALLOC_FAILED;
    case TRALLOC_ERROR_CALLOC_FAILED:
        return _TRALLOC_ERROR_CALLOC_FAILED;
    case TRALLOC_ERROR_REALLOC_FAILED:
        return _TRALLOC_ERROR_REALLOC_FAILED;
    case TRALLOC_ERROR_OPEN_DESCRIPTOR_FAILED:
        return _TRALLOC_ERROR_OPEN_DESCRIPTOR_FAILED;
    case TRALLOC_ERROR_CLOSE_DESCRIPTOR_FAILED:
        return _TRALLOC_ERROR_CLOSE_DESCRIPTOR_FAILED;
    case TRALLOC_ERROR_PRINTF_FAILED:
        return _TRALLOC_ERROR_PRINTF_FAILED;

    case TRALLOC_ERROR_CHILD_EQUALS_PARENT:
        return _TRALLOC_ERROR_CHILD_EQUALS_PARENT;
    case TRALLOC_ERROR_CHILD_HAS_SAME_PARENT:
        return _TRALLOC_ERROR_CHILD_HAS_SAME_PARENT;

#   if defined(TRALLOC_EXTENSIONS)
    case TRALLOC_ERROR_NO_SUCH_EXTENSION:
        return _TRALLOC_ERROR_NO_SUCH_EXTENSION;
#   endif

#   if defined(TRALLOC_POOL)
    case TRALLOC_ERROR_POOL_CANT_BE_REALLOCATED:
        return _TRALLOC_ERROR_POOL_CANT_BE_REALLOCATED;
#   endif

#   if defined(TRALLOC_UTILS_BUFFER)
    case TRALLOC_ERROR_UTILS_BUFFER_OVERFLOW:
        return _TRALLOC_ERROR_UTILS_BUFFER_OVERFLOW;
#   endif

#   if defined(TRALLOC_THREADS)
    case TRALLOC_ERROR_MUTEX_FAILED:
        return _TRALLOC_ERROR_MUTEX_FAILED;
    case TRALLOC_ERROR_SPINLOCK_FAILED:
        return _TRALLOC_ERROR_SPINLOCK_FAILED;

    case TRALLOC_ERROR_NO_CHILDREN_LOCK:
        return _TRALLOC_ERROR_NO_CHILDREN_LOCK;
    case TRALLOC_ERROR_NO_SUBTREE_LOCK:
        return _TRALLOC_ERROR_NO_SUBTREE_LOCK;
#   endif

    default:
        return NULL;
    }
}

#endif
