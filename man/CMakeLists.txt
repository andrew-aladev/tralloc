set (man     "${CMAKE_CURRENT_BINARY_DIR}/tralloc.3")
set (man_src "${CMAKE_CURRENT_SOURCE_DIR}/tralloc.txt")
set (asciidoc_arguments -d manpage -f manpage -D ${CMAKE_CURRENT_BINARY_DIR} ${man_src})
add_custom_command (
    OUTPUT  ${man}
    COMMAND ${ASCIIDOC_A2X_EXECUTABLE} ${asciidoc_arguments}
    DEPENDS ${man_src}
    COMMENT "Building manpage"
)
add_custom_target (man ALL DEPENDS ${man})
install (FILES ${man} DESTINATION ${CMAKE_INSTALL_MANDIR})