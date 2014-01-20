function (check_c99_inline)
    if (DEFINED HAVE_C99_INLINE)
        return ()
    endif ()
    
    include (CheckC99)
    check_c99 ()
    
    try_run (
        CHECK_C99_INLINE_RUN_RESULT CHECK_C99_INLINE_COMPILE_RESULT
        ${PROJECT_BINARY_DIR}
        SOURCES
            "${PROJECT_SOURCE_DIR}/cmake/test_files/c99_inline/test.c"
            "${PROJECT_SOURCE_DIR}/cmake/test_files/c99_inline/main.c"
        COMPILE_DEFINITIONS ${C99_CFLAGS}
    )
    if (${CHECK_C99_INLINE_COMPILE_RESULT} AND CHECK_C99_INLINE_RUN_RESULT EQUAL 0)
        set (HAVE_C99_INLINE true CACHE STRING "Status of C99 inline support")
        message (STATUS "Check for C compiler C99 inline support - ok")
        return ()
    endif ()
    
    set (HAVE_C99_INLINE false CACHE STRING "Status of C99 inline support")
    message (FATAL_ERROR "Check for C compiler C99 inline support - no")
endfunction ()