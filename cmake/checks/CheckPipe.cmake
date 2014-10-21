function (check_pipe)
    if (DEFINED TRALLOC_HAVE_PIPE)
        return ()
    endif ()
    
    try_compile (
        CHECK_PIPE_COMPILE_RESULT
        "${PROJECT_BINARY_DIR}/CMakeTmp/basic"
        "${PROJECT_SOURCE_DIR}/cmake/checks/basic" "check_basic"
        CMAKE_FLAGS "-DCMAKE_C_FLAGS:STRING = ${CMAKE_C_FLAGS} -pipe"
    )
    if (${CHECK_PIPE_COMPILE_RESULT})
        set (TRALLOC_HAVE_PIPE true CACHE STRING "Status of pipe support")
        set (PIPE_CFLAGS "-pipe" CACHE STRING "pipe cflags")
        message (STATUS "Check for C compiler -pipe support - yes")
        return ()
    endif ()
    
    set (TRALLOC_HAVE_PIPE false CACHE STRING "Status of pipe support")
    message (STATUS "Check for C compiler -pipe support - no")
endfunction ()
