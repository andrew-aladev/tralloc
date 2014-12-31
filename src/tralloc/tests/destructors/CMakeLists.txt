set (SOURCES
    "${CMAKE_CURRENT_SOURCE_DIR}/errors.c"
    "${CMAKE_CURRENT_SOURCE_DIR}/common.c"
    "${CMAKE_CURRENT_SOURCE_DIR}/test.c"
)

if (${TRALLOC_COLLECT_SOURCES})
    join ("${SOURCES}" " " FILES)
    set (SOURCES "${CMAKE_CURRENT_BINARY_DIR}/sources.c")
    execute_process (COMMAND sh -c "cat ${FILES} > ${SOURCES}")
endif ()

if (${TRALLOC_SHARED})
    add_library ("${TRALLOC_TARGET}-libtest-destructors" SHARED ${SOURCES})
    target_link_libraries ("${TRALLOC_TARGET}-libtest-destructors" "${TRALLOC_TARGET}-libtest" ${TRALLOC_TARGET})
endif ()
if (${TRALLOC_STATIC})
    add_library ("${TRALLOC_TARGET}-libtest-destructors-static" STATIC ${SOURCES})
    target_link_libraries ("${TRALLOC_TARGET}-libtest-destructors-static" "${TRALLOC_TARGET}-libtest-static" "${TRALLOC_TARGET}-static")
    set_target_properties ("${TRALLOC_TARGET}-libtest-destructors-static" PROPERTIES OUTPUT_NAME "${TRALLOC_TARGET}-libtest-destructors-static")
endif ()

set (SOURCES
    "${CMAKE_CURRENT_SOURCE_DIR}/main.c"
)

if (${TRALLOC_SHARED})
    set (NAME "${TRALLOC_TARGET}-test-destructors")
    add_executable (${NAME} ${SOURCES})
    target_link_libraries (${NAME} "${TRALLOC_TARGET}-libtest-destructors")
    
    if (${TRALLOC_CAN_RUN_EXE})
        add_test (${NAME} ${NAME})
    endif ()
endif ()
if (${TRALLOC_STATIC})
    set (NAME "${TRALLOC_TARGET}-test-destructors-static")
    add_executable (${NAME} ${SOURCES})
    target_link_libraries (${NAME} "${TRALLOC_TARGET}-libtest-destructors-static")
    
    if (${TRALLOC_CAN_RUN_EXE})
        add_test (${NAME} ${NAME})
    endif ()
endif ()
