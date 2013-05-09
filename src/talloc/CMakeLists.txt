set (INCLUDES tree.h)
set (SOURCES  tree.c)

add_library(talloc SHARED ${SOURCES})
target_link_libraries(talloc)
