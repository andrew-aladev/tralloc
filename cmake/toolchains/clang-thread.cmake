set (CMAKE_SYSTEM_NAME "Linux")
set (CMAKE_C_COMPILER "clang")
set (_CMAKE_TOOLCHAIN_PREFIX "llvm-")
set (CMAKE_C_FLAGS "-Werror -fsanitize=thread -fno-omit-frame-pointer" CACHE STRING "clang thread cflags")
