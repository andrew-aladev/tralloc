// This file is part of tralloc. Copyright (C) 2013 Andrew Aladjev aladjev.andrew@gmail.com
// tralloc is free software: you can redistribute it and/or modify it under the terms of the GNU General Public License as published by the Free Software Foundation, either version 3 of the License, or (at your option) any later version.
// tralloc is distributed in the hope that it will be useful, but WITHOUT ANY WARRANTY; without even the implied warranty of MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU General Public License for more details.
// You should have received a copy of the GNU General Public License along with tralloc. If not, see <http://www.gnu.org/licenses/>.

#include "common.h"
#include <tralloc/tree.h>
#include <tralloc/helpers/string.h>
#include <tralloc/helpers/file.h>
#include <tralloc/destructor/main.h>
#include <sys/types.h>
#include <unistd.h>


bool test_file_errors ( tralloc_context * ctx )
{
    if (
        tralloc_open                      ( NULL, NULL, NULL, 0 )       != TRALLOC_ERROR_CONTEXT_IS_NULL ||
        tralloc_open_with_extensions      ( NULL, NULL, 0, NULL, 0 )    != TRALLOC_ERROR_CONTEXT_IS_NULL ||
        tralloc_open_mode                 ( NULL, NULL, NULL, 0, 0 )    != TRALLOC_ERROR_CONTEXT_IS_NULL ||
        tralloc_open_mode_with_extensions ( NULL, NULL, 0, NULL, 0, 0 ) != TRALLOC_ERROR_CONTEXT_IS_NULL
    ) {
        return false;
    }

    int * hosts;
    if (
        tralloc_open                      ( ctx, &hosts, "/etc/hosts", O_RDONLY | O_DIRECTORY )       != TRALLOC_ERROR_OPEN_DESCRIPTOR_FAILED ||
        tralloc_open_with_extensions      ( ctx, &hosts, 0, "/etc/hosts", O_RDONLY | O_DIRECTORY )    != TRALLOC_ERROR_OPEN_DESCRIPTOR_FAILED ||
        tralloc_open_mode                 ( ctx, &hosts, "/etc/hosts", O_RDONLY | O_DIRECTORY, 0 )    != TRALLOC_ERROR_OPEN_DESCRIPTOR_FAILED ||
        tralloc_open_mode_with_extensions ( ctx, &hosts, 0, "/etc/hosts", O_RDONLY | O_DIRECTORY, 0 ) != TRALLOC_ERROR_OPEN_DESCRIPTOR_FAILED
    ) {
        return false;
    }

    return true;
}

static
tralloc_error destructor_unlink_file ( tralloc_context * UNUSED ( chunk_context ), void * user_data )
{
    char * file_name = user_data;
    if ( unlink ( file_name ) != 0 ) {
        return 1;
    } else {
        return 0;
    }
}

bool test_file ( tralloc_context * ctx )
{
    if ( !test_file_errors ( ctx ) ) {
        return false;
    }

    int * hosts;
    if ( tralloc_open ( ctx, &hosts, "/etc/hosts", O_RDONLY ) != 0 ) {
        return false;
    }
    if ( tralloc_free ( hosts ) != 0 ) {
        return false;
    }

    char * file_name;
    int * test_file;
    if (
        tralloc_strdup ( ctx, &file_name, "/tmp/tralloc_test_file" ) != 0 ||
        tralloc_open_mode ( ctx, &test_file, file_name, O_CREAT | O_WRONLY, S_IRUSR | S_IWUSR | S_IRGRP | S_IROTH ) != 0 // 0644
    ) {
        tralloc_free ( file_name );
        return false;
    }
    if ( tralloc_move ( file_name, test_file ) != 0 ) {
        tralloc_free ( test_file );
        tralloc_free ( file_name );
        return false;
    }
    if ( tralloc_append_destructor ( test_file, destructor_unlink_file, file_name ) != 0 ) {
        tralloc_free ( test_file );
        return false;
    }
    if ( tralloc_free ( test_file ) != 0 ) {
        return false;
    }
    return true;
}