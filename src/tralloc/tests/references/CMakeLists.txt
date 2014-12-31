set (SOURCES
    "${CMAKE_CURRENT_SOURCE_DIR}/errors.c"
    "${CMAKE_CURRENT_SOURCE_DIR}/add.c"
    "${CMAKE_CURRENT_SOURCE_DIR}/move_and_resize.c"
    "${CMAKE_CURRENT_SOURCE_DIR}/free_subtree.c"
    "${CMAKE_CURRENT_SOURCE_DIR}/test.c"
)

if (${TRALLOC_COLLECT_SOURCES})
    join ("${SOURCES}" " " FILES)
    set (SOURCES "${CMAKE_CURRENT_BINARY_DIR}/sources.c")
    execute_process (COMMAND sh -c "cat ${FILES} > ${SOURCES}")
endif ()

if (${TRALLOC_SHARED})
    add_library ("${TRALLOC_TARGET}-libtest-references" SHARED ${SOURCES})
    target_link_libraries ("${TRALLOC_TARGET}-libtest-references" "${TRALLOC_TARGET}-libtest" ${TRALLOC_TARGET} "m")
endif ()
if (${TRALLOC_STATIC})
    add_library ("${TRALLOC_TARGET}-libtest-references-static" STATIC ${SOURCES})
    target_link_libraries ("${TRALLOC_TARGET}-libtest-references-static" "${TRALLOC_TARGET}-libtest-static" "${TRALLOC_TARGET}-static" "m")
    set_target_properties ("${TRALLOC_TARGET}-libtest-references-static" PROPERTIES OUTPUT_NAME "${TRALLOC_TARGET}-libtest-references-static")
endif ()

set (SOURCES
    "${CMAKE_CURRENT_SOURCE_DIR}/main.c"
)

if (${TRALLOC_SHARED})
    set (NAME "${TRALLOC_TARGET}-test-references")
    add_executable (${NAME} ${SOURCES})
    target_link_libraries (${NAME} "${TRALLOC_TARGET}-libtest-references")
    
    if (${TRALLOC_CAN_RUN_EXE})
        add_test (${NAME} ${NAME})
    endif ()
endif ()
if (${TRALLOC_STATIC})
    set (NAME "${TRALLOC_TARGET}-test-references-static")
    add_executable (${NAME} ${SOURCES})
    target_link_libraries (${NAME} "${TRALLOC_TARGET}-libtest-references-static")
    
    if (${TRALLOC_CAN_RUN_EXE})
        add_test (${NAME} ${NAME})
    endif ()
endif ()
