function (check_c99)
    if (DEFINED HAVE_C99)
        return ()
    endif ()
    
    try_compile (
        CHECK_C99_COMPILE_RESULT
        ${PROJECT_BINARY_DIR}
        SOURCES "${PROJECT_SOURCE_DIR}/cmake/test_files/empty.c"
        COMPILE_DEFINITIONS "-std=gnu99"
    )
    if (${CHECK_C99_COMPILE_RESULT})
        set (HAVE_C99 true CACHE STRING "Status of C99 support")
        set (C99_CFLAGS "-std=gnu99" CACHE STRING "c99 cflags")
        message (STATUS "Check for C compiler C99 support - gnu99")
        return ()
    endif ()
    
    try_compile (
        CHECK_C99_COMPILE_RESULT
        ${PROJECT_BINARY_DIR}
        SOURCES "${PROJECT_SOURCE_DIR}/cmake/test_files/empty.c"
        COMPILE_DEFINITIONS "-std=c99"
    )
    if (${CHECK_C99_COMPILE_RESULT})
        set (HAVE_C99 true CACHE STRING "Status of C99 support")
        set (C99_CFLAGS "-std=c99" CACHE STRING "c99 cflags")
        message (STATUS "Check for C compiler C99 support - c99")
        return ()
    endif ()
    
    set (HAVE_C99 false CACHE STRING "Status of C99 support")
    set (C99_CFLAGS "" CACHE STRING "c99 cflags")
    message (STATUS "Check for C compiler C99 support - no")
endfunction ()
