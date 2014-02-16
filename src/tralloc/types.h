// This file is part of tralloc. Copyright (C) 2013 Andrew Aladjev aladjev.andrew@gmail.com
// tralloc is free software: you can redistribute it and/or modify it under the terms of the GNU General Lesser Public License as published by the Free Software Foundation, either version 3 of the License, or (at your option) any later version.
// tralloc is distributed in the hope that it will be useful, but WITHOUT ANY WARRANTY; without even the implied warranty of MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU General Lesser Public License for more details.
// You should have received a copy of the GNU General Lesser Public License along with tralloc. If not, see <http://www.gnu.org/licenses/>.

#ifndef TRALLOC_TYPES_H
#define TRALLOC_TYPES_H

#include <stdint.h>
#include <stddef.h>
#include <stdbool.h>

#include "config.h"


typedef void tralloc_context;

enum {
    TRALLOC_ERROR_REQUIRED_ARGUMENT_IS_NULL = 1,
    TRALLOC_ERROR_MALLOC_FAILED,
    TRALLOC_ERROR_CALLOC_FAILED,
    TRALLOC_ERROR_REALLOC_FAILED,
    TRALLOC_ERROR_OPEN_DESCRIPTOR_FAILED,
    TRALLOC_ERROR_CLOSE_DESCRIPTOR_FAILED,
    TRALLOC_ERROR_PRINTF_FAILED,

    TRALLOC_ERROR_CHILD_EQUALS_PARENT,
    TRALLOC_ERROR_CHILD_HAS_SAME_PARENT,

#if defined(TRALLOC_EXTENSIONS)
    TRALLOC_ERROR_NO_SUCH_EXTENSION,
#endif

#if defined(TRALLOC_REFERENCE)
    TRALLOC_ERROR_BOTH_REFERENCES_AND_REFERENSE,
#endif

#if defined(TRALLOC_POOL)
    TRALLOC_ERROR_POOL_CANT_BE_REALLOCATED,
#endif

#if defined(TRALLOC_UTILS_BUFFER)
    TRALLOC_ERROR_UTILS_BUFFER_OVERFLOW,
#endif

};
typedef uint8_t tralloc_error;

#if defined(TRALLOC_EXTENSIONS)
enum {

#if defined(TRALLOC_LENGTH)
    TRALLOC_EXTENSION_LENGTH = 1,
#endif

#if defined(TRALLOC_DESTRUCTOR)
    TRALLOC_EXTENSION_DESTRUCTORS = 1 << 1,
#endif

#if defined(TRALLOC_REFERENCE)
    TRALLOC_EXTENSION_REFERENCE  = 1 << 2,
    TRALLOC_EXTENSION_REFERENCES = 1 << 3,
#endif

#if defined(TRALLOC_POOL)
    TRALLOC_EXTENSION_POOL_CHILD = 1 << 4,
    TRALLOC_EXTENSION_POOL       = 1 << 5,
#endif

};
typedef uint8_t tralloc_extensions;
#endif

#if defined(TRALLOC_LENGTH)
typedef struct _tralloc_length_type {
    size_t length;
} _tralloc_length;
#endif

#if defined(TRALLOC_DESTRUCTOR)
typedef tralloc_error ( * tralloc_destructor_function ) ( tralloc_context * chunk_context, void * user_data );

typedef struct _tralloc_destructor_type {
    struct _tralloc_destructor_type * next;
    tralloc_destructor_function function;
    void * user_data;
} _tralloc_destructor;

typedef struct _tralloc_destructors_type {
    _tralloc_destructor * first_destructor;
    _tralloc_destructor * last_destructor;
} _tralloc_destructors;
#endif

#if defined(TRALLOC_REFERENCE)
typedef struct _tralloc_reference_type {
    struct _tralloc_references_type * references;
    struct _tralloc_reference_type * prev;
    struct _tralloc_reference_type * next;
} _tralloc_reference;

typedef struct _tralloc_references_type {
    _tralloc_reference * first_reference;
    tralloc_extensions extensions;
    bool autofree;
} _tralloc_references;
#endif

#if defined(TRALLOC_POOL)
// pool_child should be the first in the stack of extensions
typedef struct _tralloc_pool_child_type {
    struct _tralloc_pool_type * pool;
    struct _tralloc_pool_child_type * prev;
    struct _tralloc_pool_child_type * next;
    size_t length;
} _tralloc_pool_child;

typedef struct _tralloc_pool_fragment_type {
    struct _tralloc_pool_fragment_type * prev;
    struct _tralloc_pool_fragment_type * next;
    _tralloc_pool_child * prev_child;
    _tralloc_pool_child * next_child;
    size_t length;
} _tralloc_pool_fragment;

typedef struct _tralloc_pool_type {
    _tralloc_pool_child    * first_child;
    _tralloc_pool_fragment * max_fragment;
    void * memory;
    size_t length;
    bool autofree;
} _tralloc_pool;
#endif

typedef struct _tralloc_chunk_type {
    struct _tralloc_chunk_type * parent;
    struct _tralloc_chunk_type * prev;
    struct _tralloc_chunk_type * next;
    struct _tralloc_chunk_type * first_child;

#if defined(TRALLOC_EXTENSIONS)
    tralloc_extensions extensions;
#endif

#if defined(TRALLOC_DEBUG)
    size_t chunk_length;
    size_t length;
#endif

} _tralloc_chunk;


#endif
