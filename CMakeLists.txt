cmake_minimum_required(VERSION 2.8)
project(talloc)

set(CMAKE_VERBOSE_MAKEFILE on)

set(CMAKE_BUILD_TYPE Debug)
set(CMAKE_C_FLAGS "${CMAKE_C_FLAGS} -Wall -Wextra -std=gnu99")
set(CMAKE_C_FLAGS_DEBUG "-g -DDEBUG")

add_subdirectory(src)
include_directories("${CMAKE_CURRENT_SOURCE_DIR}/src/")
link_directories("${CMAKE_CURRENT_SOURCE_DIR}/src/talloc")

enable_testing()
add_subdirectory(tests)
# add_subdirectory(examples)
