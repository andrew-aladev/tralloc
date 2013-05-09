add_executable(01-basic 01-basic.c)
target_link_libraries(01-basic talloc)
add_test(01-basic 01-basic)