set (SOURCES
    "${CMAKE_CURRENT_SOURCE_DIR}/main.c"
    "${CMAKE_CURRENT_SOURCE_DIR}/dynarr.c"
)

if (${TRALLOC_COLLECT_SOURCES})
    join ("${SOURCES}" " " FILES)
    set (SOURCES "${CMAKE_CURRENT_BINARY_DIR}/sources.c")
    execute_process (COMMAND sh -c "cat ${FILES} > ${SOURCES}")
endif ()

if (${TRALLOC_SHARED})
    add_library ("${TRALLOC_TARGET}-libtest" SHARED ${SOURCES})
endif ()
if (${TRALLOC_STATIC})
    add_library ("${TRALLOC_TARGET}-libtest-static" STATIC ${SOURCES})
    set_target_properties ("${TRALLOC_TARGET}-libtest-static" PROPERTIES OUTPUT_NAME "${TRALLOC_TARGET}-libtest-static")
endif ()
