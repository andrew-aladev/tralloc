// This file is part of tralloc. Copyright (C) 2013 Andrew Aladjev aladjev.andrew@gmail.com
// tralloc is free software: you can redistribute it and/or modify it under the terms of the GNU General Lesser Public License as published by the Free Software Foundation, either version 3 of the License, or (at your option) any later version.
// tralloc is distributed in the hope that it will be useful, but WITHOUT ANY WARRANTY; without even the implied warranty of MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU General Lesser Public License for more details.
// You should have received a copy of the GNU General Lesser Public License along with tralloc. If not, see <http://www.gnu.org/licenses/>.

#ifndef TRALLOC_MACRO_H
#define TRALLOC_MACRO_H

#include "config.h"

#ifdef HAVE_C99_INLINE
#    define INLINE_IN_HEADER inline
#    define INLINE_IN_OBJECT
#else
#    define INLINE_IN_HEADER extern inline
#    define INLINE_IN_OBJECT
#endif

#ifdef __GNUC__
#    define UNUSED(x) UNUSED_ ## x __attribute__((__unused__))
#else
#    define UNUSED(x) UNUSED_ ## x
#endif


#endif
