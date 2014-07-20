install (FILES "chunk.h" "fragment.h" "pool.h" "pool_child.h" DESTINATION "${CMAKE_INSTALL_INCLUDEDIR}/${CURRENT_INCLUDEDIR}")
set (SOURCES ${SOURCES}
    "${CMAKE_CURRENT_SOURCE_DIR}/chunk.c"
    "${CMAKE_CURRENT_SOURCE_DIR}/fragment.c"
    "${CMAKE_CURRENT_SOURCE_DIR}/pool.c"
    "${CMAKE_CURRENT_SOURCE_DIR}/pool_child.c"
)

if (${TRALLOC_THREADS})
    install (FILES "lock.h" DESTINATION "${CMAKE_INSTALL_INCLUDEDIR}/${CURRENT_INCLUDEDIR}")
    set (SOURCES ${SOURCES}
        "${CMAKE_CURRENT_SOURCE_DIR}/lock.c"
    )
endif ()

set (SOURCES ${SOURCES} PARENT_SCOPE)
