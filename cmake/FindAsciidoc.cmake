find_program (ASCIIDOC_EXECUTABLE asciidoc)
find_program (ASCIIDOC_A2X_EXECUTABLE a2x)
mark_as_advanced (ASCIIDOC_EXECUTABLE ASCIIDOC_A2X_EXECUTABLE)

set (ASCIIDOC_FOUND false CACHE BOOL "Tool to make man from asciidoc")
if (ASCIIDOC_EXECUTABLE AND ASCIIDOC_A2X_EXECUTABLE)
    set (ASCIIDOC_FOUND true)
endif ()

IF (NOT ASCIIDOC_FOUND AND Asciidoc_FIND_REQUIRED)
    message (FATAL_ERROR "Could not find asciidoc")
endif ()