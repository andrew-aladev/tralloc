set (SOURCES
    "${CMAKE_CURRENT_SOURCE_DIR}/test.c"
    "${CMAKE_CURRENT_SOURCE_DIR}/str.c"
)

if (${TRALLOC_FILE})
    set (SOURCES ${SOURCES}
        "${CMAKE_CURRENT_SOURCE_DIR}/file.c"
    )
endif ()

if (${TRALLOC_SHARED})
    add_library ("${TRALLOC_TARGET}-libtest-helpers" SHARED ${SOURCES})
    target_link_libraries ("${TRALLOC_TARGET}-libtest-helpers" ${TRALLOC_TARGET})
endif ()
if (${TRALLOC_STATIC})
    add_library ("${TRALLOC_TARGET}-libtest-helpers-static" STATIC ${SOURCES})
    target_link_libraries ("${TRALLOC_TARGET}-libtest-helpers-static" "${TRALLOC_TARGET}-static")
    set_target_properties ("${TRALLOC_TARGET}-libtest-helpers-static" PROPERTIES OUTPUT_NAME "${TRALLOC_TARGET}-libtest-helpers-static")
endif ()

set (SOURCES
    "${CMAKE_CURRENT_SOURCE_DIR}/main.c"
)

if (${TRALLOC_SHARED})
    set (NAME "${TRALLOC_TARGET}-test-helpers")
    add_executable (${NAME} ${SOURCES})
    target_link_libraries (${NAME} "${TRALLOC_TARGET}-libtest-helpers")
    add_test (${NAME} ${NAME})
endif ()
if (${TRALLOC_STATIC})
    set (NAME "${TRALLOC_TARGET}-test-helpers-static")
    add_executable (${NAME} ${SOURCES})
    target_link_libraries (${NAME} "${TRALLOC_TARGET}-libtest-helpers-static")
    add_test (${NAME} ${NAME})
endif ()
