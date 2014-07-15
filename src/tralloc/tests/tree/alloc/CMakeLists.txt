set (SOURCES ${SOURCES}
    "${CMAKE_CURRENT_SOURCE_DIR}/errors.c"
    "${CMAKE_CURRENT_SOURCE_DIR}/main.c"
    "${CMAKE_CURRENT_SOURCE_DIR}/test.c"
)

if (${TRALLOC_THREADS})
    set (SOURCES ${SOURCES}
        "${CMAKE_CURRENT_SOURCE_DIR}/threads.c"
    )
endif ()

set (SOURCES ${SOURCES} PARENT_SCOPE)
