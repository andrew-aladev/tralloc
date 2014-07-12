function (check_pthread_rwlock)
    if (DEFINED TRALLOC_HAVE_PTHREAD_RWLOCK)
        return ()
    endif ()
    
    include (CheckPthread)
    check_pthread ()
    
    if (NOT ${TRALLOC_HAVE_PTHREAD})
        set (TRALLOC_HAVE_PTHREAD_RWLOCK false CACHE STRING "Status of pthread_rwlock support")
        message (STATUS "Check for pthread_rwlock support - no")
        return ()
    endif ()
    
    try_compile (
        CHECK_PTHREAD_RWLOCK_COMPILE_RESULT
        "${PROJECT_BINARY_DIR}/CMakeTmp/pthread_rwlock"
        "${PROJECT_SOURCE_DIR}/cmake/checks/pthread_rwlock" "check_pthread_rwlock"
        CMAKE_FLAGS "-DCMAKE_C_FLAGS:STRING = ${CMAKE_C_FLAGS} ${PTHREAD_CFLAGS}" "-DCMAKE_LD_FLAGS:STRING = ${CMAKE_LD_FLAGS} ${PTHREAD_LDLAGS}"
    )
    if (${CHECK_PTHREAD_RWLOCK_COMPILE_RESULT})
        set (TRALLOC_HAVE_PTHREAD_RWLOCK true CACHE STRING "Status of pthread_rwlock support")
        message (STATUS "Check for pthread_rwlock support - yes")
        return ()
    endif ()
    
    set (TRALLOC_HAVE_PTHREAD_RWLOCK false CACHE STRING "Status of pthread_rwlock support")
    message (STATUS "Check for pthread_rwlock support - no")
endfunction ()
