function (check_pthread_rwlock)
    if (DEFINED TRALLOC_HAVE_PTHREAD_RWLOCK)
        return ()
    endif ()
    
    include (CheckVerbose)
    include (CheckPthread)
    check_verbose ()
    check_pthread ()
    
    if (NOT ${TRALLOC_HAVE_PTHREAD})
        set (TRALLOC_HAVE_PTHREAD_RWLOCK false CACHE STRING "Status of pthread_rwlock support")
        message (STATUS "Check for pthread_rwlock support - no")
        return ()
    endif ()
    
    set (BINARY_DIR "${PROJECT_BINARY_DIR}/CMakeTmp/pthread_rwlock")
    set (SOURCE_DIR "${PROJECT_SOURCE_DIR}/cmake/checks/pthread_rwlock")
    set (NAME "check_pthread_rwlock")
    
    try_compile (CHECK_PTHREAD_RWLOCK_RESULT ${BINARY_DIR} ${SOURCE_DIR} ${NAME}
        CMAKE_FLAGS
            "-DCMAKE_C_FLAGS=${CMAKE_C_FLAGS} ${VERBOSE_CFLAGS} ${WERROR_CFLAGS} ${PTHREAD_CFLAGS}"
            "-DCMAKE_EXE_LINKER_FLAGS=${CMAKE_EXE_LINKER_FLAGS} ${PTHREAD_LDLAGS}"
            "-DCMAKE_VERBOSE_MAKEFILE=${CMAKE_CONFIG_VERBOSE_MAKEFILE}"
        OUTPUT_VARIABLE CHECK_PTHREAD_RWLOCK_COMPILE_RESULT
    )
    if (${CMAKE_CONFIG_VERBOSE_MAKEFILE})
        message (${CHECK_PTHREAD_RWLOCK_COMPILE_RESULT})
    endif ()
    FILE (REMOVE_RECURSE ${BINARY_DIR})
    
    if (${CHECK_PTHREAD_RWLOCK_RESULT})
        set (TRALLOC_HAVE_PTHREAD_RWLOCK true CACHE STRING "Status of pthread_rwlock support")
        message (STATUS "Check for pthread_rwlock support - yes")
        return ()
    endif ()
    
    set (TRALLOC_HAVE_PTHREAD_RWLOCK false CACHE STRING "Status of pthread_rwlock support")
    message (STATUS "Check for pthread_rwlock support - no")
endfunction ()
