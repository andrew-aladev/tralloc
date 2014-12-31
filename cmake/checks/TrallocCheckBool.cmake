function (tralloc_check_bool)
    if (DEFINED TRALLOC_HAVE_BOOL)
        return ()
    endif ()
    
    include (TrallocCheckVerbose)
    tralloc_check_verbose ()
    
    set (BINARY_DIR "${PROJECT_BINARY_DIR}/CMakeTmp/bool")
    set (SOURCE_DIR "${PROJECT_SOURCE_DIR}/cmake/checks/bool")
    set (NAME "tralloc_check_bool")
    
    try_compile (CHECK_BOOL_RESULT ${BINARY_DIR} ${SOURCE_DIR} ${NAME}
        CMAKE_FLAGS
            "-DCMAKE_C_FLAGS=${CMAKE_C_FLAGS} ${VERBOSE_CFLAGS} ${WERROR_CFLAGS}"
            "-DCMAKE_VERBOSE_MAKEFILE=${CMAKE_CONFIG_VERBOSE_MAKEFILE}"
        OUTPUT_VARIABLE CHECK_BOOL_COMPILE_RESULT
    )
    if (${CMAKE_CONFIG_VERBOSE_MAKEFILE})
        message (STATUS ${CHECK_BOOL_COMPILE_RESULT})
    endif ()
    FILE (REMOVE_RECURSE ${BINARY_DIR})
    
    if (${CHECK_BOOL_RESULT})
        set (TRALLOC_HAVE_BOOL true CACHE STRING "Status of Bool support")
        message (STATUS "Check for Bool support - yes")
        return ()
    endif ()
    
    set (TRALLOC_HAVE_BOOL false CACHE STRING "Status of Bool support")
    message (STATUS "Check for Bool support - no")
endfunction ()
