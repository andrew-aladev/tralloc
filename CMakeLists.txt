cmake_minimum_required (VERSION 2.8)
project (talloc)

set (TALLOC_EXT true)
set (TALLOC_STATIC true)

set (CMAKE_VERBOSE_MAKEFILE false)
set (CMAKE_BUILD_TYPE DEBUG)

set (CMAKE_C_FLAGS "${CMAKE_C_FLAGS} -Wall -Wextra -Winline -std=gnu99")
set (CMAKE_C_FLAGS_DEBUG "-O0 -g")

add_subdirectory (src)

include_directories ("src/")

enable_testing ()
add_subdirectory (tests)