set (SOURCES
    "${CMAKE_CURRENT_SOURCE_DIR}/errors.c"
    "${CMAKE_CURRENT_SOURCE_DIR}/add.c"
    "${CMAKE_CURRENT_SOURCE_DIR}/move.c"
    "${CMAKE_CURRENT_SOURCE_DIR}/free.c"
    "${CMAKE_CURRENT_SOURCE_DIR}/test.c"
)

add_subdirectory ("resize")

if (${TRALLOC_SHARED})
    add_library ("${TRALLOC_TARGET}-libtest-pool" SHARED ${SOURCES})
    target_link_libraries ("${TRALLOC_TARGET}-libtest-pool" ${TRALLOC_TARGET})
endif ()
if (${TRALLOC_STATIC})
    add_library ("${TRALLOC_TARGET}-libtest-pool-static" STATIC ${SOURCES})
    target_link_libraries ("${TRALLOC_TARGET}-libtest-pool-static" "${TRALLOC_TARGET}-static")
    set_target_properties ("${TRALLOC_TARGET}-libtest-pool-static" PROPERTIES OUTPUT_NAME "${TRALLOC_TARGET}-libtest-pool-static")
endif ()

set (SOURCES
    "${CMAKE_CURRENT_SOURCE_DIR}/main.c"
)

if (${TRALLOC_SHARED})
    set (NAME "${TRALLOC_TARGET}-test-pool")
    add_executable (${NAME} ${SOURCES})
    target_link_libraries (${NAME} "${TRALLOC_TARGET}-libtest-pool")
    add_test (${NAME} ${NAME})
endif ()
if (${TRALLOC_STATIC})
    set (NAME "${TRALLOC_TARGET}-test-pool-static")
    add_executable (${NAME} ${SOURCES})
    target_link_libraries (${NAME} "${TRALLOC_TARGET}-libtest-pool-static")
    add_test (${NAME} ${NAME})
endif ()
