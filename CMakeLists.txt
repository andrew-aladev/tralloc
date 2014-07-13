cmake_minimum_required (VERSION "2.8")
project ("tralloc" "C")


set (TRALLOC_VERSION "0.8.0")

set (TRALLOC_THREADS      true CACHE BOOL "threads feature")
set (TRALLOC_LENGTH       true CACHE BOOL "length feature")
set (TRALLOC_DESTRUCTOR   true CACHE BOOL "destructor feature")
set (TRALLOC_REFERENCE    true CACHE BOOL "reference feature")
set (TRALLOC_POOL         true CACHE BOOL "pool feature")
set (TRALLOC_UTILS_BUFFER true CACHE BOOL "buffer util")

set (TRALLOC_DEBUG_THREADS   true CACHE BOOL "threads in debug feature")
set (TRALLOC_DEBUG_CALLBACKS true CACHE BOOL "callbacks in debug feature")
set (TRALLOC_DEBUG_STATS     true CACHE BOOL "stats in debug feature")
set (TRALLOC_DEBUG_LOG       true CACHE BOOL "log in debug feature")

# You can force these locks to be "rwlock", "mutex" or "spinlock".
set (TRALLOC_DEBUG_STATS_LOCK_TYPE   "rwlock" CACHE STRING "type of debug stats lock")
set (TRALLOC_DEBUG_THREADS_LOCK_TYPE "rwlock" CACHE STRING "type of debug threads lock")
set (TRALLOC_SUBTREE_LOCK_TYPE       "rwlock" CACHE STRING "type of subtree lock")
set (TRALLOC_CHILDREN_LOCK_TYPE      "rwlock" CACHE STRING "type of children lock")

set (TRALLOC_SHARED   true CACHE BOOL "build shared libs")
set (TRALLOC_STATIC   true CACHE BOOL "build static libs")
set (TRALLOC_MAN      true CACHE BOOL "create man")
set (TRALLOC_TESTS    true CACHE BOOL "build tests")
set (TRALLOC_EXAMPLES true CACHE BOOL "build examples")

# Use this feature only for release. One big source file is inconvenient for debug.
set (TRALLOC_COLLECT_SOURCES false CACHE BOOL "collect all .c files in one to make compilation faster")


if (NOT DEFINED CMAKE_INSTALL_LIBDIR)
    set (CMAKE_INSTALL_LIBDIR "lib" CACHE PATH "Output directory for libraries")
endif ()
if (NOT DEFINED CMAKE_INSTALL_INCLUDEDIR)
  set (CMAKE_INSTALL_INCLUDEDIR "include" CACHE PATH "Output directory for include files")
endif ()
if (NOT DEFINED CMAKE_INSTALL_MANDIR)
  set (CMAKE_INSTALL_MANDIR "share/man/man3" CACHE PATH "Output directory for include files")
endif ()


if (NOT DEFINED TRALLOC_TARGET)
    set (TRALLOC_TARGET ${PROJECT_NAME})
endif ()

set (CMAKE_MODULE_PATH "${PROJECT_SOURCE_DIR}/cmake/checks" "${PROJECT_SOURCE_DIR}/cmake/finds")

include (CheckVerbose)
include (CheckC99)
include (CheckC99_inline)
include (CheckLTO)
include (CheckBool)
include (CheckFile)
include (CheckPthread)
include (CheckPthreadRwlock)
include (CheckPthreadMutex)
include (CheckPthreadSpinlock)
check_verbose ()
check_c99 ()
check_c99_inline ()
check_lto ()
check_bool ()
check_file ()
check_pthread ()
check_pthread_rwlock ()
check_pthread_mutex ()
check_pthread_spinlock ()

find_package (Asciidoc)

if (${TRALLOC_THREADS})
    if (NOT ${TRALLOC_HAVE_PTHREAD})
        message (WARNING "TRALLOC_THREADS is disabled, because you have no pthread.")
        set (TRALLOC_THREADS false)
    elseif (NOT ${TRALLOC_HAVE_PTHREAD_RWLOCK} AND NOT ${TRALLOC_HAVE_PTHREAD_MUTEX} AND NOT ${TRALLOC_HAVE_PTHREAD_SPINLOCK})
        message (WARNING "TRALLOC_THREADS is disabled, because you have no rwlock, mutex and spinlock.")
        set (TRALLOC_THREADS false)
    endif ()
endif ()

set (TRALLOC_THREADS_RWLOCK   1)
set (TRALLOC_THREADS_MUTEX    2)
set (TRALLOC_THREADS_SPINLOCK 3)

foreach (LOCK_TYPE TRALLOC_DEBUG_STATS_LOCK_TYPE TRALLOC_DEBUG_THREADS_LOCK_TYPE TRALLOC_SUBTREE_LOCK_TYPE TRALLOC_CHILDREN_LOCK_TYPE)
    if (${${LOCK_TYPE}} STREQUAL "rwlock")
        set (${LOCK_TYPE} ${TRALLOC_THREADS_RWLOCK})
    elseif (${${LOCK_TYPE}} STREQUAL "mutex")
        set (${LOCK_TYPE} ${TRALLOC_THREADS_MUTEX})
    elseif (${${LOCK_TYPE}} STREQUAL "spinlock")
        set (${LOCK_TYPE} ${TRALLOC_THREADS_SPINLOCK})
    else ()
        unset (${LOCK_TYPE})
    endif ()
endforeach ()

