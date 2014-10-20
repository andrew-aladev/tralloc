// This file is part of tralloc. Copyright (C) 2013 Andrew Aladjev aladjev.andrew@gmail.com
// tralloc is free software: you can redistribute it and/or modify it under the terms of the GNU General Lesser Public License as published by the Free Software Foundation, either version 3 of the License, or (at your option) any later version.
// tralloc is distributed in the hope that it will be useful, but WITHOUT ANY WARRANTY; without even the implied warranty of MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU General Lesser Public License for more details.
// You should have received a copy of the GNU General Lesser Public License along with tralloc. If not, see <http://www.gnu.org/licenses/>.

#if !defined ( TRALLOC_TREE_ALLOC_H )
#define TRALLOC_TREE_ALLOC_H

#include "../macro.h"
#include "../types.h"

#undef _TRALLOC_INLINE
#if defined ( _TRALLOC_INCLUDED_FROM_TREE_ALLOC_C )
#    define _TRALLOC_INLINE _TRALLOC_INLINE_IN_OBJECT
#else
#    define _TRALLOC_INLINE _TRALLOC_INLINE_IN_HEADER
#endif


typedef struct _tralloc_alloc_options_type {
    tralloc_context * parent_context;
    size_t length;
    tralloc_bool zero;

#   if defined ( TRALLOC_EXTENSIONS )
    tralloc_extensions extensions;
#   endif

#   if defined ( TRALLOC_DEBUG_LOG )
    const char * file;
    size_t line;
#   endif

} _tralloc_alloc_options;

tralloc_error _tralloc_alloc_internal ( _tralloc_alloc_options * options, tralloc_context ** child_context );

_TRALLOC_INLINE
tralloc_error _tralloc_alloc ( _tralloc_alloc_options * options, tralloc_context * parent_context, tralloc_context ** child_context, size_t length )
{
    options->parent_context = parent_context;
    options->length         = length;
    return _tralloc_alloc_internal ( options, child_context );
}
_TRALLOC_INLINE
tralloc_error _tralloc_forward_to_alloc ( _tralloc_alloc_options * options, tralloc_context * parent_context, tralloc_context ** child_context, size_t length )
{

#   if defined ( TRALLOC_EXTENSIONS )
    options->extensions = 0;
#   endif

    return _tralloc_alloc ( options, parent_context, child_context, length );
}

#if defined ( TRALLOC_EXTENSIONS )
_TRALLOC_INLINE
tralloc_error _tralloc_forward_to_alloc_with_extensions ( _tralloc_alloc_options * options, tralloc_context * parent_context, tralloc_context ** child_context, size_t length, tralloc_extensions extensions )
{
    options->extensions = extensions;
    return _tralloc_alloc ( options, parent_context, child_context, length );
}
#endif

#if defined ( TRALLOC_DEBUG_LOG )
_TRALLOC_INLINE
tralloc_error _tralloc_debug_log_forward_to_alloc ( _tralloc_alloc_options * options, const char * file, size_t line, tralloc_context * parent_context, tralloc_context ** child_context, size_t length )
{
    options->file = file;
    options->line = line;
    return _tralloc_forward_to_alloc ( options, parent_context, child_context, length );
}

_TRALLOC_INLINE
tralloc_error _tralloc_debug_log_new ( const char * file, size_t line, tralloc_context * parent_context, tralloc_context ** child_context, size_t length )
{
    _tralloc_alloc_options options;
    options.zero = false;
    return _tralloc_debug_log_forward_to_alloc ( &options, file, line, parent_context, child_context, length );
}
_TRALLOC_INLINE
tralloc_error _tralloc_debug_log_new_zero ( const char * file, size_t line, tralloc_context * parent_context, tralloc_context ** child_context, size_t length )
{
    _tralloc_alloc_options options;
    options.zero = true;
    return _tralloc_debug_log_forward_to_alloc ( &options, file, line, parent_context, child_context, length );
}
_TRALLOC_INLINE
tralloc_error _tralloc_debug_log_new_empty ( const char * file, size_t line, tralloc_context * parent_context, tralloc_context ** child_context )
{
    return _tralloc_debug_log_new ( file, line, parent_context, child_context, 0 );
}

