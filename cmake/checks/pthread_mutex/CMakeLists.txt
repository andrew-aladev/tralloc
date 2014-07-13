cmake_minimum_required (VERSION "2.8")
project ("check_pthread_mutex" "C")

add_executable (
    "main"
    "${CMAKE_CURRENT_SOURCE_DIR}/main.c"
)
