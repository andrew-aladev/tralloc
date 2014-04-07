// This file is part of tralloc. Copyright (C) 2013 Andrew Aladjev aladjev.andrew@gmail.com
// tralloc is free software: you can redistribute it and/or modify it under the terms of the GNU General Lesser Public License as published by the Free Software Foundation, either version 3 of the License, or (at your option) any later version.
// tralloc is distributed in the hope that it will be useful, but WITHOUT ANY WARRANTY; without even the implied warranty of MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU General Lesser Public License for more details.
// You should have received a copy of the GNU General Lesser Public License along with tralloc. If not, see <http://www.gnu.org/licenses/>.

#define _TRALLOC_COMMON_INCLUDED_FROM_OBJECT
#include "common.h"

#include <string.h>


static const char _TRALLOC_ERROR_REQUIRED_ARGUMENT_IS_NULL[] = "TRALLOC_ERROR_REQUIRED_ARGUMENT_IS_NULL";
static const char _TRALLOC_ERROR_MALLOC_FAILED[]             = "TRALLOC_ERROR_MALLOC_FAILED";
static const char _TRALLOC_ERROR_CALLOC_FAILED[]             = "TRALLOC_ERROR_CALLOC_FAILED";
static const char _TRALLOC_ERROR_REALLOC_FAILED[]            = "TRALLOC_ERROR_REALLOC_FAILED";
static const char _TRALLOC_ERROR_OPEN_DESCRIPTOR_FAILED[]    = "TRALLOC_ERROR_OPEN_DESCRIPTOR_FAILED";
static const char _TRALLOC_ERROR_CLOSE_DESCRIPTOR_FAILED[]   = "TRALLOC_ERROR_CLOSE_DESCRIPTOR_FAILED";
static const char _TRALLOC_ERROR_PRINTF_FAILED[]             = "TRALLOC_ERROR_PRINTF_FAILED";

static const char _TRALLOC_ERROR_CHILD_EQUALS_PARENT[]   = "TRALLOC_ERROR_CHILD_EQUALS_PARENT";
static const char _TRALLOC_ERROR_CHILD_HAS_SAME_PARENT[] = "TRALLOC_ERROR_CHILD_HAS_SAME_PARENT";

#if defined(TRALLOC_EXTENSIONS)
static const char _TRALLOC_ERROR_NO_SUCH_EXTENSION[] = "TRALLOC_ERROR_NO_SUCH_EXTENSION";
#endif

#if defined(TRALLOC_REFERENCE)
static const char _TRALLOC_ERROR_BOTH_REFERENCES_AND_REFERENSE[] = "TRALLOC_ERROR_BOTH_REFERENCES_AND_REFERENSE";
#endif

#if defined(TRALLOC_POOL)
static const char _TRALLOC_ERROR_POOL_CANT_BE_REALLOCATED[] = "TRALLOC_ERROR_POOL_CANT_BE_REALLOCATED";
#endif

#if defined(TRALLOC_UTILS_BUFFER)
static const char _TRALLOC_ERROR_UTILS_BUFFER_OVERFLOW[] = "TRALLOC_ERROR_UTILS_BUFFER_OVERFLOW";
#endif

#if defined(TRALLOC_THREADS)
static const char _TRALLOC_ERROR_MUTEX_FAILED[]    = "TRALLOC_ERROR_MUTEX_FAILED";
static const char _TRALLOC_ERROR_SPINLOCK_FAILED[] = "TRALLOC_ERROR_SPINLOCK_FAILED";

static const char _TRALLOC_ERROR_NO_PARENT_LOCK[]      = "TRALLOC_ERROR_NO_PARENT_LOCK";
static const char _TRALLOC_ERROR_NO_FIRST_CHILD_LOCK[] = "TRALLOC_ERROR_NO_FIRST_CHILD_LOCK";
static const char _TRALLOC_ERROR_NO_PREV_LOCK[]        = "TRALLOC_ERROR_NO_PREV_LOCK";
static const char _TRALLOC_ERROR_NO_NEXT_LOCK[]        = "TRALLOC_ERROR_NO_NEXT_LOCK";

#endif

const char * tralloc_get_string_for_error ( tralloc_error error )
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

#   if defined(TRALLOC_REFERENCE)
    case TRALLOC_ERROR_BOTH_REFERENCES_AND_REFERENSE:
        return _TRALLOC_ERROR_BOTH_REFERENCES_AND_REFERENSE;
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

    case TRALLOC_ERROR_NO_PARENT_LOCK:
        return _TRALLOC_ERROR_NO_PARENT_LOCK;
    case TRALLOC_ERROR_NO_FIRST_CHILD_LOCK:
        return _TRALLOC_ERROR_NO_FIRST_CHILD_LOCK;
    case TRALLOC_ERROR_NO_PREV_LOCK:
        return _TRALLOC_ERROR_NO_PREV_LOCK;
    case TRALLOC_ERROR_NO_NEXT_LOCK:
        return _TRALLOC_ERROR_NO_NEXT_LOCK;
