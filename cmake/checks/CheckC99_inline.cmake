function (check_c99_inline)
    if (DEFINED TRALLOC_HAVE_C99_INLINE)
        return ()
    endif ()
    
    include (CheckC99)
    check_c99 ()

    if (NOT ${TRALLOC_HAVE_C99})
        set (TRALLOC_HAVE_C99_INLINE false CACHE STRING "Status of C99 inline support")
        message (STATUS "Check for C compiler C99 inline support - no")
        return ()
    endif ()
    
    try_compile (
        CHECK_C99_INLINE_COMPILE_RESULT
        "${PROJECT_BINARY_DIR}/CMakeTmp/C99_inline"
        "${PROJECT_SOURCE_DIR}/cmake/checks/C99_inline" "check_C99_inline"
        CMAKE_FLAGS "-DCMAKE_C_FLAGS:STRING = ${C99_CFLAGS}"
    )
    if (${CHECK_C99_INLINE_COMPILE_RESULT})
        set (TRALLOC_HAVE_C99_INLINE true CACHE STRING "Status of C99 inline support")
        message (STATUS "Check for C compiler C99 inline support - yes")
        return ()
    endif ()
    
    set (TRALLOC_HAVE_C99_INLINE false CACHE STRING "Status of C99 inline support")
    message (STATUS "Check for C compiler C99 inline support - no")
endfunction ()
