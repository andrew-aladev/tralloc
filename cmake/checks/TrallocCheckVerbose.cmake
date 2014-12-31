function (tralloc_check_verbose)
    if (DEFINED TRALLOC_HAVE_VERBOSE)
        return ()
    endif ()
    
    set (TRALLOC_VERBOSE_CFLAGS "")
    
    set (BINARY_DIR "${PROJECT_BINARY_DIR}/CMakeTmp/basic")
    set (SOURCE_DIR "${PROJECT_SOURCE_DIR}/cmake/checks/basic")
    set (NAME "tralloc_check_basic")
    
    try_compile (CHECK_VERBOSE_RESULT ${BINARY_DIR} ${SOURCE_DIR} ${NAME}
        CMAKE_FLAGS
            "-DCMAKE_C_FLAGS=${CMAKE_C_FLAGS} -Werror"
            "-DCMAKE_VERBOSE_MAKEFILE=${CMAKE_CONFIG_VERBOSE_MAKEFILE}"
        OUTPUT_VARIABLE CHECK_VERBOSE_COMPILE_RESULT
    )
    if (${CMAKE_CONFIG_VERBOSE_MAKEFILE})
        message (STATUS ${CHECK_VERBOSE_COMPILE_RESULT})
    endif ()
    FILE (REMOVE_RECURSE ${BINARY_DIR})
    
    if (${CHECK_VERBOSE_RESULT})
        set (TRALLOC_WERROR_CFLAGS "-Werror" CACHE STRING "Werror cflags")
        message (STATUS "Check for C compiler -Werror support - yes")
    else ()
        set (TRALLOC_WERROR_CFLAGS "" CACHE STRING "Werror cflags")
        message (STATUS "Check for C compiler -Werror support - no")
    endif ()
    
    try_compile (CHECK_VERBOSE_RESULT ${BINARY_DIR} ${SOURCE_DIR} ${NAME}
        CMAKE_FLAGS
            "-DCMAKE_C_FLAGS=${CMAKE_C_FLAGS} ${TRALLOC_WERROR_CFLAGS} -pedantic"
            "-DCMAKE_VERBOSE_MAKEFILE=${CMAKE_CONFIG_VERBOSE_MAKEFILE}"
        OUTPUT_VARIABLE CHECK_VERBOSE_COMPILE_RESULT
    )
    if (${CMAKE_CONFIG_VERBOSE_MAKEFILE})
        message (STATUS ${CHECK_VERBOSE_COMPILE_RESULT})
    endif ()
    FILE (REMOVE_RECURSE ${BINARY_DIR})
    
    if (${CHECK_VERBOSE_RESULT})
        set (TRALLOC_VERBOSE_CFLAGS "${TRALLOC_VERBOSE_CFLAGS} -pedantic")
        message (STATUS "Check for C compiler -pedantic support - yes")
    else ()
        message (STATUS "Check for C compiler -pedantic support - no")
    endif ()
    
    try_compile (CHECK_VERBOSE_RESULT ${BINARY_DIR} ${SOURCE_DIR} ${NAME}
        CMAKE_FLAGS
            "-DCMAKE_C_FLAGS=${CMAKE_C_FLAGS} ${TRALLOC_WERROR_CFLAGS} -Wall"
            "-DCMAKE_VERBOSE_MAKEFILE=${CMAKE_CONFIG_VERBOSE_MAKEFILE}"
        OUTPUT_VARIABLE CHECK_VERBOSE_COMPILE_RESULT
    )
    if (${CMAKE_CONFIG_VERBOSE_MAKEFILE})
        message (STATUS ${CHECK_VERBOSE_COMPILE_RESULT})
    endif ()
    FILE (REMOVE_RECURSE ${BINARY_DIR})
    
    if (${CHECK_VERBOSE_RESULT})
        set (TRALLOC_VERBOSE_CFLAGS "${TRALLOC_VERBOSE_CFLAGS} -Wall")
        message (STATUS "Check for C compiler -Wall support - yes")
    else ()
        message (STATUS "Check for C compiler -Wall support - no")
    endif ()
    
    try_compile (CHECK_VERBOSE_RESULT ${BINARY_DIR} ${SOURCE_DIR} ${NAME}
        CMAKE_FLAGS
            "-DCMAKE_C_FLAGS=${CMAKE_C_FLAGS} ${TRALLOC_WERROR_CFLAGS} -Wextra"
            "-DCMAKE_VERBOSE_MAKEFILE=${CMAKE_CONFIG_VERBOSE_MAKEFILE}"
        OUTPUT_VARIABLE CHECK_VERBOSE_COMPILE_RESULT
    )
    if (${CMAKE_CONFIG_VERBOSE_MAKEFILE})
        message (STATUS ${CHECK_VERBOSE_COMPILE_RESULT})
    endif ()
    FILE (REMOVE_RECURSE ${BINARY_DIR})
    
    if (${CHECK_VERBOSE_RESULT})
        set (TRALLOC_VERBOSE_CFLAGS "${TRALLOC_VERBOSE_CFLAGS} -Wextra")
        message (STATUS "Check for C compiler -Wextra support - yes")
    else ()
        message (STATUS "Check for C compiler -Wextra support - no")
    endif ()
    
    set (TRALLOC_VERBOSE_CFLAGS ${TRALLOC_VERBOSE_CFLAGS} CACHE STRING "verbose cflags")
    set (TRALLOC_HAVE_VERBOSE true CACHE STRING "Status of verbose support")
endfunction ()
