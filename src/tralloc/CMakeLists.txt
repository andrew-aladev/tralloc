configure_file (${CMAKE_CURRENT_SOURCE_DIR}/config.h.in ${CMAKE_CURRENT_SOURCE_DIR}/config.h)

install (FILES config.h types.h common.h chunk.h tree.h DESTINATION "${CMAKE_INSTALL_INCLUDEDIR}/${CURRENT_INCLUDEDIR}")
set (SOURCES common.c chunk.c)

if (${TRALLOC_DEBUG})
    install (FILES events.h DESTINATION "${CMAKE_INSTALL_INCLUDEDIR}/${CURRENT_INCLUDEDIR}")
    set (SOURCES ${SOURCES} events.c)
endif ()

set (INCLUDEDIR ${CURRENT_INCLUDEDIR})

set (CURRENT_INCLUDEDIR "${INCLUDEDIR}/tree")
add_subdirectory ("tree")

if (${TRALLOC_DESTRUCTOR})
    set (CURRENT_INCLUDEDIR "${INCLUDEDIR}/destructor")
    add_subdirectory ("destructor")
endif()
if (${TRALLOC_REFERENCE})
    set (CURRENT_INCLUDEDIR "${INCLUDEDIR}/reference")
    add_subdirectory ("reference")
endif ()

set (CURRENT_INCLUDEDIR "${INCLUDEDIR}/helpers")
add_subdirectory ("helpers")

if (${TRALLOC_UTILS})
    set (CURRENT_INCLUDEDIR "${INCLUDEDIR}/utils")
    add_subdirectory ("utils")
endif ()

if (${TRALLOC_SHARED})
    add_library (${TRALLOC_TARGET} SHARED ${SOURCES})
    install (TARGETS ${TRALLOC_TARGET} DESTINATION ${CMAKE_INSTALL_LIBDIR})
endif ()
if (${TRALLOC_STATIC})
    add_library (${TRALLOC_TARGET}_static STATIC ${SOURCES})
    set_target_properties (${TRALLOC_TARGET}_static PROPERTIES OUTPUT_NAME ${TRALLOC_TARGET})
    install (TARGETS ${TRALLOC_TARGET}_static DESTINATION ${CMAKE_INSTALL_LIBDIR})
endif ()