cmake_minimum_required (VERSION 2.8)
project (tralloc)

set (CMAKE_MODULE_PATH "${PROJECT_SOURCE_DIR}/cmake")

set (TRALLOC_VERSION "0.7.0")

set (TRALLOC_DEBUG        true  CACHE BOOL "debug features")
set (TRALLOC_DESTRUCTOR   true  CACHE BOOL "destructor feature")
set (TRALLOC_REFERENCE    true  CACHE BOOL "reference feature")
set (TRALLOC_UTILS_BUFFER true  CACHE BOOL "buffer util")
set (TRALLOC_UTILS_DYNARR true  CACHE BOOL "dynamic array util")
set (TRALLOC_SHARED       true  CACHE BOOL "build shared libs")
set (TRALLOC_STATIC       true  CACHE BOOL "build static libs")
set (TRALLOC_MAN          true  CACHE BOOL "create man")
set (TRALLOC_TEST         true  CACHE BOOL "build tests")
set (TRALLOC_EXAMPLES     false CACHE BOOL "build examples")

if (NOT DEFINED CMAKE_INSTALL_LIBDIR)
    set (CMAKE_INSTALL_LIBDIR "lib" CACHE PATH "Output directory for libraries")
endif ()
if (NOT DEFINED CMAKE_INSTALL_INCLUDEDIR)
  set (CMAKE_INSTALL_INCLUDEDIR "include" CACHE PATH "Output directory for include files")
endif ()
if (NOT DEFINED CMAKE_INSTALL_MANDIR)
  set (CMAKE_INSTALL_MANDIR "share/man/man3" CACHE PATH "Output directory for include files")
endif ()

if (${TRALLOC_UTILS_BUFFER} OR ${TRALLOC_UTILS_DYNARR})
    set (TRALLOC_UTILS true)
endif ()
if (${TRALLOC_DESTRUCTOR} OR ${TRALLOC_REFERENCE})
    set (TRALLOC_EXTENSIONS true)
endif ()

if (NOT ${TRALLOC_STATIC} AND NOT ${TRALLOC_SHARED})
    message (FATAL_ERROR "Tralloc can not be compiled. Please enable building of shared or static libraries")
endif ()

if (NOT DEFINED TRALLOC_TARGET)
    set (TRALLOC_TARGET ${PROJECT_NAME})
endif ()

if (NOT DEFINED CMAKE_BUILD_TYPE OR CMAKE_BUILD_TYPE STREQUAL "")
    set (CMAKE_BUILD_TYPE DEBUG)
endif ()

set (CMAKE_C_FLAGS "${CMAKE_C_FLAGS} -Wall -Wextra -Winline -std=gnu99")

set (CMAKE_C_FLAGS_DEBUG    "-O0 -g")
set (CMAKE_C_FLAGS_RELEASE  "-O2 -flto")
set (CMAKE_LD_FLAGS_RELEASE "-flto")

add_subdirectory (src)

include_directories ("src")

if (${TRALLOC_TEST})
    enable_testing ()
    add_subdirectory ("tests")
    
    if (${TRALLOC_EXAMPLES})
        add_subdirectory ("examples")
    endif ()
endif ()

if (${TRALLOC_MAN})
    add_subdirectory ("man")
endif ()

include (CPackConfig.cmake)
include (CPack)