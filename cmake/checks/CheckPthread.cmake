function (check_pthread)
    if (DEFINED TRALLOC_HAVE_PTHREAD)
        return ()
    endif ()
    
    include (CheckVerbose)
    check_verbose ()
    
    set (BINARY_DIR "${PROJECT_BINARY_DIR}/CMakeTmp/pthread")
    set (SOURCE_DIR "${PROJECT_SOURCE_DIR}/cmake/checks/pthread")
    set (NAME "check_pthread")
    
    try_compile (CHECK_PTHREAD_RESULT ${BINARY_DIR} ${SOURCE_DIR} ${NAME}
        CMAKE_FLAGS
            "-DCMAKE_C_FLAGS=${CMAKE_C_FLAGS} ${VERBOSE_CFLAGS} ${WERROR_CFLAGS} -pthread"
            "-DCMAKE_EXE_LINKER_FLAGS=${CMAKE_EXE_LINKER_FLAGS} -pthread"
            "-DCMAKE_VERBOSE_MAKEFILE=${CMAKE_CONFIG_VERBOSE_MAKEFILE}"
        OUTPUT_VARIABLE CHECK_PTHREAD_COMPILE_RESULT
    )
    if (${CMAKE_CONFIG_VERBOSE_MAKEFILE})
        message (STATUS ${CHECK_PTHREAD_COMPILE_RESULT})
    endif ()
    FILE (REMOVE_RECURSE ${BINARY_DIR})
    
    if (${CHECK_PTHREAD_RESULT})
        set (TRALLOC_HAVE_PTHREAD true CACHE STRING "Status of pthread support")
        set (PTHREAD_CFLAGS "-pthread" CACHE STRING "pthread cflags")
        set (PTHREAD_LDLAGS "-pthread" CACHE STRING "pthread ldflags")
        message (STATUS "Check for pthread support - yes")
        return ()
    endif ()
    
    set (TRALLOC_HAVE_PTHREAD false CACHE STRING "Status of pthread support")
    set (PTHREAD_CFLAGS "" CACHE STRING "pthread cflags")
    set (PTHREAD_LDLAGS "" CACHE STRING "pthread ldflags")
    message (STATUS "Check for pthread support - no")
endfunction ()
