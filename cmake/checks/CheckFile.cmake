function (check_file)
    if (DEFINED TRALLOC_HAVE_FILE)
        return ()
    endif ()
    
    try_compile (
        CHECK_FILE_COMPILE_RESULT
        "${PROJECT_BINARY_DIR}/CMakeTmp/file"
        "${PROJECT_SOURCE_DIR}/cmake/checks/file" "check_file"
    )
    if (${CHECK_FILE_COMPILE_RESULT})
        set (TRALLOC_HAVE_FILE true CACHE STRING "Status of File support")
        message (STATUS "Check for File support - yes")
        return ()
    endif ()
    
    set (TRALLOC_HAVE_FILE false CACHE STRING "Status of File support")
    message (STATUS "Check for File support - no")
endfunction ()
