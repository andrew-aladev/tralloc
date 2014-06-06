install (FILES "common.h" "fragment.h" "chunk.h" "head_chunk.h" DESTINATION "${CMAKE_INSTALL_INCLUDEDIR}/${CURRENT_INCLUDEDIR}")
set (SOURCES ${SOURCES}
    "${CMAKE_CURRENT_SOURCE_DIR}/common.c"
    "${CMAKE_CURRENT_SOURCE_DIR}/fragment.c"
    "${CMAKE_CURRENT_SOURCE_DIR}/chunk.c"
    "${CMAKE_CURRENT_SOURCE_DIR}/head_chunk.c"
PARENT_SCOPE)