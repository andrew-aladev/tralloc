set (CMAKE_SYSTEM_NAME "Linux")
set (CMAKE_C_COMPILER "clang")
set (CMAKE_C_FLAGS "-Werror -Wno-maybe-uninitialized -fsanitize=thread -fno-omit-frame-pointer" CACHE STRING "clang thread cflags")