if (${TRALLOC_THREADS})
    if (${TRALLOC_HAVE_PTHREAD_RWLOCK})
        set (AVAILABLE_LOCK ${TRALLOC_THREADS_RWLOCK})
    elseif (${TRALLOC_HAVE_PTHREAD_MUTEX})
        set (AVAILABLE_LOCK ${TRALLOC_THREADS_MUTEX})
    elseif (${TRALLOC_HAVE_PTHREAD_SPINLOCK})
        set (AVAILABLE_LOCK ${TRALLOC_THREADS_SPINLOCK})
    endif ()
    
    foreach (LOCK_TYPE TRALLOC_DEBUG_STATS_LOCK_TYPE TRALLOC_DEBUG_THREADS_LOCK_TYPE TRALLOC_SUBTREE_LOCK_TYPE TRALLOC_CHILDREN_LOCK_TYPE)
        if (
            NOT ${TRALLOC_HAVE_PTHREAD_RWLOCK}   AND ${LOCK_TYPE} MATCHES ${TRALLOC_THREADS_RWLOCK} OR
            NOT ${TRALLOC_HAVE_PTHREAD_MUTEX}    AND ${LOCK_TYPE} MATCHES ${TRALLOC_THREADS_MUTEX}  OR
            NOT ${TRALLOC_HAVE_PTHREAD_SPINLOCK} AND ${LOCK_TYPE} MATCHES ${TRALLOC_THREADS_SPINLOCK}
        )
            message (WARNING "${LOCK_TYPE} is set to ${AVAILABLE_LOCK}, because you have no ${${LOCK_TYPE}}.")
            set (${LOCK_TYPE} ${AVAILABLE_LOCK})
        endif ()
    endforeach ()
endif ()

if (${TRALLOC_DESTRUCTOR} AND ${TRALLOC_HAVE_FILE})
    set (TRALLOC_FILE true)
endif ()
if (NOT ${TRALLOC_THREADS} AND ${TRALLOC_DEBUG_THREADS})
    message (WARNING "TRALLOC_DEBUG_THREADS is disabled, because TRALLOC_THREADS is disabled.")
    set (TRALLOC_DEBUG_THREADS false)
endif ()

if (${TRALLOC_UTILS_BUFFER})
    set (TRALLOC_UTILS true)
endif ()

if (${TRALLOC_LENGTH} OR ${TRALLOC_DESTRUCTOR} OR ${TRALLOC_REFERENCE} OR ${TRALLOC_POOL} OR ${TRALLOC_THREADS})
    set (TRALLOC_EXTENSIONS true)
endif ()
if (${TRALLOC_DEBUG_THREADS} OR ${TRALLOC_DEBUG_CALLBACKS} OR ${TRALLOC_DEBUG_STATS})
    set (TRALLOC_DEBUG true)
else ()
    set (TRALLOC_DEBUG false)
endif ()
if (NOT ${TRALLOC_DEBUG} AND ${TRALLOC_DEBUG_LOG})
    message (WARNING "TRALLOC_DEBUG_LOG is disabled, because TRALLOC_DEBUG is disabled.")
    set (TRALLOC_DEBUG_LOG false)
endif ()

if (NOT ASCIIDOC_A2X_EXECUTABLE)
    message (WARNING "TRALLOC_MAN is disabled, because you have no \"asciidoc\".")
    set (TRALLOC_MAN false)
endif ()

if (NOT ${TRALLOC_STATIC} AND NOT ${TRALLOC_SHARED})
    message (FATAL_ERROR "Tralloc can not be compiled. Please enable building of shared or static libraries.")
endif ()


if (NOT DEFINED CMAKE_BUILD_TYPE OR CMAKE_BUILD_TYPE STREQUAL "")
    set (CMAKE_BUILD_TYPE DEBUG)
endif ()

set (CMAKE_C_FLAGS "${CMAKE_C_FLAGS} ${VERBOSE_CFLAGS} ${C99_CFLAGS} ${PTHREAD_CFLAGS}")
set (CMAKE_LD_FLAGS "${CMAKE_LD_FLAGS} ${PTHREAD_LDLAGS}")
set (CMAKE_C_FLAGS_DEBUG    "-pipe -O0 -g")
set (CMAKE_C_FLAGS_RELEASE  "-pipe -O2 ${LTO_CFLAGS}")
set (CMAKE_LD_FLAGS_RELEASE ${LTO_LDLAGS})
set (CMAKE_C_FLAGS_RELEASE_EMBED  "-pipe -Os ${LTO_CFLAGS}")
set (CMAKE_LD_FLAGS_RELEASE_EMBED ${LTO_LDLAGS})

function(join VALUES GLUE OUTPUT)
  string (REGEX REPLACE "([^\\]|^);" "\\1${GLUE}" _TMP_STR "${VALUES}")
  string (REGEX REPLACE "[\\](.)" "\\1" _TMP_STR "${_TMP_STR}") #fixes escaping
  set (${OUTPUT} "${_TMP_STR}" PARENT_SCOPE)
endfunction()

if (${TRALLOC_TESTS} OR ${TRALLOC_EXAMPLES})
    enable_testing()
endif ()

include_directories ("src")
add_subdirectory ("src")

if (${TRALLOC_EXAMPLES})
    add_subdirectory ("examples")
endif ()

if (${TRALLOC_MAN})
    add_subdirectory ("man")
endif ()


include (CPackConfig.cmake)
include (CPack)
