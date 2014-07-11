function (check_pthread_mutex)
    if (DEFINED TRALLOC_HAVE_PTHREAD_MUTEX)
        return ()
    endif ()
    
    include (CheckPthread)
    check_pthread ()
    
    if (NOT ${TRALLOC_HAVE_PTHREAD})
        set (TRALLOC_HAVE_PTHREAD_MUTEX false CACHE STRING "Status of pthread_mutex support")
        message (STATUS "Check for pthread_mutex support - no")
        return ()
    endif ()
    
    try_compile (
        CHECK_PTHREAD_MUTEX_COMPILE_RESULT
        "${PROJECT_BINARY_DIR}/CMakeTmp/pthread_mutex"
        "${PROJECT_SOURCE_DIR}/cmake/checks/pthread_mutex" "check_pthread_mutex"
        CMAKE_FLAGS "-DCMAKE_C_FLAGS:STRING = ${CMAKE_C_FLAGS} ${PTHREAD_CFLAGS}" "-DCMAKE_LD_FLAGS:STRING = ${CMAKE_LD_FLAGS} ${PTHREAD_LDLAGS}"
    )
    if (${CHECK_PTHREAD_MUTEX_COMPILE_RESULT})
        set (TRALLOC_HAVE_PTHREAD_MUTEX true CACHE STRING "Status of pthread_mutex support")
        message (STATUS "Check for pthread_mutex support - yes")
        return ()
    endif ()
    
    set (TRALLOC_HAVE_PTHREAD_MUTEX false CACHE STRING "Status of pthread_mutex support")
    message (STATUS "Check for pthread_mutex support - no")
endfunction ()
