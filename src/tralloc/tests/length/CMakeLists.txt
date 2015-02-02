set (SOURCES
    "${CMAKE_CURRENT_SOURCE_DIR}/common.c"
    "${CMAKE_CURRENT_SOURCE_DIR}/test.c"
)

if (${TRALLOC_COLLECT_SOURCES})
    join_path ("${SOURCES}" " " FILES)
    set (SOURCES "${CMAKE_CURRENT_BINARY_DIR}/sources.c")
    execute_process (COMMAND sh -c "cat ${FILES} > '${SOURCES}'")
endif ()

if (${TRALLOC_SHARED})
    add_library ("${TRALLOC_TARGET}-libtest-length" SHARED ${SOURCES})
    target_link_libraries ("${TRALLOC_TARGET}-libtest-length" ${TRALLOC_TARGET})
endif ()
if (${TRALLOC_STATIC})
    add_library ("${TRALLOC_TARGET}-libtest-length-static" STATIC ${SOURCES})
    target_link_libraries ("${TRALLOC_TARGET}-libtest-length-static" "${TRALLOC_TARGET}-static")
    set_target_properties ("${TRALLOC_TARGET}-libtest-length-static" PROPERTIES OUTPUT_NAME "${TRALLOC_TARGET}-libtest-length-static")
endif ()

set (SOURCES
    "${CMAKE_CURRENT_SOURCE_DIR}/main.c"
)

if (${TRALLOC_SHARED})
    set (NAME "${TRALLOC_TARGET}-test-length")
    add_executable (${NAME} ${SOURCES})
    target_link_libraries (${NAME} "${TRALLOC_TARGET}-libtest-length")
    
    if (${TRALLOC_CAN_RUN_EXE})
        add_test (${NAME} ${NAME})
    endif ()
endif ()
if (${TRALLOC_STATIC})
    set (NAME "${TRALLOC_TARGET}-test-length-static")
    add_executable (${NAME} ${SOURCES})
    target_link_libraries (${NAME} "${TRALLOC_TARGET}-libtest-length-static")
    
    if (${TRALLOC_CAN_RUN_EXE})
        add_test (${NAME} ${NAME})
    endif ()
endif ()
