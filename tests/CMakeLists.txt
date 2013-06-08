set (BASIC   01-basic.c)
set (HELPERS 02-helpers.c)
set (EXT     03-ext.c)
set (UTILS   04-utils.c)

if (TALLOC_SHARED MATCHES true)
    add_executable (${TALLOC_TARGET}-basic ${BASIC})
    target_link_libraries (${TALLOC_TARGET}-basic ${TALLOC_TARGET})
    add_test (${TALLOC_TARGET}-basic ${TALLOC_TARGET}-basic)

    add_executable (${TALLOC_TARGET}-helpers ${HELPERS})
    target_link_libraries (${TALLOC_TARGET}-helpers ${TALLOC_TARGET})
    add_test (${TALLOC_TARGET}-helpers ${TALLOC_TARGET}-helpers)

    add_executable (${TALLOC_TARGET}-ext ${EXT})
    target_link_libraries (${TALLOC_TARGET}-ext ${TALLOC_TARGET})
    add_test (${TALLOC_TARGET}-ext ${TALLOC_TARGET}-ext)
    
    add_executable (${TALLOC_TARGET}-utils ${UTILS})
    target_link_libraries (${TALLOC_TARGET}-utils ${TALLOC_TARGET})
    add_test (${TALLOC_TARGET}-utils ${TALLOC_TARGET}-utils)
endif ()

if (TALLOC_STATIC MATCHES true)
    add_executable (${TALLOC_TARGET}-static-basic ${BASIC})
    target_link_libraries (${TALLOC_TARGET}-static-basic ${TALLOC_TARGET}_static)
    add_test (${TALLOC_TARGET}-static-basic ${TALLOC_TARGET}-static-basic)

    add_executable (${TALLOC_TARGET}-static-helpers ${HELPERS})
    target_link_libraries (${TALLOC_TARGET}-static-helpers ${TALLOC_TARGET}_static)
    add_test (${TALLOC_TARGET}-static-helpers ${TALLOC_TARGET}-static-helpers)

    add_executable (${TALLOC_TARGET}-static-ext ${EXT})
    target_link_libraries (${TALLOC_TARGET}-static-ext ${TALLOC_TARGET}_static)
    add_test (${TALLOC_TARGET}-static-ext ${TALLOC_TARGET}-static-ext)
    
    add_executable (${TALLOC_TARGET}-static-utils ${UTILS})
    target_link_libraries (${TALLOC_TARGET}-static-utils ${TALLOC_TARGET})
    add_test (${TALLOC_TARGET}-static-utils ${TALLOC_TARGET}-static-utils)
endif ()