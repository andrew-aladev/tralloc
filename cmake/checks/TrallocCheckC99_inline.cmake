function (tralloc_check_c99_inline)
    if (DEFINED TRALLOC_HAVE_C99_INLINE)
        return ()
    endif ()
    
    include (TrallocCheckVerbose)
    include (TrallocCheckC99)
    tralloc_check_verbose ()
    tralloc_check_c99 ()

    if (NOT ${TRALLOC_HAVE_C99})
        set (TRALLOC_HAVE_C99_INLINE false CACHE STRING "Status of C99 inline support")
        message (STATUS "Check for C compiler C99 inline support - no")
        return ()
    endif ()
    
    set (BINARY_DIR "${PROJECT_BINARY_DIR}/CMakeTmp/C99_inline")
    set (SOURCE_DIR "${PROJECT_SOURCE_DIR}/cmake/checks/C99_inline")
    set (NAME "tralloc_check_C99_inline")
    
    try_compile (CHECK_C99_INLINE_RESULT ${BINARY_DIR} ${SOURCE_DIR} ${NAME}
        CMAKE_FLAGS
            "-DCMAKE_C_FLAGS=${CMAKE_C_FLAGS} ${TRALLOC_VERBOSE_CFLAGS} ${TRALLOC_WERROR_CFLAGS} ${TRALLOC_C99_CFLAGS}"
            "-DCMAKE_VERBOSE_MAKEFILE=${CMAKE_CONFIG_VERBOSE_MAKEFILE}"
        OUTPUT_VARIABLE CHECK_C99_INLINE_COMPILE_RESULT
    )
    if (${CMAKE_CONFIG_VERBOSE_MAKEFILE})
        message (STATUS ${CHECK_C99_INLINE_COMPILE_RESULT})
    endif ()
    FILE (REMOVE_RECURSE ${BINARY_DIR})
    
    if (${CHECK_C99_INLINE_RESULT})
        set (TRALLOC_HAVE_C99_INLINE true CACHE STRING "Status of C99 inline support")
        message (STATUS "Check for C compiler C99 inline support - yes")
        return ()
    endif ()
    
    set (TRALLOC_HAVE_C99_INLINE false CACHE STRING "Status of C99 inline support")
    message (STATUS "Check for C compiler C99 inline support - no")
endfunction ()
