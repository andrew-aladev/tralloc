// This file is part of tralloc. Copyright (C) 2013 Andrew Aladjev aladjev.andrew@gmail.com
// tralloc is free software: you can redistribute it and/or modify it under the terms of the GNU General Public License as published by the Free Software Foundation, either version 3 of the License, or (at your option) any later version.
// tralloc is distributed in the hope that it will be useful, but WITHOUT ANY WARRANTY; without even the implied warranty of MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU General Public License for more details.
// You should have received a copy of the GNU General Public License along with tralloc. If not, see <http://www.gnu.org/licenses/>.

#include <tralloc/tests/tree/free/common.h>
#include <tralloc/tests/tree/common.h>


tralloc_bool test_tree_free ( test_tree_data * tr )
{
    if (
        !test_tree_free_errors () ||
        !test_tree_free_main   ( tr )
    ) {
        return TRALLOC_FALSE;
    }
    return TRALLOC_TRUE;
}
