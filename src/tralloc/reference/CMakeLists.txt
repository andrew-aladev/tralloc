install (FILES "chunk.h" "clear.h" "move.h" "reference.h" "references.h" DESTINATION "${CMAKE_INSTALL_INCLUDEDIR}/${CURRENT_INCLUDEDIR}")
set (SOURCES ${SOURCES}
    "${CMAKE_CURRENT_SOURCE_DIR}/chunk.c"
    "${CMAKE_CURRENT_SOURCE_DIR}/clear.c"
    "${CMAKE_CURRENT_SOURCE_DIR}/move.c"
    "${CMAKE_CURRENT_SOURCE_DIR}/reference.c"
    "${CMAKE_CURRENT_SOURCE_DIR}/references.c"
PARENT_SCOPE)