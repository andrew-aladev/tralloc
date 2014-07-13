function (check_pthread_spinlock)
    if (DEFINED TRALLOC_HAVE_PTHREAD_SPINLOCK)
        return ()
    endif ()
    
    include (CheckVerbose)
    include (CheckPthread)
    check_verbose ()
    check_pthread ()
    
    if (NOT ${TRALLOC_HAVE_PTHREAD})
        set (TRALLOC_HAVE_PTHREAD_SPINLOCK false CACHE STRING "Status of pthread_spinlock support")
        message (STATUS "Check for pthread_spinlock support - no")
        return ()
    endif ()
    
    try_compile (
        CHECK_PTHREAD_SPINLOCK_COMPILE_RESULT
        "${PROJECT_BINARY_DIR}/CMakeTmp/pthread_spinlock"
        "${PROJECT_SOURCE_DIR}/cmake/checks/pthread_spinlock" "check_pthread_spinlock"
        CMAKE_FLAGS "-DCMAKE_C_FLAGS:STRING = ${CMAKE_C_FLAGS} ${VERBOSE_CFLAGS} ${PTHREAD_CFLAGS}" "-DCMAKE_LD_FLAGS:STRING = ${CMAKE_LD_FLAGS} ${PTHREAD_LDLAGS}"
        OUTPUT_VARIABLE OUTPUT
    )
    if (${CHECK_PTHREAD_SPINLOCK_COMPILE_RESULT})
        set (TRALLOC_HAVE_PTHREAD_SPINLOCK true CACHE STRING "Status of pthread_spinlock support")
        message (STATUS "Check for pthread_spinlock support - yes")
        return ()
    endif ()
    
    set (TRALLOC_HAVE_PTHREAD_SPINLOCK false CACHE STRING "Status of pthread_spinlock support")
    message (STATUS "Check for pthread_spinlock support - no")
endfunction ()
