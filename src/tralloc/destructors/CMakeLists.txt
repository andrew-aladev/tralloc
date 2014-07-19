install (FILES "append.h" "chunk.h" "clear.h" "delete.h" "destructors.h" DESTINATION "${CMAKE_INSTALL_INCLUDEDIR}/${CURRENT_INCLUDEDIR}")
set (SOURCES ${SOURCES}
    "${CMAKE_CURRENT_SOURCE_DIR}/append.c"
    "${CMAKE_CURRENT_SOURCE_DIR}/chunk.c"
    "${CMAKE_CURRENT_SOURCE_DIR}/clear.c"
    "${CMAKE_CURRENT_SOURCE_DIR}/delete.c"
    "${CMAKE_CURRENT_SOURCE_DIR}/destructors.c"
PARENT_SCOPE)