add_executable (01-basic 01-basic.c)
target_link_libraries (01-basic talloc)
add_test (talloc-basic 01-basic)

add_executable (02-helpers 02-helpers.c)
target_link_libraries (02-helpers talloc m)
add_test (talloc-helpers 02-helpers)

add_executable (03-ext 03-ext.c)
target_link_libraries (03-ext talloc)
add_test (talloc-ext 03-ext)