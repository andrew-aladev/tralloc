cmake_minimum_required (VERSION 2.8)
project (check_C99_inline C)

add_executable (
    "main"
    "${PROJECT_SOURCE_DIR}/test.c"
    "${PROJECT_SOURCE_DIR}/main.c"
)
