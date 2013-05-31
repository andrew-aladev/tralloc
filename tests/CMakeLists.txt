set (BASIC   01-basic.c)
set (HELPERS 02-helpers.c)
set (EXT     03-ext.c)

if (TALLOC_SHARED MATCHES true)
    add_executable (01-basic ${BASIC})
    target_link_libraries (01-basic ${PROJECT_NAME})
    add_test (${PROJECT_NAME}-basic 01-basic)

    add_executable (02-helpers ${HELPERS})
    target_link_libraries (02-helpers ${PROJECT_NAME})
    add_test (${PROJECT_NAME}-helpers 02-helpers)

    add_executable (03-ext ${EXT})
    target_link_libraries (03-ext ${PROJECT_NAME})
    add_test (${PROJECT_NAME}-ext 03-ext)
endif ()

if (TALLOC_STATIC MATCHES true)
    add_executable (01-static-basic ${BASIC})
    target_link_libraries (01-static-basic ${PROJECT_NAME}_static)
    add_test (${PROJECT_NAME}-static-basic 01-static-basic)

    add_executable (02-static-helpers ${HELPERS})
    target_link_libraries (02-static-helpers ${PROJECT_NAME}_static)
    add_test (${PROJECT_NAME}-static-helpers 02-static-helpers)

    add_executable (03-static-ext ${EXT})
    target_link_libraries (03-static-ext ${PROJECT_NAME}_static)
    add_test (${PROJECT_NAME}-static-ext 03-static-ext)
endif ()