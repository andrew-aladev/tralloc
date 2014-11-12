function (check_file)
    if (DEFINED TRALLOC_HAVE_FILE)
        return ()
    endif ()
    
    include (CheckVerbose)
    check_verbose ()
    
    set (BINARY_DIR "${PROJECT_BINARY_DIR}/CMakeTmp/file")
    set (SOURCE_DIR "${PROJECT_SOURCE_DIR}/cmake/checks/file")
    set (NAME "check_file")
    
    try_compile (CHECK_FILE_RESULT ${BINARY_DIR} ${SOURCE_DIR} ${NAME}
        CMAKE_FLAGS
            "-DCMAKE_C_FLAGS=${CMAKE_C_FLAGS} ${VERBOSE_CFLAGS} ${WERROR_CFLAGS}"
            "-DCMAKE_VERBOSE_MAKEFILE=${CMAKE_CONFIG_VERBOSE_MAKEFILE}"
        OUTPUT_VARIABLE CHECK_FILE_COMPILE_RESULT
    )
    if (${CMAKE_CONFIG_VERBOSE_MAKEFILE})
        message (STATUS ${CHECK_FILE_COMPILE_RESULT})
    endif ()
    FILE (REMOVE_RECURSE ${BINARY_DIR})
    
    if (${CHECK_FILE_RESULT})
        set (TRALLOC_HAVE_FILE true CACHE STRING "Status of File support")
        message (STATUS "Check for File support - yes")
        return ()
    endif ()
    
    set (TRALLOC_HAVE_FILE false CACHE STRING "Status of File support")
    message (STATUS "Check for File support - no")
endfunction ()
