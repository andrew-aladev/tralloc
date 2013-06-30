// This file is part of talloc2. Copyright (C) 2013 Andrew Aladjev aladjev.andrew@gmail.com
// talloc2 is free software: you can redistribute it and/or modify it under the terms of the GNU General Public License as published by the Free Software Foundation, either version 3 of the License, or (at your option) any later version.
// talloc2 is distributed in the hope that it will be useful, but WITHOUT ANY WARRANTY; without even the implied warranty of MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU General Public License for more details.
// You should have received a copy of the GNU General Public License along with talloc2. If not, see <http://www.gnu.org/licenses/>.

#include <talloc2/utils/list.h>

bool test_list ( void * ctx )
{
    talloc_list * list = talloc_list_new ( ctx );
    char * str_1       = talloc_strdup ( list, "str_1" );
    char * str_2       = talloc_strdup ( list, "str_2" );
    char * str_3       = talloc_strdup ( list, "str_3" );
    if ( list == NULL || str_1 == NULL || str_2 == NULL || str_3 == NULL ) {
        return false;
    }

    if (
        talloc_list_push    ( list, str_2 ) != 0 ||
        talloc_list_push    ( list, str_1 ) != 0 ||
        talloc_list_push    ( list, str_2 ) != 0 ||
        talloc_list_push    ( list, str_3 ) != 0 ||
        talloc_list_unshift ( list, str_2 ) != 0 ||
        talloc_list_unshift ( list, str_2 ) != 0 ||
        talloc_list_unshift ( list, str_3 ) != 0
    ) {
        talloc_free ( list );
        return false;
    }

    talloc_list_item * item = list->first_item;

    if (
        talloc_list_get_length ( list ) != 7 ||
        item->data != str_3 || ( item = item->next ) == NULL ||
        item->data != str_2 || ( item = item->next ) == NULL ||
        item->data != str_2 || ( item = item->next ) == NULL ||
        item->data != str_2 || ( item = item->next ) == NULL ||
        item->data != str_1 || ( item = item->next ) == NULL ||
        item->data != str_2 || ( item = item->next ) == NULL ||
        item->data != str_3
    ) {
        talloc_free ( list );
        return false;
    }

    if ( talloc_free ( str_3 ) != 0 ) {
        talloc_free ( list );
        return false;
    }

    item = list->first_item;

    if (
        talloc_list_get_length ( list ) != 5 ||
        item->data != str_2 || ( item = item->next ) == NULL ||
        item->data != str_2 || ( item = item->next ) == NULL ||
        item->data != str_2 || ( item = item->next ) == NULL ||
        item->data != str_1 || ( item = item->next ) == NULL ||
        item->data != str_2
    ) {
        talloc_free ( list );
        return false;
    }

    if ( talloc_free ( str_2 ) != 0 ) {
        talloc_free ( list );
        return false;
    }

    item = list->first_item;

    if (
        talloc_list_get_length ( list ) != 1 ||
        item->data != str_1
    ) {
        talloc_free ( list );
        return false;
    }

    if ( talloc_free ( str_1 ) != 0 ) {
        talloc_free ( list );
        return false;
    }

    if (
        talloc_list_get_length ( list ) != 0
    ) {
        talloc_free ( list );
        return false;
    }

    char * str_4 = talloc_strdup ( list, "str_4" );
    char * str_5 = talloc_strdup ( list, "str_5" );
    char * str_6 = talloc_strdup ( list, "str_6" );

    if (
        talloc_list_push    ( list, str_4 ) != 0 ||
        talloc_list_push    ( list, str_6 ) != 0 ||
        talloc_list_push    ( list, str_5 ) != 0 ||
        talloc_list_unshift ( list, str_5 ) != 0 ||
        talloc_list_unshift ( list, str_6 ) != 0 ||
        talloc_list_unshift ( list, str_4 ) != 0
    ) {
        talloc_free ( list );
        return false;
    }

    item = list->first_item;

    if (
        talloc_list_get_length ( list ) != 6 ||
        item->data != str_4 || ( item = item->next ) == NULL ||
        item->data != str_6 || ( item = item->next ) == NULL ||
        item->data != str_5 || ( item = item->next ) == NULL ||
        item->data != str_4 || ( item = item->next ) == NULL ||
        item->data != str_6 || ( item = item->next ) == NULL ||
        item->data != str_5
    ) {
        talloc_free ( list );
        return false;
    }

    if (
        talloc_list_pop   ( list ) != 0 ||
        talloc_list_pop   ( list ) != 0 ||
        talloc_list_shift ( list ) != 0 ||
        talloc_list_shift ( list ) != 0
    ) {
        talloc_free ( list );
        return false;
    }

    item = list->first_item;

    if (
        talloc_list_get_length ( list ) != 2 ||
        item->data != str_5 || ( item = item->next ) == NULL ||
        item->data != str_4
    ) {
        talloc_free ( list );
        return false;
    }

    if ( talloc_free ( str_4 ) != 0 ) {
        talloc_free ( list );
        return false;
    }
    
    item = list->first_item;
    
    if (
        talloc_list_get_length ( list ) != 1 ||
        item->data != str_5
    ) {
        talloc_free ( list );
        return false;
    }

    if ( talloc_free ( list ) != 0 ) {
        return false;
    }

    return true;
}