#define tralloc_new(...)       _tralloc_debug_log_new       (__FILE__, __LINE__, __VA_ARGS__)
#define tralloc_new_zero(...)  _tralloc_debug_log_new_zero  (__FILE__, __LINE__, __VA_ARGS__)
#define tralloc_new_empty(...) _tralloc_debug_log_new_empty (__FILE__, __LINE__, __VA_ARGS__)

#if defined ( TRALLOC_EXTENSIONS )
_TRALLOC_INLINE
tralloc_error _tralloc_debug_log_forward_to_alloc_with_extensions ( _tralloc_alloc_options * options, const char * file, size_t line, tralloc_context * parent_context, tralloc_context ** child_context, size_t length, tralloc_extensions extensions )
{
    options->file = file;
    options->line = line;
    return _tralloc_forward_to_alloc_with_extensions ( options, parent_context, child_context, length, extensions );
}
_TRALLOC_INLINE
tralloc_error _tralloc_debug_log_new_with_extensions ( const char * file, size_t line, tralloc_context * parent_context, tralloc_context ** child_context, size_t length, tralloc_extensions extensions )
{
    _tralloc_alloc_options options;
    options.zero = false;
    return _tralloc_debug_log_forward_to_alloc_with_extensions ( &options, file, line, parent_context, child_context, length, extensions );
}
_TRALLOC_INLINE
tralloc_error _tralloc_debug_log_new_zero_with_extensions ( const char * file, size_t line, tralloc_context * parent_context, tralloc_context ** child_context, size_t length, tralloc_extensions extensions )
{
    _tralloc_alloc_options options;
    options.zero = true;
    return _tralloc_debug_log_forward_to_alloc_with_extensions ( &options, file, line, parent_context, child_context, length, extensions );
}
_TRALLOC_INLINE
tralloc_error _tralloc_debug_log_new_empty_with_extensions ( const char * file, size_t line, tralloc_context * parent_context, tralloc_context ** child_context, tralloc_extensions extensions )
{
    return _tralloc_debug_log_new_with_extensions ( file, line, parent_context, child_context, 0, extensions );
}

#define tralloc_new_with_extensions(...)       _tralloc_debug_log_new_with_extensions       (__FILE__, __LINE__, __VA_ARGS__)
#define tralloc_new_zero_with_extensions(...)  _tralloc_debug_log_new_zero_with_extensions  (__FILE__, __LINE__, __VA_ARGS__)
#define tralloc_new_empty_with_extensions(...) _tralloc_debug_log_new_empty_with_extensions (__FILE__, __LINE__, __VA_ARGS__)

#endif

#else
_TRALLOC_INLINE
tralloc_error tralloc_new ( tralloc_context * parent_context, tralloc_context ** child_context, size_t length )
{
    _tralloc_alloc_options options;
    options.zero = false;
    return _tralloc_forward_to_alloc ( &options, parent_context, child_context, length );
}
_TRALLOC_INLINE
tralloc_error tralloc_new_zero ( tralloc_context * parent_context, tralloc_context ** child_context, size_t length )
{
    _tralloc_alloc_options options;
    options.zero = true;
    return _tralloc_forward_to_alloc ( &options, parent_context, child_context, length );
}
_TRALLOC_INLINE
tralloc_error tralloc_new_empty ( tralloc_context * parent_context, tralloc_context ** child_context )
{
    return tralloc_new ( parent_context, child_context, 0 );
}

#if defined ( TRALLOC_EXTENSIONS )
_TRALLOC_INLINE
tralloc_error tralloc_new_with_extensions ( tralloc_context * parent_context, tralloc_context ** child_context, size_t length, tralloc_extensions extensions )
{
    _tralloc_alloc_options options;
    options.zero = false;
    return _tralloc_forward_to_alloc_with_extensions ( &options, parent_context, child_context, length, extensions );
}
_TRALLOC_INLINE
tralloc_error tralloc_new_zero_with_extensions ( tralloc_context * parent_context, tralloc_context ** child_context, size_t length, tralloc_extensions extensions )
{
    _tralloc_alloc_options options;
    options.zero = true;
    return _tralloc_forward_to_alloc_with_extensions ( &options, parent_context, child_context, length, extensions );
}
_TRALLOC_INLINE
tralloc_error tralloc_new_empty_with_extensions ( tralloc_context * parent_context, tralloc_context ** child_context, tralloc_extensions extensions )
{
    return tralloc_new_with_extensions ( parent_context, child_context, 0, extensions );
}
#endif

#endif


#endif
