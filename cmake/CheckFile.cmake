function (check_file)
    if (DEFINED TRALLOC_HAVE_FILE)
        return ()
    endif ()
    
    try_compile (
        CHECK_FILE_COMPILE_RESULT
        ${PROJECT_BINARY_DIR}
        SOURCES "${PROJECT_SOURCE_DIR}/cmake/test_files/file.c"
    )
    if (${CHECK_FILE_COMPILE_RESULT})
        set (TRALLOC_HAVE_FILE true CACHE STRING "Status of File support")
        message (STATUS "Check for C compiler File support - yes")
        return ()
    endif ()
    
    set (TRALLOC_HAVE_FILE false CACHE STRING "Status of File support")
    message (STATUS "Check for C compiler File support - no")
endfunction ()
