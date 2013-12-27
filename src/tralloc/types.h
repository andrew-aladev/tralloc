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
    struct _tralloc_references_t * references;
    struct _tralloc_reference_t *  prev;
    struct _tralloc_reference_t *  next;
} _tralloc_reference;
#endif

#if defined(TRALLOC_REFERENCE)
typedef struct _tralloc_references_t {
    _tralloc_reference * first_reference;
} _tralloc_references;
#endif

#if defined(TRALLOC_REFERENCE)
enum {
    TRALLOC_MODE_REFERENCES = 0,
    TRALLOC_MODE_REFERENCE
};
#endif

typedef struct _tralloc_chunk_t {
    struct _tralloc_chunk_t * parent;
    struct _tralloc_chunk_t * prev;
    struct _tralloc_chunk_t * next;
    struct _tralloc_chunk_t * first_child;

#if defined(TRALLOC_DESTRUCTOR)
    _tralloc_destructors * destructors;
#endif

#if defined(TRALLOC_REFERENCE)
    uint8_t mode;
#endif

#if defined(TRALLOC_DEBUG)
    size_t chunk_length;
    size_t length;
#endif

} _tralloc_chunk;

#endif