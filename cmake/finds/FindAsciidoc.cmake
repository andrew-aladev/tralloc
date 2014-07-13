if (DEFINED ASCIIDOC_A2X_EXECUTABLE)
    return ()
endif ()

find_program (ASCIIDOC_A2X_EXECUTABLE "a2x")

if (ASCIIDOC_A2X_EXECUTABLE)
    message (STATUS "Look for \"asciidoc\" - yes")
else ()
    message (STATUS "Look for \"asciidoc\" - no")
endif ()
