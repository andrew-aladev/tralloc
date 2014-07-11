cmake_minimum_required (VERSION "2.8")
project ("check_pthread_mutex" "C")

set (CMAKE_C_FLAGS "${CMAKE_C_FLAGS} -pedantic -Wall -Wextra")

add_executable (
    "main"
    "${CMAKE_CURRENT_SOURCE_DIR}/main.c"
)
