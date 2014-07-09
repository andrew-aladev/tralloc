set (SOURCES
    "${CMAKE_CURRENT_SOURCE_DIR}/add.c"
    "${CMAKE_CURRENT_SOURCE_DIR}/resize.c"
    "${CMAKE_CURRENT_SOURCE_DIR}/move.c"
    "${CMAKE_CURRENT_SOURCE_DIR}/free.c"
    "${CMAKE_CURRENT_SOURCE_DIR}/test.c"
)

if (${TRALLOC_COLLECT_SOURCES})
    join ("${SOURCES}" " " FILES)
    set (SOURCES "${CMAKE_CURRENT_BINARY_DIR}/sources.c")
    execute_process(COMMAND sh -c "cat ${FILES} > ${SOURCES}")
endif ()

if (${TRALLOC_SHARED})
    add_library ("${TRALLOC_TARGET}-libtest-debug" SHARED ${SOURCES})
    target_link_libraries ("${TRALLOC_TARGET}-libtest-debug" "${TRALLOC_TARGET}-libtest" ${TRALLOC_TARGET})
endif ()
if (${TRALLOC_STATIC})
    add_library ("${TRALLOC_TARGET}-libtest-debug-static" STATIC ${SOURCES})
    target_link_libraries ("${TRALLOC_TARGET}-libtest-debug-static" "${TRALLOC_TARGET}-libtest-static" "${TRALLOC_TARGET}-static")
    set_target_properties ("${TRALLOC_TARGET}-libtest-debug-static" PROPERTIES OUTPUT_NAME "${TRALLOC_TARGET}-libtest-debug-static")
endif ()

set (SOURCES
    "${CMAKE_CURRENT_SOURCE_DIR}/main.c"
)

if (${TRALLOC_SHARED})
    set (NAME "${TRALLOC_TARGET}-test-debug")
    add_executable (${NAME} ${SOURCES})
    target_link_libraries (${NAME} "${TRALLOC_TARGET}-libtest-debug")
    add_test (${NAME} ${NAME})
endif ()
if (${TRALLOC_STATIC})
    set (NAME "${TRALLOC_TARGET}-test-debug-static")
    add_executable (${NAME} ${SOURCES})
    target_link_libraries (${NAME} "${TRALLOC_TARGET}-libtest-debug-static")
    add_test (${NAME} ${NAME})
endif ()
