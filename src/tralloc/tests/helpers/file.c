// This file is part of tralloc. Copyright (C) 2013 Andrew Aladjev aladjev.andrew@gmail.com
// tralloc is free software: you can redistribute it and/or modify it under the terms of the GNU General Public License as published by the Free Software Foundation, either version 3 of the License, or (at your option) any later version.
// tralloc is distributed in the hope that it will be useful, but WITHOUT ANY WARRANTY; without even the implied warranty of MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU General Public License for more details.
// You should have received a copy of the GNU General Public License along with tralloc. If not, see <http://www.gnu.org/licenses/>.

#include <tralloc/tests/helpers/common.h>
#include <tralloc/tree.h>
#include <tralloc/helpers/string.h>
#include <tralloc/helpers/file.h>
#include <tralloc/destructors.h>

#include <sys/stat.h>
#include <unistd.h>


static
tralloc_error test_helpers_file_destructor_unlink_file ( tralloc_context * _TRALLOC_UNUSED ( chunk_context ), void * user_data )
{
    char * file_name = user_data;
    if ( unlink ( file_name ) != 0 ) {
        return 1;
    } else {
        return 0;
    }
}

tralloc_bool test_helpers_file ( tralloc_context * ctx )
{
    if (
        tralloc_open                      ( NULL, NULL, NULL, 0 )       != TRALLOC_ERROR_REQUIRED_ARGUMENT_IS_NULL ||
        tralloc_open_with_extensions      ( NULL, NULL, 0, NULL, 0 )    != TRALLOC_ERROR_REQUIRED_ARGUMENT_IS_NULL ||
        tralloc_open_mode                 ( NULL, NULL, NULL, 0, 0 )    != TRALLOC_ERROR_REQUIRED_ARGUMENT_IS_NULL ||
        tralloc_open_mode_with_extensions ( NULL, NULL, 0, NULL, 0, 0 ) != TRALLOC_ERROR_REQUIRED_ARGUMENT_IS_NULL
    ) {
        return TRALLOC_FALSE;
    }

    int * hosts;
    if (
        tralloc_open                      ( ctx, &hosts, "/tmp/tralloc_file_not_exists", O_RDONLY )       != TRALLOC_ERROR_OPEN_DESCRIPTOR_FAILED ||
        tralloc_open_with_extensions      ( ctx, &hosts, 0, "/tmp/tralloc_file_not_exists", O_RDONLY )    != TRALLOC_ERROR_OPEN_DESCRIPTOR_FAILED ||
        tralloc_open_mode                 ( ctx, &hosts, "/tmp/tralloc_file_not_exists", O_RDONLY, 0 )    != TRALLOC_ERROR_OPEN_DESCRIPTOR_FAILED ||
        tralloc_open_mode_with_extensions ( ctx, &hosts, 0, "/tmp/tralloc_file_not_exists", O_RDONLY, 0 ) != TRALLOC_ERROR_OPEN_DESCRIPTOR_FAILED
    ) {
        return TRALLOC_FALSE;
    }

    if ( tralloc_open ( ctx, &hosts, "/etc/hosts", O_RDONLY ) != 0 ) {
        return TRALLOC_FALSE;
    }
    if ( tralloc_free ( hosts ) != 0 ) {
        return TRALLOC_FALSE;
    }

    char * file_name;
    int * test_file;

    mode_t mode = S_IRUSR | S_IWUSR;

#   if defined ( S_IRGRP )
    mode |= S_IRGRP;
#   endif

#   if defined ( S_IROTH )
    mode |= S_IROTH;
#   endif

    // mode should be 0644 in posix systems

    if (
        tralloc_strdup ( ctx, &file_name, "/tmp/tralloc_test_file" ) != 0 ||
        tralloc_open_mode ( ctx, &test_file, file_name, O_CREAT | O_WRONLY, mode ) != 0
    ) {
        tralloc_free ( file_name );
        return TRALLOC_FALSE;
    }
    if ( tralloc_move ( file_name, test_file ) != 0 ) {
        tralloc_free ( test_file );
        tralloc_free ( file_name );
        return TRALLOC_FALSE;
    }
    if ( tralloc_append_destructor ( test_file, test_helpers_file_destructor_unlink_file, file_name ) != 0 ) {
        tralloc_free ( test_file );
        return TRALLOC_FALSE;
    }
    if ( tralloc_free ( test_file ) != 0 ) {
        return TRALLOC_FALSE;
    }
    return TRALLOC_TRUE;
}
