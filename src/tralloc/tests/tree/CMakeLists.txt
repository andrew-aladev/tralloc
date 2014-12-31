set (SOURCES
    "${CMAKE_CURRENT_SOURCE_DIR}/test.c"
)

add_subdirectory ("alloc")
add_subdirectory ("move")
add_subdirectory ("realloc")
add_subdirectory ("free")

if (${TRALLOC_COLLECT_SOURCES})
    join ("${SOURCES}" " " FILES)
    set (SOURCES "${CMAKE_CURRENT_BINARY_DIR}/sources.c")
    execute_process(COMMAND sh -c "cat ${FILES} > ${SOURCES}")
endif ()

if (${TRALLOC_SHARED})
    add_library ("${TRALLOC_TARGET}-libtest-tree" SHARED ${SOURCES})
    target_link_libraries ("${TRALLOC_TARGET}-libtest-tree" "${TRALLOC_TARGET}-libtest" ${TRALLOC_TARGET} "m")
endif ()
if (${TRALLOC_STATIC})
    add_library ("${TRALLOC_TARGET}-libtest-tree-static" STATIC ${SOURCES})
    target_link_libraries ("${TRALLOC_TARGET}-libtest-tree-static" "${TRALLOC_TARGET}-libtest-static" "${TRALLOC_TARGET}-static" "m")
    set_target_properties ("${TRALLOC_TARGET}-libtest-tree-static" PROPERTIES OUTPUT_NAME "${TRALLOC_TARGET}-libtest-tree-static")
endif ()

set (SOURCES
    "${CMAKE_CURRENT_SOURCE_DIR}/main.c"
)

if (${TRALLOC_SHARED})
    set (NAME "${TRALLOC_TARGET}-test-tree")
    add_executable (${NAME} ${SOURCES})
    target_link_libraries (${NAME} "${TRALLOC_TARGET}-libtest-tree")
    
    if (${TRALLOC_CAN_RUN_EXE})
        add_test (${NAME} ${NAME})
    endif ()
endif ()
if (${TRALLOC_STATIC})
    set (NAME "${TRALLOC_TARGET}-test-tree-static")
    add_executable (${NAME} ${SOURCES})
    target_link_libraries (${NAME} "${TRALLOC_TARGET}-libtest-tree-static")
    
    if (${TRALLOC_CAN_RUN_EXE})
        add_test (${NAME} ${NAME})
    endif ()
endif ()
