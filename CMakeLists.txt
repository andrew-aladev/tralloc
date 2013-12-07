cmake_minimum_required (VERSION 2.8)
project (talloc2)

set (TALLOC_DEBUG        true CACHE BOOL "")
set (TALLOC_DESTRUCTOR   true CACHE BOOL "")
set (TALLOC_REFERENCE    true CACHE BOOL "")
set (TALLOC_UTILS_BUFFER true CACHE BOOL "")
set (TALLOC_UTILS_DYNARR true CACHE BOOL "")
set (TALLOC_SHARED       true CACHE BOOL "")
set (TALLOC_STATIC       true CACHE BOOL "")
set (TALLOC_TEST         true CACHE BOOL "")

set (TALLOC_DEBUG        true)
set (TALLOC_DESTRUCTOR   false)
set (TALLOC_REFERENCE    false)
set (TALLOC_UTILS_BUFFER true)
set (TALLOC_UTILS_DYNARR false)
set (TALLOC_SHARED       true)
set (TALLOC_STATIC       true)
set (TALLOC_TEST         true)

if (${TALLOC_UTILS_BUFFER} OR ${TALLOC_UTILS_DYNARR})
    set (TALLOC_UTILS true)
endif ()
if (${TALLOC_DESTRUCTOR} OR ${TALLOC_REFERENCE})
    set (TALLOC_EXTENSIONS true)
endif ()

if (NOT ${TALLOC_STATIC} AND NOT ${TALLOC_SHARED})
    message (FATAL_ERROR "Talloc can not be compiled. Please enable building of shared or static libraries")
endif ()

if (NOT DEFINED TALLOC_TARGET)
    set (TALLOC_TARGET ${PROJECT_NAME})
endif ()

if (NOT DEFINED CMAKE_BUILD_TYPE OR CMAKE_BUILD_TYPE STREQUAL "")
    set (CMAKE_BUILD_TYPE DEBUG)
endif ()

set (CMAKE_C_FLAGS "${CMAKE_C_FLAGS} -Wall -Wextra -Winline -std=gnu99")

set (CMAKE_C_FLAGS_DEBUG    "-O0 -g")
set (CMAKE_C_FLAGS_RELEASE  "-O2 -flto")
set (CMAKE_LD_FLAGS_RELEASE "-flto")

add_subdirectory (src)

include_directories ("src/")

if (${TALLOC_TEST})
    enable_testing ()
    add_subdirectory (tests)
endif ()