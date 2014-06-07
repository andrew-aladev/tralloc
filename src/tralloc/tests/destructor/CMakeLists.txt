set (SOURCES
    "${CMAKE_CURRENT_SOURCE_DIR}/errors.c"
    "${CMAKE_CURRENT_SOURCE_DIR}/common.c"
    "${CMAKE_CURRENT_SOURCE_DIR}/../lib/dynarr.c"
    "${CMAKE_CURRENT_SOURCE_DIR}/test.c"
)

if (${TRALLOC_SHARED})
    add_library ("${TRALLOC_TARGET}-libtest-destructor" SHARED ${SOURCES})
    target_link_libraries ("${TRALLOC_TARGET}-libtest-destructor" ${TRALLOC_TARGET})
endif ()
if (${TRALLOC_STATIC})
    add_library ("${TRALLOC_TARGET}-libtest-destructor-static" STATIC ${SOURCES})
    target_link_libraries ("${TRALLOC_TARGET}-libtest-destructor-static" "${TRALLOC_TARGET}-static")
    set_target_properties ("${TRALLOC_TARGET}-libtest-destructor-static" PROPERTIES OUTPUT_NAME "${TRALLOC_TARGET}-libtest-destructor-static")
endif ()

set (SOURCES
    "${CMAKE_CURRENT_SOURCE_DIR}/main.c"
)

if (${TRALLOC_SHARED})
    set (NAME "${TRALLOC_TARGET}-test-destructor")
    add_executable (${NAME} ${SOURCES})
    target_link_libraries (${NAME} "${TRALLOC_TARGET}-libtest-destructor")
    add_test (${NAME} ${NAME})
endif ()
if (${TRALLOC_STATIC})
    set (NAME "${TRALLOC_TARGET}-test-destructor-static")
    add_executable (${NAME} ${SOURCES})
    target_link_libraries (${NAME} "${TRALLOC_TARGET}-libtest-destructor-static")
    add_test (${NAME} ${NAME})
endif ()
