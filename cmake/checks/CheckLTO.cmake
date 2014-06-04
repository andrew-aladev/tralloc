function (check_lto)
    if (DEFINED TRALLOC_HAVE_LTO)
        return ()
    endif ()
    
    try_compile (
        CHECK_LTO_COMPILE_RESULT
        "${PROJECT_BINARY_DIR}/CMakeTmp/LTO"
        "${PROJECT_SOURCE_DIR}/cmake/checks/LTO" "check_LTO"
        CMAKE_FLAGS "-DCMAKE_C_FLAGS:STRING = -flto -fuse-linker-plugin" "-DCMAKE_LD_FLAGS:STRING = -flto -fuse-linker-plugin"
    )
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
