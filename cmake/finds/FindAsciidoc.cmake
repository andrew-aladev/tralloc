if (DEFINED ASCIIDOC_A2X_EXECUTABLE)
    return ()
endif ()

find_program (ASCIIDOC_A2X_EXECUTABLE "a2x")

if (ASCIIDOC_A2X_EXECUTABLE)
    set (ASCIIDOC_A2X_FOUND true CACHE STRING "Status of asciidoc")
    message (STATUS "Look for \"asciidoc\" - yes")
else ()
    set (ASCIIDOC_A2X_FOUND false CACHE STRING "Status of asciidoc")
    message (STATUS "Look for \"asciidoc\" - no")
endif ()
