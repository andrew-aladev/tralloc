function (check_verbose)
    if (DEFINED TRALLOC_HAVE_VERBOSE)
        return ()
    endif ()
    
    set (VERBOSE_CFLAGS "")
    
    set (BINARY_DIR "${PROJECT_BINARY_DIR}/CMakeTmp/basic")
    set (SOURCE_DIR "${PROJECT_SOURCE_DIR}/cmake/checks/basic")
    set (NAME "check_basic")
    
    try_compile (CHECK_VERBOSE_COMPILE_RESULT ${BINARY_DIR} ${SOURCE_DIR} ${NAME}
        CMAKE_FLAGS "-DCMAKE_C_FLAGS:STRING = ${CMAKE_C_FLAGS} -pedantic"
    )
    FILE (REMOVE_RECURSE ${BINARY_DIR})
    
    if (${CHECK_VERBOSE_COMPILE_RESULT})
        set (VERBOSE_CFLAGS "${VERBOSE_CFLAGS} -pedantic")
        message (STATUS "Check for C compiler -pedantic support - yes")
    else ()
        message (STATUS "Check for C compiler -pedantic support - no")
    endif ()
    
    try_compile (CHECK_VERBOSE_COMPILE_RESULT ${BINARY_DIR} ${SOURCE_DIR} ${NAME}
        CMAKE_FLAGS "-DCMAKE_C_FLAGS:STRING = ${CMAKE_C_FLAGS} -Wall"
    )
    FILE (REMOVE_RECURSE ${BINARY_DIR})
    
    if (${CHECK_VERBOSE_COMPILE_RESULT})
        set (VERBOSE_CFLAGS "${VERBOSE_CFLAGS} -Wall")
        message (STATUS "Check for C compiler -Wall support - yes")
    else ()
        message (STATUS "Check for C compiler -Wall support - no")
    endif ()
    
    try_compile (CHECK_VERBOSE_COMPILE_RESULT ${BINARY_DIR} ${SOURCE_DIR} ${NAME}
        CMAKE_FLAGS "-DCMAKE_C_FLAGS:STRING = ${CMAKE_C_FLAGS} -Wextra"
    )
    FILE (REMOVE_RECURSE ${BINARY_DIR})
    
    if (${CHECK_VERBOSE_COMPILE_RESULT})
        set (VERBOSE_CFLAGS "${VERBOSE_CFLAGS} -Wextra")
        message (STATUS "Check for C compiler -Wextra support - yes")
    else ()
        message (STATUS "Check for C compiler -Wextra support - no")
    endif ()
    
    set (VERBOSE_CFLAGS ${VERBOSE_CFLAGS} CACHE STRING "verbose cflags")
    set (TRALLOC_HAVE_VERBOSE true CACHE STRING "Status of verbose support")
endfunction ()
