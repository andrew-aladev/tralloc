if (DEFINED TRALLOC_ASCIIDOC_A2X_EXECUTABLE)
    return ()
endif ()

find_program (TRALLOC_ASCIIDOC_A2X_EXECUTABLE "a2x")

if (TRALLOC_ASCIIDOC_A2X_EXECUTABLE)
    set (TRALLOC_ASCIIDOC_A2X_FOUND true CACHE STRING "Status of asciidoc")
    message (STATUS "Look for \"asciidoc\" - yes")
else ()
    set (TRALLOC_ASCIIDOC_A2X_FOUND false CACHE STRING "Status of asciidoc")
    message (STATUS "Look for \"asciidoc\" - no")
endif ()
