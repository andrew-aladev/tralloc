set (CMAKE_SYSTEM_NAME "Linux")
set (CMAKE_C_COMPILER "gcc-4.8.4")
set (CMAKE_C_FLAGS "${CMAKE_C_FLAGS} -Werror -Wno-maybe-uninitialized -fsanitize=address -fno-omit-frame-pointer" CACHE STRING "gcc-4.8.4 address cflags")
