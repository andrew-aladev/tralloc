// This file is part of tralloc. Copyright (C) 2013 Andrew Aladjev aladjev.andrew@gmail.com
// tralloc is free software: you can redistribute it and/or modify it under the terms of the GNU General Public License as published by the Free Software Foundation, either version 3 of the License, or (at your option) any later version.
// tralloc is distributed in the hope that it will be useful, but WITHOUT ANY WARRANTY; without even the implied warranty of MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU General Public License for more details.
// You should have received a copy of the GNU General Public License along with tralloc. If not, see <http://www.gnu.org/licenses/>.

#include "common.h"
#include <tralloc/tree.h>
#include <tralloc/length/main.h>


bool test_errors ( tralloc_context * ctx )
{
    size_t length;
    if (
        tralloc_get_length ( NULL, &length ) != TRALLOC_ERROR_REQUIRED_ARGUMENT_IS_NULL ||
        tralloc_get_length ( ctx,  &length ) != TRALLOC_ERROR_NO_SUCH_EXTENSION
    ) {
        return false;
    }

    return true;
}
