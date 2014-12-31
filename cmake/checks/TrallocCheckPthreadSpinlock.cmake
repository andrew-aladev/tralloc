function (tralloc_check_pthread_spinlock)
    if (DEFINED TRALLOC_HAVE_PTHREAD_SPINLOCK)
        return ()
    endif ()
    
    include (TrallocCheckVerbose)
    include (TrallocCheckPthread)
    tralloc_check_verbose ()
    tralloc_check_pthread ()
    
    if (NOT ${TRALLOC_HAVE_PTHREAD})
        set (TRALLOC_HAVE_PTHREAD_SPINLOCK false CACHE STRING "Status of pthread_spinlock support")
        message (STATUS "Check for pthread_spinlock support - no")
        return ()
    endif ()
    
    set (BINARY_DIR "${PROJECT_BINARY_DIR}/CMakeTmp/pthread_spinlock")
    set (SOURCE_DIR "${PROJECT_SOURCE_DIR}/cmake/checks/pthread_spinlock")
    set (NAME "tralloc_check_pthread_spinlock")
    
    try_compile (CHECK_PTHREAD_SPINLOCK_RESULT ${BINARY_DIR} ${SOURCE_DIR} ${NAME}
        CMAKE_FLAGS
            "-DCMAKE_C_FLAGS=${CMAKE_C_FLAGS} ${TRALLOC_VERBOSE_CFLAGS} ${TRALLOC_WERROR_CFLAGS} ${TRALLOC_PTHREAD_CFLAGS}"
            "-DCMAKE_EXE_LINKER_FLAGS=${CMAKE_EXE_LINKER_FLAGS} ${TRALLOC_PTHREAD_LDLAGS}"
            "-DCMAKE_VERBOSE_MAKEFILE=${CMAKE_CONFIG_VERBOSE_MAKEFILE}"
        OUTPUT_VARIABLE CHECK_PTHREAD_SPINLOCK_COMPILE_RESULT
    )
    if (${CMAKE_CONFIG_VERBOSE_MAKEFILE})
        message (STATUS ${CHECK_PTHREAD_SPINLOCK_COMPILE_RESULT})
    endif ()
    FILE (REMOVE_RECURSE ${BINARY_DIR})
    
    if (${CHECK_PTHREAD_SPINLOCK_RESULT})
        set (TRALLOC_HAVE_PTHREAD_SPINLOCK true CACHE STRING "Status of pthread_spinlock support")
        message (STATUS "Check for pthread_spinlock support - yes")
        return ()
    endif ()
    
    set (TRALLOC_HAVE_PTHREAD_SPINLOCK false CACHE STRING "Status of pthread_spinlock support")
    message (STATUS "Check for pthread_spinlock support - no")
endfunction ()
