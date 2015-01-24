install (FILES "reference_data.h" "references_data.h" DESTINATION "${CMAKE_INSTALL_INCLUDEDIR}/${CURRENT_INCLUDEDIR}")
set (SOURCES ${SOURCES}
    "${CMAKE_CURRENT_SOURCE_DIR}/clear.c"
    "${CMAKE_CURRENT_SOURCE_DIR}/move.c"
    "${CMAKE_CURRENT_SOURCE_DIR}/reference_data.c"
    "${CMAKE_CURRENT_SOURCE_DIR}/references_data.c"
PARENT_SCOPE)
