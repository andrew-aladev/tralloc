function (check_bool)
    if (DEFINED TRALLOC_HAVE_BOOL)
        return ()
    endif ()
    
    include (CheckVerbose)
    check_verbose ()
    
    try_compile (
        CHECK_BOOL_COMPILE_RESULT
        "${PROJECT_BINARY_DIR}/CMakeTmp/bool"
        "${PROJECT_SOURCE_DIR}/cmake/checks/bool" "check_bool"
        CMAKE_FLAGS "-DCMAKE_C_FLAGS:STRING = ${CMAKE_C_FLAGS} ${VERBOSE_CFLAGS}"
    )
    if (${CHECK_BOOL_COMPILE_RESULT})
        set (TRALLOC_HAVE_BOOL true CACHE STRING "Status of Bool support")
        message (STATUS "Check for Bool support \t\t\t- yes")
        return ()
    endif ()
    
    set (TRALLOC_HAVE_BOOL false CACHE STRING "Status of Bool support")
    message (STATUS "Check for Bool support \t\t\t- no")
endfunction ()
