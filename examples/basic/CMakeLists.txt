set (SOURCES
    "${CMAKE_CURRENT_SOURCE_DIR}/main.c"
)

if (${TRALLOC_SHARED})
    set (NAME "${TRALLOC_TARGET}-examples-basic")
    add_executable (${NAME} ${SOURCES})
    target_link_libraries (${NAME} ${TRALLOC_TARGET})
    
    if (${TRALLOC_CAN_RUN_EXE})
        add_test (${NAME} ${NAME})
    endif ()
endif ()
if (${TRALLOC_STATIC})
    set (NAME "${TRALLOC_TARGET}-examples-basic-static")
    add_executable (${NAME} ${SOURCES})
    target_link_libraries (${NAME} "${TRALLOC_TARGET}-static")
    
    if (${TRALLOC_CAN_RUN_EXE})
        add_test (${NAME} ${NAME})
    endif ()
endif ()
