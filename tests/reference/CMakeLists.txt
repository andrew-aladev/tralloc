set (SOURCES errors.c add.c move_and_resize.c free_subtree.c test.c)

if (${TRALLOC_SHARED})
    add_library (${TRALLOC_TARGET}-libtest-reference SHARED ${SOURCES})
    target_link_libraries (${TRALLOC_TARGET}-libtest-reference ${TRALLOC_TARGET} m)
endif ()
if (${TRALLOC_STATIC})
    add_library (${TRALLOC_TARGET}-libtest-reference-static STATIC ${SOURCES})
    target_link_libraries (${TRALLOC_TARGET}-libtest-reference-static ${TRALLOC_TARGET}-static m)
    set_target_properties (${TRALLOC_TARGET}-libtest-reference-static PROPERTIES OUTPUT_NAME ${TRALLOC_TARGET}-libtest-reference-static)
endif ()

if (${TRALLOC_SHARED})
    set (NAME ${TRALLOC_TARGET}-test-reference)
    add_executable (${NAME} main.c)
    target_link_libraries (${NAME} ${TRALLOC_TARGET}-libtest-reference)
    add_test (${NAME} ${NAME})
endif ()
if (${TRALLOC_STATIC})
    set (NAME ${TRALLOC_TARGET}-test-reference-static)
    add_executable (${NAME} main.c)
    target_link_libraries (${NAME} ${TRALLOC_TARGET}-libtest-reference-static)
    add_test (${NAME} ${NAME})
endif ()
