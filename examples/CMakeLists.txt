add_subdirectory ("basic")

if (${TRALLOC_DESTRUCTOR})
    add_subdirectory ("destructor")
endif ()

add_subdirectory ("helpers")

if (${TRALLOC_LENGTH})
    add_subdirectory ("length")
endif ()

if (${TRALLOC_REFERENCE})
    add_subdirectory ("reference")
endif ()

if (${TRALLOC_POOL})
    add_subdirectory ("pool")
endif ()

if (${TRALLOC_UTILS})
    add_subdirectory ("utils")
endif ()