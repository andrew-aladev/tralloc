// This file is part of tralloc. Copyright (C) 2013 Andrew Aladjev aladjev.andrew@gmail.com
// tralloc is free software: you can redistribute it and/or modify it under the terms of the GNU General Public License as published by the Free Software Foundation, either version 3 of the License, or (at your option) any later version.
// tralloc is distributed in the hope that it will be useful, but WITHOUT ANY WARRANTY; without even the implied warranty of MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU General Public License for more details.
// You should have received a copy of the GNU General Public License along with tralloc. If not, see <http://www.gnu.org/licenses/>.

#ifndef TRALLOC_TESTS_COMMON_MAIN_H
#define TRALLOC_TESTS_COMMON_MAIN_H

#include <tralloc/types.h>
#include <tralloc/macro.h>

#include <math.h>

#undef _TRALLOC_INLINE
#ifdef _TRALLOC_TESTS_COMMON_MAIN_INCLUDED_FROM_OBJECT
#    define _TRALLOC_INLINE _TRALLOC_INLINE_IN_OBJECT
#else
#    define _TRALLOC_INLINE _TRALLOC_INLINE_IN_HEADER
#endif

_TRALLOC_INLINE
tralloc_bool compare_float ( float a, float b )
{
    return fabs ( a - b ) < 0.000001;
}

#endif
