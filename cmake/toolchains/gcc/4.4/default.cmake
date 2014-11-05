set (CMAKE_SYSTEM_NAME "Linux")
set (CMAKE_C_COMPILER "gcc-4.4.7")
# "-Werror" can't be used, because maybe uninitialized is broken and "-Wno-maybe-uninitialized" is not available.