add_executable(01-basic 01-basic.c)
target_link_libraries(01-basic talloc)
add_test(01-basic 01-basic)

add_executable(02-helpers 02-helpers.c)
target_link_libraries(02-helpers talloc)
add_test(01-helpers 02-helpers)