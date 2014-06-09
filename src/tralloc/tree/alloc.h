// This file is part of tralloc. Copyright (C) 2013 Andrew Aladjev aladjev.andrew@gmail.com
// tralloc is free software: you can redistribute it and/or modify it under the terms of the GNU General Lesser Public License as published by the Free Software Foundation, either version 3 of the License, or (at your option) any later version.
// tralloc is distributed in the hope that it will be useful, but WITHOUT ANY WARRANTY; without even the implied warranty of MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU General Lesser Public License for more details.
// You should have received a copy of the GNU General Lesser Public License along with tralloc. If not, see <http://www.gnu.org/licenses/>.

#ifndef TRALLOC_TREE_ALLOC_H
#define TRALLOC_TREE_ALLOC_H

#include "../macro.h"
#include "../types.h"

#undef _TRALLOC_INLINE
#ifdef _TRALLOC_TREE_ALLOC_INCLUDED_FROM_OBJECT
#    define _TRALLOC_INLINE _TRALLOC_INLINE_IN_OBJECT
#else
#    define _TRALLOC_INLINE _TRALLOC_INLINE_IN_HEADER
#endif


#if defined(TRALLOC_DEBUG_LOG)

tralloc_error _tralloc_debug_log_new_with_extensions ( const char * file, size_t line, tralloc_context * parent_context, tralloc_context ** child_context, tralloc_extensions extensions, size_t length );

_TRALLOC_INLINE
tralloc_error _tralloc_debug_log_new ( const char * file, size_t line, tralloc_context * parent_context, tralloc_context ** child_context, size_t length )
{
    return _tralloc_debug_log_new_with_extensions ( file, line, parent_context, child_context, 0, length );
}

tralloc_error _tralloc_debug_log_new_zero_with_extensions ( const char * file, size_t line, tralloc_context * parent_context, tralloc_context ** child_context, tralloc_extensions extensions, size_t length );

_TRALLOC_INLINE
tralloc_error _tralloc_debug_log_new_zero ( const char * file, size_t line, tralloc_context * parent_context, tralloc_context ** child_context, size_t length )
{
    return _tralloc_debug_log_new_zero_with_extensions ( file, line, parent_context, child_context, 0, length );
}

_TRALLOC_INLINE
tralloc_error _tralloc_debug_log_new_empty ( const char * file, size_t line, tralloc_context * parent_context, tralloc_context ** child_context )
{
    return _tralloc_debug_log_new ( file, line, parent_context, child_context, 0 );
}

_TRALLOC_INLINE
tralloc_error _tralloc_debug_log_new_empty_with_extensions ( const char * file, size_t line, tralloc_context * parent_context, tralloc_context ** child_context, tralloc_extensions extensions )
{
    return _tralloc_debug_log_new_with_extensions ( file, line, parent_context, child_context, extensions, 0 );
}

#define tralloc_new_with_extensions(...)       _tralloc_debug_log_new_with_extensions       (__FILE__, __LINE__, __VA_ARGS__)
#define tralloc_new(...)                       _tralloc_debug_log_new                       (__FILE__, __LINE__, __VA_ARGS__)
#define tralloc_new_zero_with_extensions(...)  _tralloc_debug_log_new_zero_with_extensions  (__FILE__, __LINE__, __VA_ARGS__)
#define tralloc_new_zero(...)                  _tralloc_debug_log_new_zero                  (__FILE__, __LINE__, __VA_ARGS__)
#define tralloc_new_empty(...)                 _tralloc_debug_log_new_empty                 (__FILE__, __LINE__, __VA_ARGS__)
#define tralloc_new_empty_with_extensions(...) _tralloc_debug_log_new_empty_with_extensions (__FILE__, __LINE__, __VA_ARGS__)

#else

tralloc_error tralloc_new_with_extensions ( tralloc_context * parent_context, tralloc_context ** child_context, tralloc_extensions extensions, size_t length );

_TRALLOC_INLINE
tralloc_error tralloc_new ( tralloc_context * parent_context, tralloc_context ** child_context, size_t length )
{
    return tralloc_new_with_extensions ( parent_context, child_context, 0, length );
}

tralloc_error tralloc_new_zero_with_extensions ( tralloc_context * parent_context, tralloc_context ** child_context, tralloc_extensions extensions, size_t length );

_TRALLOC_INLINE
tralloc_error tralloc_new_zero ( tralloc_context * parent_context, tralloc_context ** child_context, size_t length )
{
    return tralloc_new_zero_with_extensions ( parent_context, child_context, 0, length );
}

_TRALLOC_INLINE
tralloc_error tralloc_new_empty ( tralloc_context * parent_context, tralloc_context ** child_context )
{
    return tralloc_new ( parent_context, child_context, 0 );
}

_TRALLOC_INLINE
tralloc_error tralloc_new_empty_with_extensions ( tralloc_context * parent_context, tralloc_context ** child_context, tralloc_extensions extensions )
{
    return tralloc_new_with_extensions ( parent_context, child_context, extensions, 0 );
}

#endif


#endif
