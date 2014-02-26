function (check_bool)
    if (DEFINED HAVE_BOOL)
        return ()
    endif ()
    
    try_compile (
        CHECK_BOOL_COMPILE_RESULT
        ${PROJECT_BINARY_DIR}
        SOURCES "${PROJECT_SOURCE_DIR}/cmake/test_files/bool.c"
    )
    if (${CHECK_BOOL_COMPILE_RESULT})
        set (HAVE_BOOL true CACHE STRING "Status of Bool support")
        message (STATUS "Check for C compiler Bool support - yes")
        return ()
    endif ()
    
    set (HAVE_BOOL false CACHE STRING "Status of Bool support")
    message (STATUS "Check for C compiler Bool support - no")
endfunction ()
