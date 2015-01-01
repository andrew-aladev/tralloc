add_subdirectory ("basic")

if (${TRALLOC_DESTRUCTORS})
    add_subdirectory ("destructors")
endif ()

add_subdirectory ("helpers")

if (${TRALLOC_LENGTH})
    add_subdirectory ("length")
endif ()

if (${TRALLOC_REFERENCES})
    add_subdirectory ("references")
endif ()

if (${TRALLOC_POOL})
    add_subdirectory ("pool")
endif ()

if (${TRALLOC_UTILS})
    add_subdirectory ("utils")
endif ()
