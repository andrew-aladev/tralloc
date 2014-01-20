function (check_lto)
    if (DEFINED HAVE_LTO)
        return ()
    endif ()
    
    try_run (
        CHECK_LTO_RUN_RESULT CHECK_LTO_COMPILE_RESULT
        ${PROJECT_BINARY_DIR}
        SOURCES "${PROJECT_SOURCE_DIR}/cmake/test_files/empty.c"
        COMPILE_DEFINITIONS "-flto"
    )
    if (${CHECK_LTO_COMPILE_RESULT} AND CHECK_LTO_RUN_RESULT EQUAL 0)
        set (HAVE_LTO true CACHE STRING "Status of LTO support")
        set (LTO_CFLAGS "-flto" CACHE STRING "LTO cflags")
        set (LTO_LDLAGS "-flto" CACHE STRING "LTO ldflags")
        message (STATUS "Check for C compiler LTO support - ok")
        return ()
    endif ()
    
    set (HAVE_LTO false CACHE STRING "Status of LTO support")
    set (LTO_CFLAGS "" CACHE STRING "LTO cflags")
    set (LTO_LDLAGS "" CACHE STRING "LTO ldflags")
    message (WARNING "Check for C compiler LTO support - no")
endfunction ()
