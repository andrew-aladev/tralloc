// This file is part of tralloc. Copyright (C) 2013 Andrew Aladjev aladjev.andrew@gmail.com
// tralloc is free software: you can redistribute it and/or modify it under the terms of the GNU General Lesser Public License as published by the Free Software Foundation, either version 3 of the License, or (at your option) any later version.
// tralloc is distributed in the hope that it will be useful, but WITHOUT ANY WARRANTY; without even the implied warranty of MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU General Lesser Public License for more details.
// You should have received a copy of the GNU General Lesser Public License along with tralloc. If not, see <http://www.gnu.org/licenses/>.

#include <tralloc/helpers/string.h>
#include <tralloc/reference/main.h>

#if defined(TRALLOC_DEBUG)
#include <tralloc/events.h>
#endif

uint8_t code_1 ( char * common )
{
    void * common_reference = tralloc_reference ( common, NULL );
    if ( common_reference == NULL ) {
        return 1;
    }
    return tralloc_free ( common_reference ); // may free common
}

uint8_t code_2 ( char * common )
{
    void * common_reference = tralloc_reference ( common, NULL );
    if ( common_reference == NULL ) {
        return 1;
    }
    return tralloc_free ( common_reference ); // may free common
}

int main ()
{
    char * common = tralloc_strdup ( NULL,
                                     "This is big data, that you dont want to duplicate."
                                     "Different code needs it."
                                     "You want it to be fried automaticaly when all code have finished it's processing."
                                   );
    if ( common == NULL ) {
        return 1;
    }
    if (
        code_1 ( common ) != 0 ||
        code_2 ( common ) != 0
    ) {
        tralloc_free ( common );
        return 2;
    }

#if defined(TRALLOC_DEBUG)
    if ( tralloc_get_chunks_count() != 0 ) {
        return 3;
    }
#endif

    return 0;
}
