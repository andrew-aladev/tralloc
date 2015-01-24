install (FILES "fragment.h" "pool_data.h" "pool_child_data.h" DESTINATION "${CMAKE_INSTALL_INCLUDEDIR}/${CURRENT_INCLUDEDIR}")
set (SOURCES ${SOURCES}
    "${CMAKE_CURRENT_SOURCE_DIR}/fragment.c"
    "${CMAKE_CURRENT_SOURCE_DIR}/pool_data.c"
    "${CMAKE_CURRENT_SOURCE_DIR}/pool_child_data.c"
)

if (${TRALLOC_THREADS})
    install (FILES "lock.h" DESTINATION "${CMAKE_INSTALL_INCLUDEDIR}/${CURRENT_INCLUDEDIR}")
    set (SOURCES ${SOURCES}
        "${CMAKE_CURRENT_SOURCE_DIR}/lock.c"
    )
endif ()

set (SOURCES ${SOURCES} PARENT_SCOPE)
