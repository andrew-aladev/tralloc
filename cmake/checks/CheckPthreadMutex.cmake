function (check_pthread_mutex)
    if (DEFINED TRALLOC_HAVE_PTHREAD_MUTEX)
        return ()
    endif ()
    
    include (CheckVerbose)
    include (CheckPthread)
    check_verbose ()
    check_pthread ()
    
    if (NOT ${TRALLOC_HAVE_PTHREAD})
        set (TRALLOC_HAVE_PTHREAD_MUTEX false CACHE STRING "Status of pthread_mutex support")
        message (STATUS "Check for pthread_mutex support - no")
        return ()
    endif ()
    
    set (BINARY_DIR "${PROJECT_BINARY_DIR}/CMakeTmp/pthread_mutex")
    set (SOURCE_DIR "${PROJECT_SOURCE_DIR}/cmake/checks/pthread_mutex")
    set (NAME "check_pthread_mutex")
    
    try_compile (CHECK_PTHREAD_MUTEX_COMPILE_RESULT ${BINARY_DIR} ${SOURCE_DIR} ${NAME}
        CMAKE_FLAGS "-DCMAKE_C_FLAGS:STRING = ${CMAKE_C_FLAGS} ${VERBOSE_CFLAGS} ${PTHREAD_CFLAGS}" "-DCMAKE_LD_FLAGS:STRING = ${CMAKE_LD_FLAGS} ${PTHREAD_LDLAGS}"
    )
    FILE (REMOVE_RECURSE ${BINARY_DIR})
    
    if (${CHECK_PTHREAD_MUTEX_COMPILE_RESULT})
        set (TRALLOC_HAVE_PTHREAD_MUTEX true CACHE STRING "Status of pthread_mutex support")
        message (STATUS "Check for pthread_mutex support - yes")
        return ()
    endif ()
    
    set (TRALLOC_HAVE_PTHREAD_MUTEX false CACHE STRING "Status of pthread_mutex support")
    message (STATUS "Check for pthread_mutex support - no")
endfunction ()
