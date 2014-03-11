function (check_pthread)
    if (DEFINED TRALLOC_HAVE_PTHREAD)
        return ()
    endif ()
    
    try_compile (
        CHECK_PTHREAD_COMPILE_RESULT
        ${PROJECT_BINARY_DIR}
        SOURCES "${PROJECT_SOURCE_DIR}/cmake/test_files/pthread.c"
        COMPILE_DEFINITIONS "-pthread"
        LINK_LIBRARIES      "-pthread"
    )
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
