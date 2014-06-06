install (FILES "append.h" "chunk.h" "clear.h" "delete.h" DESTINATION "${CMAKE_INSTALL_INCLUDEDIR}/${CURRENT_INCLUDEDIR}")
set (SOURCES ${SOURCES}
    "${CMAKE_CURRENT_SOURCE_DIR}/append.c"
    "${CMAKE_CURRENT_SOURCE_DIR}/chunk.c"
    "${CMAKE_CURRENT_SOURCE_DIR}/clear.c"
    "${CMAKE_CURRENT_SOURCE_DIR}/delete.c"
PARENT_SCOPE)