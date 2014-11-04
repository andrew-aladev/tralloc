function (check_pthread)
    if (DEFINED TRALLOC_HAVE_PTHREAD)
        return ()
    endif ()
    
    include (CheckVerbose)
    check_verbose ()
    
    set (BINARY_DIR "${PROJECT_BINARY_DIR}/CMakeTmp/pthread")
    set (SOURCE_DIR "${PROJECT_SOURCE_DIR}/cmake/checks/pthread")
    set (NAME "check_pthread")
    
    try_compile (CHECK_PTHREAD_COMPILE_RESULT ${BINARY_DIR} ${SOURCE_DIR} ${NAME}
        CMAKE_FLAGS "-DCMAKE_C_FLAGS:STRING = ${CMAKE_C_FLAGS} ${VERBOSE_CFLAGS} -pthread" "-DCMAKE_LD_FLAGS:STRING = ${CMAKE_LD_FLAGS} -pthread"
    )
    FILE (REMOVE_RECURSE ${BINARY_DIR})
    
    if (${CHECK_PTHREAD_COMPILE_RESULT})
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
