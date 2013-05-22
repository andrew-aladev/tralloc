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

add_library (talloc SHARED ${SOURCES})
target_link_libraries (talloc)