// This file is part of tralloc. Copyright (C) 2013 Andrew Aladjev aladjev.andrew@gmail.com
// tralloc is free software: you can redistribute it and/or modify it under the terms of the GNU General Public License as published by the Free Software Foundation, either version 3 of the License, or (at your option) any later version.
// tralloc is distributed in the hope that it will be useful, but WITHOUT ANY WARRANTY; without even the implied warranty of MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU General Public License for more details.
// You should have received a copy of the GNU General Public License along with tralloc. If not, see <http://www.gnu.org/licenses/>.

#ifndef TRALLOC_TESTS_UTILS_COMMON_H
#define TRALLOC_TESTS_UTILS_COMMON_H

#include <tralloc/types.h>


#if defined(TRALLOC_UTILS_BUFFER)
tralloc_bool test_utils_buffer ( tralloc_context * ctx );
#endif

int test_utils ( tralloc_context * ctx );


#endif
