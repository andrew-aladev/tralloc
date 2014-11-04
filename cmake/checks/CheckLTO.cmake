function (check_lto)
    if (DEFINED TRALLOC_HAVE_LTO)
        return ()
    endif ()
    
    include (CheckVerbose)
    check_verbose ()
    
    set (BINARY_DIR "${PROJECT_BINARY_DIR}/CMakeTmp/basic")
    set (SOURCE_DIR "${PROJECT_SOURCE_DIR}/cmake/checks/basic")
    set (NAME "check_basic")
    
    try_compile (CHECK_LTO_COMPILE_RESULT ${BINARY_DIR} ${SOURCE_DIR} ${NAME}
        CMAKE_FLAGS "-DCMAKE_C_FLAGS:STRING = ${CMAKE_C_FLAGS} ${VERBOSE_CFLAGS} -flto -fuse-linker-plugin" "-DCMAKE_LD_FLAGS:STRING = ${CMAKE_LD_FLAGS} -flto -fuse-linker-plugin"
    )
    FILE (REMOVE_RECURSE ${BINARY_DIR})
    
    if (${CHECK_LTO_COMPILE_RESULT})
        set (TRALLOC_HAVE_LTO true CACHE STRING "Status of LTO support")
        set (LTO_CFLAGS "-flto -fuse-linker-plugin" CACHE STRING "LTO cflags")
        set (LTO_LDLAGS "-flto -fuse-linker-plugin" CACHE STRING "LTO ldflags")
        message (STATUS "Check for C compiler LTO support - yes")
        return ()
    endif ()
    
    set (TRALLOC_HAVE_LTO false CACHE STRING "Status of LTO support")
    set (LTO_CFLAGS "" CACHE STRING "LTO cflags")
    set (LTO_LDLAGS "" CACHE STRING "LTO ldflags")
    message (STATUS "Check for C compiler LTO support - no")
endfunction ()
