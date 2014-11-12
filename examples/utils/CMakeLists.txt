set (SOURCES
    "${CMAKE_CURRENT_SOURCE_DIR}/main.c"
)

if (${TRALLOC_SHARED})
    set (NAME "${TRALLOC_TARGET}-examples-utils")
    add_executable (${NAME} ${SOURCES})
    target_link_libraries (${NAME} ${TRALLOC_TARGET})
    
    if (${CMAKE_CAN_RUN_EXE})
        add_test (${NAME} ${NAME})
    endif ()
endif ()
if (${TRALLOC_STATIC})
    set (NAME "${TRALLOC_TARGET}-examples-utils-static")
    add_executable (${NAME} ${SOURCES})
    target_link_libraries (${NAME} "${TRALLOC_TARGET}-static")
    
    if (${CMAKE_CAN_RUN_EXE})
        add_test (${NAME} ${NAME})
    endif ()
endif ()
