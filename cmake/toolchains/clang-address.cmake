set (CMAKE_SYSTEM_NAME "Linux")
set (CMAKE_C_COMPILER "clang")
set (CMAKE_C_FLAGS "-Werror -fsanitize=address -fno-omit-frame-pointer" CACHE STRING "clang address cflags")
