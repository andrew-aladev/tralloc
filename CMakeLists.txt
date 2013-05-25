cmake_minimum_required (VERSION 2.8)
project (talloc)

if (NOT DEFINED TALLOC_EXT)
    set (TALLOC_EXT true)
endif ()
if (NOT DEFINED TALLOC_SHARED)
    set (TALLOC_SHARED true)
endif ()
if (NOT DEFINED TALLOC_STATIC)
    set (TALLOC_STATIC true)
endif ()

set (CMAKE_BUILD_TYPE DEBUG)

set (CMAKE_C_FLAGS "${CMAKE_C_FLAGS} -Wall -Wextra -Winline -std=gnu99")
set (CMAKE_C_FLAGS_DEBUG "-O0 -g")

add_subdirectory (src)

include_directories ("src/")

enable_testing ()
add_subdirectory (tests)
