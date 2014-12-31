cmake_minimum_required (VERSION "2.8")
project ("tralloc_check_C99_inline" "C")

add_executable (
    "main"
    "${CMAKE_CURRENT_SOURCE_DIR}/test.c"
    "${CMAKE_CURRENT_SOURCE_DIR}/main.c"
)
