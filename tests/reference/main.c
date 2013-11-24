// This file is part of talloc2. Copyright (C) 2013 Andrew Aladjev aladjev.andrew@gmail.com
// talloc2 is free software: you can redistribute it and/or modify it under the terms of the GNU General Public License as published by the Free Software Foundation, either version 3 of the License, or (at your option) any later version.
// talloc2 is distributed in the hope that it will be useful, but WITHOUT ANY WARRANTY; without even the implied warranty of MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU General Public License for more details.
// You should have received a copy of the GNU General Public License along with talloc2. If not, see <http://www.gnu.org/licenses/>.

#include <stdbool.h>
#include <talloc2/tree.h>
#include <talloc2/reference/main.h>

void * root;

bool init_data ()
{
    root = talloc_new ( NULL );
    if ( root == NULL ) {
        return false;
    }
    return true;
}

void free_data ()
{
    talloc_free ( root );
}

int main ()
{
    if ( !init_data () ) {
        return 1;
    }

    free_data ();
    return 0;
}