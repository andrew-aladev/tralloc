function (tralloc_check_c99)
    if (DEFINED TRALLOC_HAVE_C99)
        return ()
    endif ()
    
    include (TrallocCheckVerbose)
    tralloc_check_verbose ()
    
    set (BINARY_DIR "${PROJECT_BINARY_DIR}/CMakeTmp/C99")
    set (SOURCE_DIR "${PROJECT_SOURCE_DIR}/cmake/checks/C99")
    set (NAME "tralloc_check_C99")
    
    try_compile (CHECK_C99_RESULT ${BINARY_DIR} ${SOURCE_DIR} ${NAME}
        CMAKE_FLAGS
            "-DCMAKE_C_FLAGS=${CMAKE_C_FLAGS} ${TRALLOC_VERBOSE_CFLAGS} ${TRALLOC_WERROR_CFLAGS} -std=gnu99"
            "-DCMAKE_VERBOSE_MAKEFILE=${CMAKE_CONFIG_VERBOSE_MAKEFILE}"
        OUTPUT_VARIABLE CHECK_C99_COMPILE_RESULT
    )
    if (${CMAKE_CONFIG_VERBOSE_MAKEFILE})
        message (STATUS ${CHECK_C99_COMPILE_RESULT})
    endif ()
    FILE (REMOVE_RECURSE ${BINARY_DIR})
    
    if (${CHECK_C99_RESULT})
        set (TRALLOC_HAVE_C99 true CACHE STRING "Status of C99 support")
        set (TRALLOC_C99_CFLAGS "-std=gnu99" CACHE STRING "c99 cflags")
        message (STATUS "Check for C compiler C99 support - gnu99")
        return ()
    endif ()
    
    try_compile (CHECK_C99_RESULT ${BINARY_DIR} ${SOURCE_DIR} ${NAME}
        CMAKE_FLAGS
            "-DCMAKE_C_FLAGS=${CMAKE_C_FLAGS} ${TRALLOC_VERBOSE_CFLAGS} ${TRALLOC_WERROR_CFLAGS} -std=c99"
            "-DCMAKE_VERBOSE_MAKEFILE=${CMAKE_CONFIG_VERBOSE_MAKEFILE}"
        OUTPUT_VARIABLE CHECK_C99_COMPILE_RESULT
    )
    if (${CMAKE_CONFIG_VERBOSE_MAKEFILE})
        message (STATUS ${CHECK_C99_COMPILE_RESULT})
    endif ()
    FILE (REMOVE_RECURSE ${BINARY_DIR})
    
    if (${CHECK_C99_RESULT})
        set (TRALLOC_HAVE_C99 true CACHE STRING "Status of C99 support")
        set (TRALLOC_C99_CFLAGS "-std=c99" CACHE STRING "c99 cflags")
        message (STATUS "Check for C compiler C99 support - c99")
        return ()
    endif ()
    
    try_compile (CHECK_C99_RESULT ${BINARY_DIR} ${SOURCE_DIR} ${NAME}
        CMAKE_FLAGS
            "-DCMAKE_C_FLAGS=${CMAKE_C_FLAGS} ${TRALLOC_VERBOSE_CFLAGS} ${TRALLOC_WERROR_CFLAGS}"
            "-DCMAKE_VERBOSE_MAKEFILE=${CMAKE_CONFIG_VERBOSE_MAKEFILE}"
        OUTPUT_VARIABLE CHECK_C99_COMPILE_RESULT
    )
    if (${CMAKE_CONFIG_VERBOSE_MAKEFILE})
        message (STATUS ${CHECK_C99_COMPILE_RESULT})
    endif ()
    FILE (REMOVE_RECURSE ${BINARY_DIR})
    
    if (${CHECK_C99_RESULT})
        set (TRALLOC_HAVE_C99 true CACHE STRING "Status of C99 support")
        set (TRALLOC_C99_CFLAGS "" CACHE STRING "c99 cflags")
        message (STATUS "Check for C compiler C99 support - vanilla")
        return ()
    endif ()
    
    set (TRALLOC_HAVE_C99 false CACHE STRING "Status of C99 support")
    set (TRALLOC_C99_CFLAGS "" CACHE STRING "c99 cflags")
    message (STATUS "Check for C compiler C99 support - no")
endfunction ()
