function (check_file)
    if (DEFINED TRALLOC_HAVE_FILE)
        return ()
    endif ()
    
    include (CheckVerbose)
    check_verbose ()
    
    try_compile (
        CHECK_FILE_COMPILE_RESULT
        "${PROJECT_BINARY_DIR}/CMakeTmp/file"
        "${PROJECT_SOURCE_DIR}/cmake/checks/file" "check_file"
        CMAKE_FLAGS "-DCMAKE_C_FLAGS:STRING = ${CMAKE_C_FLAGS} ${VERBOSE_CFLAGS}"
    )
    if (${CHECK_FILE_COMPILE_RESULT})
        set (TRALLOC_HAVE_FILE true CACHE STRING "Status of File support")
        message (STATUS "Check for File support \t\t\t- yes")
        return ()
    endif ()
    
    set (TRALLOC_HAVE_FILE false CACHE STRING "Status of File support")
    message (STATUS "Check for File support \t\t\t- no")
endfunction ()
