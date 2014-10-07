install (FILES "events.h" DESTINATION "${CMAKE_INSTALL_INCLUDEDIR}/${CURRENT_INCLUDEDIR}")
set (SOURCES ${SOURCES}
    "${CMAKE_CURRENT_SOURCE_DIR}/events.c"
)

set (INCLUDEDIR ${CURRENT_INCLUDEDIR})

if (${TRALLOC_DEBUG_CALLBACKS})
    set (CURRENT_INCLUDEDIR "${INCLUDEDIR}/callbacks")
    add_subdirectory ("callbacks")
endif ()

if (${TRALLOC_DEBUG_LOG})
    set (CURRENT_INCLUDEDIR "${INCLUDEDIR}/log")
    add_subdirectory ("log")
endif ()

if (${TRALLOC_DEBUG_STATS})
    set (CURRENT_INCLUDEDIR "${INCLUDEDIR}/stats")
    add_subdirectory ("stats")
endif ()

if (${TRALLOC_DEBUG_THREADS})
    set (CURRENT_INCLUDEDIR "${INCLUDEDIR}/threads")
    add_subdirectory ("threads")
endif ()

set (SOURCES ${SOURCES} PARENT_SCOPE)
