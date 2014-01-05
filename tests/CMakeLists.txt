add_subdirectory ("basic")

if (${TRALLOC_LENGTH})
add_subdirectory ("length")
endif ()

if (${TRALLOC_DESTRUCTOR})
add_subdirectory ("destructor")
endif ()

if (${TRALLOC_DEBUG})
add_subdirectory ("events")
endif ()

add_subdirectory ("helpers")

if (${TRALLOC_REFERENCE})
add_subdirectory ("reference")
endif ()

add_subdirectory ("utils")