#   endif

    default:
        return NULL;
    }
}

#if defined(TRALLOC_LENGTH)
static const char _TRALLOC_EXTENSION_LENGTH[] = "TRALLOC_EXTENSION_LENGTH";
#endif

#if defined(TRALLOC_DESTRUCTOR)
static const char _TRALLOC_EXTENSION_DESTRUCTORS[] = "TRALLOC_EXTENSION_DESTRUCTORS";
#endif

#if defined(TRALLOC_REFERENCE)
static const char _TRALLOC_EXTENSION_REFERENCE[]  = "TRALLOC_EXTENSION_REFERENCE";
static const char _TRALLOC_EXTENSION_REFERENCES[] = "TRALLOC_EXTENSION_REFERENCES";
#endif

#if defined(TRALLOC_POOL)
static const char _TRALLOC_EXTENSION_POOL_CHILD[] = "TRALLOC_EXTENSION_POOL_CHILD";
static const char _TRALLOC_EXTENSION_POOL[]       = "TRALLOC_EXTENSION_POOL";
#endif

#if defined(TRALLOC_THREADS)
static const char _TRALLOC_EXTENSION_LOCK_PARENT[]      = "TRALLOC_EXTENSION_LOCK_PARENT";
static const char _TRALLOC_EXTENSION_LOCK_FIRST_CHILD[] = "TRALLOC_EXTENSION_LOCK_FIRST_CHILD";
static const char _TRALLOC_EXTENSION_LOCK_PREV[]        = "TRALLOC_EXTENSION_LOCK_PREV";
static const char _TRALLOC_EXTENSION_LOCK_NEXT[]        = "TRALLOC_EXTENSION_LOCK_NEXT";
#endif

const char * tralloc_get_string_for_extension ( tralloc_extensions extension )
{

#if defined(TRALLOC_LENGTH)
    if ( extension & TRALLOC_EXTENSION_LENGTH ) {
        return _TRALLOC_EXTENSION_LENGTH;
    }
#endif

#if defined(TRALLOC_DESTRUCTOR)
    if ( extension & TRALLOC_EXTENSION_DESTRUCTORS ) {
        return _TRALLOC_EXTENSION_DESTRUCTORS;
    }
#endif

#if defined(TRALLOC_REFERENCE)
    if ( extension & TRALLOC_EXTENSION_REFERENCE ) {
        return _TRALLOC_EXTENSION_REFERENCE;
    } else if ( extension & TRALLOC_EXTENSION_REFERENCES ) {
        return _TRALLOC_EXTENSION_REFERENCES;
    }
#endif

#if defined(TRALLOC_POOL)
    if ( extension & TRALLOC_EXTENSION_POOL_CHILD ) {
        return _TRALLOC_EXTENSION_POOL_CHILD;
    } else if ( extension & TRALLOC_EXTENSION_POOL ) {
        return _TRALLOC_EXTENSION_POOL;
    }
#endif

#if defined(TRALLOC_THREADS)
    if ( extension & TRALLOC_EXTENSION_LOCK_PARENT ) {
        return _TRALLOC_EXTENSION_LOCK_PARENT;
    } else if ( extension & TRALLOC_EXTENSION_LOCK_FIRST_CHILD ) {
        return _TRALLOC_EXTENSION_LOCK_FIRST_CHILD;
    } else if ( extension & TRALLOC_EXTENSION_LOCK_PREV ) {
        return _TRALLOC_EXTENSION_LOCK_PREV;
    } else if ( extension & TRALLOC_EXTENSION_LOCK_NEXT ) {
        return _TRALLOC_EXTENSION_LOCK_NEXT;
    }
#endif

    return NULL;
}

size_t tralloc_predict_chunk_length ( tralloc_extensions extensions )
{
    size_t extensions_length = 0;

#   if defined(TRALLOC_LENGTH)
    if ( extensions & TRALLOC_EXTENSION_LENGTH ) {
        extensions_length += sizeof ( _tralloc_length );
    }
#   endif

#   if defined(TRALLOC_DESTRUCTOR)
    if ( extensions & TRALLOC_EXTENSION_DESTRUCTORS ) {
        extensions_length += sizeof ( _tralloc_destructors );
    }
#   endif

#   if defined(TRALLOC_REFERENCE)
    if ( extensions & TRALLOC_EXTENSION_REFERENCES ) {
        extensions_length += sizeof ( _tralloc_references );
    } else if ( extensions & TRALLOC_EXTENSION_REFERENCE ) {
        extensions_length += sizeof ( _tralloc_reference );
    }
#   endif

#   if defined(TRALLOC_POOL)
    if ( extensions & TRALLOC_EXTENSION_POOL ) {
        extensions_length += sizeof ( _tralloc_pool );
    } else if ( extensions & TRALLOC_EXTENSION_POOL_CHILD ) {
        extensions_length += sizeof ( _tralloc_pool_child );
    }
#   endif

    return extensions_length + sizeof ( _tralloc_chunk );
}
