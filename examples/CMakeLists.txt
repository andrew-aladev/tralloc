add_subdirectory ("basic")

if (${TRALLOC_DESTRUCTOR})
    add_subdirectory ("destructor")
endif ()

add_subdirectory ("helpers")

if (${TRALLOC_LENGTH})
    add_subdirectory ("length")
endif ()

if (${TRALLOC_UTILS})
    add_subdirectory ("utils")
endif ()