set (SOURCES
    "${CMAKE_CURRENT_SOURCE_DIR}/test.c"
)

if (${TRALLOC_UTILS_BUFFER})
    set (SOURCES ${SOURCES}
        "${CMAKE_CURRENT_SOURCE_DIR}/buffer.c"
    )
endif ()

if (${TRALLOC_COLLECT_SOURCES})
    join ("${SOURCES}" " " FILES)
    set (SOURCES "${CMAKE_CURRENT_BINARY_DIR}/sources.c")
    execute_process(COMMAND sh -c "cat ${FILES} > ${SOURCES}")
endif ()

if (${TRALLOC_SHARED})
    add_library ("${TRALLOC_TARGET}-libtest-utils" SHARED ${SOURCES})
    target_link_libraries ("${TRALLOC_TARGET}-libtest-utils" ${TRALLOC_TARGET})
endif ()
if (${TRALLOC_STATIC})
    add_library ("${TRALLOC_TARGET}-libtest-utils-static" STATIC ${SOURCES})
    target_link_libraries ("${TRALLOC_TARGET}-libtest-utils-static" "${TRALLOC_TARGET}-static")
    set_target_properties ("${TRALLOC_TARGET}-libtest-utils-static" PROPERTIES OUTPUT_NAME "${TRALLOC_TARGET}-libtest-utils-static")
endif ()

set (SOURCES
    "${CMAKE_CURRENT_SOURCE_DIR}/main.c"
)

if (${TRALLOC_SHARED})
    set (NAME "${TRALLOC_TARGET}-test-utils")
    add_executable (${NAME} ${SOURCES})
    target_link_libraries (${NAME} "${TRALLOC_TARGET}-libtest-utils")
    
    if (${TRALLOC_CAN_RUN_EXE})
        add_test (${NAME} ${NAME})
    endif ()
endif ()
if (${TRALLOC_STATIC})
    set (NAME "${TRALLOC_TARGET}-test-utils-static")
    add_executable (${NAME} ${SOURCES})
    target_link_libraries (${NAME} "${TRALLOC_TARGET}-libtest-utils-static")
    
    if (${TRALLOC_CAN_RUN_EXE})
        add_test (${NAME} ${NAME})
    endif ()
endif ()
