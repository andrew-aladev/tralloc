function (check_bool)
    if (DEFINED TRALLOC_HAVE_BOOL)
        return ()
    endif ()
    
    try_compile (
        CHECK_BOOL_COMPILE_RESULT
        ${PROJECT_BINARY_DIR}
        SOURCES "${PROJECT_SOURCE_DIR}/cmake/test_files/bool.c"
    )
    if (${CHECK_BOOL_COMPILE_RESULT})
        set (TRALLOC_HAVE_BOOL true CACHE STRING "Status of Bool support")
        message (STATUS "Check for Bool support - yes")
        return ()
    endif ()
    
    set (TRALLOC_HAVE_BOOL false CACHE STRING "Status of Bool support")
    message (STATUS "Check for Bool support - no")
endfunction ()
