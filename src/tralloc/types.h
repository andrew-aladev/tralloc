// This file is part of tralloc. Copyright (C) 2013 Andrew Aladjev aladjev.andrew@gmail.com
// tralloc is free software: you can redistribute it and/or modify it under the terms of the GNU General Lesser Public License as published by the Free Software Foundation, either version 3 of the License, or (at your option) any later version.
// tralloc is distributed in the hope that it will be useful, but WITHOUT ANY WARRANTY; without even the implied warranty of MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU General Lesser Public License for more details.
// You should have received a copy of the GNU General Lesser Public License along with tralloc. If not, see <http://www.gnu.org/licenses/>.

#ifndef TRALLOC_TYPES_H
#define TRALLOC_TYPES_H

#include <stdint.h>
#include <stddef.h>

#include "config.h"


typedef void tralloc_context;

#if defined(TRALLOC_LENGTH)
typedef struct _tralloc_length_t {
    size_t length;
} _tralloc_length;
#endif

#if defined(TRALLOC_DESTRUCTOR)
typedef uint8_t ( * tralloc_destructor_function ) ( tralloc_context * chunk_context, void * user_data );

typedef struct _tralloc_destructor_t {
    struct _tralloc_destructor_t * next;
    tralloc_destructor_function function;
    void * user_data;
} _tralloc_destructor;

typedef struct _tralloc_destructors_t {
    _tralloc_destructor * first_destructor;
    _tralloc_destructor * last_destructor;
} _tralloc_destructors;
#endif

#if defined(TRALLOC_REFERENCE)
typedef struct _tralloc_reference_t {
    struct _tralloc_chunk_t * references;
    struct _tralloc_chunk_t * prev;
    struct _tralloc_chunk_t * next;
} _tralloc_reference;
#endif

#if defined(TRALLOC_REFERENCE)
typedef struct _tralloc_references_t {
    struct _tralloc_chunk_t * first_reference;
} _tralloc_references;
#endif

#if defined(TRALLOC_EXTENSIONS)
enum {

#if defined(TRALLOC_DESTRUCTOR)
    TRALLOC_HAVE_LENGTH = 1,
#endif

#if defined(TRALLOC_LENGTH)
    TRALLOC_HAVE_DESTRUCTORS = 1 << 1,
#endif

#if defined(TRALLOC_REFERENCE)
    TRALLOC_HAVE_REFERENCE  = 1 << 2,
    TRALLOC_HAVE_REFERENCES = 1 << 3,
#endif

};
#endif

enum {
    TRALLOC_ERROR_CONTEXT_IS_NULL = 1,

#if defined(TRALLOC_EXTENSIONS)
    TRALLOC_ERROR_NO_SUCH_EXTENSION,
#endif

};

typedef struct _tralloc_chunk_t {
    struct _tralloc_chunk_t * parent;
    struct _tralloc_chunk_t * prev;
    struct _tralloc_chunk_t * next;
    struct _tralloc_chunk_t * first_child;

#if defined(TRALLOC_EXTENSIONS)
    uint8_t extensions;
    size_t length;
#endif

#if defined(TRALLOC_DEBUG)
    uint8_t chunk_length;
#endif

} _tralloc_chunk;


#endif
