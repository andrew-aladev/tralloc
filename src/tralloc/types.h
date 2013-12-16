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

typedef struct tralloc_destructor_t {
    struct tralloc_destructor_t * next;
    tralloc_destructor_function function;
    void * user_data;
} tralloc_destructor;
#endif

#if defined(TRALLOC_REFERENCE)
typedef struct tralloc_reference_t {
    struct tralloc_extensions_t * parent_extensions;
    struct tralloc_reference_t *  prev;
    struct tralloc_reference_t *  next;

#if defined(TRALLOC_DESTRUCTOR)
    tralloc_destructor * first_destructor;
#endif

} tralloc_reference;
#endif

#if defined(TRALLOC_EXTENSIONS)
typedef struct tralloc_extensions_t {

#if defined(TRALLOC_DESTRUCTOR)
    tralloc_destructor * first_destructor;
#endif

#if defined(TRALLOC_REFERENCE)
    tralloc_reference * first_reference;
#endif

} tralloc_extensions;
#endif

#if defined(TRALLOC_REFERENCE)
enum {
    TRALLOC_MODE_EXTENSIONS = 0,
    TRALLOC_MODE_REFERENCE
};
#endif

typedef struct tralloc_chunk_t {
    struct tralloc_chunk_t * parent;
    struct tralloc_chunk_t * prev;
    struct tralloc_chunk_t * next;
    struct tralloc_chunk_t * first_child;

#if defined(TRALLOC_REFERENCE)
    uint8_t mode;
#endif

#if defined(TRALLOC_DEBUG)
    size_t chunk_length;
    size_t length;
#endif

} tralloc_chunk;

#endif