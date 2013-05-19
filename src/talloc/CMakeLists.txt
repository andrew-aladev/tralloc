if (CMAKE_BUILD_TYPE MATCHES DEBUG)
    set (TALLOC_DEBUG true)
endif ()
if (TALLOC_EXT_DESTRUCTOR MATCHES true)
    set (TALLOC_EXT true)
endif ()

configure_file (${CMAKE_CURRENT_SOURCE_DIR}/config.h.in ${CMAKE_CURRENT_SOURCE_DIR}/config.h)

set (INCLUDES config.h ext.h helpers.h tree.h types.h)
set (SOURCES  tree.c externs.c)

add_subdirectory (ext)

add_library (talloc SHARED ${SOURCES})
target_link_libraries (talloc)