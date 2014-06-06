#!/bin/bash

CC="gcc-4.8.2" CFLAGS="-Werror -Wno-maybe-uninitialized" ../combinations.sh -DCMAKE_BUILD_TYPE="RELEASE"
NO_TESTS=1 CFLAGS="-Werror -Wno-maybe-uninitialized" ../combinations.sh -DCMAKE_TOOLCHAIN_FILE=../cmake/toolchains/arduino.cmake -DTRALLOC_SHARED=0 -DCMAKE_BUILD_TYPE="RELEASE"
CC="tcc" CFLAGS="-Werror" ../combinations.sh -DTRALLOC_SHARED=0 -DCMAKE_BUILD_TYPE="RELEASE"
CC="clang" CFLAGS="-Werror" ../combinations.sh -DCMAKE_BUILD_TYPE="RELEASE"
CC="ccc-analyzer" CFLAGS="-Werror" ../combinations.sh -DCMAKE_BUILD_TYPE="RELEASE"
CC="clang" CFLAGS="-Werror -fsanitize=leak -fno-omit-frame-pointer" ../combinations.sh -DCMAKE_BUILD_TYPE="RELEASE"
CC="clang" CFLAGS="-Werror -fsanitize=address -fno-omit-frame-pointer" ../combinations.sh -DCMAKE_BUILD_TYPE="RELEASE"
CC="clang" CFLAGS="-Werror -fsanitize=thread -fno-omit-frame-pointer" ../combinations.sh -DCMAKE_BUILD_TYPE="RELEASE"
