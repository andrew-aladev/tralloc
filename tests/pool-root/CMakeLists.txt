if (${TRALLOC_SHARED})
    set (NAME ${TRALLOC_TARGET}-test-pool-root)
    add_executable (${NAME} main.c)
    
    set (
        LIBS
        ${TRALLOC_TARGET}-libtest-basic
        ${TRALLOC_TARGET}-libtest-helpers
        ${TRALLOC_TARGET}-libtest-pool
    )
    if (${TRALLOC_DESTRUCTOR})
        set (LIBS ${LIBS} ${TRALLOC_TARGET}-libtest-destructor)
    endif ()
    if (${TRALLOC_DEBUG})
        set (LIBS ${LIBS} ${TRALLOC_TARGET}-libtest-events)
    endif ()
    if (${TRALLOC_LENGTH})
        set (LIBS ${LIBS} ${TRALLOC_TARGET}-libtest-length)
    endif ()
    if (${TRALLOC_REFERENCE})
        set (LIBS ${LIBS} ${TRALLOC_TARGET}-libtest-reference)
    endif ()
    if (${TRALLOC_UTILS})
        set (LIBS ${LIBS} ${TRALLOC_TARGET}-libtest-utils)
    endif ()
    
    target_link_libraries (${NAME} ${LIBS})
    add_test (${NAME} ${NAME})
endif ()
if (${TRALLOC_STATIC})
    set (NAME ${TRALLOC_TARGET}-test-pool-root-static)
    add_executable (${NAME} main.c)
    
    set (
        LIBS
        ${TRALLOC_TARGET}-libtest-basic-static
        ${TRALLOC_TARGET}-libtest-helpers-static
        ${TRALLOC_TARGET}-libtest-pool-static
    )
    if (${TRALLOC_DESTRUCTOR})
        set (LIBS ${LIBS} ${TRALLOC_TARGET}-libtest-destructor-static)
    endif ()
    if (${TRALLOC_DEBUG})
        set (LIBS ${LIBS} ${TRALLOC_TARGET}-libtest-events-static)
    endif ()
    if (${TRALLOC_LENGTH})
        set (LIBS ${LIBS} ${TRALLOC_TARGET}-libtest-length-static)
    endif ()
    if (${TRALLOC_REFERENCE})
        set (LIBS ${LIBS} ${TRALLOC_TARGET}-libtest-reference-static)
    endif ()
    if (${TRALLOC_UTILS})
        set (LIBS ${LIBS} ${TRALLOC_TARGET}-libtest-utils-static)
    endif ()
    
    target_link_libraries (${NAME} ${LIBS})
    add_test (${NAME} ${NAME})
endif ()
