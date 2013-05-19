if (CMAKE_BUILD_TYPE MATCHES DEBUG)
    set (TALLOC_DEBUG true)
endif ()
if (TALLOC_EXT_DESTRUCTOR MATCHES true)
    set (TALLOC_EXT true)
endif ()

configure_file (${CMAKE_CURRENT_SOURCE_DIR}/config.h.in ${CMAKE_CURRENT_SOURCE_DIR}/config.h)

set (INCLUDES config.h tree.h helpers.h ext.h)
set (SOURCES  tree.c ext.c externs.c)

add_library (talloc SHARED ${SOURCES})
target_link_libraries (talloc)