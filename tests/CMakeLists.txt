set (BASIC   01-basic.c)
set (HELPERS 02-helpers.c)
set (EXT     03-ext.c)

add_executable (01-basic ${BASIC})
target_link_libraries (01-basic talloc)
add_test (talloc-basic 01-basic)

add_executable (02-helpers ${HELPERS})
target_link_libraries (02-helpers talloc)
add_test (talloc-helpers 02-helpers)

add_executable (03-ext ${EXT})
target_link_libraries (03-ext talloc)
add_test (talloc-ext 03-ext)

if (TALLOC_STATIC MATCHES true)
    add_executable (01-static-basic ${BASIC})
    target_link_libraries (01-static-basic talloc_static)
    add_test (talloc-static-basic 01-static-basic)

    add_executable (02-static-helpers ${HELPERS})
    target_link_libraries (02-static-helpers talloc_static)
    add_test (talloc-static-helpers 02-static-helpers)

    add_executable (03-static-ext ${EXT})
    target_link_libraries (03-static-ext talloc_static)
    add_test (talloc-static-ext 03-static-ext)
endif ()