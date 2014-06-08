set (CMAKE_SYSTEM_NAME "Linux")
set (CMAKE_C_COMPILER "clang")
set (CMAKE_C_FLAGS "-Werror -fsanitize=leak -fno-omit-frame-pointer" CACHE STRING "clang leak cflags")
