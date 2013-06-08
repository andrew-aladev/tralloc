if (CMAKE_BUILD_TYPE MATCHES DEBUG)
    set (TALLOC_DEBUG true)
endif ()
if (TALLOC_DEBUG MATCHES true OR TALLOC_EXT MATCHES true)
    set (TALLOC_EVENTS true)
endif ()

configure_file (${CMAKE_CURRENT_SOURCE_DIR}/config.h.in ${CMAKE_CURRENT_SOURCE_DIR}/config.h)

set (INCLUDES config.h events.h ext.h helpers.h tree.h types.h)
set (SOURCES  events.c ext.c helpers.c tree.c)

add_subdirectory (ext)
add_subdirectory (utils)

if (TALLOC_SHARED MATCHES true)
    add_library (${TALLOC_TARGET} SHARED ${SOURCES})
endif ()

if (TALLOC_STATIC MATCHES true)
    add_library (${TALLOC_TARGET}_static STATIC ${SOURCES})
    set_target_properties (${TALLOC_TARGET}_static PROPERTIES OUTPUT_NAME ${TALLOC_TARGET})
endif ()