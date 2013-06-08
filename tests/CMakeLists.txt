set (BASIC   01-basic.c)
set (HELPERS 02-helpers.c)
set (EXT     03-ext.c)

if (TALLOC_SHARED MATCHES true)
    add_executable (${TALLOC_TARGET}-01-basic ${BASIC})
    target_link_libraries (${TALLOC_TARGET}-01-basic ${TALLOC_TARGET})
    add_test (${TALLOC_TARGET}-basic ${TALLOC_TARGET}-01-basic)

    add_executable (${TALLOC_TARGET}-02-helpers ${HELPERS})
    target_link_libraries (${TALLOC_TARGET}-02-helpers ${TALLOC_TARGET})
    add_test (${TALLOC_TARGET}-helpers ${TALLOC_TARGET}-02-helpers)

    add_executable (${TALLOC_TARGET}-03-ext ${EXT})
    target_link_libraries (${TALLOC_TARGET}-03-ext ${TALLOC_TARGET})
    add_test (${TALLOC_TARGET}-ext ${TALLOC_TARGET}-03-ext)
endif ()

if (TALLOC_STATIC MATCHES true)
    add_executable (${TALLOC_TARGET}-01-static-basic ${BASIC})
    target_link_libraries (${TALLOC_TARGET}-01-static-basic ${TALLOC_TARGET}_static)
    add_test (${TALLOC_TARGET}-static-basic ${TALLOC_TARGET}-01-static-basic)

    add_executable (${TALLOC_TARGET}-02-static-helpers ${HELPERS})
    target_link_libraries (${TALLOC_TARGET}-02-static-helpers ${TALLOC_TARGET}_static)
    add_test (${TALLOC_TARGET}-static-helpers ${TALLOC_TARGET}-02-static-helpers)

    add_executable (${TALLOC_TARGET}-03-static-ext ${EXT})
    target_link_libraries (${TALLOC_TARGET}-03-static-ext ${TALLOC_TARGET}_static)
    add_test (${TALLOC_TARGET}-static-ext ${TALLOC_TARGET}-03-static-ext)
endif ()