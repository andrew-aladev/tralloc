function (check_c99_inline)
    if (DEFINED HAVE_C99_INLINE)
        return ()
    endif ()
    
    include (CheckC99)
    check_c99 ()

    if (NOT ${HAVE_C99})
        set (HAVE_C99_INLINE false CACHE STRING "Status of C99 inline support")
        message (STATUS "Check for C compiler C99 inline support - no")
        return ()
    endif ()
    
    try_compile (
        CHECK_C99_INLINE_COMPILE_RESULT
        ${PROJECT_BINARY_DIR}
        SOURCES
            "${PROJECT_SOURCE_DIR}/cmake/test_files/c99_inline/test.c"
            "${PROJECT_SOURCE_DIR}/cmake/test_files/c99_inline/main.c"
        COMPILE_DEFINITIONS ${C99_CFLAGS}
    )
    if (${CHECK_C99_INLINE_COMPILE_RESULT})
        set (HAVE_C99_INLINE true CACHE STRING "Status of C99 inline support")
        message (STATUS "Check for C compiler C99 inline support - yes")
        return ()
    endif ()
    
    set (HAVE_C99_INLINE false CACHE STRING "Status of C99 inline support")
    message (STATUS "Check for C compiler C99 inline support - no")
endfunction ()
