cmake_minimum_required (VERSION 2.8)
project (talloc2)

set (
    BOOLEANS
    TALLOC_DEBUG
    TALLOC_EXT_DESTRUCTOR TALLOC_REFERENCE
    TALLOC_UTILS_BUFFER TALLOC_UTILS_DYNARR
    TALLOC_SHARED TALLOC_STATIC TALLOC_TEST
)
foreach (bool ${BOOLEANS})
    if (NOT DEFINED ${bool})
        set (${bool} true)
    endif ()
endforeach()

if (TALLOC_UTILS_BUFFER MATCHES true OR TALLOC_UTILS_DYNARR MATCHES true)
    set (TALLOC_UTILS true)
endif ()
if (TALLOC_EXT_DESTRUCTOR MATCHES true OR TALLOC_REFERENCE MATCHES true)
    set (TALLOC_EXT true)
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

if (TALLOC_TEST MATCHES true)
    enable_testing ()
    add_subdirectory (tests)
endif ()