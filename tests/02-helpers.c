// This file is part of talloc. Copyright (C) 2013 Andrew Aladjev aladjev.andrew@gmail.com
// talloc is free software: you can redistribute it and/or modify it under the terms of the GNU General Public License as published by the Free Software Foundation, either version 3 of the License, or (at your option) any later version.
// talloc is distributed in the hope that it will be useful, but WITHOUT ANY WARRANTY; without even the implied warranty of MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU General Public License for more details.
// You should have received a copy of the GNU General Public License along with talloc. If not, see <http://www.gnu.org/licenses/>.

#include <talloc/tree.h>
#include <talloc/helpers.h>

int main () {
    char * full = talloc_strdup ( NULL, "Viktor Tsoi" );
    char * part = talloc_strndup ( full, "Viktor Tsoi", 6 );
    if (
        ! (
            full != NULL &&
            part != NULL &&
            !strcmp ( full, "Viktor Tsoi" ) &&
            !strcmp ( part, "Viktor" )
        )
    ) {
        talloc_free ( full );
        return 1;
    }

    talloc_free ( full );

    return 0;
}

