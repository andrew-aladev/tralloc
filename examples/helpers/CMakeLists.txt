set (SOURCES
    "${CMAKE_CURRENT_SOURCE_DIR}/main.c"
)

if (${TRALLOC_SHARED})
    set (NAME "${TRALLOC_TARGET}-examples-helpers")
    add_executable (${NAME} ${SOURCES})
    target_link_libraries (${NAME} ${TRALLOC_TARGET})
    add_test (${NAME} ${NAME})
endif ()
if (${TRALLOC_STATIC})
    set (NAME "${TRALLOC_TARGET}-examples-helpers-static")
    add_executable (${NAME} ${SOURCES})
    target_link_libraries (${NAME} "${TRALLOC_TARGET}-static")
    add_test (${NAME} ${NAME})
endif ()