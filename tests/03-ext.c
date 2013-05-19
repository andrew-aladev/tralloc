// This file is part of talloc. Copyright (C) 2013 Andrew Aladjev aladjev.andrew@gmail.com
// talloc is free software: you can redistribute it and/or modify it under the terms of the GNU General Public License as published by the Free Software Foundation, either version 3 of the License, or (at your option) any later version.
// talloc is distributed in the hope that it will be useful, but WITHOUT ANY WARRANTY; without even the implied warranty of MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU General Public License for more details.
// You should have received a copy of the GNU General Public License along with talloc. If not, see <http://www.gnu.org/licenses/>.

#include <stdbool.h>
#include <string.h>

#include <talloc/helpers.h>
#include <talloc/ext.h>

#include "utils/dynarr.h"

void * root;
static talloc_dynarr * history;

bool init_data () {
    root = talloc_new ( NULL );
    if ( root == NULL ) {
        return false;
    }
    history = talloc_dynarr_new ( 16 );
    if ( history == NULL ) {
        talloc_free ( root );
        return false;
    }
    return true;
}

void destructor ( void * text ) {
    talloc_dynarr_append ( history, text );
}

bool test_destructor () {
    char * text_01 = talloc_strdup ( root, "test text 01" );
    if ( text_01 == NULL ) {
        return false;
    }
    char * text_02 = talloc_strdup ( root, "test text 02" );
    if ( text_02 == NULL ) {
        return false;
    }
    char * text_03 = talloc_strdup ( root, "test text 03" );
    if ( text_03 == NULL ) {
        return false;
    }

#ifdef TALLOC_EXT_DESTRUCTOR
    if (
        ! (
            !talloc_set_destructor ( text_01, destructor ) &&
            !talloc_set_destructor ( text_02, destructor ) &&
            !talloc_set_destructor ( text_03, destructor )
        )
    ) {
        return false;
    }
#endif

    talloc_free ( text_02 );
    talloc_free ( text_01 );
    talloc_free ( text_03 );

#ifdef TALLOC_EXT_DESTRUCTOR
    if (
        ! (
            !strcmp ( talloc_dynarr_get ( history, 0 ), "test text 02" ) &&
            !strcmp ( talloc_dynarr_get ( history, 1 ), "test text 01" ) &&
            !strcmp ( talloc_dynarr_get ( history, 2 ), "test text 03" )
        )
    ) {
        return false;
    }
#endif

    return true;
}

void free_data () {
    if ( root != NULL ) {
        talloc_free ( root );
    }
    if ( history != NULL ) {
        talloc_dynarr_free ( history );
    }
}

int main () {
    if ( !init_data () ) {
        return 1;
    }

#ifdef TALLOC_EXT_DESTRUCTOR
    if ( !test_destructor () ) {
        free_data ();
        return 2;
    }
#endif

    free_data ();
    return 0;
}

